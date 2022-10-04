/*
* Project: POP3 Client
* File: args.hpp
* Author: Tomas Tomala (xtomal02)
*/


#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED

#include <string.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector> //vector
#include <algorithm> //find
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream> //ifstream
#include <regex>

using namespace std;

struct arguments {
        string server = "";
        int p = -1;
        bool d = false;
        bool n = false;
        bool T = false;
        bool S = false;
        string c = "";
        string C = "";
        string a = "";
        string o = "";
};

struct authInfo {
        string username = "";
        string password = "";
};

//src: https://java2blog.com/check-if-string-is-number-cpp/
//Checks if string is positive number
bool isNumber(const string& s);

//Checks if string is in vector
//src: https://stackoverflow.com/a/20303915
bool in_array(const string &value, const vector<string> &array);

//Gets arguments from command line and checks validity of them
arguments argsParsing(int argc, char *argv[]);

//Checks if files <auth_file> and <certfile> exist
//Inspired by: https://stackoverflow.com/a/12774387

bool checkFileExist(const string name);

//Check if directories <out_dir> and <certaddr> exist
//Inspired by: https://stackoverflow.com/a/18101042 and https://stackoverflow.com/a/2341857
bool checkDirectories(const string name);

//Retrieves authentification info from <authfile>
//Some lines inspired by: https://stackoverflow.com/a/7868986
authInfo getAuthorizationInfo(arguments *args);
#endif
