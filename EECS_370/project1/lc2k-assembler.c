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

    char *lt[10000] = {NULL}; //label tracker
    
    int lc = 0; // line counter
    // if returns false means end of file
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if(label[0] != '\0'){
            //Duplicate Check
            if(lab_check(label, lt, 1) == -1){
                int label_len = strlen(label);
                lt[lc] = malloc(label_len + 1);
                strcpy(lt[lc], label);
                lt[lc][label_len] = '\0';
            }
            else{
                printf("DUPLICATE LABEL");
                exit(1);
            }
        }
        
        lc++;
    }
    
    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    lc = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        
        int output = 0;
        
        int i_arg0 = 0;
        int i_arg1 = 0;
        int i_arg2 = 0;
        
        int mask = 0x0000ffff;
        
        //000
        if(strcmp(opcode, "add") == 0){
            output += 0;//opcode
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
        }
        //001
        else if(strcmp(opcode, "nor") == 0){
            output += 1;//opcode
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
        }
        //010
        else if(strcmp(opcode, "lw") == 0){
            output += 2;//opcode
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
            // Error checking on offset fields bounds
            if(i_arg2 > 32767 || i_arg2 < -32768){
                printf("offset field too large");
                exit(1);
            }
            i_arg2 = i_arg2 & mask;
        }
        //011
        else if(strcmp(opcode, "sw") == 0){
            output += 3;
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
            // Error checking on offset fields bounds
            if(i_arg2 > 32767 || i_arg2 < -32768){
                printf("offset field too large");
                exit(1);
            }
            i_arg2 = i_arg2 & mask;
        }
        //100
        else if(strcmp(opcode, "beq") == 0){
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
            i_arg2 = isNumber(arg2) ? atoi(arg2) : lab_check(arg2, lt, 2);
            // Error checking on offset fields bounds
            if(i_arg2 > 32767 || i_arg2 < -32768){
                printf("offset field too large");
                exit(1);
            }
            if(!isNumber(arg2)){
                i_arg2 = i_arg2 - lc - 1; //desired = current + 1 + offset
            }
            i_arg2 = i_arg2 & mask;
            output += 4;
        }
        //101
        else if(strcmp(opcode, "jalr") == 0){
            output += 5;
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            i_arg1 = isNumber(arg1) ? atoi(arg1) : lab_check(arg1, lt, 2);
        }
        //110
        else if(strcmp(opcode, "halt") == 0){
            i_arg0 = 0;
            i_arg1 = 0;
            i_arg2 = 0;
            output += 6;
        }
        //111
        else if(strcmp(opcode, "noop") == 0){
            i_arg0 = 0;
            i_arg1 = 0;
            i_arg2 = 0;
            output += 7;
        }
        else if(strcmp(opcode, ".fill") == 0){
            i_arg0 = isNumber(arg0) ? atoi(arg0) : lab_check(arg0, lt, 2);
            output += i_arg0;
            fprintf(outFilePtr, "%i\n", output);
            lc++;
            continue;
        }
        //Unrecognized Opcode
        else{
            printf("Unrecognized opcode");
            exit(1);
        }
        output = output << 22;
        
        i_arg0 = i_arg0 << 19;
        i_arg1 = i_arg1 << 16;
        output = output + i_arg0 | i_arg1 | i_arg2;

        fprintf(outFilePtr, "%i\n", output);
        lc++;
    }

    return(0);
}

int lab_check(char* label, char *lt[10000], int run_num){
    for(int i = 0; i < 10000; i++){
        if(lt[i]&& strcmp(label, lt[i]) == 0){
            return i;
        }
    }
    if (run_num == 2){
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
