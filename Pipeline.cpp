#include "Pipeline.h"
#include "ASMParser.h"
#include "MachLangParser.h"
#include <string>

Pipeline::Pipeline(string inputFile)
// TODO
{
    string ext = inputFile.substr(inputFile.find_last_of('.')+1);

    if (ext == "asm")
    {
        ASMParser *parser = new ASMParser(inputFile);
    }
    else if (ext == "mach")
    {
        MachLangParser *parser = new MachLangParser(inputFile);
    }
}


Pipeline::~Pipeline()
// TODO
{}


void Pipeline::execute()
// TODO
{}


void Pipeline::print()
// TODO
{}


bool Pipeline::isFormatCorrect()
// TODO
{
    return myFormatCorrect;
}
