#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Command;
ostream& operator<<(ostream& out, Command c);

class Parser {
private:
    vector<string> input;
    vector<Command> commands;
    vector<string> output;
    // bool hasMoreCommands;
    // int currentCommand;
    // string arg1;
    // int arg2;

public:

    Parser(string file);
    void input_helper(string file, vector<string>& v);
    void clean_input(vector<string>& v);
    void print_output(ostream& out);
    // string command_type(string cmd);
    // void args(string cmd);
};


class Command {
private:
    string command_type;
    string arg1; 
    int    arg2;
public: 
    string data;
    Command(string s);
    string command_id();
    void args();
    void set_data(string d) { data = d; }
    void set_command_type(string ct) { command_type = ct; }
    void set_arg1(string arg) { arg1 = arg; }
    void set_arg2(int arg) { arg2 = arg; }
    int length() { return data.length(); }

    string get_command_type() { return command_type;}
    string get_arg1()         { return arg1; }
    int    get_arg2()         { return arg2; }

    void print_cmd();

    vector<string> convert_command(int pos);

    vector<string> convert_push(vector<string>& v);
    vector<string> push_constant(vector<string>& v);

    vector<string> convert_pop(vector<string>& v);


    vector<string> convert_arithmetic(vector<string>& v);

    vector<string> convert_comp(vector<string>& v, int pos);

    vector<string> convert_logical(vector<string>& v);

    vector<string> convert_unary(vector<string>& v);
};
