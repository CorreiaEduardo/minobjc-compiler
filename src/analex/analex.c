#include "../include/analex.h"

const char BLANK = ' ';
int literalCount = 0;

int findKw(char lexema[]);
int createLiteral(char literal[]);

Token nextToken(FILE *file) {
  printf("--- DEBUG: Scanning next token...");

  int currentState = 0, lexemeLength = 0, numberLength = 0, literalLength = 0;
  char ch = (char) 0;
  char lexeme[LEXEME_MAX_LENGTH] = {'\0'}, number[NUMBER_MAX_LENGTH] = {'\0'}, literal[LT_MAX_LENGTH] = {'\0'};

  Token nextToken;
  while(1) {
    char c = fgetc(file);
    
    if (currentState == 0) {
      if (c == BLANK) continue;
      else if (isalpha(c)) {
        currentState = 1;
        lexeme[lexemeLength] = c;
      }
      else if (isdigit(c)) {
        currentState = 2;
        number[numberLength] = c;
      }
      else if (c == '&') currentState = 7;
      else if (c == '|') currentState = 8;
      else if (c == '!') currentState = 9;
      else if (c == '=') currentState = 33; // state 9 variant
      else if (c == '<') currentState = 34; // state 9 variant
      else if (c == '>') currentState = 35; // state 9 variant
      else if (c == '\'') currentState = 11;
      else if (c == '\"') currentState = 13;
      else if (c == '/') currentState = 28;
      else if (c == '+') {
        currentState = 36; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = PLUS;
        return nextToken;
      }
      else if (c == '-') {
        currentState = 37; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = MINUS;
        return nextToken;
      }
      else if (c == '*') {
        currentState = 38; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = ASTERISK;
        return nextToken;
      }
      else if (c == ';') {
        currentState = 39; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = SEMI_COLON;
        return nextToken;
      }
      else if (c == ':') {
        currentState = 40; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = COLON;
        return nextToken;
      }
      else if (c == '_') {
        currentState = 41; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = UNDERSCORE;
        return nextToken;
      }
      else if (c == ',') {
        currentState = 42; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = COMMA;
        return nextToken;
      }
      else if (c == '.') {
        currentState = 43; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = DOT;
        return nextToken;
      }
      else if (c == '{') {
        currentState = 44; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = OP_BRACES;
        return nextToken;
      }
      else if (c == '}') {
        currentState = 45; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = CL_BRACES;
        return nextToken;
      }
      else if (c == '(') {
        currentState = 46; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = OP_PARENTHESIS;
        return nextToken;
      }
      else if (c == ')') {
        currentState = 47; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = CL_PARENTHESIS;
        return nextToken;
      }
      else if (c == '[') {
        currentState = 48; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = OP_BRACKETS;
        return nextToken;
      }
      else if (c == ']') {
        currentState = 49; // state 3 variant
        nextToken.type = SN;
        nextToken.tableIdx = CL_BRACKETS;
        return nextToken;
      }
    }
    else if (currentState == 1) {
      if (isalpha(c)) {
        currentState = 1;
        lexeme[++lexemeLength] = c;
      } else {
        currentState = 10;

        int keyword = findKw(lexeme);
        if (keyword >= 0) {
          nextToken.type = KW;
          nextToken.tableIdx = keyword;
        } else {
          nextToken.type = ID;
          strcpy(nextToken.lexeme, lexeme);
        }

        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 2) {
      if (isdigit(c)) {
        currentState = 2;
        number[++numberLength] = c;
      }
      else if (c == '.') {
        currentState = 5;
        number[++numberLength] = c;
      }
      else {
        currentState = 14;
        nextToken.type = ICT;
        nextToken.integerVal = atoi(number);
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 5) {
      if (isdigit(c)) {
        currentState = 6;
        number[++numberLength] = c;
      } else {
        error("Unexpected character");
      }
    }
    else if (currentState == 6) {
      if (isdigit(c)) {
        currentState = 6;
        number[++numberLength] = c;
      }
      else {
        currentState = 18;
        nextToken.type = FCT;
        nextToken.floatVal = atof(number);
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 7) {
      if (c == '&') {
        currentState = 15;
        nextToken.type = SN;
        nextToken.tableIdx = DOUBLE_AMP;
        return nextToken;
      } else {
        error("Unexpected character");
      }
    }
    else if (currentState == 8) {
      if (c == '|') {
        currentState = 16;
        nextToken.type = SN;
        nextToken.tableIdx = DOUBLE_PIPE;
        return nextToken;
      } else {
        error("Unexpected character");
      }
    }
    else if (currentState == 9) {
      if (c == '=') {
        currentState = 17;
        nextToken.type = SN;
        nextToken.tableIdx = NOT_EQ;
        return nextToken;
      } else {
        currentState = 27;
        nextToken.type = SN;
        nextToken.tableIdx = EXCLAMATION_MARK;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 33) { // state 9 variant
      if (c == '=') {
        currentState = 43;
        nextToken.type = SN;
        nextToken.tableIdx = DOUBLE_EQ;
        return nextToken;
      } else {
        currentState = 53;
        nextToken.type = SN;
        nextToken.tableIdx = EQ;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 34) { // state 9 variant
      if (c == '=') {
        currentState = 44;
        nextToken.type = SN;
        nextToken.tableIdx = LESS_EQ;
        return nextToken;
      } else {
        currentState = 54;
        nextToken.type = SN;
        nextToken.tableIdx = LESS;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 35) { // state 9 variant
      if (c == '=') {
        currentState = 45;
        nextToken.type = SN;
        nextToken.tableIdx = GREATER_EQ;
        return nextToken;
      } else {
        currentState = 55;
        nextToken.type = SN;
        nextToken.tableIdx = GREATER;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 11) {
      if (isprint(c)) {
        currentState = 19;
        ch = c;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 13) {
      if (isprint(c)) {
        currentState = 20;
        literal[literalLength++] = c;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 19) {
      if (c == '\'') {
        currentState = 12;
        nextToken.type = CCT;
        nextToken.charVal = ch;
        return nextToken;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 20) {
      if (c == '\"') {
        literal[literalLength+1] = '\0';
        currentState = 21;
        nextToken.type = LT;
        nextToken.tableIdx = createLiteral(literal);
        return nextToken;
      }
      else if (isprint(c)) {
        currentState = 20;
        literal[literalLength++] = c;
      }
    }
    else if (currentState == 28) {
      if (c == '*') {
        currentState = 30;
      } else {
        currentState = 29;
        nextToken.type = SN;
        nextToken.tableIdx = SLASH;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 30) {
      if (c == '*') {
        currentState = 31;
      } else continue;
    }
    else if (currentState == 31) {
      if (c == '/') {
        currentState = 0;
      } else {
        currentState = 30;
      }
    }
    if (c == EOF) {
      nextToken.type = EOP;
      return nextToken;
    }
  }

  return nextToken;
}

int findKw(char lexema[]) {
  int i;
  for (i = 1; i < (sizeof(kwTable)) / (sizeof(kwTable[0])) - 1; i++)
  {
    if (stricmp(lexema, kwTable[i]) == 0) return i;
  }

  return -1;
}

int createLiteral(char literal[]) {
  int idx = literalCount++;
  literalTable[idx] = literal;

  return idx;
}

void printToken(Token token) {
  switch (token.type)
  {
    case ID:
      printf("<ID, %s> \n", token.lexeme);
      break;

    case ICT:
      printf("<ICT, %d> \n", token.integerVal);
      break;

    case FCT:
      printf("<FCT, %f> \n", token.floatVal);
      break;

    case CCT:
      printf("<CCT, %c> \n", token.charVal);
      break;

    case LT:
      printf("<LT, %s> \n", literalTable[token.tableIdx]);
      break;

    case KW:
      printf("<KW, %s> \n", kwTable[token.tableIdx]);
      break;

    case SN:
      printf("<SN, %s> \n", signTable[token.tableIdx]);
      break;

    case EOP:
      printf("Fim de arquivo.");
      break;
  }
}
