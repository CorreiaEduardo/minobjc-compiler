#include "../include/anasint.h"

FILE *fd;
Token token;

Token getToken();
void process();

void executeSyntaxAnalysis(FILE *file) {
  fd = file;
  token = nextToken(fd);

  prog();
}

void prog() {
  printf("\n--- DEBUG: EXECUTING PROG ROUTINE...");
  getToken();
  while (token.type != EOP)
  {
    if (token.type == KW && token.tableIdx == CLASS) {
      process();
      objDef();
    }

    // TODO decl';' | func
    getToken();
  }
  printf("\n--- DEBUG: FINISHED PROG ROUTINE...");
}

// Class já foi processado
void objDef() {
  printf("\n--- DEBUG: EXECUTING OBJDEF ROUTINE...");
  processNextIf(matches(ID, -1));
  processNextIf(matches(SN, OP_BRACES));

  dataSec();
  methodSec();

  processNextIf(matches(SN, CL_BRACES));
  printf("\n--- DEBUG: FINISHED OBJDEF ROUTINE...");
}

void dataSec() {
  printf("\n--- DEBUG: EXECUTING DATASEC ROUTINE...");
  processNextIf(matches(KW, DATA));
  processNextIf(matches(SN, COLON));

  do {
    varList();
    processNextIf(matches(SN, SEMI_COLON));
    getToken();
  } while (isTypeOrVoid(token));
  printf("\n--- DEBUG: FINISHED DATASEC ROUTINE...");
}

void methodSec() {
  printf("\n--- DEBUG: EXECUTING METHODSEC ROUTINE...");
  processNextIf(matches(KW, CODE));
  processNextIf(matches(SN, COLON));

  getToken();
  while (isTypeOrVoid(token)) {
    funcPrototype();
    getToken();
  }
  if (token.type == KW && token.tableIdx == INTERN) {
    process(token);
    processNextIf(matches(SN, COLON));

    getToken();
    while (isTypeOrVoid(token)) {
      funcPrototype();
      getToken();
    }
  } else {
    syntaxError("Syntax error");
  }
  printf("\n--- DEBUG: FINISHED METHODSEC ROUTINE...");
}

void varList() {
  printf("\n--- DEBUG: EXECUTING VARLIST ROUTINE...");
  getToken();
  if (!isTypeOrVoid(token)) {
    error("Syntax error");
  }
  process(token);
  getToken();

  if (token.type == ID) {
    varDecl();
  } else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
    process(token);
  } else error("Syntax error");

  getToken();
  while (token.type == SN && token.tableIdx == COMMA) {
    process(token);
    if (token.type == ID) {
      varDecl();
    } else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
      process(token);
    } else error("Syntax error");

    getToken();
  }
  printf("\n--- DEBUG: FINISHED VARLIST ROUTINE...");
}

void decl() {

}

void varDecl() {
  printf("\n--- DEBUG: EXECUTING VARDECL ROUTINE...");
  processNextIf(matches(ID, -1));
  getToken();
  if (token.type == SN && token.tableIdx == OP_BRACES) {
    process(token);
    processNextIf(matches(ICT, -1));
    processNextIf(matches(SN, CL_BRACES));
  }
  printf("\n--- DEBUG: FINISHED VARDECL ROUTINE...");
}

void type() {

}

void paramType() {
  printf("\n--- DEBUG: EXECUTING PARAMTYPE ROUTINE...");
  getToken();

  while (isTypeOrVoid(token)) {
    process();
    getToken();

    if (token.type == SN && token.tableIdx == AMP) {
      process();
      getToken();
      if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
        process();
      }
      processNextIf(matches(ID, -1));
      getToken();
    }
    else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
      process();
      processNextIf(matches(ID, -1));

      getToken();
      if (token.type == SN && token.tableIdx == OP_BRACKETS) {
        process();
        processNextIf(matches(SN, CL_BRACKETS));
        getToken();
      }
    } else {
      processNextIf(matches(ID, -1));
      getToken();

      if (token.type == SN && token.tableIdx == OP_BRACKETS) {
        process();
        processNextIf(matches(SN, CL_BRACKETS));
        getToken();
      }
    }
    
    if (token.type == SN && token.tableIdx == COMMA) {
      process();
      getToken();
      if (!isTypeOrVoid(token)) syntaxError("Syntax error");
    }
  }

  printf("\n--- DEBUG: FINISHED PARAMTYPE ROUTINE...");
}

void func() {

}

void funcPrototype() {
  printf("\n--- DEBUG: EXECUTING FUNCPROTOTYPE ROUTINE...");
  getToken();
  if (!isTypeOrVoid(token)) {
    error("Syntax error");
  }
  process(token);
  getToken();

  if (token.type == ID) {
    process(token);
  } else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
    process(token);
  } else error("Syntax error");

  processNextIf(matches(SN, OP_PARENTHESIS));
  paramType();
  processNextIf(matches(SN, CL_PARENTHESIS));
  processNextIf(matches(SN, SEMI_COLON));
  printf("\n--- DEBUG: FINISHED FUNCPROTOTYPE ROUTINE...");
}

void scope() {

}

void cmd() {

}


void atrib() {

}

void expr() {

}

void simpleExpr() {

}

void term() {

}

void factor() {

}

void relationalOp() {

}

Token getToken() {
  if (token.processed == 1) token = nextToken(fd);
  printf("\n    --- DEBUG: REQUESTED TOKEN: ");
  printToken(token);
  return token;
}

Token processNextIf(TokenMatcher matcher) {
  getToken();
  printf("    --- DEBUG: CONDITIONAL PROCESSING WITH: type = %d AND idx = %d", matcher.type, matcher.idx);
  printf("\n    --- DEBUG: TOKEN PROCESSING ATTEMPT: type = %d AND idx = %d", token.type, token.tableIdx);
  if (token.type != matcher.type || (matcher.idx != -1 && token.tableIdx != matcher.idx))
  {
    syntaxError(matcher.type);
  }
  
  process();
}

TokenMatcher matches(enum TOKEN_TYPE type, int idx) {
  TokenMatcher matcher;
  matcher.type = type;
  matcher.idx = idx;

  return matcher;
}

void syntaxError(enum TOKEN_TYPE type) {
  char errorMessage[24];
  snprintf(errorMessage, 24, "\n%s expected", typeNames[type]);
  error(errorMessage);
}

int isTypeOrVoid(Token tk) {
  return tk.type == KW && (
    tk.tableIdx == CHAR_TYPE || tk.tableIdx == INT_TYPE || 
    tk.tableIdx == FLOAT_TYPE || tk.tableIdx == BOOL_TYPE || 
    tk.tableIdx == VOID_TYPE
  );
}

void process() {
  printf("\n    --- DEBUG: PROCESSED TOKEN: ");
  token.processed = 1;
  printToken(token);
}