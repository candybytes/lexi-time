// TEAM

// First team member name  : Robert Valladares

// Second team member name : Danish Waheed

/**
 *  lex.c
 *  Lexical Analizer
 *
 *  Created by Robert Valladares and Danish Waheed on 06/01/15.
 *  Copyright (c) 2015 ROBERT VALLADARES and DANISH WAHEED. All rights reserved.
 */

// Library declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//------------------ global constants and given constants by assigment -------

/*
 #define norw        15              // number of reserved words
 #define imax     32767              // maximum integer value
 #define cmax        11              // maximum number if chars for indents
 #define nestmax      5              // maximum depth of block nesting
 #define strmax     256              // maximum length of strings
 */
#define MAX_NUMBER_DIGITS 5                 // Defines how many digits an integer can have
#define MAX_IDENTIFIER_LENGTH 11            // Defines how long an identifier string can be
#define MAX_NAMERECORD_TABLE_LENGTH 1000    // Defines how many tokens can be read
int m_nCleanCount = 0;


//global strings for input output file names
char *FPS[] = {"input.txt", "cleaninput.txt", "lexemetable.txt", "lexemelist.txt"};

//------------------------- global data structures ------------------------
// enumerator of the symbols
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqlsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym,
    writesym, readsym, elsesym } token_type;

//structure of the symbol table record
typedef struct {
    
    int kind;           // constant = 1; var = 2, proc = 3
    char name[12];      // name up to 11 characters long, 11 + 1 for \0
    int val;            // number (ASCII value)
    int level;          // L level
    int adr;            // M address
    
} namerecord_t;


//  reserved words
char *word[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write" };
//  reserved words numerical representation from the token_type enum
int wsym[] = { nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};






//------------------- Read ahead function prototypes--------------
int charCount(FILE *fp);
void readInput(FILE *fp, char foo[]);
void fileReadError(char fileName[], int reading);
void cleanInput(FILE *fp, char src[], int count, char cleanSrc[]);



// -----------------Initial call to program  -----------------
int main(int argc, char *argv[]) {
    
    // if a file name for input is passed
    // use that name instead of the default in
    // FPS[0] = input.txt
    //char code[] = readInput(char fileName[]);
    
    int i = 0;
    char *filename = NULL;
    if(argc > 1) {
        filename = argv[1];
    }
    else {
        filename = FPS[0];
    }
    // declare input file pointer
    FILE *ifp;
    ifp = fopen(filename,"r");
    if (ifp == NULL) {
        fileReadError(filename, 1);
        exit(EXIT_FAILURE);
    }
    
    
    // declare clean input file output
    FILE *cifp;
    cifp = fopen(FPS[1],"w");
    if (cifp == NULL) {
        fileReadError(FPS[1], 0);
        exit(EXIT_FAILURE);
    }
    
    
    // how many characters in file, add 1 for '\0' terminating char
    int count = charCount(ifp);
    if (count < 0) {
        fileReadError(filename, 1);
        // this is fatal error
        exit(EXIT_FAILURE);
    }
    //add 1 to count for '\0' terminating char
    //count;
    char code[count];
    // cleanCode will have input without comments
    char cleanCode[count];
    // initiaalize code arrays
    for (i = 0; i < count; i++) {
        code[i] = '@';
        cleanCode[i] = '@';
    }
    // read input file into array code[]
    readInput(ifp, code);
    //code[count] = 0;
    
    // close the input file
    fclose(ifp);
    //fclose(cifp);
    cleanInput(cifp, code, count, cleanCode);
    //cleanCode[m_nCleanCount+1] = 0;
    printf("clean count %d\n", m_nCleanCount);
    
    i = 0;
    while (cleanCode[i] != EOF) {
        printf("%c\n", cleanCode[i] );
        i++;
    }
    
    fclose(cifp);
    
    return 0;
}

/**
 * "int charCount(FILE *fp)"
 * count the amount of characters in input file
 * without reading the file
 * return an integer , count of characters
 *
 */
int charCount(FILE *fp){
    
    long off_end;
    int rc;
    size_t fSize;
    int c;
    char lett;
    int i = 0;
    
    // if the file pointer is null return error
    if (fp != NULL) {
        
        // go to end of file
        rc = fseek(fp, 0L, SEEK_END);
        if (rc != 0) {
            // error ocurred
            return -1;
        }
        // Byte offset to the end of the file (size)
        if (0 > ( off_end = ftell(fp) ) ){
            return -1;
        }
        
        fSize = (size_t)off_end;
        // reset the file reader pointer to the begining of the file
        rc = 0;
        rc = fseek(fp, 0L, SEEK_SET);
        if (rc != 0) {
            // error ocurred
            return -1;
        }
        // return the count of characters in the input file
        return (int)fSize;
    }
    
    return -1;
    
}

/**
 * "readInput(FILE *fp, char src[])"
 * read each individual character into array passed from main
 * input file will be then store for use by other functions
 *
 */

void readInput(FILE *fp, char src[]){
    
    int i = 0;
    int c;
    int rc = 0;
    
    if (fp == NULL || src == NULL) {
        //if the file pointer or the char array is null, return error
        return;
    }
    
    // declare char array to store file input
    
    // return to the begining of file reader pointer
    rc = fseek(fp, 0L, SEEK_SET);
    if (rc != 0) {
        // error ocurred
        fileReadError(FPS[0], 1);
        // this is a fatal error
        exit(EXIT_FAILURE);
    }
    // store each character into -array passed- from main
    while ( (c = fgetc(fp)) != EOF ){
        src[i++] = c;
    }
    
    return;
}

void cleanInput(FILE *fp, char src[], int count, char cleanSrc[]){
    
    int p = 1;
    int i = 0;
    while (i < count) {
        // check if this is the begiining of a comment block
        if (src[i] == '/' && p == 1) {
            if (src[i + 1] == '*') {
                p = 0;
                i += 2;
            }
        }
        // check if this is the end of a comment block
        if (src[i] == '*' && p == 0) {
            if (src[i + 1] == '/') {
                p = 1;
                i += 2;
            }
        }
        
        if (p) {
            fprintf( fp, "%c", src[i] );
            // copy input code without comments into new array
            cleanSrc[ m_nCleanCount++] = src[i];
        }
        
        i++;
        
    }
    // check if file only contains a comment opening statement
    if (p == 0){
        printf("ERROR, input file contains a open ended comment line\n");
        exit(EXIT_FAILURE);
    }
    // reset the end of file for clean code array
    cleanSrc[m_nCleanCount] = EOF;
    
}

/**
 * "fileReadError(char fileName[], int reading)"
 * prints a standard file read-open-write error message
 * if int reading = 1, then print reading, else reading = 0 = writing
 *
 */
void fileReadError(char fileName[], int reading ){
    
    char *str[] = {"writing", "reading"};
    printf("Warning!, Could not open the file %s for %s\n", fileName, str[reading]);
    return;
    
}






