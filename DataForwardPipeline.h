#ifndef _DATA_FORWARD_PIPELINE_H_
#define _DATA_FORWARD_PIPELINE_H_

#include "Pipeline.h"

/* The stages in the pipeline in the order they appear in the datapath */
// enum PipelineStages{
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
class DataForwardPipeline: public Pipeline{
 

 public:

  /* The constructor is passed either the name of a MIPS assembly file or
   * the name of a MIPS machine instruction file.  If the input file is
   * not a valid input file (due to syntactical errors or unsupported
   * instructions being specified), the format is considered incorrect.
   */
  DataForwardPipeline(string inputFile);


 private:

 /* Although Pipeline::checkHazards is virtual, reuse it here since no
  * modiciation is necessary for this pipeline model
  */
  using Pipeline::checkHazards;

 
 /* control instructions need an extra delay slot because branch prediction
  * is not implemented, so if the previous instruction was a control instruction,
  * then add one more stall cycle
  */
  bool checkControlDelay(int i);

 /* check if a data hazard-related stall exists. According to our knowledge of where
  * data is produced and where it is needed (myDataSchedule), tries to do full data
  * forwarding where possible, but stalls if it must.
  */
  bool checkStallDelay(int i);


};

#endif
