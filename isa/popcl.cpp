
#include "args.hpp"
#include "connections.hpp"

using namespace std;


int main(int argc, char *argv[]) {
    //Parses arguments
    arguments args = argsParsing(argc, argv);
    //Retrives authorization info from authfile
    authInfo auth = getAuthorizationInfo(&args);
    //establishes connection to specified server
    connections conns = connectToServer(args);
    //Authorization of user 
    authorization(auth, conns.bio);
    //Retrieve all messages
    getAllMessages(conns.bio, args);
    //End of connection
    quitConnection(conns.bio);
    return 0;
}