#include "Pipeline.h"


Pipeline::Pipeline(string inputFile)
    : myDependencyChecker()
// TODO
{
    myTime = NUM_STAGES;
    myFormatCorrect = false;
    myOutput.myPipelineType = "IDEAL";
   
    myDataSchedule.insert(make_pair(ARITHM_I, ValueSchedule(EXECUTE, EXECUTE))); 
    myDataSchedule.insert(make_pair(MEMORY_I, ValueSchedule(EXECUTE, MEMORY))); 
    myDataSchedule.insert(make_pair(CONTROL_I, ValueSchedule(DECODE, DECODE))); 

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
// TODO
{}


void Pipeline::execute()
// TODO
{
    for (unsigned int i=0; i<myInstructions.size(); i++)
        myCompletionTimes.push_back((myTime++) + getDelay(myInstructions.at(i)));
    
}


void Pipeline::print()
// TODO
{
    // print out the type of pipeline
    cout << myOutput.myPipelineType << ':' << endl;
   
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

    cout << myOutput.footer << --myTime << endl << endl;
}

