start	lw      0       1       five    beq test and negative masking
	    lw      1       2       -32132       load reg2 with -1 (numeric address)
	    add     1       2       1       decrement reg1
        beq     0       1       -12332       goto end of program when reg1==0
        beq     0       0       start   go back to the beginning of the loop
        sw		0		0		-23213
        .fill	23
        .fill	-123
        noop
done    halt                            end of program
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start                   will contain the address of start (2)
