#include "StallPipeline.h"

StallPipeline::StallPipeline(string inputFile)
    : Pipeline(inputFile)
// TODO
{
    myOutput.myPipelineType = "STALL";
}


int StallPipeline::getDelay(int i)
// TODO
{
    myDataSchedule.insert(make_pair(ARITHM_I, ValueSchedule(DECODE, WRITEBACK))); 
    myDataSchedule.insert(make_pair(MEMORY_I, ValueSchedule(DECODE, WRITEBACK))); 
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(DECODE, DECODE)));

    Instruction curInst = myInstructions.at(i);
    InstFunc curFunc = opcodes.getInstFunc(curInst.getOpcode());
    PipelineStages reqd = myDataSchedule[curFunc].required;

    
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (prev>0)
    {
        //cout << "PREV: " << prev << " CURR: " << i << endl;
        
        Instruction depInst = myInstructions.at(prev);
        InstFunc depFunc = opcodes.getInstFunc(depInst.getOpcode());
        PipelineStages prod = myDataSchedule[depFunc].produced;


        //cout << "PROD: " << prod << " REQD: " << reqd << endl << endl;

        if ((int)reqd > (int)prod)
            return 0;

        return (NUM_STAGES-1) - reqd - (i - prev);

    }
    if (curFunc == CONTROL_I)
       myTime++; 

    return 0;
}
