// aalok sathe
#ifndef _STALL_PIPELINE_H_
#define _STALL_PIPELINE_H_

#include "Pipeline.h"

/* The stages in the pipeline in the order they appear in the datapath */
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


 private:
 
 /* Although Pipeline::checkHazards is virtual, reuse it here since no
  * modiciation is necessary for this pipeline model
  */
  using Pipeline::checkHazards;


 /* Given the index of an instruction, i, computes and returns the
  * delay in number of additional stall cycles that instruction will encounter
  * according to the current model.
  */
  int getDelay(int i);


 /* control instructions need an extra delay slot because branch prediction
  * is not implemented, so if the previous instruction was a control instruction,
  * then add one more stall cycle. return if stall is needed (true) or not (false)
  */
  bool checkControlDelay(int i);
  

 /* check if a data hazard-related stall exists. return if stall is needed (true)
  * or not (false)
  */
  bool checkStallDelay(int i);


};

#endif
