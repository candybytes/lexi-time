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
// TO-DO write ctype functions, can not include <ctype.h> library
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
#define MAX_WORDS 15                        // define the number of reserved words
#define INVALID_INT 2147483647              // define the long_max int value in case number string is invalid
#define MAX_VAR_LEN 11                      // defines the masx length of a normal variable


//global strings for input output file names
char *LBLS[] = {"lexeme", "token type"};
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
// special punctuation symbols
char m_caSpecialSymbols[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';', ':'};
// special punctuation symbols enumerator values
int m_naSpecialSymbols[] = {plussym, minussym, multsym, slashsym, lparentsym, rparentsym, eqlsym, commasym, periodsym, lessym, gtrsym, semicolonsym, becomessym};







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
int isReserverdWord(char *str);
//----test as of now
void IdentifyInputToken(char *caCleanInputTokens[]); //----test as of now
int stringIsNumber(char *str);
int isValidVariableAndNotReserved(char *str);

//--- Replacement functions for ctype.h. Name changed slightly to avoid conflicting names -----// Added 6/12/2015
// WARNING: PLEASE USE FUNCTION CALL EXACTLY AS SPELLED. All calls within the program have been updated to reflect changes.
int isAlpha(char c);
int isDigit(char c);
int isPunct(char c);


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
    //for (i = 0; i < 0; i++) {
    // use these functions to build lexeme data structure
    // TO-D0 check for symbols groups (i.e >=, <=, < >, :=)
    printf("%-12s%-10s%-10s%-10s\n","lexeme","reserved","numerical","variable");
    for (i = 0; i < m_nCleanInputTokens ; i++) {
        printf("%-12s", caCleanInputTokens[i] );
        printf("%-10d", isReserverdWord(caCleanInputTokens[i]) ? 1 : 0);
        printf("%-10d", stringIsNumber(caCleanInputTokens[i]) );
        printf("%-10d\n", isValidVariableAndNotReserved(caCleanInputTokens[i]) );
        
    }
    printf("token count %d\n\n\n", m_nCleanInputTokens );
    IdentifyInputToken(caCleanInputTokens);
    
    //----------test print end-----------//
    
    ////// TO-D0-----some stuff from list bellow will go here
    
    
    // call freeInputTokenCalloc after finishing the use of the array
    freeInputTokenCalloc(caCleanInputTokens);
    
    ////// TO-D0-----write ctype functions, can not include <ctype.h> library
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
        if ( (charType(cleanSrc[i]) == 0 ) || isSpecialChar(cleanSrc[i])) {
            // if at least one chacter is in local token array, print it and reset token
            if(j) {
                // allocate space for token, store token, increase token count
                caCleanInputTokens[m_nCleanInputTokens] = cleanInputTokenCalloc(j);
                strcpy(caCleanInputTokens[m_nCleanInputTokens], tkn);
                m_nCleanInputTokens++;
                
            }
            
            // reset local token array
            memset(tkn, 0, sizeof(tkn));
            j = 0;
            
            // check if this is a new line or space (empty character)
            if  (charType(cleanSrc[i]) == 0 ){
                // increase cleanSrc index
                i++;
                // skip code beyond here and continue to next character
                continue;
            }
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
            // i+1 will be used to identify enum value later
            return (i + 1);
        }
    }
    return 0;
}
/*
 *   "char *cleanInputTokenCalloc(int tknSize)"
 *   allocate memory for caCleanInputTokens[index] strings
 */
char *cleanInputTokenCalloc(int tknSize){
    char *temp = calloc(tknSize + 1, sizeof(char));
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

/*
 * "int isReserverdWord(char *str)"
 * check is a string passed is a reserved word from the *word[] array
 * if its reserved, it will return the index i + 1, else it will return 0
 */
int isReserverdWord(char *str){
    
    int i = 0;
    for (i = 0; i < MAX_WORDS; i++) {
        if(strcmp(str, word[i]) == 0){
            // i+1 will be used to identify enum value later
            return (i + 1);
        }
    }
    return 0;
    
}

/*
 *   "long stringIsNumber(char *str);"
 *   check if string is numerical, it will not check if its of 5 digits
 *   we need to know if it is a number regardless of legth
 *   a false (false return) would interfere with other checks, i.e. variable check
 */

int stringIsNumber(char *str){
    
    
    int i = 0;
    // base case check for null
    if (str == NULL) {
        return 0;
    }
    // get the string length
    int len = strlen(str);
    // if any of the string characters is not a numerical, then string is not numerical
    // return a defined invalid int value
    for (i = 0; i < len; i++) {
        if (charType(str[i]) != 1) {
            return 0;
        }
    }
    
    // if string is numerical, but has more than 5 digits, exit error
    if (len > 5) {
        printf("Error, string: %s is numerical, but it has more than 5 digits\n", str);
        exit(EXIT_FAILURE);
    }
    
    // if the string is numerical, then return 1
    //strtol(str, (char **)NULL, 10)
    return 1;
    
}
/*
 *   "int isValidVariable(char *str)"
 *   check if string of valid length, does not start with number or symbol
 *   does not contain symbols in the middle, special symbols have been check by now
 */
int isValidVariableAndNotReserved(char *str){
    
    // base case check for null or is a reserved word or is numerical
    if (str == NULL || isReserverdWord(str) || stringIsNumber(str)) {
        return 0;
    }
    
    // get the string length
    int len = strlen(str);
    
    // is token a punctuation or symbol
    if (len == 1) {
        if (charType(str[0]) == 3){
            return 0;
        }
    }
    
    // does variable start with a letter, or is of legal size
    if ((charType(str[0]) != 2) || len > MAX_VAR_LEN ) {
        return 0; //  remove this line later
        /*/ add this section after completing testing of method _____________________
         printf("Error, variable %s is invalid\n", str);
         exit(EXIT_FAILURE);
         */ //_______________________________________________________________________
    }
    
    // at this point, isnumber, is symbol, is reservedword, is special char have all been checked
    
    return 1;
}

void IdentifyInputToken(char *caCleanInputTokens[]){
    
    int ss = 0;
    int rw = 0;
    int tknlen = 0;
    int i = 0;
    
    for (i = 0; i < m_nCleanInputTokens ; i++) {
        
        char *str = caCleanInputTokens[i];
        tknlen = strlen(str);
        
        // check if it is a reserved word
        if ( ( rw = isReserverdWord(str) ) && tknlen > 1 ){
            printf("%-25s %-12s token type: %-4d\n","reserved word lexeme:", str, m_naWsym[(rw - 1)] );
            continue;
        }
        
        // check if token is number
        if  (stringIsNumber(str)){
            printf("%-25s %-12s token type: %-4d\n","number lexeme:", str, 3 );
            continue;
        }
        
        // check if token is variable
        if  (isValidVariableAndNotReserved(str)){
            printf("%-25s %-12s token type: %-4d\n","variable lexeme", str, 2 );
            continue;
        }
        
        // check if token special symbol
        if  ( ( ss = isSpecialChar(str[0]) ) && tknlen == 1 ){
            printf("%-25s %-12s token type: %-4d\n","spec char lexeme:", str, m_naSpecialSymbols[(ss - 1)] );
            continue;
        }
    }
    
    
}

// ctype functions are not being used right now
int isAlpha(char c)
{
    // We are assuming all characters in the program are lowercase as specified by Pawel
    switch(c)
    {
            // Compact notation used for switch cases to save space
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            // Just in case the person grading the program is a total jerk
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return 1;
            break;
            
        default:
            return 0;
            break;
    }
}

// Helper function to replace isdigit() in ctype.h
int isDigit(char c)
{
    switch(c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return 1;
            break;
            
        default:
            return 0;
            break;
    }
}

int isPunct(char c)
{
    switch(c)
    {
        case '.':
        case ',':
        case '!':
        case '?':
        case ';':
            return 1;
            break;
            
        default:
            return 0;
            break;
    }
}






