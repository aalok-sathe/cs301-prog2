#ifndef __OPCODE_H__
#define __OPCODE_H__

using namespace std;

#include <iostream>
#include <string>
#include <bitset>


// Listing of all supported MIPS instructions
enum Opcode{
    ADD,  // 0
    ADDI, // 1
    SUB,  // 2
    MULT, // 3
    MFHI, // 4
    SRL,  // 5
    SRA,  // 6
    SLTI, // 7
    LW,   // 8
    J,    // 9
    BEQ,  // 10
    UNDEFINED
};


// Different types of MIPS encodings
enum InstType{
  RTYPE,
  ITYPE,
  JTYPE
};


// Different types of MIPS instructions based on their function
enum InstFunc{
    ARITHM,
    MEMORY,
    CONTROL
};


/* This class represents templates for supported MIPS instructions.  For every supported
 * MIPS instruction, the OpcodeTable includes information about the opcode, expected
 * operands, and other fields.
 */
class OpcodeTable
{
    public:
        // Initializes all the fields for every instruction in Opcode enum
        OpcodeTable();

        // Given a valid MIPS assembly mnemonic, returns an Opcode which represents a
        // template for that instruction.
        Opcode getOpcode(string str);

        // Given an Opcode, returns number of expected operands.
        int numOperands(Opcode o);

        // Given an Opcode, returns the position of RS field.  If field is not
        // appropriate for this Opcode, returns -1.
        int RSposition(Opcode o);

        // Given an Opcode, returns the position of RT  field.  If field is not
        // appropriate for this Opcode, returns -1.
        int RTposition(Opcode o);

        // Given an Opcode, returns the position of RD field.  If field is not
        // appropriate for this Opcode, returns -1.
        int RDposition(Opcode o);

        // Given an Opcode, returns the position of IMM field.  If field is not
        // appropriate for this Opcode, returns -1.
        int IMMposition(Opcode o);

        // Given an Opcode, returns true if instruction expects a label in the instruction.
        // See "J".
        bool isIMMLabel(Opcode o);

        // Given an Opcode, returns instruction type.
        InstType getInstType(Opcode o);

        // Given an Opcode, returns a string representing the binary encoding of the opcode
        // field.
        string getOpcodeField(Opcode o);

        // Given an Opcode, returns a string representing the binary encoding of the function
        // field.
        string getFunctField(Opcode o);


        // Given a string of opcode bits and a string of the funct field
        // (if applicable), returns the Opcode (enum member) corresponding
        // to the instruction determined by that opcode and funct field values
        Opcode getInstr(string opc, string funct);


        // Given a valid Opcode (enum member) o, returns
        // the string name of the instruction corresponding
        // to it
        string getName(Opcode o);


        // Given a valid Opcode (enum member) o, returns the
        // InstFunc of the corresponding instruction, saying what
        // function the instruction performs (MEMORY/CONTROL/ARITHM)
        InstFunc getInstFunc(Opcode o);


    private:

        // Provides information about how where to find values in a MIPS assembly
        // instruction and what pre-defined fields (opcode/funct) will be in
        // the encoding for the given instruction.
        struct OpcodeTableEntry
        {
            string name;
            int numOps;
            int rdPos;
            int rsPos;
            int rtPos;
            int immPos;
            bool immLabel;

            InstType instType;
            InstFunc instFunc;
            string op_field;
            string funct_field;

            // Creates an initial OpcodeTableEntry with default values
            OpcodeTableEntry()
            {
                numOps = 0;
                rdPos = rsPos = rtPos = immPos = -1;
                immLabel = false;
                instFunc = ARITHM; // basic kind of instruction function
            };
        };

        // The array of OpcodeTableEntries, one for each MIPS instruction supported
        OpcodeTableEntry myArray[UNDEFINED];

        static const int OPCODE_LEN = 6;

};


#endif
