// aalok sathe
#include "Pipeline.h"


Pipeline::Pipeline(string inputFile)
/* Default constructor for the Pipeline class. Given an input string for
 * a filename, calls an appropriate parser instance on the file (according
 * to file extension), and processes the instructions in the file by simulating
 * each one's execution in the CPU
 */
{
    myTime = 1;                         // start clock at '1'
    myOutput.myPipelineType = "IDEAL";  // name this pipeline model

    // initialize a parser, and construct appropriate derived class based on
    // input file kind
    Parser* parser;
    string ext = inputFile.substr(inputFile.find_last_of('.')+1);
    if (ext == "asm")
        parser = new ASMParser(inputFile);
    else if (ext == "mach")
        parser = new MachLangParser(inputFile);
    else
        return;

    // check if parser is able to parse the input file correctly and
    // exit if it is not able
    myFormatCorrect = parser->isFormatCorrect();
    if (not myFormatCorrect)
        return;

    // add each instruction parsed by the parser into a container storing
    // instructions, and into the dependency checker
    for (Instruction i = parser->getNextInstruction();
         i.getOpcode() != UNDEFINED;
         i = parser->getNextInstruction())
        {
            myInstructions.push_back(i);
            myDependencyChecker.addInstruction(i);
        }

    // free allocated memory
    delete parser;
}


Pipeline::~Pipeline()
/* Deconstructor for Pipeline class
 */
{}


void Pipeline::execute()
/* Executes the instructions passed into the constructor.  The method
 * determines the dependences and calculates the completion time of each
 * instruction and the overall execution time for the set of instructions,
 * storing that information for later printing.
 */
{
    // there needs to be at least one instruction
    if (myInstructions.empty())
        return;
    
    // iterate through instructions, one after another,
    // and simulate the execution of each instruction. store the completion
    // time for each instruction in a container for later use. continue loop
    // even after all instructions are already in pipeline in order to finish
    // executing them. use i to keep track of insertion and j for completion.
    int i = 0, j = 0;
    while (i < (int)myInstructions.size() or myPipeline[myInstructions.size()-1] < NUM_STAGES)
    {
        // if instruction is not in the pipeline yet, add it to the pipeline
        if (myPipeline.find(i) == myPipeline.end())
            myPipeline.insert(make_pair(i, FETCH));
       
        // update clock tick and pipeline stages
        stepPipeline();
      
        // if instruction at j is finished executing, add the current clock
        // tick as its completion time 
        if (myPipeline[j] == NUM_STAGES)
        {
            myCompletionTimes.push_back(myTime);
            j++;
        }

        // if there are still more instructions to add and there's an opening
        // in the pipeline to add an instruction, then proceed to next instr.
        if (i < (int)myInstructions.size() and myPipeline[i] > FETCH)
            i++;

        // increment clock tick
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

    // print out column headers to organize data by
    cout << myOutput.columnHeaders << endl;
   
    // print each instruction number, completion time, and mnemonic
    // one after another
    vector<Instruction>::iterator iiter = myInstructions.begin();
    vector<int>::iterator citer = myCompletionTimes.begin();
    int i = 0;
    while (iiter != myInstructions.end())
        cout << i++ << "\t" << *(citer++) << "\t\t\t|"
             << (iiter++)->getAssembly() << endl;

    // finally state the total execution time
    cout << myOutput.footer << myCompletionTimes.back() << endl << endl;
}


void Pipeline::stepPipeline()
/* A method that increments the clock tick by 1, and makes updates to the
 * pipeline as appropriate. It checks to see if any instructions can finish
 * executing in this clock tick, and if any new instructions can be added.
 * It makes a call to checkHazards, to make sure to stall if any unresovlable
 * hazard is present according to the current pipeline model. 
 */
{
    // check to see each of the instructions in the pipeline, in order
    // added. if an instruction is not present, then no instructions thereafter
    // are being executed yet so exit. if an instruction has finished executing,
    // its stage would be 'NUM_STAGES', so skip that and move on to next instr
    for (unsigned int i=0; i<myInstructions.size(); i++)
    {
        // if not yet in pipeline, stop updating
        if (myPipeline.find(i) == myPipeline.end())
            break; 

        PipelineStages prevStage = NUM_STAGES;
        if (i>0)
            prevStage = myPipeline[i-1];

        // move on to next state if it is still mid-execution, and if the prev
        // instruction has moved onto the next stage, and if no hazards exist;
        // otherwise stall this instruction in this clock tick
        if ((myPipeline[i]+1 < prevStage or myPipeline[i]+1 == NUM_STAGES)
            and (not checkHazards(i)))
        {
            myPipeline[i] = (PipelineStages) ((int)myPipeline[i] + 1);
        }
        else
            continue;
    }
}


bool Pipeline::checkHazards(int i)
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
