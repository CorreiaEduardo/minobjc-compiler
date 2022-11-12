#include "../include/analex.h"

typedef struct {
  enum TOKEN_TYPE type;
  int idx;
} TokenMatcher;

enum VAR_TYPE {
  CHAR_TYPE = CHAR,
  INT_TYPE = INT,
  FLOAT_TYPE = FLOAT,
  BOOL_TYPE = BOOL,
  VOID_TYPE = VOID,
};

void executeSyntaxAnalysis(FILE *file);

TokenMatcher matches(enum TOKEN_TYPE type, int idx);

Token processNextIf(TokenMatcher matcher);

void syntaxError();

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
