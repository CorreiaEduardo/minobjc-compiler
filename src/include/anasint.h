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

void executeSyntaxAnalysis(FILE *file);
TokenMatcher matches(enum TOKEN_TYPE type, int idx);
Token processNextIf(TokenMatcher matcher);
void syntaxError(char message[]);

void prog(); // prog
void objDef(); // obj_def
void dataSec(); // data_sec
void methodSec(); // meth_sec
void varList(enum PUSH_BEHAVIOR pushBehavior); // decl_list_var
void decl(Symbol *sb); // decl
void varDecl(Symbol *sb); // decl_var
void type(Symbol *sb); // tipo
void paramType(enum PUSH_BEHAVIOR pushBehavior); // tipos_param
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
