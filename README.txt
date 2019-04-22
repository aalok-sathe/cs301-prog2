name:   aalok sathe
what:   cs301-prog2


Testing
---

1: extra1.asm
    in this file, there's an obvious dependence in the first
    two instructions. this should cause a stall. furthermore,
    the first value doesn't become available until after the
    memory stage so the second instruction has to at least
    stall one additional cycle.
    this file tests the behavior of stall after control
    instructions, and expects a stall after the jump instruction.
    this file also has a two-away dependence between inst#4
    and inst#6. expect a stall due to this in stalling pipeline
    but data forwarding means it doesn't need to stall, in
    addition to having one instruction in between them.
    finally, the dependence (6,7) should cause stalling in
    the stall pipeline but not in data forwarding.
    expected output: ideal 12, stalling 20, dataforward 14.

2. extra2.asm
    in this file, we test the behavior of beq (branch equals)
    instruction, which is both, a control and an I-type
    instruction. particularly, it has a label, so both its
    register operands are read, creating potential RAW deps.
    as we would expect, StallPipeline stalls beq for $4. there
    is a one cycle stall after beq since it is also a control
    instruction.
    additionally, the dependence between 3--5 is a long-distance
    one, and causes additional stall in the StallPipeline.
    however, data forwarding is able to avoid that stall.
    expected output: ideal 10, stalling 16, dataforward 11.

3. extra3.asm
    this file serves to demonstrate how execution times on the
    pipeline models can differ drastically: 15, 25, 17.

4. extra4.mach
    this file serves to test whether the program can successfully
    take machine language files as input as well.
