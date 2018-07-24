#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Parser.h"

using namespace std;
string convert_to_asm(string s) {
    string result = "";
    int i = 0;
    bool go = true;
    while (go) {
        if (s.at(i) == '.') {
            s.erase(s.begin() + i, s.end());
            go = false;
        }
        ++i;
    }
    result = s + ".asm";
    // cout << "result: " << result << endl;
    return result;
}

int main (int argc, char** argv) {
    string simple_add   = "SimpleAdd.vm";
    string stack_test   = "StackTest.vm";
    string basic_test   = "BasicTest.vm"; 
    string pointer_test = "PointerTest.vm";
    string static_test  = "StaticTest.vm";
    Parser p(argv[1]);
    
    string out_file = convert_to_asm(argv[1]);
    ofstream ofs;
    ofs.open(out_file);
    p.print_output(ofs);

    ofs.close();
    // ofs.open()
    return 0;
}

