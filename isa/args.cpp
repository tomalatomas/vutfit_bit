#include "args.hpp"

bool checkFileExist(const string name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

string checkDirectoryExist(const string name){
    struct stat info;
    if( stat(name.c_str(), &info ) != 0 )
        return ""; // cannot access
    else if( info.st_mode & S_IFDIR ){
        char resolvedPath[4096]; 
        realpath(name.c_str(), resolvedPath); 
        return resolvedPath;
    } else
        return ""; // is not directory
}

bool isNumber(const string& s)
{
    for (char const &ch : s) {
        if (isdigit(ch) == 0) 
            return false;
    }
    return true;
}

bool in_array(const string &value, const vector<string> &array)
{
    return find(array.begin(), array.end(), value) != array.end();
}

authInfo getAuthorizationInfo(arguments *args){
    string filename = args->a;
    authInfo auth;     

    ifstream input(filename);
    for( string line; getline( input, line );){
        regex rgxUsername("username = (\\w+).*");
        regex rgxPassword("password = (\\w+).*");
        smatch match;
        //Searching for username and password on first two lines
        if (regex_search(line,match, rgxUsername)){
            if(match[1].compare("") == 0){
                cerr << "Username in <authfile> is not specified \n";
                exit(1);
            }
            auth.username = match[1];
            if (auth.password.compare("") != 0 && auth.username.compare("") != 0){
                //If password and username are retrieved, no point in continuing -> breaking out of for loop
                break;
            }
        } else if (regex_search(line,match, rgxPassword)){
            if(match[1].compare("") == 0){
                cerr << "Password in <authfile> is not specified \n";
                exit(1);
            }
            auth.password = match[1];
            if (auth.password.compare("") != 0 && auth.username.compare("") != 0){
                //If password and username are retrieved, no point in continuing -> breaking out of for loop
                break;
            }
        } else {
            cerr << "Invalid <authfile> format! \n";
            exit(1);
        }
    }
    //Checking authInfo
    if (auth.password.compare("") == 0 || auth.username.compare("") == 0){
        cerr << "Password or Username not found in <authfile>! \n";
    }
    return auth;
}

arguments argsParsing(int argc, char *argv[]) {
    vector<string> specifyingArgs = {"-p", "-c", "-C", "-a", "-o"};
    arguments args;     
    //Checking for help argument
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[1], "-h") || !strcmp(argv[i], "--help") ){
            cout << "Usage: popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>\n";
			exit(0);
        }
    }

    if (argc > 16) {
        cerr << "Invalid arguments! Argument \"-h\" specifies usage\n";
        exit(1);
    }
    //For loop parsing arguments one by one
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-d")){
            if (args.d){
                cerr << "Duplicate argument \"-d\"!Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.d = true;
        } else if(!strcmp(argv[i], "-n")){
            if (args.n){
                cerr << "Duplicate argument \"-n\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.n = true;
        } else if(!strcmp(argv[i], "-T")){
            if (args.T){
                cerr << "Duplicate argument \"-T\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.T = true;
        } else if(!strcmp(argv[i], "-S")){
            if (args.S){
                cerr << "Duplicate argument \"-S\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.S = true;
        } else if(!strcmp(argv[i], "-p")){
            if(i+1 > argc-1){
                cerr << "Unable to parse  argument\"-p\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            string optarg = argv[i+1]; // Check if optarg is positive number
            if(!isNumber(optarg)){
                cerr << "Port argument \"-p\" requires argument in form of positive integer! Argument \"-h\" specifies usage\n";
                exit(1);
            }     
            args.p = stoi(optarg);       
        } else if(!strcmp(argv[i], "-a")){
            if(i+1 > argc-1){
                cerr << "Unable to parse -a argument\"-S\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.a = argv[i+1];
        } else if(!strcmp(argv[i], "-o")){
            if(i+1 > argc-1){
                cerr << "Unable to parse -o argument\"-S\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.o = argv[i+1];
        } else if(!strcmp(argv[i], "-c")){
            if(i+1 > argc-1){
                cerr << "Unable to parse -c argument\"-S\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.c = argv[i+1];
        } else if(!strcmp(argv[i], "-C")){
            if(i+1 > argc-1){
                cerr << "Unable to parse -C argument\"-S\"! Argument \"-h\" specifies usage\n";
                exit(1);
            }
            args.C = argv[i+1];
        }else { //Getting possible server domain name or IP
            if(!in_array(argv[i-1],specifyingArgs)){
                //Checks if argv[i] is optarg of another option
                if(args.server.compare("") == 0){
                    args.server = argv[i];
                } else {
                    cerr << "Invalid arguments! Argument \"-h\" specifies usage\n";
                    //Duplicate options for server or invalid argument
                    exit(1);
                }
            }
        }
    }

    //Checking validity of args
    if(args.T && args.S){
        cerr << "Invalid combinations of arguments T and S! Argument \"-h\" specifies usage\n";
        exit(1);
    }

    //Checking if authfile is specified
    if(args.a.compare("") == 0){
        cerr << "Argument \"-a\" is required with specified authentication file! Argument \"-h\" specifies usage\n";        
        exit(1);
    }
    //Checking if authfile exists
    if(!checkFileExist(args.a)){
        cerr << "Could not find specified <authfile>" << args.a << endl;
        exit(1);
    }
    //Checking if outdir is specified
    if(args.o.compare("") == 0){
        cerr << "Argument \"-o\" is required with address to output directory! Argument \"-h\" specifies usage\n";
        exit(1);
    }
    args.o = checkDirectoryExist(args.o);
    if(args.o  == ""){
        cerr << "Cannot access output directory!" << endl;
        exit(1);
    }

    //Checking if outdir exists
    if(!checkFileExist(args.o)){
        cerr << "Could not access <outdir>" << args.o << endl;
        exit(1);
    }
    //Checking if server address is specified
    if(args.server.compare("") == 0){
        cerr << "Server address is required! Argument \"-h\" specifies usage\n";
        exit(1);
    }

    if(args.c.compare("") != 0){
        if(!checkFileExist(args.c)){
            cerr << "Could not find specified <certfile> "<< args.c <<endl;
            exit(1);
        }
    }
    
    if(args.C.compare("") != 0){
        if(!checkFileExist(args.C)){
            cerr << "Could not access <certdir> "<< args.C <<endl;
            exit(1);
        }
    }

    if(args.p == -1){
        //If SSL connection is required, use port 995, otherwise 110
        args.T ? args.p=995 : args.p=110;
    } else {
        if(args.p > 65535){ //Port is not valid
            cerr << "Invalid port number: "<< args.p << endl;
            exit(1);
        }
    }
    //Returning filled out struct
    return args;
}
