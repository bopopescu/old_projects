start   beq 0 2 return  branch to return if r = 0, base case handling
        beq 1 2 return  branch to return if n = r
        lw  0 6 pos1    load 1 into reg6
        sw  5 7 Stack   Store return addr into stack
        add 5 6 5       Increment stack pointer
        lw  0 6 neg1    load -1 into reg6
        add 1 6 1       subtract 1 from n
        lw  0 6 Caddr   load start of function into 6
        jalr 6 7        recursive call
        lw  0 6 pos1    load 1 into reg6
        sw  5 3 Stack   store answer of first part in memory
        add 5 6 5       Increment stack pointer
        lw  0 6 neg1    load -1 into reg6
        add 2 6 2       subtract 1 from r
        lw  0 6 Caddr   load start of function into 6
        jalr 6 7        recursive call
        lw  0 6 neg1    load -1 into reg6
        add 5 6 5       decrement stack pointer
        lw  5 4 Stack   retrieve first part answer from mem
        add 3 4 3       add the two parts together
        add 5 6 5       decrement stack pointer
        lw  5 7 Stack   Load return address into reg7
        lw  0 6 pos1    load 1 into reg6
        add 1 6 1       re increment n since we're going up one call stack
        add 2 6 2       re increment r since we're going up one call stack
        jalr 7 4        return call stack
return  lw  0 3 pos1
        jalr 7 4        return base case
pos1    .fill   1
neg1    .fill   -1
Caddr   .fill start
