#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

int convertNum(int num){
        /* convert a 16-bit number into a 32-bit Linux integer */
        if (num & (1<<15) ) {
            num -= (1<<16);
        }
        return(num);
 }

int main(int argc, char *argv[]) {
    char line[MAXLINELENGTH];
    stateType state;
    state.pc = 0;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    
    int mask = 0b111;
    int offs_mask = 0xFFFF;
    //state.pc= program counter
    for(state.pc = 0; state.pc< state.numMemory; state.pc++){
        printState(&state);
        int opcode = state.mem[state.pc] >> 22 & mask;
        //add
        if(opcode == 0){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            int destreg = state.mem[state.pc] & mask;
            state.reg[destreg] = state.reg[regA] + state.reg[regB];
        }
        //nor
        else if(opcode == 1){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            int destreg = state.mem[state.pc] & mask;
            state.reg[destreg] = ~(state.reg[regA] | state.reg[regB]);
        }
        //lw
        else if(opcode == 2){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            int offset = convertNum(state.mem[state.pc] & offs_mask);
            state.reg[regB] = state.mem[ state.reg[regA] + offset ];
        }
        //sw
        else if(opcode == 3){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            int offset = convertNum(state.mem[state.pc] & offs_mask);
            state.mem[ state.reg[regA] + offset ] = state.reg[regB];
        }
        //beq
        else if(opcode == 4){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            int offset = convertNum(state.mem[state.pc] & offs_mask);
            if(state.reg[regA] == state.reg[regB]){
                state.pc = state.pc + offset;
            }
        }
        //jalr
        else if(opcode == 5){
            int regA = (state.mem[state.pc] >> 19) & mask;
            int regB = (state.mem[state.pc] >> 16) & mask;
            state.reg[regB] = state.pc + 1;
            state.pc = state.reg[regA];
            state.pc--;
            continue;
        }
        //halt
        else if(opcode == 6){
            state.pc++;
            printState(&state);
            break;
        }
        //noop
        else if(opcode == 7){
        }
        else{
            printf("ERROR, SHOULD NOT HAPPEN\n");
        }
    }

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}
