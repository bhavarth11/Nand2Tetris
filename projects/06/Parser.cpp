#include"Parser.h"

Parser::Parser(string& inputfile)
{
    ifile.open(inputfile); //Read input file
    
    if (ifile.fail()) {      //If file not found exit
        cout << inputfile << " not found." << endl;
        exit(1);
    }

     // Populate the instruction map table.
    instructionTable['@'] = 'A';
    instructionTable['A'] = 'C';
    instructionTable['D'] = 'C';
    instructionTable['M'] = 'C';
    instructionTable['0'] = 'C';
    instructionTable['1'] = 'C';
    instructionTable['-'] = 'C';
    instructionTable['!'] = 'C';
    instructionTable['('] = 'L';

}

bool Parser::hasMoreCommands()
{
    return !ifile.eof();
}

void Parser::advance(unsigned long& lineNumber)
{
    string line;
    bool instructionFound = false;
    long commentpos;

    while(!instructionFound && getline(ifile, line)) {

        // Increment the line number corresponding to the source file,
        //  used for error tracking.
        lineNumber++;

        //Removing whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); 

        //Removing comments
        commentpos = line.find("//");
        if(commentpos!=string::npos) {
            line.erase(commentpos, line.length());
        }
        // else  cout<<line<<endl;
        //If line is not empty means instruction is found
        instructionFound = !line.empty();

    }

    currentInstruction = line;
}


char Parser::instructionType(unsigned long& lineNumber)
{
    //If valid command is found in instruction table, return type of command corresponding to first character of instruction
    if(instructionTable.find(currentInstruction[0]) != instructionTable.end())
    {
        return instructionTable[currentInstruction[0]];
    }

    // If an invalid command/syntax is found, output an error message and line number.
    cout<<"Invalid command at line "<<lineNumber;
    exit(1);
}

string Parser::symbol() {
    unsigned long openBracketPos, closeBracketPos;

    openBracketPos = currentInstruction.find('(');
    closeBracketPos = currentInstruction.find(')');

    // A-instruction: return everything after the '@' character.
    if (currentInstruction[0] == '@') {
        return currentInstruction.substr(1, currentInstruction.length() - 1);
    }
    // L-instruction: return everything in between the '(' and ')' characters.
    else if (openBracketPos != string::npos && closeBracketPos != string::npos) {
        return currentInstruction.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1);
    }

    // If the function was called in error, return a blank string.
    return "";
}

string Parser::dest() {
    unsigned long equalSignPos;

    equalSignPos = currentInstruction.find('=');

    // Return everything before the '=' character.
    if (equalSignPos != string::npos) {
        return currentInstruction.substr(0, equalSignPos);
    }

    // If no destination was specified, return a blank string.
    return "";
}

string Parser::comp() {
    unsigned long equalSignPos, semiColonPos;

    equalSignPos = currentInstruction.find('=');
    semiColonPos = currentInstruction.find(';');

    // Return the computation mnemonic based on three cases.
    if (equalSignPos != string::npos) {
        if (semiColonPos != string::npos) {
            // Case 1: dest = comp ; jump
            return currentInstruction.substr(equalSignPos + 1, semiColonPos - equalSignPos - 1);
        }
        // Case 2: dest = comp
        return currentInstruction.substr(equalSignPos + 1, currentInstruction.length() - equalSignPos - 1);
    }
    else if (semiColonPos != string::npos) {
        // Case 3: comp ; jump
        return currentInstruction.substr(0, semiColonPos);
    }

    // If no computation was specified, return a blank string.
    // This will result in an error message at the line number.
    // The error is handled by CodeTranslator.comp().
    return "";
}

string Parser::jump() {
    unsigned long semiColonPos;

    semiColonPos = currentInstruction.find(';');

    // Return everything after the ';' character.
    if (semiColonPos != string::npos) {
        return currentInstruction.substr(semiColonPos + 1, currentInstruction.length() - semiColonPos - 1);
    }

    // If a jump was not specified, return a blank string.
    return "";
}