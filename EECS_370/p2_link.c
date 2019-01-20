#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
    char label[7];
    char location;
    int offset;
    int file;
};

struct RelocationTableEntry {
    int offset;
    char inst[7];
    char label[7];
    int file;
};

struct FileData {
    int textSize;
    int dataSize;
    int symbolTableSize;
    int relocationTableSize;
    int textStartingLine; // in final executible
    int dataStartingLine; // in final executible
    int text[MAXSIZE];
    int data[MAXSIZE];
    SymbolTableEntry symbolTable[MAXSIZE];
    RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
    int text[MAXSIZE];
    int data[MAXSIZE];
    SymbolTableEntry symTable[MAXSIZE];
    RelocationTableEntry relocTable[MAXSIZE];
    int textSize;
    int dataSize;
    int symTableSize;
    int relocTableSize;
};

//Duplicate label check on combining, returns symbol table entry on second run
int lab_check(char* label, struct SymbolTableEntry symTable[MAXSIZE], int stc, int run){
    //Special case of stack
    if(run == 1 && strcmp(label, "Stack") == 0){
        printf("Can't define Stack");
        exit(1);
    }
    if(run == 2 && strcmp(label, "Stack") == 0){
        return -1;
    }
      // Regular label handling
    for(int i = 0; i < stc; i++){
        if(strcmp(label, symTable[i].label) == 0){
            if(run == 1){
                printf("DUPLICATE GLOBAL LABEL");
                exit(1);
            }
            return i;
        }

    }
    //If undefined global label
    if (run == 2){
        printf("UNDEFINED LABEL");
        exit(1);
    }
    printf("Not in sym tab");
    return -2;
}

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    int i, j;

    if (argc <= 2) {
        printf("error: usage: %s <obj file> ... <output-exe-file>\n",
                argv[0]);
        exit(1);
    }

    outFileString = argv[argc - 1];

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    FileData files[MAXFILES];

    //Reads in all files and combines into master
    for (i = 0; i < argc - 2; i++) {
        inFileString = argv[i+1];
        inFilePtr = fopen(inFileString, "r");
        printf("opening %s\n", inFileString);

        if (inFilePtr == NULL) {
            printf("error in opening %s\n", inFileString);
            exit(1);
        }

        char line[MAXLINELENGTH];
        int sizeText, sizeData, sizeSymbol, sizeReloc;

        // parse first line
        fgets(line, MAXSIZE, inFilePtr);
        sscanf(line, "%d %d %d %d",
                &sizeText, &sizeData, &sizeSymbol, &sizeReloc);

        files[i].textSize = sizeText;
        files[i].dataSize = sizeData;
        files[i].symbolTableSize = sizeSymbol;
        files[i].relocationTableSize = sizeReloc;

        // read in text
        int instr;
        for (j = 0; j < sizeText; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            instr = atoi(line);
            files[i].text[j] = instr;
        }

        // read in data
        int data;
        for (j = 0; j < sizeData; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            data = atoi(line);
            files[i].data[j] = data;
        }

        // read in the symbol table
        char label[7];
        char type;
        int addr;
        for (j = 0; j < sizeSymbol; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            sscanf(line, "%s %c %d",
                    label, &type, &addr);
            files[i].symbolTable[j].offset = addr;
            strcpy(files[i].symbolTable[j].label, label);
            files[i].symbolTable[j].location = type;
            files[i].symbolTable[j].file = i;
        }

        // read in relocation table
        char opcode[7];
        for (j = 0; j < sizeReloc; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            sscanf(line, "%d %s %s",
                    &addr, opcode, label);
            files[i].relocTable[j].offset = addr;
            strcpy(files[i].relocTable[j].inst, opcode);
            strcpy(files[i].relocTable[j].label, label);
            files[i].relocTable[j].file    = i;
        }
        fclose(inFilePtr);
    } // end reading files
    
    struct CombinedFiles mc;
    mc.textSize = 0;
    mc.dataSize = 0;
    mc.symTableSize = 0;
    mc.relocTableSize = 0;

    for(int i = 0; i < argc - 2; i++){
        files[i].textStartingLine = mc.textSize;
        files[i].dataStartingLine = mc.dataSize;

        for(int j = 0; j < files[i].textSize; j++){
            mc.text[mc.textSize] = files[i].text[j];
            mc.textSize++;
        }
        for(int j = 0; j < files[i].dataSize; j++){
            mc.data[mc.dataSize] = files[i].data[j];
            mc.dataSize++;
        }
        for(int j = 0; j < files[i].symbolTableSize; j++){
            if(files[i].symbolTable[j].location != 'U'){
                // Error checking
                lab_check(files[i].symbolTable[j].label, mc.symTable, mc.symTableSize, 1);
                mc.symTable[mc.symTableSize] = files[i].symbolTable[j];
                mc.symTableSize++;
            }
        }
        for(int j = 0; j < files[i].relocationTableSize; j++){
            mc.relocTable[mc.relocTableSize] = files[i].relocTable[j];
            mc.relocTableSize++;
        }
    }

    // Resolving reloc table
    for(int i = 0; i < mc.relocTableSize; i++){
        char sect = (strcmp(mc.relocTable[i].inst, ".fill") != 0 ) ? 'T' : 'D'; // Section of label
        int fnum = mc.relocTable[i].file; // File number
        int l_offs = mc.relocTable[i].offset; // Initial line offset from text or data section
        int adj = (sect == 'T') ? (files[fnum].textStartingLine + l_offs) : (files[fnum].dataStartingLine + l_offs); // Line to adjust

        int master_l = 0;
        //Global Label adjust
        if(mc.relocTable[i].label[0] >= 'A' && mc.relocTable[i].label[0] <= 'Z'){
            int symE = lab_check(mc.relocTable[i].label, mc.symTable, mc.symTableSize, 2); //Symbol entry
            // Stack label consideration
            if (symE == -1){
                master_l =  mc.textSize + mc.dataSize;
            }
            else{
                //If the label is used in text section
                fnum = mc.symTable[symE].file;
                if(mc.symTable[symE].location == 'T'){
                    master_l = files[fnum].textStartingLine + mc.symTable[symE].offset; //Location of label def
                }
                //else label used in data section
                else{
                    master_l = mc.textSize + files[fnum].dataStartingLine + mc.symTable[symE].offset; //Location of label def
                }
            }

        }
        //Local label adjust
        else{
            int ori_offs = 0x0000FFFF; //Mask to get original offset represented by label
            
            if(sect == 'T'){
                ori_offs = mc.text[adj] & ori_offs;
            }
            else{
                ori_offs = mc.data[adj] & ori_offs;
            }

            //Label defined in text
            if(ori_offs < files[fnum].textSize){
                master_l = files[fnum].textStartingLine + ori_offs;
            }
            // Label defined in data
            else{
                master_l = mc.textSize + files[fnum].dataStartingLine + ori_offs - files[fnum].textSize;
            }

        }
        
        int mask = 0xFFFF0000;
        if(sect == 'T'){
            mc.text[adj] = (mc.text[adj] & mask) | master_l;
        }
        else{
            mc.data[adj] = (mc.data[adj] & mask) | master_l;
        }
    }
    //Print linked data and text
    for(int i = 0; i < mc.textSize; i++){
        fprintf(outFilePtr, "%i\n", mc.text[i]);
    }
    for(int i = 0; i < mc.dataSize; i++){
        fprintf(outFilePtr, "%i\n", mc.data[i]);
    }
    fclose(outFilePtr);

} // end main
