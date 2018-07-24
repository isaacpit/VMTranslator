#include "Parser.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream> 
#include <algorithm>

using namespace std;

/****************************************************************************
 PARSER
****************************************************************************/

Parser::Parser(string file) {
    commands = {};
    input = {};

    input_helper(file, input);
    clean_input(input);
    // debug
    
    for (int i = 0; i < input.size(); ++i) {
        Command temp(input.at(i));
        vector<string> cmds;
        commands.push_back(temp);
        cout << commands.at(i);
        cmds = commands.at(i).convert_command(i);
        output.insert(output.end(), cmds.begin(), cmds.end());
        cout << endl;
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
        // Command temp2(temp); 
        // cout << temp2.data << endl;
        // commands.push_back(temp2);
        input.push_back(temp);
    }
    ifs.close();
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

void Parser::print_output(ostream& out) {
    for (auto it : output) {
        out << it << endl;
    }
}

/****************************************************************************
 COMMAND
****************************************************************************/

Command::Command(string s) 
    : data(s) 
{
    command_id();
    args();
}

string Command::command_id() {
    vector<string> keywords = {"push", "pop", "add", "sub", "eq", 
        "lt", "gt", "neg", "or", "and", "not"};
    string arith = "C_ARITHMETIC";
    string push  = "C_PUSH";
    string pop   = "C_POP";
    string comp  = "C_COMP";
    string unary = "C_UNARY";
    string logical = "C_LOGICAL";
    string result = "";

    for (int i = 0; i < keywords.size(); ++i) {
        if (data.find(keywords.at(i)) != string::npos) {
            // cout << "found " << keywords.at(i) << " in here " << endl;
            if (i == 0) {
                // push
                command_type = push;
            }
            else if (i == 1) {
                // pop
                command_type = pop;
            }
            else if (i == 2 || i == 3) {
                // add || sub
                command_type = arith;
            }
            else if (i == 4 || i == 5 || i == 6) {
                // eq || lt || gt
                command_type = comp;
            }
            else if (i == 7 || i == 10) {
                // neg || not
                command_type = unary;
            }
            else if (i == 8 || i == 9) {
                // and || or 
                command_type = logical;
            }
            else {
                cout << "ERROR: '" << data << "' command did not match"
                    " any of the keywords." << endl;
            }
        }
    }
    return command_type;
}

void Command::args() {
    string temp_s = "";
    unsigned int temp;
    unsigned int temp2;
    string arg1_substr = "";

    if (command_id().compare("C_ARITHMETIC") == 0 || 
        command_id().compare("C_UNARY") == 0 || 
        command_id().compare("C_COMP") == 0 ||
        command_id().compare("C_LOGICAL") == 0) {
        arg1 = data.substr(0, data.length() - 1); // removes space
        arg2 = { -1 };
    }
    else if (data.find(' ') != string::npos) {
        // cout << "space at: " << cmd.find(' ') << endl;
        temp = data.find(' ');
        // cout << "space at : " << temp << endl;
        arg1_substr = data.substr(temp + 1, string::npos - temp);
        // cout << "arg1_substr: " << arg1_substr << endl;
        if (arg1_substr.find(' ') != string::npos) {
            temp2 = arg1_substr.find(' ');
            arg1 = arg1_substr.substr(0, temp2);
            temp_s = arg1_substr.substr(temp2 + 1, string::npos - temp2);
            // cout << "arg2? " << temp_s << endl;
            arg2 = stoi(temp_s);
        }
        // result = arg1_substr;
    }
    // cout << "arg1: " << arg1 << endl;
    // cout << "arg2: " << arg2 << endl;
    return;
}

ostream& operator<<(ostream& out, Command c) {
    // out << "command: " << c.data << endl;
    // out << "type   : " << c.get_command_type() << endl;
    // out << "arg1   : " << c.get_arg1() << endl;
    // out << "arg2   : " << c.get_arg2() << endl;
    c.print_cmd();
    return out;
}

void Command::print_cmd() {
    cout << "command: " << data << endl;
    cout << "type   : " << get_command_type() << endl;
    cout << "arg1   : " << get_arg1() << endl;
    cout << "arg2   : " << get_arg2() << endl;
}

vector<string> Command::convert_command(int pos) {
    vector<string> v;
    cout << "arg1: " << arg1 << endl;
    if      (get_command_type().compare("C_PUSH")       == 0) {
        v = convert_push(v);
    }
    else if (get_command_type().compare("C_POP")        == 0) {
        v = convert_pop(v);
    }
    else if (get_command_type().compare("C_ARITHMETIC") == 0) {
        v = convert_arithmetic(v);
    }
    else if (get_command_type().compare("C_COMP")       == 0) {
        v = convert_comp(v, pos);
    }
    else if (get_command_type().compare("C_LOGICAL")    == 0) {
        v = convert_logical(v);
    }
    else if (get_command_type().compare("C_UNARY")      == 0) {
        v = convert_unary(v);
    }

    // debug
    for (auto it : v) {
        cout << it << endl;
    }
    return v;
}

vector<string> Command::convert_push(vector<string>& v) {
    string dest;
    if      (arg1.compare("static") == 0) {
        v.push_back("\t@16");
        v.push_back("\tD = A");
        v.push_back("\t@" + to_string(arg2));
        v.push_back("\tA = D + A");
        v.push_back("\tD = M");
        v.push_back("\t@SP");
        v.push_back("\tAM = M + 1");
        v.push_back("\tA = A - 1");
        v.push_back("\tM = D");
    }
    else if (arg1.compare("pointer") != 0) {
        if      (arg1.compare("constant") == 0) {
            v = push_constant(v);
            return v;
        }
        else if (arg1.compare("local") == 0) {
            cout << "push local!" << endl;
            v.push_back("\t@LCL");
        }
        else if (arg1.compare("argument") == 0) {
            cout << "push argument!" << endl;
            v.push_back("\t@ARG");
        }
        else if (arg1.compare("this") == 0) {
            cout << "push this!" << endl;
            v.push_back("\t@THIS");
        }
        else if (arg1.compare("that") == 0) {
            cout << "push that!" << endl;
            v.push_back("\t@THAT");
        }
        else if (arg1.compare("temp") == 0) {
            cout << "push temp!" << endl;
            v.push_back("\t@5");
            v.push_back("\tD = A");
        }

        if (arg1.compare("temp") != 0) 
            v.push_back("\tD = M");

        v.push_back("\t@" + to_string(arg2));
        v.push_back("\tA = D + A");
        v.push_back("\tD = M");
        v.push_back("\t@SP");
        v.push_back("\tM = M + 1");
        v.push_back("\tA = M - 1");
        v.push_back("\tM = D");
    }
    else if (arg1.compare("pointer") == 0) {
        if      (arg2 == 0) {
            v.push_back("\t@THIS");
        }
        else if (arg2 == 1) {
            v.push_back("\t@THAT");
        }
        v.push_back("\tD = M");
        v.push_back("\t@SP");
        v.push_back("\tAM = M + 1");
        v.push_back("\tA = A - 1");
        v.push_back("\tM = D");
    }

    return v;
}

vector<string> Command::push_constant(vector<string>& v) {
    v.push_back("\t@" + to_string(arg2));
    v.push_back("\tD = A");
    v.push_back("\t@SP");
    v.push_back("\tA = M");
    v.push_back("\tM = D");
    v.push_back("\t@SP");
    v.push_back("\tM = M + 1");
    return v;
}

vector<string> Command::convert_pop(vector<string>& v) {

    if      (arg1.compare("static") == 0) {
        int loc = 16 + arg2;

        v.push_back("\t@SP");
        v.push_back("\tAM = M - 1");
        v.push_back("\tD = M");
        v.push_back("\t@" + to_string(loc));
        v.push_back("\tM = D");
    }
    else if (arg1.compare("pointer") != 0) {
        v.push_back("\t@SP");
        v.push_back("\tAM = M - 1");
        v.push_back("\tD = M");
        v.push_back("\t@13");
        v.push_back("\tM = D");
        if      (arg1.compare("local") == 0) {
            cout << "pop local!" << endl;
            v.push_back("\t@LCL");
            v.push_back("\tD = M");
        }
        else if (arg1.compare("argument") == 0) {
            cout << "pop argument!" << endl;
            v.push_back("\t@ARG");
            v.push_back("\tD = M");
        }
        else if (arg1.compare("this") == 0) {
            cout << "pop this!" << endl;
            v.push_back("\t@THIS");
            v.push_back("\tD = M");
        }
        else if (arg1.compare("that") == 0) {
            cout << "pop that!" << endl;
            v.push_back("\t@THAT");
            v.push_back("\tD = M");
        }
        else if (arg1.compare("temp") == 0) {
            cout << "pop temp!" << endl;
            v.push_back("\t@5");
            v.push_back("\tD = A");
        }
        v.push_back("\t@" + to_string(arg2));
        v.push_back("\tD = D + A");
        v.push_back("\t@14");
        v.push_back("\tM = D");
        v.push_back("\t@13");
        v.push_back("\tD = M");
        v.push_back("\t@14");
        v.push_back("\tA = M");
        v.push_back("\tM = D");
    }
    else if (arg1.compare("pointer") == 0) {
        v.push_back("\t@SP");
        v.push_back("\tAM = M - 1");
        v.push_back("\tD = M");
        if      (arg2 == 0) {
            v.push_back("\t@THIS");
        }
        else if (arg2 == 1) {
            v.push_back("\t@THAT");
        }
        v.push_back("\tM = D");
    }



    return v;
}

vector<string> Command::convert_arithmetic(vector<string>& v) {
    v.push_back("\t@SP");
    v.push_back("\tAM = M - 1");
    v.push_back("\tD = M");
    v.push_back("\tA = A - 1");
    cout << "this is arg1: " << arg1 << endl;
    // cout << arg1.length() << endl;
    if      (arg1.compare("add") == 0) {
        v.push_back("\tM = M + D");
    }
    else if (arg1.compare("sub") == 0) {
        v.push_back("\tM = M - D");
    }


    return v;
}

vector<string> Command::convert_comp(vector<string>& v, int pos) {
    string upper_arg1 = arg1;
    transform(upper_arg1.begin(), upper_arg1.end(), upper_arg1.begin(), ::toupper);
    cout << "upper_arg1 : " << upper_arg1 << endl;
    v.push_back("\t@SP");

    v.push_back("\tAM = M - 1");
    v.push_back("\tD = M");
    v.push_back("\t@SP");
    v.push_back("\tAM = M - 1");
    v.push_back("\tD = M - D");
    v.push_back("\t@" + upper_arg1 + to_string(pos));

    if      (arg1.compare("eq") == 0) {
        v.push_back("\tD; JEQ");
    }
    else if (arg1.compare("gt") == 0) {
        v.push_back("\tD; JGT");
    }
    else if (arg1.compare("lt") == 0) {
        v.push_back("\tD; JLT");
    }
    v.push_back("\t@SP");
    v.push_back("\tA = M");
    v.push_back("\tM = 0");
    v.push_back("\t@END" + to_string(pos));
    v.push_back("\t0; JMP");

    if      (arg1.compare("eq") == 0) {
        v.push_back("(EQ" + to_string(pos) + ")");
    }
    else if (arg1.compare("gt") == 0) {
        v.push_back("(GT" + to_string(pos) + ")");
    }
    else if (arg1.compare("lt") == 0) {
        v.push_back("(LT" + to_string(pos) + ")");
    }
    
    v.push_back("\t@SP");
    v.push_back("\tA = M");
    v.push_back("\tM = -1");
    v.push_back("(END" + to_string(pos) + ")");
    v.push_back("\t@SP");
    v.push_back("\tM = M + 1");
    return v;
}

vector<string> Command::convert_logical(vector<string>& v) {
    v.push_back("\t@SP");
    v.push_back("\tAM = M - 1");
    v.push_back("\tA = A - 1");
    if      (arg1.compare("and") == 0) {
        v.push_back("\tM = D & M");
    }
    else if (arg1.compare("or") == 0) {
        v.push_back("\tM = D | M");
    }

    return v;
}

vector<string> Command::convert_unary(vector<string>& v) {
    v.push_back("\t@SP");
    v.push_back("\tA = M - 1");
    if      (arg1.compare("neg") == 0) {
        v.push_back("\tD = - M");
    }
    else if (arg1.compare("not") == 0) {
        v.push_back("\tD = ! M");
    }
    v.push_back("\tM = D");

    return v;
}