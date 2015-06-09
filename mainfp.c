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
#include <ctype.h>


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
//#define MAX_NAMERECORD_TABLE_LENGTH 1000    // Defines how many tokens can be read
#define MAX_FILES   4                       // Defines the count of files for input output
int m_nCleanCount = 0;                      // global variable to track count of characters in input
int m_nNameRecords = 0;                     // global variable to track count of NameRecord in input
#define MAX_PUNCT   13                      // maximum amount of special symbols
#define MAX_STR     256                     // maximum length of strings
int m_nCleanInputTokens = 0;                // global variable to track count of clean tokens in input


//global strings for input output file names
char *FNS[] = {"input.txt", "cleaninput.txt", "lexemetable.txt", "lexemelist.txt"};
typedef enum {input_txt, cleaninput_txt, lexemetable_txt, lexemelist_txt} eFNS;
// file pointer array
FILE *m_FPS[MAX_FILES];

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
int m_naWsym[] = { nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

char m_caSpecialSymbols[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';', ':'};







//------------------- Read ahead function prototypes--------------
void createFilePointers();
int charCount(FILE *fp);
void readInput(FILE *fp, char foo[]);
void fileReadError(char fileName[], int reading);
void cleanInput(FILE *fp, char src[], int count, char cleanSrc[]);
int charType(char c);
void splitInputTokens(char cleanSrc[], char *caCleanInputTokens[]);
int isSpecialChar(char c);
char *cleanInputTokenCalloc(int tknSize);
void freeInputTokenCalloc(char *caCleanInputTokens[]);



// -----------------Initial call to program  -----------------
int main(int argc, char *argv[]) {
    
    // if a file name for input is passed
    // use that name instead of the default in
    // FNS[0] = input.txt
    
    int i = 0;
    
    if(argc > 1) {
        FNS[input_txt] = argv[1];
    }
    
    // create file pointers for input output
    // createFilePointers handles null pointer exception
    createFilePointers();
    // copy input file pointer
    FILE *ifp = m_FPS[input_txt];
    // copy clean input file pointer
    FILE *cifp = m_FPS[cleaninput_txt];
    
    
    // how many characters in file
    int count = charCount(ifp);
    if (count < 0) {
        fileReadError(FNS[input_txt], input_txt);
        // this is fatal error
        exit(EXIT_FAILURE);
    }
    
    char code[count];
    // cleanCode will have input without comments
    char cleanCode[count];
    // initialize code arrays
    for (i = 0; i < count; i++) {
        code[i] = ' ';
        cleanCode[i] = ' ';
    }
    
    // read input file into array code[]
    readInput(ifp, code);
    // close the input file
    fclose(ifp);
    
    
    // remove comments from input
    //cleanCode[] will contain -comments free- input
    cleanInput(cifp, code, count, cleanCode);
    // close the -clean input- file
    fclose(cifp);
    
    // there will be at most m_nCleanCount separate string tokens
    char *caCleanInputTokens[m_nCleanCount];
    // separate cleanCode into tokens, allocate space as needed with calloc
    // need to free each caCleanInputTokens[] array index that calloc was done to
    // if caCleanInputTokens[] is not null, free it before exiting program
    // or after printing it to file
    splitInputTokens(cleanCode, caCleanInputTokens);
    
    
    //----------test print start---------//
    for (i = 0; i < m_nCleanInputTokens ; i++) {
        printf("%s\n", caCleanInputTokens[i] );
    }
    printf("token count %d\n", m_nCleanInputTokens );
    
    //----------test print end-----------//
    
    ////// TO-D0-----some stuff from list bellow will go here
    
    
    // call freeInputTokenCalloc after finishing the use of the array
    freeInputTokenCalloc(caCleanInputTokens);
    
    
    ////// TO-D0-----check that each token is a valid token (char length, declaration, integer size, etc...)
    ////// TO-D0-----store token into array of namerecord_t;
    ////// TO-D0-----get info for each token to fill variables of namerecord_t;
    ////// TO-D0-----print namerecord_t array records (tokens, and tokens values)
    ////// TO-D0-----close the last two file pointers created by createFilePointers();
    ////// TO-D0-----celebrate this assigment is awesome and done;
    
    
    // replace file pointers by knicnames created just like in line 115 and 117
    // knicknames for file pointers examples FILE *ifp = m_FPS[input_txt];
    // file pointer is already open and active, knickname is local
    fclose(m_FPS[lexemetable_txt]);
    fclose(m_FPS[lexemelist_txt]);
    
    
    return 0;
}

/*
 * "createFilePointers()"
 * create all the file pointers and initialize them
 * file pointers are stored in an array for global use
 * handle null pointer exeptions immediatly
 */

void createFilePointers(){
    
    char *mode = "r";
    int i = 0;
    for (i = 0; i < MAX_FILES; i++) {
        // initialize the file pointer array index
        m_FPS[i] = NULL;
        
        if (i) {
            mode = "w";
        }
        // if file pointer fails to open filename
        // then a null is stored into array
        FILE *fp = NULL;
        fp = fopen(FNS[i], mode);
        if (fp == NULL) {
            fileReadError(FNS[i], i);
        }
        m_FPS[i] = fp;
    }
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
        fileReadError(FNS[input_txt], input_txt);
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
                // set print to false and skip the initial comment "/*" signal
                p = 0;
                i += 2;
            }
        }
        // check if this is the end of a comment block
        if (src[i] == '*' && p == 0) {
            if (src[i + 1] == '/') {
                // set print to true and skip the ending comment "*/" signal
                p = 1;
                i += 2;
                // continue, dont print immediatly, check if there are
                // adjacent comment blocks
                continue;
            }
        }
        
        if (p) {
            // print character to file
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
void fileReadError(char fileName[], int writing ){
    
    int i = 1;
    if (writing) {
        i = 0;
    }
    char *str[] = {"writing", "reading"};
    printf("Warning!, Could not open the file %s for %s\n", fileName, str[i]);
    return;
    
}

/*
 *  "int charType(char c)"
 *  return 0, 1, 2, 3 for the character type
 *  0 for neither, 1 for numerical, 2 for letter, 3 for punctuation
 */
int charType(char c){
    // is c a number
    if (isdigit(c)) {
        return 1;
    }
    // is c a letter
    if (isalpha(c)) {
        return 2;
    }
    // is c a punctuation
    if (ispunct(c)) {
        return 3;
    }
    // else default, return 0, is neither of the three types
    return 0;
}

/*
 *   "void splitInputTokens(char cleanSrc[])"
 *   split clean source code into tokens
 */

void splitInputTokens(char cleanSrc[], char *caCleanInputTokens[]){
    
    int i = 0;
    int j = 0;
    //int newTkn = 0;
    char tkn[MAX_STR] = " ";
    //int tokens = 0;
    
    
    while (i <= m_nCleanCount ) {
        
        // if this is a non empty character, store it into local token array
        if (charType(cleanSrc[i])) {
            // increase both token index and cleanSrc index
            tkn[j++] = cleanSrc[i++];
        }
        // check if this is a new line or space (empty character)
        if (charType(cleanSrc[i]) == 0) {
            // if at least one chacter is in local token array, print it and reset token
            if(j) {
                // allocate space for token, store token, increase token count
                caCleanInputTokens[m_nCleanInputTokens] = cleanInputTokenCalloc(j);
                strcpy(caCleanInputTokens[m_nCleanInputTokens], tkn);
                m_nCleanInputTokens++;
                
            }
            // increase cleanSrc index
            i++;
            // reset local token array
            memset(tkn, 0, sizeof(tkn));
            j = 0;
            // skip code beyond here and continue to next character
            continue;
        }
        // if this is a special Symbol punctuation
        if (isSpecialChar(cleanSrc[i])) {
            // if at least one char is in local token
            if (j) {
                // allocate space for token, store token, increase token count
                caCleanInputTokens[m_nCleanInputTokens] = cleanInputTokenCalloc(j);
                strcpy(caCleanInputTokens[m_nCleanInputTokens], tkn);
                m_nCleanInputTokens++;
                
            }
            // reset the token string
            memset(tkn, 0, sizeof(tkn));
            // reset the token index
            j = 0;
        }
        // check next character in the loop after all 3 if cases
        
    }
    
}
/*
 *   "isSpecialChar(char c)"
 *   check if character passed is a special Symbol punctuation
 */
int isSpecialChar(char c){
    int i = 0;
    for (i = 0; i <= MAX_PUNCT ; i++) {
        if (m_caSpecialSymbols[i] == c) {
            return 1;
        }
    }
    return 0;
}
/*
 *   "char *cleanInputTokenCalloc(int tknSize)"
 *   allocate memory for caCleanInputTokens[index] strings
 */
char *cleanInputTokenCalloc(int tknSize){
    char *temp = calloc(tknSize, sizeof(char));
    if (temp == NULL) {
        printf("Error using calloc to create space for token string\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}
/*
 *   "void freeInputTokenCalloc(char *caCleanInputTokens[])"
 *   free memory allocated for clean tokens
 */
void freeInputTokenCalloc(char *caCleanInputTokens[]){
    
    int i = 0;
    // check each array index for non null strings
    for (i = 0; i < m_nCleanInputTokens ; i++) {
        if (caCleanInputTokens[i] != NULL) {
            // free the memory allocated to avoid seg fault
            free(caCleanInputTokens[i] );
        }
        
    }
    
    
}


