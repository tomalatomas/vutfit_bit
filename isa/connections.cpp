#include "connections.hpp"

void initSecureComm(connections *conns, arguments args){
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    //Structure to hold the SSL information = context
    conns->ctx = SSL_CTX_new(SSLv23_client_method());
    if (conns->ctx == NULL) {
		cerr << "Creating SSL Context failed, check version of your OpenSSL"<< endl;
		exit(1);
	}

    //Load the trust store file or directory with certificates
    if (args.c != "") {
        if(!SSL_CTX_load_verify_locations(conns->ctx, args.c.c_str(), NULL)){
            cerr << "Verifying trust store file failed!"<< endl;
            exit(1);
        }
    }
    else if (args.C != "") {
        if(!SSL_CTX_load_verify_locations(conns->ctx, NULL, args.C.c_str())){
            cerr << "Verifying directory with certificates failed!"<< endl;
            exit(1);
        }
    }
    else{
        SSL_CTX_set_default_verify_paths(conns->ctx);
    }
}

connections connectToServer(arguments args){
    connections conns;
    string hostname = args.server + ":" + to_string(args.p);
    if(args.T){
        //Initializing OpenSSL Library
        initSecureComm(&conns, args);
        //Creating bio object
        conns.bio = BIO_new_ssl_connect(conns.ctx);
        BIO_get_ssl(conns.bio, &(conns.ssl));
        SSL_set_mode(conns.ssl, SSL_MODE_AUTO_RETRY);
        BIO_set_conn_hostname(conns.bio, hostname.c_str());
    }
    else {
        //Create connection
        conns.bio = BIO_new_connect(hostname.c_str());
    }

    if(conns.bio == NULL){
        cerr << "Could not create connection with hostname "<< hostname<<"!"<< endl;
        exit(1);
    }
    //Establish connection
    if(BIO_do_connect(conns.bio) <= 0){
        cerr << "Connection to "<<hostname<<" failed !"<< endl;
        exit(1);
    }
    string response = readResponse(conns.bio, "\r\n");
    //Checking response from the server
    if(!checkResponse(response)){
        cerr << "Invalid response from "<<hostname<<"!"<< endl;
        exit(1);
    }
    
    if(args.S){
        //Requesting TLS communication
        if(!writeCommand(conns.bio, "STLS")){
            cerr << "Sending TLS request failed!"<< endl;
            exit(1);
        }
        if(!checkResponse(readResponse(conns.bio, "\r\n"))){
            cerr << "TLS request failed!"<< endl;
            exit(1);
        }
        //TLS communication initialized
        initSecureComm(&conns, args);
        BIO* securebio = BIO_new_ssl(conns.ctx, 1);
        conns.bio = BIO_push(securebio, conns.bio);
        BIO_get_ssl(conns.bio, &(conns.ssl));
        SSL_set_mode(conns.ssl, SSL_MODE_AUTO_RETRY);
        if (BIO_do_handshake(conns.bio) <= 0) {
            cerr << "Handshake when configuring TLS failed!"<< endl;
            exit(1);
        }
    } 

    if(args.T || args.S){
        //Checking verification of certificates
        if(SSL_get_verify_result(conns.ssl) != X509_V_OK){
            cerr << "Certificate verification failed!"<< endl;
            exit(1);
        }
        if(SSL_get_peer_certificate(conns.ssl) == NULL){
            cerr << "No Certificate received!"<< endl;
            exit(1);
        }       
    } 

    return conns;
}

void authorization(authInfo auth, BIO* bio){
    //USER and PASS commands
    if(!writeCommand(bio, "USER "+ auth.username)){
        cerr << "Passing username information to server failed!"<< endl;
        exit(1);
    }
    if(!checkResponse(readResponse(bio, "\r\n"))){
        cerr << "Invalid username!"<< endl;
        exit(1);
    }
    if(!writeCommand(bio, "PASS "+ auth.password)){
        cerr << "Passing password information to server failed!"<< endl;
        exit(1);
    }
    if(!checkResponse(readResponse(bio, "\r\n"))){
        cerr << "Invalid password!"<< endl;
        exit(1);
    }
}

void getAllMessages(BIO* bio, arguments args){
    int numberofDownloadedMessages = 0;
    //Getting message count
    if(!writeCommand(bio, "STAT")){
        cerr << "Passing username information to server failed!"<< endl;
        exit(1);
    }
    int messageCount = getMessageCount(readResponse(bio, "\r\n"));
    if(messageCount == -1){
        cerr << "Retrieving information about mailbox failed!"<< endl;
        exit(1);
    }
    if(messageCount == 0){
        cerr << "Your mailbox is empty!"<< endl;
        exit(0);
    }
    for (int i = 1; i <= messageCount; i++) {
        //For every message read 
        string command = "RETR " + to_string(i);
        if(!writeCommand(bio, command)){
            cerr << "Retrieving message no."<< i <<" failed!!"<< endl;
        }
        string message = readResponse(bio, "\r\n.\r\n");
        if(processMessage(bio,message, args, i)){
            numberofDownloadedMessages++;
        }
    }
    //Print out number of messages downloaded
    if(args.n){
        cout << to_string(numberofDownloadedMessages) << " new messages"<< " downloaded."<< endl;
    } else {
        cout << to_string(numberofDownloadedMessages) << " messages"<< " downloaded."<< endl;
    }
}

void quitConnection(BIO* bio){
    //Finishing the connection -> UPDATE state 
    string command = "QUIT";
    if(!writeCommand(bio, command)){
        cerr << "Error while closing connection!"<< endl;
    }
    if(!checkResponse(readResponse(bio, "\r\n"))){
        cerr << "Error while closing connection!"<< endl;
    }
    BIO_free_all(bio);
}


bool processMessage(BIO* bio,string message, arguments args, int index){
    bool saved = false;
    if(!checkResponse(message)){
        cerr << "Error in retrieved message!"<< endl;
        return false;
    }
    //Remove unwanted stuff from message        
    message = removeMessageEnd(message);
    message = removeResponseStatus(message);
    message = removeBytestuffing(message);
    //Get message id
    string messageID = getMessageID(message);
    if(messageID == ""){
        cerr << "Message with invalid ID received!"<< endl;
        return saved;
    }
    if(message == ""){
        cerr << "Message in invalid format received!"<< endl;
        return saved;
    }
    //Checking if message was already downloaded once
    bool novelty = checkNovelty(messageID);
    if(!args.n || (args.n && novelty)){
        //If downloading all messages or message is new
        saveMessage(message,args);
        saved = true;
        if(args.d){
            //IF -d, delete message
            if(!deleteMessage(bio, index)){
                cerr << "Error while deleting message no."<< index << endl;
            }
        }
    }
    return saved;
}
void saveMessage(string message, arguments args){
        string subject = getSubject(message);
        if(subject == ""){
            subject = "No Subject";
        }
        //Limiting name size
        subject = subject.substr(0, 20);
        //Remove unwanted characters
        static regex reg("([ /.])"); 
        subject = regex_replace(subject, reg, "_");
        static regex reg1("([?=!@#$%^&:|\\`\"*-])"); 
        subject = regex_replace(subject, reg1, "");
        string filename = args.o + "/"+subject+".txt";
        int i = 1;
        //Checking if filealready exits, if yes, append number to the name
        while(checkFileExist(filename)){
            filename = args.o + "/"+to_string(i)+"_"+subject+".txt";
            i++;
        }
        std::ofstream outfile;
        outfile.open(filename); // append instead of overwrite
        outfile << message; 
        outfile.close();  
}

bool deleteMessage(BIO* bio, int index){
    if(!writeCommand(bio, "DELE "+ to_string(index))){
        return false;
    }
    if(!checkResponse(readResponse(bio, "\r\n"))){
        return false;
    }
    return true;
}

bool checkNovelty(string messageID){
    //Returns if MessageID is new, if yes, adds messageid to list
    string filename = "./.downloaded";
    std::ifstream input(filename);
    std::smatch match;
    //Reading lines
    for(std::string line; getline(input, line);){
        static std::regex rgxMessageID("messageid : ((.+))");
        if (std::regex_search(line,match,rgxMessageID)){
            if(match[1] == messageID){
                return false;
            }
        } 
    }
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app); // append instead of overwrite
    string row = "messageid : " + messageID + "\n";
    outfile << row; 
    outfile.close();  
    return true;
}

string removeResponseStatus(string message){
    //Removes first "\r\n" and everything before that
    string terminator = "\r\n";
    size_t found = message.find(terminator);

    if (found != string::npos){
        string modifiedMessage = message.substr(found+terminator.size());
        return modifiedMessage;
    } 
    return "";
}

string removeMessageEnd(string message){
    //Removes multiline terminator at the end of the message
    static regex reg("(\r\n\\.\r\n)"); 
    string replaced = regex_replace(message, reg, "");
    string messageEnd = "\r\n";
    replaced = replaced.append(messageEnd);
    return replaced;
}

string removeBytestuffing(string message){
    //finds crlf.. and removes one dot
    static regex reg("(\r\n\\.\\.)"); 
    string replaced = regex_replace(message, reg, "\r\n.");
    return replaced;
}

string getMessageID(string message){
    //Finds messageid with the use of regex
    static std::regex rgxMessageID("message-id: (.+).*",std::regex_constants::icase);
    std::smatch match;
    if (std::regex_search(message,match, rgxMessageID)){
        return match[1];
    } 
    return "";
}

string getSubject(string message){
    //Finds message subject with the use of regex
    static std::regex rgxMessageID("subject: (.+).*",std::regex_constants::icase);
        std::smatch match;
        if (std::regex_search(message,match, rgxMessageID)){
            return match[1];
        } 
        return "";
}

int getMessageCount(string stat){
    //Returns number of message from response after STAT Command
    static std::regex rgx("\\+OK (\\d+) *.");
    std::smatch match;

    if (std::regex_search(stat,match, rgx)){
        string stringCount = match[1];
        return stoi(stringCount);
    }
    return -1;
}

bool writeCommand(BIO* bio, string message){
    //Via bioobject send command
    string completeMessage = message.append("\r\n");
    const char* convertedMessage = completeMessage.c_str();
    if(BIO_write(bio, convertedMessage, strlen(convertedMessage)) <= 0) { // Bytes writen
         // Handle failed write here /
        return false;
    }
    return true;
}

string readResponse(BIO* bio, string messageTerminator){
    //Reading from server
    char *buffer = new char[BUFSIZE];
    int bytesread;
    string message;
    //Clearing/Emptying char array, 
    //Clearing with one more than is size of the buffer to avoid artefacts in message
    memset(buffer, 0, BUFSIZE);
    while ((bytesread = BIO_read(bio, buffer, BUFSIZE-1)) > 0) {
        if(bytesread == 0){
            cerr << "Failed reading response data - closed connection"<< endl;
            exit(1);
        }
        if (bytesread < 0){
            if(!BIO_should_retry(bio)){ //Trying to recover 
                cerr << "Failed reading response data"<< endl;
                exit(1);
            }
            if (bytesread < 0){
                cerr << "Failed reading response data"<< endl;
                exit(1);
            }
        }
        message.append(buffer);
        memset(buffer, 0, BUFSIZE);
        if(checkMessageTermination(message, messageTerminator)){
            break;
        }
    }
    return message;
}

bool checkMessageTermination(string message, string messageTerminator){
    string substring = lastN(message, messageTerminator.length());
    if(substring.compare(messageTerminator) == 0) return true;
    return false;
}

string lastN(string input, int n){
    int inputSize = input.size();
    return (n > 0 && inputSize > n) ? input.substr(inputSize - n) : "";
}

bool checkResponse(string message){
    //If message (drop listing) contains "+OK ", true is returned, otherwise false.
    static std::regex rgx("\\+OK.*");
    if (std::regex_search(message, rgx)){
        return true;
    }
    return false;
}




































