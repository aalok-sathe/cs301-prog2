// aalok sathe
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
 

  /* a private container to hold pipeline data. for each instruction denoted
    * by its index in myInstructions, enocdes the stage it is in, if currently
    * being executed in the pipeline. Any instruction in this container is
    * either in the pipeline or has finished executing (i.e., in stage NUM_STAGES) 
    */
    map<int, PipelineStages> myPipeline;
   

   /* Given the index of an instruction, i, determine if there is any kind of hazard
    * at all that might prevent this instruction from moving into the next stage
    * in the pipeline at the current state. is declared as virtual so that any derived
    * classes may add their own criteria for what might cause delay. returns whether
    * stall is needed because hazards exist (true) or not (false) 
    */ 
    virtual bool checkHazards(int i) { return false; };


   /* Internal instance of the DependencyChecker class to find out and query
    * dependences between instructions.
    */ 
    DependencyChecker myDependencyChecker;

 
   /* Internal instance of the OpcodeTable class
    */
    OpcodeTable myOpcodes;


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
    * using a ValueSchedule structure, with two data fields: 'required' and 'produced'.
    * This structure is housed in the Pipeline class since the design of the pipeline
    * determines how and when computation of certain values occurs.
    */
    map<InstFunc, ValueSchedule> myDataSchedule;


 private:


    /* A method that increments the current clock tick by 1 and updates the
    * stages of instructions in the pipeline wherever possible. calls checkHazards
    * before moving any particular instruction into the next stage 
    * int current: the integer index of the instruction that is currently the
    * first instruction in the pipeline (we ignore instructions that have finished
    * executing; i.e. the ones that came befoe 'current'). */ 
    void stepPipeline(int current);
 

    /* Internal variable to keep track of whether or not the input files are
    * correctly formatted. Takes its value from whichever parser is used to
    * parse the input file.
    */
    bool myFormatCorrect;


   /* A clock ticker that keeps track of how far along the CPU has progressed.
    * Is incremented by execute by at least one, or more if intermediate clock
    * ticks may be skipped (relevant to other kinds of pipeline models).
    */
    int myTime;

};

#endif
