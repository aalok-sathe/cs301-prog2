#include "Pipeline.h"


Pipeline::Pipeline(string inputFile)
    : myDependencyChecker()
/* Default constructor for the Pipeline class. Given an input string for
 * a filename, calls an appropriate parser instance on the file (according
 * to file extension), and processes the instructions in the file by simulating
 * each one's execution in the CPU
 */
{
    myTime = 1;//NUM_STAGES;
    //myClockTick = 1;
    myFormatCorrect = false;
    myOutput.myPipelineType = "IDEAL";
   

    Parser* parser;
    string ext = inputFile.substr(inputFile.find_last_of('.')+1);

    if (ext == "asm")
        parser = new ASMParser(inputFile);
    else if (ext == "mach")
        parser = new MachLangParser(inputFile);
    else
        return;

    myFormatCorrect = parser->isFormatCorrect();
    if (not myFormatCorrect)
        return;

    int index = 0;
    for (Instruction i = parser->getNextInstruction();
         i.getOpcode() != UNDEFINED;
         i = parser->getNextInstruction())
        {
            myInstructions.push_back(i);
            myDependencyChecker.addInstruction(i);
            //myPipeline.insert(make_pair(index++, FETCH));
        }

    //myPipeline.insert(make_pair(0, FETCH));
    //myPipeline.begin()->second = FETCH;

    delete parser;
}


Pipeline::~Pipeline()
// Deconstructor for Pipeline class
{}


void Pipeline::execute()
/* Executes the instructions passed into the constructor.  The method
 * determines the dependences and calculates the completion time of each
 * instruction and the overall execution time for the set of instructions,
 * storing that information for later printing.
 */
{
    if (myInstructions.empty())
        return;
    
    // iterate through instructions, one after another,
    // and simulate the execution of each instruction. store the completion
    // time for each instruction in a container for later use
    int i = 0, j = 0;
    while (i < (int)myInstructions.size() or myPipeline[myInstructions.size()-1] < NUM_STAGES)
    {
        if (myPipeline.find(i) == myPipeline.end())
            myPipeline.insert(make_pair(i, FETCH));
        
        stepPipeline();
        
        if (myPipeline[j] == NUM_STAGES)
        {
            myCompletionTimes.push_back(myTime);
            j++;
        }
        if (i < (int)myInstructions.size() and myPipeline[i] > FETCH)
            i++;

        myTime++;
    }
}


void Pipeline::print()
/* Prints the pipeline time, prints the RAW dependences found in the
 * instructions, prints each instruction with its completion time, and
 * prints the total execution time.
 */
{
    // print out the type of pipeline
    cout << myOutput.myPipelineType << ": " << endl;
   
    // print out any dependences found 
    vector<string> deps = myDependencyChecker.getStringDependences(RAW);
    vector<string>::iterator it;
    for (it = deps.begin(); it != deps.end(); it++)
        cout << (*it) << endl;

    cout << myOutput.columnHeaders << endl;
    
    vector<Instruction>::iterator iiter = myInstructions.begin();
    vector<int>::iterator citer = myCompletionTimes.begin();
    int i = 0;

    while (iiter != myInstructions.end())
        cout << i++ << "\t" << *(citer++) << "\t\t\t|"
             << (iiter++)->getAssembly() << endl;

    cout << myOutput.footer << myCompletionTimes.back() << endl << endl;
}


void Pipeline::stepPipeline()
//TODO
{
    for (unsigned int i=0; i<myInstructions.size(); i++)
    {
        if (myPipeline.find(i) == myPipeline.end())
            break; 

        PipelineStages prevStage = NUM_STAGES;
        if (i>0)
            prevStage = myPipeline[i-1];

        if (((PipelineStages)(myPipeline[i]+1) < prevStage or myPipeline[i]+1 == NUM_STAGES) and not checkHazards(i))
            myPipeline[i] = (PipelineStages) ((int)myPipeline[i] + 1);
        else
            continue;
    }
}



bool Pipeline::checkHazards(int i)
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
    if (checkControlDelay(i))
        return true;
    
    // see if there's any RAW dependency of this instruction on a prior instruction
    // and if so, check for potential data hazard stalls required. 
    int prev = myDependencyChecker.getPrevDep(i, RAW);
    if (checkStallDelay(i, prev))
        return true;
    
    // if no data hazard exists, no additional delay is necessary 
    return false;
}
