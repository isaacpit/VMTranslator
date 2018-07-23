#include "Parser.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream> 
#include <algorithm>

using namespace std;

Parser::Parser(string file) {
    commands = {};

    input_helper(file, commands);
    clean_input(commands);
    // debug
    for (int i = 0; i < commands.size(); ++i) {
        cout << commands.at(i) << endl;
        cout << commandType(commands.at(i)) << endl;
        // cout << "args: " << args(commands.at(i)) << endl;
        args(commands.at(i));
    }
    if (commands.size() > 1) {
        // hasMoreCommands = true;
        // currentCommand  = 0;
        arg1 = "";
        arg2 = 0;
    }
}

void Parser::input_helper(string file, vector<string>& v) {
    ifstream ifs;
    string temp;

    ifs.open(file);
    if (ifs.eof()) {
        cout << "Error: could not open file";
        return;
    }
    
    while (!ifs.eof()) {
        getline(ifs, temp); 
        commands.push_back(temp);
    }
}

void Parser::clean_input(vector<string>& v) {
    vector<string>::iterator it = v.begin();
    bool go = true;
    bool rm = false;

    while (go) {
        for (int i = 0; i < it->size(); ++i) {
            if (it->at(i) == '/' && it->at(i) == '/') {
                v.erase(it);
                rm = true;
            }
        }
        if (it->length() <= 1) {
            v.erase(it);
            rm = true;
        }
        if (rm == false) {
            it = it + 1;
        }
        rm = false;
        if (it >= v.end()) {
            go = false;
        }
    }
}

string Parser::commandType(string cmd) {
    vector<string> keywords = {"push", "pop", "add", "sub", "eq", 
        "lt", "gt", "neg"};
    string arith = "C_ARITHMETIC";
    string push  = "C_PUSH";
    string pop   = "C_POP";
    string comp  = "C_COMP";
    string unary = "C_UNARY";
    string result = "";

    for (int i = 0; i < keywords.size(); ++i) {
        if (cmd.find(keywords.at(i)) != string::npos) {
            // cout << "found " << keywords.at(i) << " in here " << endl;

            if (i == 0) {
                result = push;
            }
            else if (i == 1) {
                result = pop;
            }
            else if (i == 2 || i == 3) {
                result = arith;
            }
            else if (i == 4 || i == 5 || i == 6) {
                result = comp;
            }
            else if (i == 7) {
                result = unary;
            }
            else {
                cout << "ERROR: '" << cmd << "' command did not match"
                    " any of the keywords." << endl;
            }
        }
    }

    return result;
}

void Parser::args(string cmd) {
    string result = "";
    unsigned int temp;
    unsigned int temp2;
    string arg1_substr = "";

    if (commandType(cmd).compare("C_ARITHMETIC") == 0 || 
        commandType(cmd).compare("C_UNARY") == 0 || 
        commandType(cmd).compare("C_COMP") == 0) {
        arg1 = cmd; 
        arg2 = -2000000000;
    }
    else if (cmd.find(' ') != string::npos) {
        // cout << "space at: " << cmd.find(' ') << endl;
        temp = cmd.find(' ');
        // cout << "space at : " << temp << endl;
        arg1_substr = cmd.substr(temp + 1, string::npos - temp);
        if (arg1_substr.find(' ') != string::npos) {
            temp2 = arg1_substr.find(' ');
            arg1 = arg1_substr.substr(0, temp2);
            arg2 = stoi((arg1_substr.substr(temp2 + 1, string::npos - temp2)));
        }
        // result = arg1_substr;
    }
    cout << "arg1: " << arg1 << endl;
    cout << "arg2: " << arg2 << endl;
    return;
}

/*****************************************************************/

Command::Command(string s) {
    
}