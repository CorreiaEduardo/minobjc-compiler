#include "../include/analex.h"

typedef struct {
  enum TOKEN_TYPE type;
  int idx;
} TokenMatcher;

enum VAR_TYPE {
  CHAR_TYPE = KW_CHAR,
  INT_TYPE = KW_INT,
  FLOAT_TYPE = KW_FLOAT,
  BOOL_TYPE = KW_BOOL,
  VOID_TYPE = KW_VOID,
};

enum SYMBOL_STEREOTYPE {
  STR_UNKNOWN = 0,
  VAR=1, // variable
  CFN, // code function
  IFN, // intern function
  ARG, // argument
  GFN, // global function
  STR_CLASS, // class stereotype
};

enum SYMBOL_TYPE {
  SB_UNKNOWN = -1,
  SB_VOID = KW_VOID,
  SB_INT = KW_INT,
  SB_CHAR = KW_CHAR,
  SB_FLOAT = KW_FLOAT,
  SB_BOOL = KW_BOOL,
  SB_CLASS = KW_CLASS
};
typedef struct {
  int addressOffset;
  char name[LEXEME_MAX_LENGTH];
  int scope; // 0 = global
  enum SYMBOL_TYPE type;
  enum SYMBOL_STEREOTYPE stereotype;
  int isArray;
  int isPointer;
  int forceReference;
} Symbol;

void executeSyntaxAnalysis(FILE *file);
TokenMatcher matches(enum TOKEN_TYPE type, int idx);
Token processNextIf(TokenMatcher matcher);
void syntaxError(char message[]);

void prog(); // prog
void objDef(); // obj_def
void dataSec(); // data_sec
void methodSec(); // meth_sec
void varList(); // decl_list_var
void decl(Symbol *sb); // decl
void varDecl(Symbol *sb); // decl_var
void type(Symbol *sb); // tipo
void paramType(); // tipos_param
void func(Symbol *sb); // func
void funcPrototype(Symbol *sb); // func_prot
void scope(Symbol *sb); // escopo
void cmd(); // cmd

void atrib(); // atrib
void expr(); // expr
void simpleExpr(); // expr_simp
void term(); // termo
void factor(); // fator
void relationalOp(); // op_rel

// GLOBALS
Symbol symbolTable[1000];

static const char * const symbolTypeNames[] = {
  [SB_VOID] = "SB_VOID",
  [SB_INT] =  "SB_INT",
  [SB_CHAR] =  "SB_CHAR",
  [SB_FLOAT] =  "SB_FLOAT",
  [SB_BOOL] =  "SB_BOOL",
  [SB_CLASS] = "SB_CLASS"
};