#include "StallPipeline.h"

StallPipeline::StallPipeline(string inputFile)
    : Pipeline(inputFile)
/* Default constructor for the StallPipeline class.
 * Uses the inherited constructor, and only sets the header title
 * to something else. Additionally initializes appropriate ValueSchedule
 * items into a map according to the pipeline model.
 */
{
    myOutput.myPipelineType = "STALL";
    
    myDataSchedule.insert(make_pair(ARITHM_I, ValueSchedule(DECODE, WRITEBACK))); 
    myDataSchedule.insert(make_pair(MEMORY_I, ValueSchedule(DECODE, WRITEBACK))); 
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(DECODE, WRITEBACK)));
}


int StallPipeline::getDelay(int i)
/* Given the index of an instruction, i, computes and returns the
 * delay in number of additional stall cycles that instruction will encounter
 * according to the current model. 
 */
{
    // get the current instruction, its function, and when its data is needed
    Instruction curInst = myInstructions.at(i);
    InstFunc curFunc = opcodes.getInstFunc(curInst.getOpcode());
    PipelineStages reqd = myDataSchedule[curFunc].required;

    // control instructions need an extra delay slot because branch prediction
    // is not implemented
    if (curFunc == CONTROL_I)
       myTime++; 
   
    // see if there's any RAW dependency of this instruction on a prior instruction
    // and if so, check for potential data hazard stalls required. 
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (prev>0)
    {
        Instruction depInst = myInstructions.at(prev);
        InstFunc depFunc = opcodes.getInstFunc(depInst.getOpcode());
        PipelineStages prod = myDataSchedule[depFunc].produced;

        // no delay if value is needed after it is produced
        if ((int)reqd > (int)prod)
            return 0;

        // additional stages remaning, minus the time available between
        // the previous instruction and this one to have advanced any more
        return (NUM_STAGES-1) - reqd - (i - prev);
    }
    
    // if no data hazard exists, no additional delay is necessary 
    return 0;
}
