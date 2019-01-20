#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int lab_check(char* label, char *lt[10000], int run_num);

struct sym_e{
    char label[7];
    char section;
    int offset;
};

struct reloc_e{
    char label[7];
    char opcode[6];
    int offset;
};

int main(int argc, char *argv[]) {
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */

    char *lt[65536] = {NULL}; //label tracker
    
    int lc = 0; // line counter
    int text = 0;
    int data = 0;
    int symbol = 0;
    int reloc = 0;
    
    struct sym_e table1[65536];
    struct reloc_e table2[65536];
    
    char in_text_sec = 1; // Is the program currently reading in the text section
    // if returns false means end of file
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        int offset;
        //Count number of data lines
        if(strcmp(opcode, ".fill") == 0){
            in_text_sec = 0;
            offset = lc - text;
            data++;
        }
        //Count number of text lines
        else{
            offset = lc;
            text++;
        }
        
        if(label[0] != '\0'){
            //Duplicate Check
            if(lab_check(label, lt, 1) == -1){
                int label_len = strlen(label);
                lt[lc] = malloc(label_len + 1);
                strcpy(lt[lc], label);
            }
            else{
                printf("DUPLICATE LABEL");
                exit(1);
            }
            //If label is global
            if(label[0] >= 'A' && label[0] <= 'Z'){
                char unique = 1;
                for(int i = 0; i < symbol; i++){
                    if(strcmp(label, table1[i].label) == 0){
                        table1[i].offset = offset;
                        table1[i].section = (in_text_sec) ? 'T':'D';
                        unique = 0;
                        break;
                    }

                }
                if(unique){
                    strcpy(table1[symbol].label, label);
                    table1[symbol].offset = offset;
                    table1[symbol].section = (in_text_sec) ? 'T':'D';
                    symbol++;
                }
            }
        }
        //Count number of symbol/relocation table lines
        if(!isNumber(arg2) && arg2[0] != '\0' && (strcmp(opcode, "lw") == 0 || strcmp(opcode, "sw") == 0)){
            //Symbol
            if(arg2[0] >= 'A' && arg2[0] <= 'Z'){
                char unique = 1;
                for(int i = 0; i < symbol; i++){
                    if(strcmp(arg2, table1[i].label) == 0){
                        unique = 0;
                        break;
                    }

                }
                if(unique){
                    strcpy(table1[symbol].label, arg2);
                    table1[symbol].offset = 0;
                    table1[symbol].section = 'U';
                    symbol++;
                }
            }
            //reloc
            strcpy(table2[reloc].opcode, opcode);
            strcpy(table2[reloc].label, arg2);
            table2[reloc].offset = offset;
            reloc++;
        }
        
        else if(!isNumber(arg0) && arg0[0] != '\0' && strcmp(opcode, ".fill") == 0){
            if(arg0[0] >= 'A' && arg0[0] <= 'Z'){
                char unique = 1;
                for(int i = 0; i < symbol; i++){
                    if(strcmp(arg0, table1[i].label) == 0){
                        unique = 0;
                        break;
                    }

                }
                if(unique){
                    strcpy(table1[symbol].label, arg0);
                    table1[symbol].offset = 0;
                    table1[symbol].section = 'U';
                    symbol++;
                }
            }
            //reloc
            strcpy(table2[reloc].opcode, opcode);
            strcpy(table2[reloc].label, arg0);
            table2[reloc].offset = offset;
            reloc++;
        }
        lc++;
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    
    fprintf(outFilePtr, "%i %i %i %i\n", text, data, symbol, reloc);
    rewind(inFilePtr);
    lc = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        
        int mask = 0x0000ffff;
        int output = 0;
        
        int i_arg0 = atoi(arg0);
        int i_arg1 = atoi(arg1);
        int i_arg2 = 0;

        //000
        if(strcmp(opcode, "add") == 0){
            output += 0;
            i_arg2 = atoi(arg2);
        }
        //001
        else if(strcmp(opcode, "nor") == 0){
            output += 1;
            i_arg2 = atoi(arg2);
        }
        //010
        else if(strcmp(opcode, "lw") == 0){
            output += 2;
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
            i_arg2 = (i_arg2 == -2) ? 0 : i_arg2; // set i_arg2 to 0 if undefined global label
        }
        //011
        else if(strcmp(opcode, "sw") == 0){
            output += 3;
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
            i_arg2 = (i_arg2 == -2) ? 0 : i_arg2; // set i_arg2 to 0 if undefined global label
        }
        //100
        else if(strcmp(opcode, "beq") == 0){
            output += 4;
            if(isNumber(arg2)){
                i_arg2 = atoi(arg2);
            }
            else{
                //beq invalid use of global label check
                if(arg2[0] >= 'A' && arg2[0] <= 'Z' && lab_check(arg2, lt, 2) == -2){
                    exit(1);
                }
                i_arg2 = lab_check(arg2, lt, 2);
                i_arg2 = i_arg2 - lc - 1; //desired = current + 1 + offset
            }
        }
        //101
        else if(strcmp(opcode, "jalr") == 0){
            output += 5;
        }
        //110
        else if(strcmp(opcode, "halt") == 0){
            output += 6;
            i_arg0 = 0;
            i_arg1 = 0;
        }
        //111
        else if(strcmp(opcode, "noop") == 0){
            output += 7;
            i_arg0 = 0;
            i_arg1 = 0;
        }
        else if(strcmp(opcode, ".fill") == 0){
            if(isNumber(arg0)){
                i_arg0 = atoi(arg0);
            }
            else{
                i_arg0 = lab_check(arg0, lt, 2);
                //Undefined global variable in fill
                if(i_arg0 == -2){
                    i_arg0 = 0;
                }
            }
            fprintf(outFilePtr, "%i\n", i_arg0);
            continue;
        }
        //Unrecognized Opcode
        else{
            printf("Unrecognized opcode");
            exit(1);
        }
        // Error checking on offset fields bounds
        if(i_arg2 > 32767 || i_arg2 < -32768){
            printf("offset field too large");
            exit(1);
        }
        // If arg2 is an undefined global label, set to zero
        output = output << 22;
        i_arg0 = i_arg0 << 19;
        i_arg1 = i_arg1 << 16;
        i_arg2 = i_arg2 & mask;
        output = output + i_arg0 | i_arg1 | i_arg2;

        fprintf(outFilePtr, "%i\n", output);
        lc++;
    }

    //Print symbol table
    for(int i = 0; i < symbol; i++){
        fprintf(outFilePtr, "%s %c %i\n", table1[i].label, table1[i].section, table1[i].offset);
    }
    // Print reloc table
    for(int i = 0; i < reloc; i++){
        fprintf(outFilePtr, "%i %s %s\n", table2[i].offset, table2[i].opcode, table2[i].label);
    }
    return(0);
}

int lab_check(char* label, char *lt[10000], int run_num){
    for(int i = 0; i < 10000; i++){
        if(lt[i] && strcmp(label, lt[i]) == 0){
            return i;
        }
    }
    //If undefined global variable
    if(label[0] >= 'A' && label[0] <= 'Z' && run_num == 2){
        return -2;
    }
    //If undefined local variable
    else if (run_num == 2){
        printf("UNDEFINED LABEL");
        exit(1);
    }
    return -1;
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2) {
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string){
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}
