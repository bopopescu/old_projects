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

struct block {
    int words[256];
    int addr;
    int valid;
    int dirty;
    int LRU;
};

int blockSizeInWords;
int numberOfSets;
int blocksPerSet;
struct block cache[256];
stateType state;

enum actionType
        {cacheToProcessor, processorToCache, memoryToCache, cacheToMemory,
         cacheToNowhere};
         
void
printAction(int address, int size, enum actionType type){
    printf("@@@ transferring word [%d-%d] ", address, address + size - 1);
    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    } else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    } else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    } else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    } else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

// Writes the specified cache block to main memory
void memWrite(int blockNum){
    // write to memory
    int start = cache[blockNum].addr;
    printAction(start, blockSizeInWords, cacheToMemory);
    for(int i = 0; i < blockSizeInWords; i++){
        state.mem[start] = cache[blockNum].words[i];
        start++;
    }
}

// Reads from mem into specified cache block
int memRead(int memAddr, int set_num, int blockNum){
    
    // If dirty write to mem
    if(cache[blockNum].dirty){
        memWrite(blockNum);
    }
    // If was previously valid, clearing out past
    else if(cache[blockNum].valid){
        printAction(cache[blockNum].addr, blockSizeInWords, cacheToNowhere);
    }

    // Read from mem
    int start = memAddr - (memAddr % blockSizeInWords);
    cache[blockNum].addr = start;
    printAction(start, blockSizeInWords, memoryToCache);
    for(int i = 0; i < blockSizeInWords; i++){
        cache[blockNum].words[i] = state.mem[start];
        start++;
    }
    cache[blockNum].valid = 1;
    cache[blockNum].dirty = 0;
    cache[blockNum].LRU = 0;

    return blockNum;
}

// Finds which block in cache to utilize, blockNum is block in cache that contains addr
int findBlockNum(int addr){
    int set_num = (addr / blockSizeInWords) % numberOfSets;
    //printf("set_num: %i\n", set_num);
    int start = set_num * blocksPerSet;
    int end = start + blocksPerSet;
    int blockNum = -1;
    int openBlock = -1;
    int max = -1;
    for(int i = start; i < end; i++){
        // If invalid, mark as first open block
        if(!cache[i].valid){
            openBlock = i;
            max = NUMMEMORY;
        }
        // Block is valid, check if it was the least used
        else {
            if(cache[i].LRU > max){
                openBlock = i;
                max = cache[i].LRU;
            }
            cache[i].LRU++;
        }
        
        if(cache[i].addr <= addr && addr < (cache[i].addr + blockSizeInWords) && cache[i].valid){
            blockNum = i;
            cache[i].LRU = 0;
        }
    }

    // If not found in cache
    if(blockNum == -1){
        blockNum = memRead(addr, set_num, openBlock);
    }
    return blockNum;
}
// Properly simulates the cache for a load from
// memory address “addr”. Returns the loaded value.
int load(int addr){
    int blockNum = findBlockNum(addr);
    int offset = addr % blockSizeInWords;
    
    printAction(addr, 1, cacheToProcessor);
    return cache[blockNum].words[offset];

}
// Properly simulates the cache for a store
void store(int addr, int data){
    int blockNum = findBlockNum(addr);
    int offset = addr % blockSizeInWords;

    printAction(addr, 1, processorToCache);
    cache[blockNum].words[offset] = data;
    cache[blockNum].dirty = 1;
}

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
    state.pc = 0;
    FILE *filePtr;

    if (argc != 5) {
        printf("error: usage: %s <machine-code file>, <blockSizeInWords> <numberOfSets> <blocksPerSet>\n", argv[0]);
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
    
    blockSizeInWords = atoi(argv[2]);
    numberOfSets = atoi(argv[3]);
    blocksPerSet = atoi(argv[4]);
    
    //Intialize cold cache
    for(int i = 0; i < 256; i++){
       for(int j = 0; j < 256; j++){
            cache[i].words[j] = 0;
        } 
        cache[i].valid = 0;
        cache[i].dirty = 0;
        cache[i].LRU = -1;
    }

    int mask = 0b111;
    int offs_mask = 0xFFFF;
    //state.pc= program counter
    for(state.pc = 0; state.pc< state.numMemory; state.pc++){
        //printState(&state);
        int instr = load(state.pc);
        int opcode = instr >> 22 & mask;
        //add
        if(opcode == 0){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
            int destreg = instr & mask;
            state.reg[destreg] = state.reg[regA] + state.reg[regB];
        }
        //nor
        else if(opcode == 1){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
            int destreg = instr & mask;
            state.reg[destreg] = ~(state.reg[regA] | state.reg[regB]);
        }
        //lw
        else if(opcode == 2){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
            int offset = convertNum(instr & offs_mask);
            state.reg[regB] = load(state.reg[regA] + offset);
            //get rid of state.reg[regB] = state.mem[ state.reg[regA] + offset ];
        }
        //sw
        else if(opcode == 3){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
            int offset = convertNum(instr & offs_mask);
            store(state.reg[regA] + offset, state.reg[regB]);
            //state.mem[ state.reg[regA] + offset ] = state.reg[regB];

        }
        //beq
        else if(opcode == 4){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
            int offset = convertNum(instr & offs_mask);
            if(state.reg[regA] == state.reg[regB]){
                state.pc = state.pc + offset;
            }
        }
        //jalr
        else if(opcode == 5){
            int regA = (instr >> 19) & mask;
            int regB = (instr >> 16) & mask;
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
    printf("\n\nstate:\n");
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
