#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <string>
#include <iostream>

using namespace std;

/* The stages in the pipeline in the order they appear in the datapath */
enum PipelineStages{
  FETCH,
  DECODE,
  EXECUTE,
  MEMORY,
  WRITEBACK,
  NUM_STAGES
};

/* This class simulates an ideal pipeline, executing the instructions
 * from an input file on an ideal pipeline and printing out information
 * about RAW dependences, the completion times of the instructions in
 * the order they appear in the file, and the total execution time for
 * the instructions from the file.
 */
class Pipeline{
 public:

  /* The constructor is passed either the name of a MIPS assembly file or
   * the name of a MIPS machine instruction file.  If the input file is
   * not a valid input file (due to syntactical errors or unsupported
   * instructions being specified), the format is considered incorrect.
   */
  Pipeline(string inputFile);

  /* TODO Destructor docstring
   */
  ~Pipeline();

  /* Executes the instructions passed into the constructor.  The method
   * determines the dependences and calculates the completion time of each
   * instruction and the overall execution time for the set of instructions,
   * storing that information for later printing.
   */
  void execute();

  /* Prints the pipeline time, prints the RAW dependences found in the
   * instructions, prints each instruction with its completion time, and
   * prints the total execution time.
   */
  void print();

  /* Returns true if the input file was syntactically correct and only
   * contained supported instructions.  Otherwise, returns false.
   */
  bool isFormatCorrect();

private:

    bool myFormatCorrect;

};

#endif
