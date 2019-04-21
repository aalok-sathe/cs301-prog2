#ifndef _PIPELINE_H_
#define _PIPELINE_H_

using namespace std;

#include <string>
#include <iostream>
#include <vector>
#include "ASMParser.h"
#include "MachLangParser.h"
#include "DependencyChecker.h"



/* The stages in the pipeline in the order they appear in the datapath */
enum PipelineStages{
  FETCH,
  DECODE,
  EXECUTE,
  MEMORY,
  WRITEBACK,
  NUM_STAGES
};



/* This class simulates an ideal pipeline, executing the instructions
 * from an input file on an ideal pipeline and printing out information
 * about RAW dependences, the completion times of the instructions in
 * the order they appear in the file, and the total execution time for
 * the instructions from the file.
 */
class Pipeline{

 public:

  /* The constructor is passed either the name of a MIPS assembly file or
   * the name of a MIPS machine instruction file.  If the input file is
   * not a valid input file (due to syntactical errors or unsupported
   * instructions being specified), the format is considered incorrect.
   */
  Pipeline(string inputFile);

  /* Default deconstructor for the Pipeline class
   */
  virtual ~Pipeline();

  /* Executes the instructions passed into the constructor.  The method
   * determines the dependences and calculates the completion time of each
   * instruction and the overall execution time for the set of instructions,
   * storing that information for later printing.
   */
  void execute();

  /* Prints the pipeline time, prints the RAW dependences found in the
   * instructions, prints each instruction with its completion time, and
   * prints the total execution time.
   */
  void print();

  /* Returns true if the input file was syntactically correct and only
   * contained supported instructions.  Otherwise, returns false.
   */
  bool isFormatCorrect() { return myFormatCorrect; };


 protected:
  
    //vector<pair<Instruction, PipelineStages> > myPipeline;
    map<int, PipelineStages> myPipeline;
    void stepPipeline();
    virtual bool checkHazards(int i) { return false; };
    virtual bool checkControlDelay(int i) { return false; };
    virtual bool checkStallDelay(int i, int prev) { return false; };
   /* Given the index of an instruction, i, computes and returns the
    * delay in number of additional stall cycles that instruction will encounter
    * according to the current model. defaults to 0 for ideal pipeline.
    * Intended to be overriden to whatever model of pipeline is being implemented
    */ 
   //TODO 
   /* Internal instance of the DependencyChecker class to find out and query
    * dependences between instructions.
    */ 
    DependencyChecker myDependencyChecker;

   /* Internal instance of the OpcodeTable class
    */
    OpcodeTable opcodes;

   /* A clock ticker that keeps track of how far along the CPU has progressed.
    * Is incremented by execute by at least one, or more if intermediate clock
    * ticks may be skipped (relevant to other kinds of pipeline models).
    */
    int myTime;

   /* An internal struct to hold data that is computed for later
    * formatting and printing.
    */
    struct outputStruct
    {
        string dependencies;    // a string with all the RAW dependencies
        string columnHeaders;   // what to print out before the main data
        vector<string> instrLines;  // lines reporting instruction number,
                                    // completion time, and encoding for
                                    // each instruction in the pipeline
        string footer;          // what to print at the end
        string myPipelineType;  // type of model, e.g., "IDEAL"
        
        outputStruct()
        {
            columnHeaders = "Instr# \t CompletionTime \t Mnemonic ";
            footer = "Total time is ";
        }

    } myOutput;

   /* Internal variable to keep track of whether or not the input files are
    * correctly formatted. Takes its value from whichever parser is used to
    * parse the input file.
    */
    bool myFormatCorrect;

   /* A container to store the instructions in the pipeline.
    * The container is ordered, and relies on instruction indices for certain
    * kinds of processing.
    */
    vector<Instruction> myInstructions;
    
   /* A container to store when each of the instructions would finish executing
    * according to the current pipeline model. The stored information may be used
    * later in outputting the data.
    */
    vector<int> myCompletionTimes;


   /* An internal structure to represent information about instructions or kinds of
    * instructions, particularly, what pipeline stage do its argument operands (if
    * any) are needed in, and what pipeline stage its output operand gets computed.
    */
    struct ValueSchedule
    {
       PipelineStages required;
       PipelineStages produced;

       ValueSchedule() {};

       ValueSchedule(PipelineStages r, PipelineStages p)
       {
         required = r;
         produced = p;
       }
    };


   /* A map that stores information pertaining to each kind of instruction
    * (arithmetic/memory/control) regarding when its operands to operate on (if any)
    * are needed and when its output (if any) is produced. This information is stored
    * using a ValueSchedule structure, with two data fields: 'required' and 'produced'
    */
    map<InstFunc, ValueSchedule> myDataSchedule;

 private:

    
};

#endif
