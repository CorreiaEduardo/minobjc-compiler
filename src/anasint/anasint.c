#include "../include/anasint.h"

FILE *fd;
Token token;

Token getToken();
void process();

void declFuncAux(Symbol *sb);
void funcPostDoubleColon(Symbol *sb);
void funcPostOpBraces(char funcName[]);
void varDeclAux(Symbol *sb);
void atribAux(char target[]);
void factorPostCircumflex(int circumflexFound);
void atribPostOpBraces(char target[]);

int isCmdStart(Token tk);
int isRelationalOperator(Token tk);
int isExprStart(Token tk);
int isPrimitiveType(Token tk);

Symbol createSymbol();
Symbol createCopy(Symbol sb);

void executeSyntaxAnalysis(FILE *file) {
  fd = file;
  token = nextToken(fd);

  prog();
}

/*----------------------
 |  Non-terminals
 -----------------------*/

void prog() {
  printf("\n--- DEBUG: EXECUTING PROG ROUTINE...");
  getToken();
  while (token.type != EOP)
  {
    if (token.type == KW && token.tableIdx == KW_CLASS) {
      process();
      objDef();
    } else if (isTypeOrVoid(token)) {
      Symbol sb = createSymbol();
      sb.scope = GLOBAL_SCOPE;
      if (isPrimitiveType(token)) {
        sb.type = token.tableIdx;
      } else {
        sb.type = SB_OBJ;
        strcpy(sb.cType, token.lexeme);
      }
      process();
      declFuncAux(&sb);
    } else {
      syntaxError("Class or type expected");
    }
    
    getToken();
  }
  printf("\n--- DEBUG: FINISHED PROG ROUTINE...");

  system("PAUSE");
  printSymbolTable();
  printTypeTable();
}

void objDef() { // Class já foi processado
  printf("\n--- DEBUG: EXECUTING OBJDEF ROUTINE...");
  processNextIf(matches(ID, -1));

  Symbol sb = createSymbol();
  sb.scope = GLOBAL_SCOPE;
  sb.stereotype = STR_CLASS;
  sb.type = SB_CLASS;
  strcpy(sb.name, token.lexeme);
  validateDeclaration(sb);
  pushToTypeTable(sb);

  processNextIf(matches(SN, OP_BRACES));

  dataSec(sb);
  methodSec(sb);

  processNextIf(matches(SN, CL_BRACES));
  printf("\n--- DEBUG: FINISHED OBJDEF ROUTINE...");
}

void dataSec(Symbol originalClass) {
  printf("\n--- DEBUG: EXECUTING DATASEC ROUTINE...");
  processNextIf(matches(KW, DATA));
  processNextIf(matches(SN, COLON));

  do {
    varList(TPT_PUSH);
    processNextIf(matches(SN, SEMI_COLON));
    getToken();
  } while (isTypeOrVoid(token));
  printf("\n--- DEBUG: FINISHED DATASEC ROUTINE...");
}

void methodSec(Symbol originalClass) {
  printf("\n--- DEBUG: EXECUTING METHODSEC ROUTINE...");
  processNextIf(matches(KW, CODE));
  processNextIf(matches(SN, COLON));

  getToken();
  while (isTypeOrVoid(token)) {
    Symbol sb = createSymbol();
    if (isPrimitiveType(token)) {
      sb.type = token.tableIdx;
    } else {
      sb.type = SB_OBJ;
      strcpy(sb.cType, token.lexeme);
    }
    strcpy(sb.class, originalClass.name);
    sb.scope = LOCAL_SCOPE;
    sb.stereotype = CFN;
    funcPrototype(&sb);
    getToken();
  }
  if (token.type == KW && token.tableIdx == INTERN) {
    process();
    processNextIf(matches(SN, COLON));

    getToken();
    while (isTypeOrVoid(token)) {
      Symbol sb = createSymbol();
      if (isPrimitiveType(token)) {
        sb.type = token.tableIdx;
      } else {
        sb.type = SB_OBJ;
        strcpy(sb.class, token.lexeme);
      }
      strcpy(sb.class, originalClass.name);
      sb.scope = LOCAL_SCOPE;
      sb.stereotype = IFN;

      funcPrototype(&sb);
      getToken();
    }
  } else {
    syntaxError("'Intern' expected");
  }
  printf("\n--- DEBUG: FINISHED METHODSEC ROUTINE...");
}

void varList(enum PUSH_BEHAVIOR pushBehavior) {
  printf("\n--- DEBUG: EXECUTING VARLIST ROUTINE...");
  getToken();
  if (!isTypeOrVoid(token)) {
    error("Syntax error");
  }

  Symbol sb = createSymbol();
  sb.scope = LOCAL_SCOPE;
  sb.stereotype = VAR;
  if (isPrimitiveType(token)) {
    sb.type = token.tableIdx;
  } else {
    sb.type = SB_OBJ;
    strcpy(sb.cType, token.lexeme);
  }

  process();
  getToken();

  if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
    sb.isPointer = 1;

    process();
    getToken();
  }

  if (token.type == ID) {
    varDecl(&sb);
    if (pushBehavior == SBT_PUSH) {
      validateDeclaration(sb);
      pushToSymbolTable(sb);
    } else if (pushBehavior == TPT_PUSH) {
      validateDeclaration(sb);
      pushToTypeTable(sb);
    }
    
  } else error("Syntax error");

  getToken();
  while (token.type == SN && token.tableIdx == COMMA) {
    Symbol sb2 = createCopy(sb);

    process();
    getToken();
    if (token.type == ID) {
      varDecl(&sb2);
    } else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
      sb2.isPointer = 1;
      process();
      varDecl(&sb2);
    } else error("Syntax error");

    getToken();
    if (pushBehavior == SBT_PUSH) {
      validateDeclaration(sb2);
      pushToSymbolTable(sb2);
    } else if (pushBehavior == TPT_PUSH) {
      validateDeclaration(sb2);
      pushToTypeTable(sb2);
    }
    
  }
  printf("\n--- DEBUG: FINISHED VARLIST ROUTINE...");
}

void varDecl(Symbol *sb) {
  printf("\n--- DEBUG: EXECUTING VARDECL ROUTINE...");
  processNextIf(matches(ID, -1));
  strcpy(sb->name, token.lexeme);
  varDeclAux(sb);
  printf("\n--- DEBUG: FINISHED VARDECL ROUTINE...");
}

void varDeclAux(Symbol *sb) {
  printf("\n--- DEBUG: EXECUTING VARDECLAUX ROUTINE...");
  getToken();
  if (token.type == SN && token.tableIdx == OP_BRACKETS) {
    process();
    processNextIf(matches(ICT, -1));
    processNextIf(matches(SN, CL_BRACKETS));

    sb->isArray = 1;
  }
  printf("\n--- DEBUG: FINISHED VARDECLAUX ROUTINE...");
}

void paramType(enum PUSH_BEHAVIOR pushBehavior) {
  printf("\n--- DEBUG: EXECUTING PARAMTYPE ROUTINE...");
  getToken();

  while (isTypeOrVoid(token)) {
    Symbol sb = createSymbol();
    sb.stereotype = ARG;
    if (isPrimitiveType(token)) {
      sb.type = token.tableIdx;
    } else {
      sb.type = SB_OBJ;
      strcpy(sb.cType, token.lexeme);
    }
    sb.scope = LOCAL_SCOPE;

    process();
    getToken();

    if (token.type == SN && token.tableIdx == AMP) {
      sb.forceReference = 1;

      process();
      getToken();
      if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
        sb.isPointer = 1;
        process();
      }
      processNextIf(matches(ID, -1));

      strcpy(sb.name, token.lexeme);

      getToken();
    }
    else if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
      sb.isPointer = 1;

      process();
      processNextIf(matches(ID, -1));

      strcpy(sb.name, token.lexeme);

      getToken();
      if (token.type == SN && token.tableIdx == OP_BRACKETS) {
        sb.isArray = 1;

        process();
        processNextIf(matches(SN, CL_BRACKETS));
        getToken();
      }
    } else {
      processNextIf(matches(ID, -1));

      strcpy(sb.name, token.lexeme);

      getToken();

      if (token.type == SN && token.tableIdx == OP_BRACKETS) {
        sb.isArray = 1;

        process();
        processNextIf(matches(SN, CL_BRACKETS));
        getToken();
      }
    }
    
    if (token.type == SN && token.tableIdx == COMMA) {
      process();
      getToken();
      if (!isTypeOrVoid(token)) syntaxError("Type expected");
    }

    if (pushBehavior == SBT_PUSH) {
      validateDeclaration(sb);
      pushToSymbolTable(sb);  
    } else if (pushBehavior == TPT_PUSH) {
      validateDeclaration(sb);
      pushToTypeTable(sb);
    }
  }

  printf("\n--- DEBUG: FINISHED PARAMTYPE ROUTINE...");
}

void declFuncAux(Symbol *sb) { // Tipo já foi processado
  printf("\n--- DEBUG: EXECUTING DECLFUNCAUX ROUTINE...");
  getToken();
  if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
    sb->isPointer = 1;
    process();
  }

  processNextIf(matches(ID, -1));
  strcpy(sb->name, token.lexeme);
  getToken();
  if (token.type == SN && token.tableIdx == DOUBLE_COLON) {
    validateReferenceInTypeTable(sb->name);

    process();
    funcPostDoubleColon(sb);
  } else if (token.type == SN && token.tableIdx == OP_PARENTHESIS) {
    process();
    sb->stereotype = GFN;
    validateDeclaration(*sb);
    pushToSymbolTable(*sb);

    paramType(SBT_PUSH);
    processNextIf(matches(SN, CL_PARENTHESIS));
    getToken();
    if (token.type == SN && token.tableIdx == OP_BRACES) {
      process();
      funcPostOpBraces(sb->name);
    } else if (token.type == SN && token.tableIdx == COMMA) {
      process();
      do {
        processNextIf(matches(ID, -1));

        Symbol sb2 = createCopy(*sb);
        strcpy(sb2.name, token.lexeme);
        validateDeclaration(sb2);
        pushToSymbolTable(sb2);

        processNextIf(matches(SN, OP_PARENTHESIS));
        paramType(SBT_PUSH);
        processNextIf(matches(SN, CL_PARENTHESIS));
        getToken();
      } while (token.type == SN && token.tableIdx == COMMA);
      processNextIf(matches(SN, SEMI_COLON));
    } else {
      processNextIf(matches(SN, SEMI_COLON));
    }
  } else if (token.type == SN && token.tableIdx == COMMA) {
    if (sb->stereotype == 0) {
      sb->stereotype = VAR;
      validateDeclaration(*sb);
      pushToSymbolTable(*sb);
    }

    do {
      Symbol sb2 = createCopy(*sb);
      sb2.stereotype = VAR;
      process();
      getToken();
      if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
        sb2.isPointer = 1;
        process();
      }
      varDecl(&sb2);
      validateDeclaration(sb2);
      pushToSymbolTable(sb2);
    } while (token.type == SN && token.tableIdx == COMMA);
    processNextIf(matches(SN, SEMI_COLON));
  } else if(token.type == SN && token.tableIdx == OP_BRACKETS) {
    sb->stereotype = VAR;
    sb->isArray = 1;
    validateDeclaration(*sb);
    pushToSymbolTable(*sb);

    process();
    processNextIf(matches(ICT, -1));
    processNextIf(matches(SN, CL_BRACKETS));
    processNextIf(matches(SN, SEMI_COLON));
  }
  else {
    if (sb->stereotype == STR_UNKNOWN) {
      sb->stereotype = VAR;
    }

    processNextIf(matches(SN, SEMI_COLON));
    validateDeclaration(*sb);
    pushToSymbolTable(*sb);
  }
  printf("\n--- DEBUG: FINISHED DECLFUNCAUX ROUTINE...");  
}

void funcPostDoubleColon(Symbol *sb) {
  printf("\n--- DEBUG: EXECUTING FUNCPOSTDOUBLECOLON ROUTINE...");
  processNextIf(matches(ID, -1));

  sb->stereotype = SFN;
  strcpy(sb->class, sb->name);
  strcpy(sb->name, token.lexeme);
  pushToSymbolTable(*sb);

  processNextIf(matches(SN, OP_PARENTHESIS));
  paramType(SBT_PUSH);
  processNextIf(matches(SN, CL_PARENTHESIS));
  validateDeclaration(*sb);

  processNextIf(matches(SN, OP_BRACES));
  funcPostOpBraces(NULL);
  printf("\n--- DEBUG: FINISHED FUNCPOSTDOUBLECOLON ROUTINE...");
}

void funcPostOpBraces(char funcName[]) {
  printf("\n--- DEBUG: EXECUTING FUNCPOSTOPBRACES ROUTINE...");

  if (funcName != NULL) {
    Symbol *sb = findInSymbolTable(funcName);
    sb->stereotype = FIMP;
    validateDeclaration(*sb);
  }

  getToken();
  while (isTypeOrVoid(token)) {
    varList(SBT_PUSH);
    processNextIf(matches(SN, SEMI_COLON));
    getToken();
  }

  while (isCmdStart(token)) {
    cmd();
    getToken();
  }

  processNextIf(matches(SN, CL_BRACES));
  printf("\n--- DEBUG: FINISHED FUNCPOSTOPBRACES ROUTINE...");
}

void funcPrototype(Symbol *sb) {
  printf("\n--- DEBUG: EXECUTING FUNCPROTOTYPE ROUTINE...");
  getToken();
  if (!isTypeOrVoid(token)) {
    error("Syntax error");
  }

  if (isPrimitiveType(token)) {
    sb->type = token.tableIdx;
  } else {
    sb->type = SB_OBJ;
    strcpy(sb->class, token.lexeme);
  }

  process();
  getToken();

  if (token.type == SN && token.tableIdx == CIRCUMFLEX) {
    sb->isPointer = 1;
    process();
  }

  processNextIf(matches(ID, -1));

  strcpy(sb->name, token.lexeme);
  validateDeclaration(*sb);
  pushToTypeTable(*sb);

  processNextIf(matches(SN, OP_PARENTHESIS));
  paramType(TPT_PUSH);
  processNextIf(matches(SN, CL_PARENTHESIS));
  processNextIf(matches(SN, SEMI_COLON));
  printf("\n--- DEBUG: FINISHED FUNCPROTOTYPE ROUTINE...");
}

void cmd() {
  printf("\n--- DEBUG: EXECUTING CMD ROUTINE...");
  getToken();
  if (token.type == KW) {
    if (token.tableIdx == IF) {
      process();
      processNextIf(matches(SN, OP_PARENTHESIS));
      expr();
      processNextIf(matches(SN, CL_PARENTHESIS));
      cmd();
      getToken();
      if (token.type == KW && token.tableIdx == ELSE) {
        process();
        cmd();
      }
    } else if (token.tableIdx == WHILE) {
      process();
      processNextIf(matches(SN, OP_PARENTHESIS));
      expr();
      processNextIf(matches(SN, CL_PARENTHESIS));
      cmd();
    } else if (token.tableIdx == FOR) {
      process();
      processNextIf(matches(SN, OP_PARENTHESIS));
      getToken();
      if (token.type == ID) {
        atrib();
      }
      processNextIf(matches(SN, SEMI_COLON));
      getToken();
      if (isExprStart(token)) {
        expr();
      }
      processNextIf(matches(SN, SEMI_COLON));
      getToken();
      if (token.type == ID) {
        atrib();
      }
      processNextIf(matches(SN, CL_PARENTHESIS));
      cmd();
    } else if (token.tableIdx == RETURN) {
      process();
      getToken();
      if (isExprStart(token)) {
        expr();
      }
      processNextIf(matches(SN, SEMI_COLON));
    } else if (token.tableIdx == DELETE) {
      process();
      processNextIf(matches(ID, -1));
      validateReferenceInSymbolTable(token.lexeme);
      validateDeleteCommand(token.lexeme);
      processNextIf(matches(SN, SEMI_COLON));
    }
  } else if (token.type == SN) {
    if (token.tableIdx == CIRCUMFLEX) {
      process();
      processNextIf(matches(ID, -1));
      validateReferenceInSymbolTable(token.lexeme);
      processNextIf(matches(SN, DOT));
      processNextIf(matches(ID, -1));
      validateReferenceInSymbolTable(token.lexeme); // TODO validate function call
      processNextIf(matches(SN, OP_PARENTHESIS));
      getToken();
      if (isExprStart(token)) {
        expr();
        getToken();
        while (token.type == SN && token.tableIdx == COMMA) {
          process();
          expr();
          getToken();
        }
      }
      processNextIf(matches(SN, CL_PARENTHESIS));
      processNextIf(matches(SN, SEMI_COLON));
    } else if (token.tableIdx == OP_BRACES) {
      process();
      getToken();
      while (isCmdStart(token)) {
        cmd();
        getToken();
      }
      processNextIf(matches(SN, CL_BRACES));
    } else if (token.tableIdx == SEMI_COLON) {
      process();
    } else {
      syntaxError("Syntax error: sign expected");
    }
  } else if (token.type == ID) {
    process();
    validateReferenceInSymbolTable(token.lexeme);
    char target[LEXEME_MAX_LENGTH];
    strcpy(target, token.lexeme);
    getToken();
    if (token.type == SN) {
      if (token.tableIdx == DOT){
        process();
        processNextIf(matches(ID, -1));
        processNextIf(matches(SN, OP_PARENTHESIS));
        getToken();
        if (isExprStart(token)) {
          expr();
          getToken();
          while (token.type == SN && token.tableIdx == COMMA) {
            process();
            expr();
            getToken();
          }
        }
        processNextIf(matches(SN, CL_PARENTHESIS));
        processNextIf(matches(SN, SEMI_COLON));
      } else if (token.tableIdx == OP_PARENTHESIS) {
        process();
        getToken();
        if (isExprStart(token)) {
          expr();
          getToken();
          while (token.type == SN && token.tableIdx == COMMA) {
            process();
            expr();
            getToken();
          }
        }
        processNextIf(matches(SN, CL_PARENTHESIS));
        processNextIf(matches(SN, SEMI_COLON));
      } else if (token.tableIdx == OP_BRACES) {
        process();
        atribPostOpBraces(target);
        processNextIf(matches(SN, SEMI_COLON));
      } else if (token.tableIdx == EQ) {
        process();
        atribAux(target);
        processNextIf(matches(SN, SEMI_COLON));
      } else syntaxError("Syntax error: sign expected");
    } else syntaxError("Syntax error: sign expected");
  }
  printf("\n--- DEBUG: FINISHED CMD ROUTINE...");
}

void atrib() {
  processNextIf(matches(ID, -1));
  validateReferenceInSymbolTable(token.lexeme);
  char target[LEXEME_MAX_LENGTH];
  strcpy(target, token.lexeme);
  getToken();
  if (token.type == SN && token.tableIdx == OP_BRACES) {
    process();
    atribPostOpBraces(target);
  }
}

void atribPostOpBraces(char target[]) {//primeiro '[' já processado
  printf("\n--- DEBUG: EXECUTING ATRIB ROUTINE...");
  expr();
  processNextIf(matches(SN, CL_BRACES));
  processNextIf(matches(SN, EQ));
  atribAux(target);
  printf("\n--- DEBUG: FINISHED ATRIB ROUTINE...");
}

void atribAux(char target[]) {//até '=' já foi processado
  printf("\n--- DEBUG: EXECUTING ATRIBAUX ROUTINE...");
  getToken();
  if (token.type == KW && token.tableIdx == NEW) {
    process();
    getToken();
    if (isTypeOrVoid(token)) {
      validateNewCommand(target, token);
      process();
    } else syntaxError("Type expected");
  } else if (isExprStart(token)) {
    expr();
  } else syntaxError("Syntax error: keyword or expression expected");

  printf("\n--- DEBUG: FINISHED ATRIBAUX ROUTINE...");
}

void expr() {
  printf("\n--- DEBUG: EXECUTING EXPR ROUTINE...");
  simpleExpr();
  getToken();
  if (isRelationalOperator(token)) {
    relationalOp();
    simpleExpr();
  }
  printf("\n--- DEBUG: FINISHED EXPR ROUTINE...");
}

void simpleExpr() {
  printf("\n--- DEBUG: EXECUTING SIMPLEEXPR ROUTINE...");
  getToken();
  if (token.type == SN && (token.tableIdx == PLUS || token.tableIdx == MINUS)) {
    process();
  }
  term();
  getToken();
  while (token.type == SN && (token.tableIdx == PLUS || token.tableIdx == MINUS || token.tableIdx == DOUBLE_PIPE)) {
    process();
    term();
    getToken();
  }
  printf("\n--- DEBUG: FINISHED SIMPLEEXPR ROUTINE...");
}

void term() {
  printf("\n--- DEBUG: EXECUTING TERM ROUTINE...");
  factor();
  getToken();
  while (token.type == SN && (token.tableIdx == ASTERISK || token.tableIdx == SLASH || token.tableIdx == DOUBLE_AMP)) {
    process();
    factor();
    getToken();
  }
  printf("\n--- DEBUG: FINISHED TERM ROUTINE...");
}

void factor() {
  printf("\n--- DEBUG: EXECUTING FACTOR ROUTINE...");
  getToken();
  if (token.type == SN) {
    if (token.tableIdx == OP_PARENTHESIS) {
      process();
      expr();
      processNextIf(matches(SN, CL_PARENTHESIS));
    } else if (token.tableIdx == EXCLAMATION_MARK) {
      process();
      factor();
    } else if (token.tableIdx == CIRCUMFLEX) {
      process();
      factorPostCircumflex(1);
    }
  } else if (token.type == ID) {
    factorPostCircumflex(0);
  } else if (token.type == ICT || token.type == FCT || token.type == CCT) {
    process();
  }
  printf("\n--- DEBUG: FINISHED FACTOR ROUTINE...");
}

void factorPostCircumflex(int circumflexFound) {
  char aux[LEXEME_MAX_LENGTH];

  processNextIf(matches(ID, -1));
  validateReferenceInSymbolTable(token.lexeme);
  strcpy(aux, token.lexeme);
  getToken();
  if (token.type == SN) {
    if (token.tableIdx == DOT) {
      process();
      processNextIf(matches(ID, -1));
      validateReferenceInSymbolTable(token.lexeme);
      validateArithmeticFactor(aux, circumflexFound);
      getToken();
      if (token.type == SN) {
        if (token.tableIdx == OP_BRACKETS) {
          process();
          expr();
          processNextIf(matches(SN, CL_BRACKETS));
        }
        else if (token.tableIdx == OP_PARENTHESIS) {
          process();
          getToken();
          if (isExprStart(token)) {
            expr();
            getToken();
            while (token.type == SN && token.tableIdx == COMMA) {
              process();
              expr();
              getToken();
            }
          }
          processNextIf(matches(SN, CL_PARENTHESIS));
        }
      }
    }
    else if (token.tableIdx == OP_BRACKETS) {
      validateArithmeticFactor(aux, circumflexFound);
      process();
      expr();
      processNextIf(matches(SN, CL_BRACKETS));
    } else {
      validateArithmeticFactor(aux, circumflexFound);
    }
  } else {
    validateArithmeticFactor(aux, circumflexFound);
  }
}

void relationalOp() {
  printf("\n--- DEBUG: EXECUTING RELATIONALOP ROUTINE...");
  getToken();
  if (isRelationalOperator(token)) {
    process();
  } else syntaxError("Relational operator expected");

  printf("\n--- DEBUG: FINISHED RELATIONALOP ROUTINE...");
}

/*----------------------
 |  Utility functions
 -----------------------*/

Token getToken() {
  if (token.processed == 1) token = nextToken(fd);
  printf("\n    --- DEBUG: REQUESTED TOKEN: ");
  printToken(token);
  return token;
}

TokenMatcher matches(enum TOKEN_TYPE type, int idx) {
  TokenMatcher matcher;
  matcher.type = type;
  matcher.idx = idx;

  return matcher;
}

Token processNextIf(TokenMatcher matcher) {
  getToken();
  printf("    --- DEBUG: CONDITIONAL PROCESSING WITH: type = %d AND idx = %d", matcher.type, matcher.idx);
  printf("\n    --- DEBUG: TOKEN PROCESSING ATTEMPT: type = %d AND idx = %d", token.type, token.tableIdx);
  if (token.type != matcher.type || (matcher.idx != -1 && token.tableIdx != matcher.idx)) {
    char errorMessage[24];
    switch (matcher.type) {
      case KW:
        snprintf(errorMessage, 24, "\n%s expected", kwTable[matcher.idx]);
        break;

      case SN:
        snprintf(errorMessage, 24, "\n%s expected", signTable[matcher.idx]);
        break;

      default:
        snprintf(errorMessage, 24, "\n%s expected", typeNames[matcher.type]);
        break;
    }
    syntaxError(errorMessage);
  }
  
  process();
}

void process() {
  printf("\n    --- DEBUG: PROCESSED TOKEN: ");
  token.processed = 1;
  printToken(token);
}

void syntaxError(char message[]) {
  char errorMessage[100];
  snprintf(errorMessage, 100, "\n%s", message);
  error(errorMessage);
}

int isPrimitiveType(Token tk) {
  return tk.type == KW && (
    tk.tableIdx == CHAR_TYPE || tk.tableIdx == INT_TYPE || 
    tk.tableIdx == FLOAT_TYPE || tk.tableIdx == BOOL_TYPE || 
    tk.tableIdx == VOID_TYPE
  );
}

int isTypeOrVoid(Token tk) {
  return isPrimitiveType(tk) || isIdDefined(tk);
}

int isCmdStart(Token tk) {
  return (tk.type == KW && (
    tk.tableIdx == IF || tk.tableIdx == WHILE || 
    tk.tableIdx == FOR || tk.tableIdx == RETURN || 
    tk.tableIdx == DELETE))
    || tk.type == ID
    || (tk.type == SN && (tk.tableIdx == OP_BRACKETS || tk.tableIdx == SEMI_COLON || tk.tableIdx == CIRCUMFLEX));
}

int isRelationalOperator(Token tk) {
  return tk.type == SN && (
    tk.tableIdx == DOUBLE_EQ ||
    tk.tableIdx == NOT_EQ ||
    tk.tableIdx == LESS_EQ ||
    tk.tableIdx == LESS ||
    tk.tableIdx == GREATER_EQ ||
    tk.tableIdx == GREATER
  );
}

int isExprStart(Token tk) {
  return (tk.type == ID 
  || (tk.type == SN && (tk.tableIdx == CIRCUMFLEX || tk.tableIdx == PLUS || tk.tableIdx == MINUS || tk.tableIdx == OP_PARENTHESIS || tk.tableIdx == EXCLAMATION_MARK))
  || tk.type == ICT
  || tk.type == FCT
  || tk.type == CCT);
}

/*----------------------
 |  Symbol table related
 -----------------------*/

Symbol createSymbol() {
  Symbol sb;
  sb.type = SB_UNKNOWN;
  sb.stereotype = STR_UNKNOWN;
  sb.forceReference = 0;
  sb.isArray = 0;
  sb.isPointer = 0;

  return sb;
}

Symbol createCopy(Symbol sb) {
  Symbol sb2 = createSymbol();

  sb2.scope = sb.scope;
  sb2.stereotype = sb.stereotype;
  sb2.type = sb.type;

  return sb2;
}
