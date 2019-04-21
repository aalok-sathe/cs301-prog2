#include "DataForwardPipeline.h"

DataForwardPipeline::DataForwardPipeline(string inputFile)
: Pipeline(inputFile) 
// TODO
{
    myOutput.myPipelineType = "FORWARDING";
}


int DataForwardPipeline::getDelay(int i)
// TODO
{
    myDataSchedule.insert(make_pair(ARITHM_I, ValueSchedule(EXECUTE, EXECUTE))); 
    myDataSchedule.insert(make_pair(MEMORY_I, ValueSchedule(EXECUTE, MEMORY))); 
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(DECODE, DECODE)));

    Instruction curInst = myInstructions.at(i);
    InstFunc curFunc = opcodes.getInstFunc(curInst.getOpcode());
    PipelineStages reqd = myDataSchedule[curFunc].required;

    if (curFunc == CONTROL_I)
       myTime++; 
    
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (prev>0)
    {
        Instruction depInst = myInstructions.at(prev);
        InstFunc depFunc = opcodes.getInstFunc(depInst.getOpcode());
        PipelineStages prod = myDataSchedule[depFunc].produced;

        if ((int)reqd > (int)prod)
            return 0;

        // the amount of time to wait till the data forwarding machinery
        // can line up
        return prod - reqd;
    }
    
    return 0;
}
