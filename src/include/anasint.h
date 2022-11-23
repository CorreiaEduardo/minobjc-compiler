#include "../include/analex.h"

#define SYMBOL_ARRAY_OFFSET 31
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
  VAR=1, // variable
  CFN, // function
  IFN, // intern function
  ARG, // argument
};

enum SYMBOL_TYPE {
  SB_INT = KW_INT,
  SB_CHAR = KW_CHAR,
  SB_FLOAT = KW_FLOAT,
  SB_BOOL = KW_BOOL,
  SB_INT_ARRAY = KW_INT + SYMBOL_ARRAY_OFFSET,
  SB_CHAR_ARRAY = KW_CHAR + SYMBOL_ARRAY_OFFSET,
  SB_FLOAT_ARRAY = KW_FLOAT + SYMBOL_ARRAY_OFFSET,
  SB_BOOL_ARRAY = KW_BOOL + SYMBOL_ARRAY_OFFSET
};
typedef struct {
  int addressOffset;
  char name[LEXEME_MAX_LENGTH];
  int scope; // 0 = global
  enum SYMBOL_TYPE type;
  enum SYMBOL_STEREOTYPE stereotype;
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
void decl(); // decl
void varDecl(); // decl_var
void type(); // tipo
void paramType(); // tipos_param
void func(); // func
void funcPrototype(); // func_prot
void scope(); // escopo
void cmd(); // cmd

void atrib(); // atrib
void expr(); // expr
void simpleExpr(); // expr_simp
void term(); // termo
void factor(); // fator
void relationalOp(); // op_rel

// GLOBALS
const Symbol *symbolTable[1000];
