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

/** Library declarations  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/** global constants and
 * given constants by assigment */
int m_nNotComment = 1;
int m_nStartOfComment = 0;
int m_nEndOfComment = 0;

#define norw        15  /* number of reserved words             */
#define imax     32767  /* maximum integer value                */
#define cmax        11  /* maximum number if chars for indents  */
#define nestmax      5  /* maximum depth of block nesting       */
#define strmax     256  /* maximum length of strings            */

// global strings for input output file names
// file names array
char *FPS[] = {"input.txt", "cleaninput.txt", "lexemetable.txt", "lexemelist.txt"};
//char *CODE[] = NULL;




/** global data structures */

/** Read ahead function prototypes */
int readInputFile(char fileName[]);
void fileReadError(char fileName[], int reading);
void initComment(char prevC, char nextC );

/*************** Initial call to program  ***********/
int main(int argc, char *argv[]) {
    
    // if a file name for input is passed
    // use that name instead of the default in
    // FPS[0] = input.txt
    
    
    char *filename = NULL;
    if(argc > 1) {
        filename = argv[1];
    }
    else {
        filename = FPS[0];
    }
    // read the input from file
    readInputFile(filename);
    
    return 0;
}

/**
 * "readInputFile(char fileName[])"
 * reads code from file
 * code is given in integer values
 * then store into a structure array
 *
 */
/*
 */
int readInputFile(char fileName[]){
    // NEED TO DO, process each string through a analyzer and store
    // the read code
    
    int cChar;
    long codeLength;
    long off_end;
    int rc;
    size_t fSize;
    char *code = NULL;
    
    if(fileName != NULL){
        FILE *ifp;
        ifp = fopen(fileName,"r");
        
        if (ifp == NULL) {
            //if the file pointer is null return error
            return 1;
        }
        
        /*** find the lenght of code ***/
        
        
        /* go to end of file */
        rc = fseek(ifp, 0L, SEEK_END);
        if (rc != 0) {
            // error ocurred
            return 1;
        }
        /* Byte offset to the end of the file (size)*/
        if (0 > ( off_end = ftell(ifp) ) ){
            return 1;
        }
        
        fSize = (size_t)off_end;
        
        /* allocate a array to hold input code */
        code = malloc(fSize);
        if (code == NULL){
            return 1;
        }
        /* return to beginning of file */
        rewind (ifp);
        
        /* put the entire code from file into code array */
        /* check that the lenght is the same */
        if (fSize != fread(code, 1, fSize, ifp)) {
            // free the malloc created
            free(code);
            return 1;
        }
        int i = 0;
        for (i = 0; i < (int)fSize; i++) {
            printf("%c", code[i]);
        }
        
        //printf("the size of the code is %d\n", (int)fSize);
        
        
    }
    // in case the file name passed is null, print error with filename default to FPS[0] and exit
    
    return 1;
}


/**
 * "void isNotComment(char c)"
 * check if a character starts or ends a commend line
 * change m_nNotComment to 1 if is not a comment, or 0 if its a comment
 *
 */
void initComment(char prevChar, char nextChar){
    //printf("prev %c next %c\n",prevChar, nextChar );
    
    if (prevChar == '/' && nextChar == '*') {
        
        m_nNotComment = 0;
        m_nStartOfComment = 1;
    }
    
    if (prevChar == '*' && nextChar == '/') {
        printf("exir Com \n");
        m_nNotComment = 1;
        m_nEndOfComment = 1;
        
    }
    
    return;
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






