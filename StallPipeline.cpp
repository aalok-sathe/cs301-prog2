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

    if (curFunc == CONTROL_I)
       myTime++; 
    
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

        // additional stages remaning, minus the time available between
        // the previous instruction and this one to have advanced any more
        return (NUM_STAGES-1) - reqd - (i - prev);

    }
    
    return 0;
}
