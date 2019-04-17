#ifndef __MACHLANGPARSER_H__
#define __MACHLANGPARSER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <bitset>
#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"

using namespace std;


// an enum to be passed to convertToInt method that
// dictates if the conversion should be signed or unsigned
enum SignFlag{
    UNSIGNED, // 0
    SIGNED    // 1
};

/* This class reads in a machine language representation of a MIPS
 * program.  If the file is correctly formatted (i.e. every line
 * contains a properly formatted 32 bit machine language/binary
 * representation of a supported MIPS instruction, this
 * class will retain a list of Instructions (one for each machine
 * language instruction from the file).  This list of Instructions
 * can be iterated through.
 */
class MachLangParser{
 public:

  // Specify a text file containing machine language/binary representations
  // of MIPS instructions.  This constructor checks the correctness of
  // each line of the file to see if it is a 32 bit binary sequence that
  // represents a supported MIPS instruction.  If it does, a list of
  // Instructions is created and stored internally.  If it does not, then
  // the class will indicate that the file is incorrectly formatted.
  //
  MachLangParser(string input_file);

  // Destructor
  ~MachLangParser();

  // Returns true if the file specified was a valid/correct MIPS instruction
  // file.  Otherwise, returns false.
  bool isFormatCorrect() { return myFormatCorrect; }

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

 private:
    // private container of Instruction objects
    vector<Instruction> myInstructions;

    // boolean saying whether format is correct
    bool myFormatCorrect;

    // iterator index to use with getNextInstruction
    int myIndex;

    // private instance of OpcodeTable for various methods
    OpcodeTable opcodes;

    // instance constants for use throughout program
    static const int OPCODE_LEN = 6;     // length of opcode
    static const int SHAMT_LEN = 5;      // width of shamt field
    static const int REG_WIDTH = 5;      // width of register bit sequence
    static const int J_ADDR_WIDTH = 26;  // width of address bit sequence
    static const int IMM_WIDTH = 16;     // width of immediate field
    static const int FUNCT_LEN = 6;      // number of bits of funct_field
    static const int ARCH_NUM_BITS = 32; // number of bits of architecture


    // takes in an input binary line and
    // checks to see if each of these conditions is matched:
    // 1.  length == ARCH_NUM_BITS
    // 2.  contains only 1s and 0s
    // Parameters:
    //  string i := the instruction whose fields need to be
    //                   initialized to their respective values
    //                   based on machine code encoding
    bool isLineCorrect(string line);


    // a method that takes in an Instruction object i by
    // reference, with an initialized encoding field that
    // is a binary string, and decodes it to figure out
    // its opcode, and registers, imm, shamt, and funct field,
    // as applicable
    // Parameters:
    //  Instruction i := the instruction whose fields need to be
    //                   initialized to their respective values
    //                   based on machine code encoding
    void decode(Instruction& i);


    // a decode helper method specific to R-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields need to be
    //                   initialized to their respective values
    //                   based on machine code encoding
    void decodeRType(Instruction& i);


    // a decode helper method specific to I-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields need to be
    //                   initialized to their respective values
    //                   based on machine code encoding
    void decodeIType(Instruction& i);


    // a decode helper method specific to J-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields need to be
    //                   initialized to their respective values
    //                   based on machine code encoding
    void decodeJType(Instruction& i);


    // Helper method that takes in a string of up to
    // ARCH_NUM_BITS bits, and treats it as either signed
    // unsigned based on the passed flag SignFlag f
    int convertToInt(string s, SignFlag f);


    // method that takes an instruction with all the
    // appropriate fields filled in, and puts together
    // an assembly instruction
    // Parameters:
    //  Instruction i := the instruction whose fields are initialized
    //                   to appropriate values and the instruction
    //                   needs to be converted to assembly
    void assemble(Instruction& i);


    // assemble helper method specific to R-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields are initialized
    //                   to appropriate values and the instruction
    //                   needs to be converted to assembly
    string assembleRType(Instruction i);


    // assemble helper method specific to I-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields are initialized
    //                   to appropriate values and the instruction
    //                   needs to be converted to assembly
    string assembleIType(Instruction i);


    // assemble helper method specific to J-type instructions
    // Parameters:
    //  Instruction i := the instruction whose fields are initialized
    //                   to appropriate values and the instruction
    //                   needs to be converted to assembly
    string assembleJType(Instruction i);


};


#endif
