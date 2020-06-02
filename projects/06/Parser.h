#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
using namespace std;

class Parser
{
private:
    /* data */
    
    map<char, char> instructionTable;
    ifstream ifile;
public:
    Parser(string& inputfile);
    string currentInstruction;
    bool hasMoreCommands();
    void advance(unsigned long& lineNumber);
    char instructionType(unsigned long& lineNumber);
    string symbol();
    string dest();
    string comp();
    string jump();
};

