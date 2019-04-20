#ifndef __PARSER_H__
#define __PARSER_H__

using namespace std;

#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"
#include <string>

/* This class reads in a MIPS assembly file and checks its syntax.  If
 * the file is syntactically correct, this class will retain a list
 * of Instructions (one for each instruction from the file).  This
 * list of Instructions can be iterated through.
 */


class Parser{
 public:

  Parser() {};

  virtual ~Parser() {};

  // Specify a text file containing MIPS assembly instructions. Function
  // checks syntactic correctness of file and creates a list of Instructions.
  Parser(string filename) {};

  // Returns true if the file specified was syntactically correct.  Otherwise,
  // returns false.
  virtual bool isFormatCorrect() = 0;

  // Iterator that returns the next Instruction in the list of Instructions.
  virtual Instruction getNextInstruction() = 0;

};

#endif
