// aalok sathe
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
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(EXECUTE, EXECUTE)));
}


bool DataForwardPipeline::checkHazards(int i)
/* Given the index of an instruction, i, determine if there is any kind of hazard
 * at all that might prevent this instruction from moving into the next stage
 * in the pipeline at the current state 
 */
{
    // control instructions may need an extra delay slot because branch prediction
    // is not implemented, so if the previous instruction was a control instruction,
    // then may need add one more stall cycle. depends on the pipeline model.
    // for ideal, no control delay exists.
    if (checkControlDelay(i))
        return true;
    
    // see if there's any RAW dependency of this instruction on a prior instruction
    // and if so, check for potential data hazard stalls required. for ideal, no
    // dependencies exist.
    if (checkStallDelay(i))
        return true;
    
    // if no data hazard exists, no additional delay is necessary 
    return false;
}


bool DataForwardPipeline::checkControlDelay(int i)
 /* control instructions need an extra delay slot because branch prediction
 * is not implemented, so if the previous instruction was a control instruction,
 * then add one more stall cycle
 */
{
    if (i > 0)
    {
        Instruction prevInst = myInstructions.at(i-1);
        InstFunc prevFunc = myOpcodes.getInstFunc(prevInst.getOpcode());
        if (prevFunc == CONTROL_I and myPipeline[i-1] == DECODE+1)
            return true;
    }

    return false;
}


bool DataForwardPipeline::checkStallDelay(int i)
/* check if a data hazard-related stall exists. According to our knowledge of where
 * data is produced and where it is needed (myDataSchedule), tries to do full data
 * forwarding where possible, but stalls if it must.
 */
{
    // get the current instruction, its function, and when its data is needed
    Instruction curInst = myInstructions.at(i);
    InstFunc curFunc = myOpcodes.getInstFunc(curInst.getOpcode());
    PipelineStages reqd = myDataSchedule[curFunc].required;


    // see if there's any RAW dependency of this instruction on a prior instruction
    // and if so, check for potential data hazard stalls required. 
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (prev>=0 and myPipeline[i]+1 == reqd)
    {
        Instruction depInst = myInstructions.at(prev);
        InstFunc depFunc = myOpcodes.getInstFunc(depInst.getOpcode());
        PipelineStages prod = myDataSchedule[depFunc].produced;

        // no delay if value is needed after it is produced
        // for StallPipeline, the W and D stages may overlap
        if ((int)myPipeline[prev]-1 >= (int)prod)
            return false;
        else
            return true;
    }
    
    return false;
}

