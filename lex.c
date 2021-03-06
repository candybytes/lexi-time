// TEAM

// First team member name  : Robert Valladares

// Second team member name : Danish Waheed

/**
 *  scanner.c
 *  Lexical Analizer
 *
 *  Created by Robert Valladares and Danish Waheed on 06/01/15.
 *  Copyright (c) 2015 ROBERT VALLADARES and DANISH WAHEED. All rights reserved.
 *  UPDATED CharType functions 6/19/2015
 */

// Library declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
#define MAX_FILES   4 //6                   // Defines the count of files for input output
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

typedef enum {lexConstant = 1, lexVar, lexProc} eLexemeKind;

//structure of the symbol table record
typedef struct {
    
    int TokenType;      // token type
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
char m_caSpecialSymbols[] = {'(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>'};

// special punctuation symbols enumerator values
int m_naSpecialSymbols[] = {lparentsym, rparentsym, multsym, plussym, commasym, minussym, periodsym, slashsym,  becomessym,   semicolonsym, lessym, eqlsym, gtrsym};

// special punctuation ascii values
int m_naSpecSymPunt[] = {40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62};

// --------------ctype enums and structs --------------
typedef enum {AlphaTop = 52, PunctTop = 33, NumberTop = 10, SpecialCharTop = 13} eCharTops;

int m_naAlphaChars[] = {65, 66,67,68,69,70,71,72,73,74,
    75,76,77,78,79,80,81,82,83,84,
    85,86,87,88,89,90,97,98,99,100,
    101,102,103,104,105,106,107,108,109,110,
    111,112,113,114,115,116,117,118,119,120,
    121,122};

int m_naPunctChars[] = {9, 33,34,35,36,37,38,39,40,41,42,
    43,44,45,46,47, 58,59,60, 61,62,
    63,64,91,92,93,94,95,96,123,124,
    125,126};

int m_naNumericalChars[] = {48,49,50,51,52,53,54,55,56,57};


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
void IdentifyInputToken(char *caCleanInputTokens[], namerecord_t *record_table);
int stringIsNumber(char *str);
int isValidVariableAndNotReserved(char *str);
int validSymbolPair(char c1, char c2);
void initializeNamerecord_table(namerecord_t *record_table);
void insertNamerecord_table(int RecordIndex, namerecord_t *record_table, int tknKind, int ComboChars, char c1, char c2, char *lexeme, int tknType);
void printNamerecord_table(namerecord_t *record_table);

//--- Replacement functions for <ctype.h> Name changed slightly to avoid conflicting names -----// Added 6/12/2015
int isAlpha(char c);
int isDigit(char c);
int isPunct(char c);
int binarySearch (int *Array, int top, int target);


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
    
    // there will be at most m_nCleanCount separate namerecord_t tokens
    namerecord_t namerecord_table[m_nCleanCount];
    initializeNamerecord_table(namerecord_table);
    
    // identify what kind of lexeme each token is
    IdentifyInputToken(caCleanInputTokens, namerecord_table);
    
    // print each record to both lexemelist.txt and lexemetable.txt
    printNamerecord_table(namerecord_table);
    
    // call freeInputTokenCalloc after finishing the use of the array
    freeInputTokenCalloc(caCleanInputTokens);
    
    // program finished, if no error were found, it will reach this point
    printf("\nYou can find the input/ output files: \n");
    for (i = 0; i < MAX_FILES; i++) {
        // print the file names that were used and created
        printf("%s \n", FNS[i]);
        
    }
    printf("in the same folder where scanner.c is located \n\n");
    
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

/**
 * "cleanInput(FILE *fp, char src[], int count, char cleanSrc[])"
 * remove the comments from the input code
 * print valid
 *
 */
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
    
    int result =0;
    int target = (int)c;
    
    if (target > 9) {
        if ( (result = isDigit( c )) || (result = isPunct( c ) ) || ( result = isAlpha( c ) ) ){
            // if any is true, then result will have the corresponding value already
            // result = 1 for numerical, result = 2 for letter, result = 3 for punctuation
            return (result);
        }
    }
    
    // else default, return 0, is neither of the three types
    return 0;
}

/*
 *   "void splitInputTokens(char cleanSrc[])"
 *   split clean source code into tokens
 */
void splitInputTokens(char cleanSrc[], char *caCleanInputTokens[]){
    
    int curCharIsSpecial = 0;
    int i = 0;
    int j = 0;
    char tkn[MAX_STR] = " ";
    
    while (i <= m_nCleanCount ) {
        
        // if this is a non empty character, store it into local token array
        if (charType(cleanSrc[i])) {
            // increase both token index and cleanSrc index
            
            // check if this character is a special character, if it is
            //mark a flag and print it, reset token in next code block
            curCharIsSpecial = isSpecialChar(cleanSrc[i]);
            
            tkn[j++] = cleanSrc[i++];
            
        }
        // check if this is a new line or space (empty character)
        if ( isSpecialChar(cleanSrc[i]) ||(charType(cleanSrc[i]) == 0 ) || curCharIsSpecial ) {
            // if at least one chacter is in local token array, print it and reset token
            //printf("clean source %c\n", cleanSrc[i] );
            if(j) {
                // allocate space for token, store token, increase token count
                //printf("tkn %s\n",tkn);
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
    
    // transform char into number
    int target = (int)c;
    int result = 0;
    
    // ascii codes for special punctuation chars are from 40 and beyond
    return ( (target > 39) && (result = binarySearch (m_naSpecSymPunt, SpecialCharTop, target)) ) ? result : 0;
    
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
        //return 0; //  remove this line later
        
        printf("Error, variable %s is invalid\n", str);
        exit(EXIT_FAILURE);
    }
    
    // at this point, isnumber, is symbol, is reserved word, is special char have all been checked
    
    return 1;
}

/*
 *  IdentifyInputToken(char *caCleanInputTokens[])
 *  take each token and check if its a reserved word, numerical or symbol pail, or variable
 *  if token is an illegal lexeme, print out error and exit program
 *
 */
void IdentifyInputToken(char *caCleanInputTokens[], namerecord_t *record_table){
    
    int ssNext = 0;
    int ss = 0;
    int rw = 0;
    int tknlen = 0;
    int tknLenNext = 0;
    int i = 0;
    int vs = 0;
    int LexRecordIndex = 0; // keep track of the lexeme table records
    
    for (i = 0; i < m_nCleanInputTokens ; i++) {
        
        char *str = caCleanInputTokens[i];
        tknlen = strlen(str);
        // typedef enum {lexConstant = 1, lexVar, lexProc} eLexemeKind;
        // [integer = constant = 1], [variable = var = 2], [reserved word = proc = 3]
        
        // check if it is a reserved word
        if ( ( rw = isReserverdWord(str) ) && tknlen > 1 ){
            // insert token record in record_table
            insertNamerecord_table(LexRecordIndex++, record_table, lexProc, 0, ' ', ' ', str, m_naWsym[(rw - 1)] );
            
            continue;
        }
        
        // check if token is number
        if  (stringIsNumber(str)){
            // if is invalid length or illegal number, it will fail at check
            
            // insert token record in record_table
            insertNamerecord_table(LexRecordIndex++, record_table, lexConstant, 0, ' ', ' ', str, 3 );
            
            continue;
        }
        
        // check if token is variable
        if  (isValidVariableAndNotReserved(str)){
            // if is invalid length or illegal variable, it will fail at check
            
            // insert token record in record_table
            insertNamerecord_table(LexRecordIndex++, record_table, lexVar, 0, ' ', ' ', str, 2 );
            
            continue;
        }
        
        // check if token special symbol
        if  ( ( ss = isSpecialChar(str[0]) ) && tknlen == 1 ){
            
            // current token is a symbol, check if next token is symbol
            // do not increase index counter, just look ahead
            if (i < m_nCleanInputTokens -1){
                char *strNext = caCleanInputTokens[i + 1];
                tknLenNext = strlen(str);
                if ( strlen(strNext) == 1 && isSpecialChar(strNext[0])) {
                    // next token is of length 1, is a symbol
                    // check if they are a legal symbol pair : >=, <=, < >, :=
                    if ((vs = validSymbolPair(str[0], strNext[0])) ) {
                        i++;
                        
                        // insert token record in record_table
                        insertNamerecord_table(LexRecordIndex++, record_table, lexProc, 1, str[0], strNext[0], " ", vs );
                        
                        continue;
                    }
                }
            }
            
            // insert token record in record_table
            insertNamerecord_table(LexRecordIndex++, record_table, lexProc, 0, ' ', ' ', str, m_naSpecialSymbols[(ss - 1)]  );
            
            continue;
        }
    }
    
}

/*
 *  "validSymbolPair(char c1, char c2)"
 *  it will take current character c1, and next char c2
 *  check if the combo of both is a legal lexime >=, <=, :=, < >
 *  it it is legal lexeme, it returns the lexeme value for the combo
 *  if it is illegal, is a fatal error and exit program
 */
int validSymbolPair(char c1, char c2){
    // c1, is current symbol, c2 is next symbol
    // check if they are a legal symbol pair : >=, <=, :=, < >
    
    int r = 0;
    
    switch (c2) {
        case '=':
            if (r += ( (c1 == '>') ? geqsym : 0 )) { break; }
            if (r += ( (c1 == '<') ? leqsym : 0 )) { break; }
            if (r += ( (c1 == ':') ? becomessym : 0 )) { break; }
            break;
            
        case '>':
            if (r += ( (c1 == '<') ? neqsym : 0 )) { break; }
            break;
            
        default:
            break;
    }
    
    if (r) {
        return r;
    } else {
        printf("the symbol pair is ilegal %c%c\n",c1,c2);
        exit(EXIT_FAILURE);
    }
}

//----------------------ctype functions ---- 1 for numerical, 2 for letter, 3 for punctuation

/*
 *  binarySearch (int *Array, int top, int target)
 *  search for a char c ascii value, if it is in list of values in Array passed
 *  return 1, else is not found return 0;
 */
int binarySearch (int *Array, int top, int target) {
    // Given a ascending sorted Array[0..top],
    // Return 1 such that Array[i] = target.
    // Return 0 if target is not in Array.
    
    int lower, middle, upper;
    lower = 0;
    upper = top;
    
    while ( upper >= lower  ) {
        middle = ( upper + lower ) / 2;
        if  (Array[middle] == target){
            return( middle + 1 );
        }
        if (Array[middle] < target){
            lower = middle + 1;
        }
        else { upper = middle - 1;}
    };
    
    // not found.
    return( 0 );
}


/*
 *  isDigit(char c)
 *  return 1 if char is a digit, 0 if not
 */
int isDigit(char c){
    // transform char into number
    int target = (int)c;
    // ascii codes for numerical chars are from 48 to 57
    return ((target > 47) && (binarySearch (m_naNumericalChars, NumberTop, target)) ) ? 1 : 0;
    
}

/*
 *  isAlpha(char c)
 *  return 2 if char is an alpha, 0 if not
 */
int isAlpha(char c){
    // transform char into number
    int target = (int)c;
    // ascii codes for alpha chars are from 65 forward
    return  ( ( (target > 64) && ( binarySearch (m_naAlphaChars, AlphaTop, target) ) ) ? 2 : 0 );
    
}

/*
 *  isPunct(char c)
 *  return 3 if char is an punctuation, 0 if not
 */
int isPunct(char c){
    // transform char into number
    int target = (int)c;
    // ascii codes for alpha chars are from [ 9 ] & 65 forward
    return ( ( (target > 9) && ( binarySearch (m_naPunctChars, PunctTop, target) ) ) ? 3 : 0 );
    
}

/*
 *  initializeNamerecord_table(namerecord_t *record_table)
 *  set the default values for the namerecord_t record_table
 *  check that strcpy was valid for the char arrray
 */
void initializeNamerecord_table(namerecord_t *record_table){
    // initialize tokens record table
    int i = 0;
    for (i = 0; i < m_nCleanCount ; i++) {
        //record_table[i].TokenType = 0;
        record_table[i].kind = 0;           // constant = 1; var = 2, proc = 3
        if ( strcpy(record_table[i].name, "") == NULL) {
            printf("Error: fail creating space to store token string\n");
            exit(EXIT_FAILURE);
        };                                  // name up to 11 characters long, 11 + 1 for \0
        record_table[i].val = 0;            // number (ASCII value)
        record_table[i].level = 0;          // L level
        record_table[i].adr = 0;            // M address
    }
    return;
    
}

/*
 *  insertNamerecord_table(.....)
 *  enter one record at a time as being passed from "IdentifyInputToken(...)"
 *
 */
void insertNamerecord_table(int RecordIndex, namerecord_t *record_table, int tknKind, int ComboChars, char c1, char c2, char *lexeme, int tknType){
    int i = RecordIndex;
    // tknKind are >= 1
    // typedef enum {lexConstant = 1, lexVar, lexProc} eLexemeKind;
    if (tknKind) {
        
        record_table[i].kind = tknKind;           // constant = 1; var = 2, proc = 3
        if (ComboChars ) {
            record_table[i].name[0] = c1;
            record_table[i].name[1] = c2;
            record_table[i].name[2] = '\0';
        } else {
            if ( strcpy(record_table[i].name, lexeme) == NULL) {
                printf("Error: fail creating space to store token string: %s \n", lexeme);
                exit(EXIT_FAILURE);
            };
        }                                   // name up to 11 characters long, 11 + 1 for \0
        record_table[i].val = tknType;      // number (ASCII value)
        record_table[i].level = 0;          // L level
        record_table[i].adr = 0;            // M address
    }
    return;
    
}

/*
 * "printNamerecord_table(namerecord_t *record_table)"
 *  print each record from the table to lexemetable and lexemelist
 */
void printNamerecord_table(namerecord_t *record_table){
    
    
    // copy lexemetable file pointer
    FILE *lexTable = m_FPS[lexemetable_txt];
    // copy lexemelist file pointer
    FILE *lexList = m_FPS[lexemelist_txt];
    
    int i = 0;
    
    if (record_table != NULL) {
        
        // print headers to screen
        printf("\n\n%-12s%s\n", LBLS[0], LBLS[1]);
        
        // print headers to lexemetable.txt file
        fprintf( lexTable, "%-12s%s\n", LBLS[0], LBLS[1] );
        
        // print each lexeme record from record_table
        for (i = 0; i < m_nCleanCount; i++) {
            if (record_table[i].kind) {
                // print lexeme to screen
                printf("%-12s%d\n",record_table[i].name, record_table[i].val );
                // print lexeme to lexemetable.txt file
                fprintf( lexTable, "%-12s%d\n", record_table[i].name, record_table[i].val );
                
                // print lexeme list
                fprintf( lexList, "%d ", record_table[i].val );
                // if lexeme is either a variable or a constant, print the variable or the constant right after the token type
                if ( (record_table[i].val == 2) || (record_table[i].val == 3) ){
                    fprintf( lexList, "%s ", record_table[i].name );
                }
                
            }
        }
    }
    
    // close the -lexemetable.txt- file pointer
    fclose(lexTable);
    // close the -lexemelist.txt- file pointer
    fclose(lexList);
    
    return;
}














