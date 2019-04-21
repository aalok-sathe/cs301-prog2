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


bool DataForwardPipeline::checkHazards(int i)
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
    // is not implemented, so if the previous instruction was a control instruction,
    // then add one more stall cycle
    if (i > 0)
    {
        Instruction prevInst = myInstructions.at(i-1);
        InstFunc prevFunc = opcodes.getInstFunc(prevInst.getOpcode());
        if (prevFunc == CONTROL_I and myPipeline[i-1] == DECODE+1)
            return true;
    }
   
    // see if there's any RAW dependency of this instruction on a prior instruction
    // and if so, check for potential data hazard stalls required. 
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (prev > 0 and myPipeline[i]+1 == reqd)
    {
        Instruction depInst = myInstructions.at(prev);
        InstFunc depFunc = opcodes.getInstFunc(depInst.getOpcode());
        PipelineStages prod = myDataSchedule[depFunc].produced;

        // no delay if value is needed after it is produced
        //if ((int)reqd > (int)prod)
        //    return false;
        if ((int)myPipeline[prev]-1 >= (int)prod)
            return false;
        else
            return true;
        // additional stages remaining, minus the time available between
        // the previous instruction and this one to have advanced any more
        //return (NUM_STAGES-1) - reqd - (i - prev);
        //myTime += (NUM_STAGES-1) - reqd - (i - prev);
    }
    
    // if no data hazard exists, no additional delay is necessary 
    return false;
}
