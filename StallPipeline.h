#ifndef _STALL_PIPELINE_H_
#define _STALL_PIPELINE_H_

#include "Pipeline.h"

/* The stages in the pipeline in the order they appear in the datapath */
// TODO
// enum StallPipelineStages{
//   FETCH,
//   DECODE,
//   EXECUTE,
//   MEMORY,
//   WRITEBACK,
//   NUM_STAGES
// };

/* This class simulates an ideal pipeline, executing the instructions
 * from an input file on an ideal pipeline and printing out information
 * about RAW dependences, the completion times of the instructions in
 * the order they appear in the file, and the total execution time for
 * the instructions from the file.
 */
class StallPipeline: public Pipeline{
 
    
 public:

  /* The constructor is passed either the name of a MIPS assembly file or
   * the name of a MIPS machine instruction file.  If the input file is
   * not a valid input file (due to syntactical errors or unsupported
   * instructions being specified), the format is considered incorrect.
   */
  StallPipeline(string inputFile);

  /* Executes the instructions passed into the constructor.  The method
   * determines the dependences and calculates the completion time of each
   * instruction and the overall execution time for the set of instructions,
   * storing that information for later printing.
   */
  void execute();

 protected:

  int getDelay(int i);

};

#endif
