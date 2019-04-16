#include "Pipeline.h"
#include "StallPipeline.h"
#include "DataForwardPipeline.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/* This program is provided the name of an input file.  The input
 * file either contains only synatically correct and supported MIPS
 * assembly instructions (one per line) or contains only synactically
 * correct and support MIPS machine instructions (one per line).  If
 * the file is not a valid input file, an error message is printed and
 * the program exits.  If the input file is valid, the instructions from
 * the file are simulated on an ideal pipeline, a stalling pipeline, and a
 * data forwarding pipeline, with information printed about the relevant
 * dependences that might cause stalling as well, when each instruction
 * completes, and the overall execution time for each pipeline.
 */
int main(int argc, char *argv[])
{
  if(argc < 2){
    cerr << "Need to enter input file " << endl;
    exit(1);
  }

  Pipeline *ideal = new Pipeline(argv[1]);
  if(ideal->isFormatCorrect() == false){
    cerr << "Input file is not formatted correctly " << endl;
    exit(1);
  }
  ideal->execute();
  ideal->print();

  Pipeline *stall = new StallPipeline(argv[1]);
  if(stall->isFormatCorrect() == false){
    cerr << "Input file is not formatted correctly " << endl;
    exit(1);
  }
  stall->execute();
  stall->print();

  Pipeline *forward = new DataForwardPipeline(argv[1]);
  if(forward->isFormatCorrect() == false){
    cerr << "Input file is not formatted correctly " << endl;
    exit(1);
  }
  forward->execute();
  forward->print();

  delete ideal;
  delete stall;
  delete forward;

}
