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

// global data structures

// Read ahead function prototypes
int charCount(FILE *fp);
void readInput(FILE *fp, char foo[]);
void fileReadError(char fileName[], int reading);
void initComment(char prevC, char nextC );
//void initComment(char prevChar, char nextChar);

// -----------------Initial call to program  -----------------//
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
    
    // how many characters in file
    int count = charCount(ifp);
    if (count < 0) {
        fileReadError(filename, 1);
        // this is fatal error
        exit(EXIT_FAILURE);
    }
    //------------test print------------//
    printf("characters count %d\n", count);
    
    // create char array to store each character from file
    char code[count];
    // read input file into array code[]
    readInput(ifp, code);
    
    
    //--------- test print  ------------//
    for (i = 0; i < (count); i++) {
        printf("%c\n", code[i]);
    }
    
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
    
    //if the file pointer is null return error
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
    
    char lett;
    int c;
    int i = 0;
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
    // store each character into array pass from main
    while ( (c = fgetc(fp)) != EOF ){
        lett = c;
        src[i++] = lett;
    }
    
    return;
}

/*---------comented out at the moment--------
 
 * "void isNotComment(char c)"
 * check if a character starts or ends a commend line
 * change m_nNotComment to 1 if is not a comment, or 0 if its a comment
 *
 
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
 
 */


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






