#include "DataForwardPipeline.h"

DataForwardPipeline::DataForwardPipeline(string inputFile)
: Pipeline(inputFile) 
/* Default constructor for the DataForwardPipeline class.
 * Uses the inherited constructor, and only sets the header title
 * to something else. Additionally initializes appropriate ValueSchedule
 * items into a map according to the pipeline model.
 */
{
    myOutput.myPipelineType = "FORWARDING";
   
    // for each kind of instruction, note down when values are needed and when
    // earliest can they be forwarded. 
    myDataSchedule.insert(make_pair(ARITHM_I, ValueSchedule(EXECUTE, EXECUTE))); 
    myDataSchedule.insert(make_pair(MEMORY_I, ValueSchedule(EXECUTE, MEMORY))); 
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(EXECUTE, WRITEBACK)));
}


int DataForwardPipeline::getDelay(int i)
/* Given the index of an instruction, i, computes and returns the
 * delay in number of additional stall cycles that instruction will encounter
 * according to the current model
 */
{
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

        // the amount of time to wait till the data forwarding machinery
        // can line up
        return prod - reqd;
    }
   
    // if no data hazard exists, no additional delay is necessary 
    return 0;
}
