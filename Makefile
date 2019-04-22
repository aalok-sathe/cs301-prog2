# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG = -DDEBUG -g -Wall
CFLAGS = -DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


PIPESIM: Pipesim.o Pipeline.o DataForwardPipeline.o StallPipeline.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o ASMParser.o MachLangParser.o
	g++ -o PIPESIM Pipesim.o Pipeline.o DataForwardPipeline.o StallPipeline.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o ASMParser.o MachLangParser.o

DependencyChecker.o: DependencyChecker.h OpcodeTable.h RegisterTable.h Instruction.h

ASMParser.o: Parser.h ASMParser.h OpcodeTable.h RegisterTable.h Instruction.h

MachLangParser.o: Parser.h MachLangParser.h OpcodeTable.h RegisterTable.h Instruction.h

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h

OpcodeTable.o: OpcodeTable.h

RegisterTable.o: RegisterTable.h

Pipeline.o: Pipeline.h

StallPipeline.o: StallPipeline.h

DataForwardPipeline.o: DataForwardPipeline.h

exec: PIPESIM
	./PIPESIM inst.asm

test: PIPESIM
	./PIPESIM inst.asm | diff -y inst.out -

test2: PIPESIM
	./PIPESIM inst2.asm | diff -y inst2.out -

turnin: clean
	turnin -v -c cs301 -p prog2 *

clean:
	/bin/rm -f PIPESIM *.o core
