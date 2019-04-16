#include "DependencyChecker.h"

DependencyChecker::DependencyChecker(int numRegisters)
/* Creates RegisterInfo entries for each of the 32 registers and creates lists for
 * dependencies and instructions.
 */
{
  RegisterInfo r;

  // Create entries for all registers
  for(int i = 0; i < numRegisters; i++){
    myCurrentState.insert(make_pair(i, r));
  }

  myCurrentInstr = 0; // start with current instr no. = 0
}

void DependencyChecker::addInstruction(Instruction i)
/* Adds an instruction to the list of instructions and checks to see if that
 * instruction results in any new data dependencies.  If new data dependencies
 * are created with the addition of this instruction, appropriate entries
 * are added to the list of dependences.
 */
{
  InstType iType = i.getInstType();

  switch(iType){

      case RTYPE:
        {
            Opcode opc = i.getOpcode();

            // check for validity of each register position
            // and process according to known roles (src or dest).
            // dest registers are written to, and source, read from.
            if (myOpcodeTable.RSposition(opc) >= 0) // src
                checkForReadDependence(i.getRS());
            if (myOpcodeTable.RTposition(opc) >= 0) // src
                checkForReadDependence(i.getRT());
            if (myOpcodeTable.RDposition(opc) >= 0) // dest
                checkForWriteDependence(i.getRD());

            break;

        }

      case ITYPE:
        {
            Opcode opc = i.getOpcode();

            // check for validity of each register position
            // and process according to known roles (src or dest).
            // dest registers are written to, and source, read from.
            if (myOpcodeTable.RSposition(opc) >= 0) // src
                checkForReadDependence(i.getRS());
            if (myOpcodeTable.RTposition(opc) >= 0) // dest
                checkForWriteDependence(i.getRT());

            break;

        }

    case JTYPE:
            // no registers in J-type instructions
            break;

      default:
            break;

  }

  myInstructions.push_back(i);
  myCurrentInstr++;

}


void DependencyChecker::checkForReadDependence(unsigned int reg)
  /* Determines if a read data dependence occurs when reg is read by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
  // get the current state of the register in question
  RegisterInfo reginfo = myCurrentState[reg];
  AccessType atype = reginfo.accessType;

  // if access type was WRITE then we have some kind of dependence,
  // because we don't care about RAR
  if (atype == WRITE)
  {
    // initialize and populate the values of a dependence structure
    Dependence d;
    d.registerNumber = reg;
    d.previousInstructionNumber = reginfo.lastInstructionToAccess;
    d.currentInstructionNumber = myCurrentInstr;
    d.dependenceType = RAW;

    // store the detected dependence
    myDependences.push_back(d);
  }

  // update most recent access of this register
  reginfo.accessType = READ;
  reginfo.lastInstructionToAccess = myCurrentInstr;
  myCurrentState[reg] = reginfo;
}


void DependencyChecker::checkForWriteDependence(unsigned int reg)
  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
    // get the current state of the register in question
    RegisterInfo reginfo = myCurrentState[reg];
    AccessType atype = reginfo.accessType;

    // if access type was either READ or WRITE (some access, not A_UNDEFINED),
    // then we have some kind of dependence
    if (atype == WRITE or atype == READ)
    {
      // initialize and populate the values of a dependence structure
      Dependence d;
      d.registerNumber = reg;
      d.previousInstructionNumber = reginfo.lastInstructionToAccess;
      d.currentInstructionNumber = myCurrentInstr;
      // if previous atype is READ, it is an WAR dependence otherwise WAW
      d.dependenceType = (atype == READ) ? WAR : WAW;

      myDependences.push_back(d);
    }

    // update most recent access of this register
    reginfo.accessType = WRITE;
    reginfo.lastInstructionToAccess = myCurrentInstr;
    myCurrentState[reg] = reginfo;
}


void DependencyChecker::printDependences()
  /* Prints out the sequence of instructions followed by the sequence of data
   * dependencies.
   */
{
  // First, print all instructions
  list<Instruction>::iterator liter;
  int i = 0;
  cout << "INSTRUCTIONS:" << endl;
  for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++){
    cout << i << ": " << (*liter).getAssembly() << endl;
    i++;
  }

  // Second, print all dependences
  list<Dependence>::iterator diter;
  cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
  for(diter = myDependences.begin(); diter != myDependences.end(); diter++){
    switch( (*diter).dependenceType){
    case RAW:
      cout << "RAW \t";
      break;
    case WAR:
      cout << "WAR \t";
      break;
    case WAW:
      cout << "WAW \t";
      break;
    default:
      break;
    }

    cout << "$" << (*diter).registerNumber << " \t";
    cout << "(" << (*diter).previousInstructionNumber << ", ";
    cout << (*diter).currentInstructionNumber << ")" << endl;
  }


}
