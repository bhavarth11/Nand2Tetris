#include<iostream>
#include<bitset>
#include<fstream>
#include"SymbolTable.h"
#include"Parser.h"
#include"CodeTranslator.h"
using namespace std;

// VERY IMPORTANT: If the source file has extra line at the end of aseembly code, the output gives an error.


int main( int argc,char *argv[])
{   
    string inputFileName, outputFileName;
    ofstream ofile; //Create ofstream object

    // Get the input and output file names, and provide usage instructions
    //  if too few or too many arguments are provided.
    if (argc < 2 || argc > 3) {
        cout << "Usage: " << argv[0] << " <inputfilename.asm> <(optional) outputfilename.hack>" << endl;
        exit(1);
    }
    else {
        inputFileName = argv[1];
        outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + ".hack";

        if (argc == 3) {
            outputFileName = argv[2];
        }
    }

    //Create output file
    ofile.open(outputFileName);

    if (ofile.fail()) {
        cout << "Failed to create output file." << endl;
        exit(1);
    }

    Parser parse(inputFileName);
    SymbolTable symbolTable;

    unsigned long sourceLine = 0;
    int ROMLine = 0;


    //First Pass to add additional symbols to the symbol table

    while(parse.hasMoreCommands()) //Iterate till end of file is reached
    {
        parse.advance(sourceLine); //Skip white space and comments till next command is reached

        //if it is A instruction or C instruction increment ROMLine
        //Otherwise add label to the symbol table

        if (parse.instructionType(sourceLine) == 'A' || parse.instructionType(sourceLine) == 'C') {
            ROMLine++;
        }

        if (parse.instructionType(sourceLine) == 'L' && !symbolTable.contains(parse.symbol())) {
            symbolTable.addEntry(parse.symbol(), ROMLine);
        }

    }

    

    /*
     * Second pass: Assemble machine code
     *  With the symbol table generated, we read the file line by line once more,
     *  this time using the assemblySource parser. Since the L-instructions have been dealt with,
     *  we are only looking for A-instructions and C-instructions.
     *
     *  If an A-instruction is encountered, the address can either be a number,
     *  a predefined symbol, or a user defined variable. If the address is a number,
     *  the string is converted to its numeric representation in decimal,
     *  which is then converted to binary and output to the file.
     *  If the address is a symbol, the corresponding address is either
     *  retrieved if it is a predefined symbol, or generated if it is a user defined variable,
     *  and this address is then converted into binary and output to the file.
     *
     *  If a C-instruction is encountered, the destination, computation and jump mnemonics
     *  are converted into binary code using the translator, and the resulting bit string
     *  is output to the file.
     */
    Parser assemblySource(inputFileName);
    CodeTranslator translator;

    sourceLine = 0;   // Reset the line number for the error handling.
    int newAddress = 16;        // Predefined symbols occupy addresses 0-15.

    while (assemblySource.hasMoreCommands()) {

        assemblySource.advance(sourceLine);


        if (assemblySource.instructionType(sourceLine) == 'A') {
            ofile << '0';    // A-instructions always start with '0'.

            // Check if the symbol is a number.
            if (assemblySource.symbol().find_first_not_of("0123456789") == string::npos) {
                // Convert the string to a decimal number, convert the decimal number to a binary number.
                ofile << bitset<15>(stoull(assemblySource.symbol(), nullptr)).to_string();
            }
            else {
                // Check if the symbol is a variable.
                if (!symbolTable.contains(assemblySource.symbol())) {
                    symbolTable.addEntry(assemblySource.symbol(), newAddress++);
                }
                // Retrieve the address, and convert the decimal number to a binary number.
                ofile << bitset<15>(static_cast<unsigned long long>(symbolTable.getAddress(assemblySource.symbol()))).to_string();
            }
            ofile << endl;
        }
        else if (assemblySource.instructionType(sourceLine) == 'C') {
            ofile << "111";  // C-instructions always start with "111".
            ofile << translator.comp(assemblySource.comp(), sourceLine);
            ofile << translator.dest(assemblySource.dest(), sourceLine);
            ofile << translator.jump(assemblySource.jump(), sourceLine);
            ofile << endl;
        }
    }
    
    ofile.close();
    return 0;
}


