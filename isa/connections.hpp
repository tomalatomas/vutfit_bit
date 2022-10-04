
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "args.hpp"

#define BUFSIZE 1024


struct connections {
        BIO* bio;   
        //Pointer to hold SSL Connection structure
        SSL *ssl;
        SSL_CTX *ctx;
};

/* connectToServer
Establishes connection for secured and unsecured communication based on arguments, return structure connections 
with initialized pointers to objects needed for connection. For secured connection calls method initSecureComm to 
initialize OpenSSL library
Inspired by:    https://developer.ibm.com/tutorials/l-openssl/
                https://gist.github.com/CarlEkerot/94ddbdc5baf4e53157e8
                https://stackoverflow.com/questions/49132242/openssl-promote-insecure-bio-to-secure-one  
                https://github.com/warmlab/study/blob/0e8ea25b79c4b7edfd0c46bc1ca80d7345b56230/openssl/ssl_server.c#L87 
*/
connections connectToServer(arguments args);


/* initSecureComm
Initializes OpenSSL library, structures and errors. Creates context for SSL pointer 
and loads trust store with certificates
*/
void initSecureComm(connections *conns, arguments args);


/* readResponse
Method reads via BIO object response from server to a buffer. Buffer is size of 1024 to be enough for the response of server, 
which are maximum size of 512. Method reads the respons, till message terminator is found. Message terminator is declared by parameters
of the message. After every BIO_read, buffer is appended to string and emptied.
Inspired by:    https://developer.ibm.com/tutorials/l-openssl/
                https://gist.github.com/CarlEkerot/94ddbdc5baf4e53157e8
*/
string readResponse(BIO* bio, string messageTerminator);


/* lastN
Returns last n characters from end of the input
Source: https://stackoverflow.com/a/49331690
*/
string lastN(string input, int n);


/* checkMessageTermination
Method checks, if substring from end of the message matches with the message terminator.
True is returned, if message end matches with the message terminator
*/
bool checkMessageTermination(string message, string messageTerminator);


/* checkResponse
With help of regular expression, method checks if response (drop listing) contains +OK at the begging or something else. 
True is returned if response is +OK
*/
bool checkResponse(string message);


/* authorization
After succesful connection, client sends commands USER and PASS to server with username and password information.
*/
void authorization(authInfo auth, BIO* bio);


/* writeCommand
Method takes parameter, appends command terminator to it and sends command to server via BIO_write method. 
Returns true if command was send without error
Inspired by: https://developer.ibm.com/tutorials/l-openssl/

*/
bool writeCommand(BIO* bio, string message);


/* getAllMessages
First client sends STAT command to retrieve information about the mailbox. Received response contains information about number of messages.
For number of messages, client sends command RETR to retrieve every single one message. To process the message method processMessage is called.
After every message is read and processed, number of downloaded messages is printed to the user
*/
void getAllMessages(BIO* bio, arguments args);


/* getMessageCount
Returns number of messages in mailbox from STAT response
*/
int getMessageCount(string stat);


/* processMessage
Method calls methods to remove redundant information like bytestuffing, response status,, message terminator
MessageID and Subject is then retrieved from the message. With help of external file we can establish if message is new or not. 
Message is then saved to a file with subject as a name. Also method to send DEL command to delete the message from server may be called.
*/
bool processMessage(BIO* bio, string message, arguments args, int index);


/* removeBytestuffing
Method serches with help of regular expression for CRLF.. in multiline respone. If bytestuffing is found, again with help of regex,
bytestuffing is removed from the message
Returns true if message was saved to a file
*/
string removeBytestuffing(string message);


/* removeResponseStatus
Response status, which indicates if response is ok or not, is removed from the message. If respons status is not +OK, message is not processed.
*/
string removeResponseStatus(string message);


/* getMessageID
With help of regular expression, message id is found and extracted from message. MessageID will be used to determine, if message is new.
Returns messageid of message
*/
string getMessageID(string message);


/* getSubject
With help of regular expression, subject of message is found and extracted from message. Subject will be used to name a file with message.
Returns subject of message
*/
string getSubject(string message);


/* removeMessageEnd
With help of regular expression, message end is removed from message. Message terminator is CRLF.CRLF, but only .CRLF is removed.
Returns message without message end
*/
string removeMessageEnd(string message);


/* checkNovelty
Method checks external file for occurence of messageid. If file contains messageid, message that is being processed is not new. 
Returns true, if message is new
Inspired by: https://stackoverflow.com/a/2393389
*/
bool checkNovelty(string messageID);


/* saveMessage
Methor retrieves subject from message via getSubject method, removes unwanted characters from subject and saves the message to a file named as the subject.
*/
void saveMessage(string message, arguments args);


/* deleteMessage
If message is to be deleted, client sends QUIT command to server.
Returns true if message was deleted
*/
bool deleteMessage(BIO* bio, int index);


/* quitConnection
Clients issus QUIT command to finish the communication and to put the server into UPDATE phase.
*/
void quitConnection(BIO* bio);

