#include "Pipeline.h"


Pipeline::Pipeline(string inputFile)
    : myFormatCorrect(false), myDependencies()
// TODO
{
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
            myDependencies.addInstruction(i);
        }

    cout << "DEBUG: sanity check: deps";
    myDependencies.printDependences();
    cout << endl;


}


Pipeline::~Pipeline()
// TODO
{}


void Pipeline::execute()
// TODO
{

}


void Pipeline::print()
// TODO
{
    cout << "IDEAL:" << endl;

    vector<string> deps = myDependencies.getStringDependences(RAW);
    vector<string>::iterator it;
    for (it = deps.begin(); it != deps.end(); it++)
        cout << (*it) << endl;

}


bool Pipeline::isFormatCorrect()
// TODO
{
    return myFormatCorrect;
}
