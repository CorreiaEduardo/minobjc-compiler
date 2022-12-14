#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define LEXEME_MAX_LENGTH 31

/*
* ID: Identificador
* KW: Keyword, ou Palavra Reservada
* SN: Sign, ou Sinal
* ICT: Constante numérica
* FCT: Constante numérica
* CCT: Constante caracter
* LT: Literal, ou cadeia de caracter
* EOP: End of program
* CNULL: CCT com valor igual a '\0'
* CNL: CCT com valor igual a \n
*/
enum TOKEN_TYPE {ID=1, KW, SN, ICT, FCT, CCT, LT, EOP, CNULL, CNL};

enum KEYWORDS {
  KW_CLASS = 1, DATA, CODE, INTERN,
  KW_CHAR, KW_INT, KW_FLOAT, KW_BOOL, KW_VOID, 
  IF, ELSE, WHILE, FOR, 
  RETURN, DELETE, NEW
};

typedef struct {
  enum TOKEN_TYPE type;
  int processed;
  union {
    char lexeme[LEXEME_MAX_LENGTH]; //ID
    int tableIdx; //KW, SN, LT
    int integerVal; // ICT
    float floatVal; // FCT
    char charVal; // CCT
  };
  
} Token;

enum SYMBOL_STEREOTYPE {
  STR_UNKNOWN = 0,
  VAR, // variable
  GFN, // global function
  CFN, // code function
  IFN, // intern function
  SFN, // scoped function implementation
  FIMP, // function implementation
  ARG, // argument
  STR_CLASS, // class stereotype
};

enum SYMBOL_TYPE {
  SB_UNKNOWN = -1,
  SB_VOID = KW_VOID,
  SB_INT = KW_INT,
  SB_CHAR = KW_CHAR,
  SB_FLOAT = KW_FLOAT,
  SB_BOOL = KW_BOOL,
  SB_CLASS = KW_CLASS,
  SB_OBJ = SB_CLASS + 100,
};

static const char * const symbolTypeNames[] = {
  [SB_VOID] = "SB_VOID",
  [SB_INT] =  "SB_INT",
  [SB_CHAR] =  "SB_CHAR",
  [SB_FLOAT] =  "SB_FLOAT",
  [SB_BOOL] =  "SB_BOOL",
  [SB_CLASS] = "SB_CLASS",
  [SB_OBJ] = "SB_OBJ"
};

static const char * const symbolStereotypeNames[] = {
  [STR_UNKNOWN] = "STR_UNKNOWN",
  [VAR] = "VAR",
  [CFN] = "CFN",
  [IFN] = "IFN",
  [ARG] = "ARG",
  [GFN] = "GFN",
  [STR_CLASS] = "STR_CLASS",
  [FIMP] = "FIMP",
  [SFN] = "SFN"
};

typedef struct {
  int addressOffset;
  char name[LEXEME_MAX_LENGTH];
  enum SYMBOL_TYPE type;
  enum SYMBOL_STEREOTYPE stereotype;
  int isArray;
  int isPointer;
  int forceReference;
  int scope; // 0 = global
  char class[LEXEME_MAX_LENGTH]; // classe detendora de uma FN
  char cType[LEXEME_MAX_LENGTH]; // tipo customizado (retorno ou instancia)
} Symbol;

enum PUSH_BEHAVIOR {
  NO_PUSH = 0,
  SBT_PUSH,
  TPT_PUSH
};

// ERROR MANAGEMENT
void error(char msg[]);

// MEM MANAGEMENT
void pushToSymbolTable(Symbol sb);
void popSymbolTable();
Symbol peekSymbolTable();
Symbol *findInSymbolTable(char lexeme[]);
void printSymbolTable();
int existInSymbolTable(char lexeme[], int scope);

void pushToTypeTable(Symbol sb);
void printTypeTable();
int existInTypeTable(char lexeme[], int scope);

int isIdDefined(Token tk);

// GLOBALS
Symbol symbolTable[1000];
Symbol typeTable[1000];
static const int GLOBAL_SCOPE = 0;
static const int LOCAL_SCOPE = 1;
int getSymbolTableTop();
int getTypeTableTop();

#endif /* CORE_H */