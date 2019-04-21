#include "Pipeline.h"


Pipeline::Pipeline(string inputFile)
    : myDependencyChecker()
/* Default constructor for the Pipeline class. Given an input string for
 * a filename, calls an appropriate parser instance on the file (according
 * to file extension), and processes the instructions in the file by simulating
 * each one's execution in the CPU
 */
{
    myTime = NUM_STAGES;
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

    for (Instruction i = parser->getNextInstruction();
         i.getOpcode() != UNDEFINED;
         i = parser->getNextInstruction())
        {
            myInstructions.push_back(i);
            myDependencyChecker.addInstruction(i);
        }

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
    // iterate through instructions, one after another,
    // and simulate the execution of each instruction. store the completion
    // time for each instruction in a container for later use
    for (unsigned int i=0; i<myInstructions.size(); i++)
    {
        myCompletionTimes.push_back(myTime + getDelay(i));
        if (++myTime <= myCompletionTimes.back())
            myTime = myCompletionTimes.back() + 1;
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

