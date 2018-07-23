#include <string>
#include <vector>

using namespace std;

class Parser {
private:
    vector<string> commands;
    // bool hasMoreCommands;
    // int currentCommand;
    string arg1;
    int arg2;

public:
    Parser(string file);
    void input_helper(string file, vector<string>& v);
    void clean_input(vector<string>& v);
    string commandType(string cmd);
    void args(string cmd);
};

class Command {
private:
    string data;
    string commandType;
public: 
    Command(string s);

};