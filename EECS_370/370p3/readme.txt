
Testing:
1. spec test
2. 
3.data forwarding with add, nor, sw, lw stalling 
4. lw where it seems like a register but is actually an offset and it loads into reg B
5. beq control hazard taken and not taken loops
6. destination reg = offset for data forwarding
7. reg a and reg b
8. forwarding from exmem
change pcplus1 back within the instruction for ifid and idex? make sure state.pc stays same as well
double lw check for exmem
account for 3 stage forwarding (wb?)
IDEX offset convert num to make sure its 16 bits?
/afs/umich.edu/class/eecs370/bin/submit370 3 p3.c test1.txt test2.txt test3.txt test4.txt test5.txt