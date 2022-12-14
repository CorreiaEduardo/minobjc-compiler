#include "../include/analex.h"
#include "../include/anasem.h"

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
void dataSec(Symbol originalClass); // data_sec
void methodSec(Symbol originalClass); // meth_sec
void varList(Symbol *originalClass, enum PUSH_BEHAVIOR pushBehavior); // decl_list_var
void varDecl(Symbol *sb); // decl_var
void paramType(enum PUSH_BEHAVIOR pushBehavior); // tipos_param
void funcPrototype(Symbol *sb); // func_prot
void cmd(char classScope[]); // cmd
void atrib(char classScope[]); // atrib
void expr(char classScope[]); // expr
void simpleExpr(char classScope[]); // expr_simp
void term(char classScope[]); // termo
void factor(char classScope[]); // fator
void relationalOp(); // op_rel
