#include "../include/analex.h"

const char BLANK = ' ';
const char NEW_LINE = '\n';
const char TAB = '\t';

int literalCount = 0;

int findKw(char lexema[]);
int createLiteral(char literal[]);

Token nextToken(FILE *file) {
  printf("\n--- DEBUG: Scanning next token...");

  int currentState = 0, lexemeLength = 0, numberLength = 0, literalLength = 0;
  char ch = (char) 0;
  char lexeme[LEXEME_MAX_LENGTH] = {'\0'}, number[NUMBER_MAX_LENGTH] = {'\0'}, literal[LT_MAX_LENGTH] = {'\0'};

  Token nextToken;
  nextToken.processed = 0;

  while(1) {
    char c = fgetc(file);
    
    if (currentState == 0) {
      if (c == BLANK || c == NEW_LINE || c == TAB) continue;
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
      else if (c == '=') currentState = 22;
      else if (c == '<') currentState = 23;
      else if (c == '>') currentState = 24;
      else if (c == '\'') currentState = 11;
      else if (c == '\"') currentState = 13;
      else if (c == '/') currentState = 28;
      else if (c == '+') {
        currentState = 3;
        nextToken.type = SN;
        nextToken.tableIdx = PLUS;
        return nextToken;
      }
      else if (c == '-') {
        currentState = 4;
        nextToken.type = SN;
        nextToken.tableIdx = MINUS;
        return nextToken;
      }
      else if (c == '*') {
        currentState = 36;
        nextToken.type = SN;
        nextToken.tableIdx = ASTERISK;
        return nextToken;
      }
      else if (c == ';') {
        currentState = 37;
        nextToken.type = SN;
        nextToken.tableIdx = SEMI_COLON;
        return nextToken;
      }
      else if (c == ':') {
        currentState = 38;
      }
      else if (c == '_') {
        currentState = 39;
        nextToken.type = SN;
        nextToken.tableIdx = UNDERSCORE;
        return nextToken;
      }
      else if (c == ',') {
        currentState = 40;
        nextToken.type = SN;
        nextToken.tableIdx = COMMA;
        return nextToken;
      }
      else if (c == '.') {
        currentState = 41;
        nextToken.type = SN;
        nextToken.tableIdx = DOT;
        return nextToken;
      }
      else if (c == '{') {
        currentState = 42;
        nextToken.type = SN;
        nextToken.tableIdx = OP_BRACES;
        return nextToken;
      }
      else if (c == '}') {
        currentState = 43;
        nextToken.type = SN;
        nextToken.tableIdx = CL_BRACES;
        return nextToken;
      }
      else if (c == '(') {
        currentState = 44;
        nextToken.type = SN;
        nextToken.tableIdx = OP_PARENTHESIS;
        return nextToken;
      }
      else if (c == ')') {
        currentState = 45;
        nextToken.type = SN;
        nextToken.tableIdx = CL_PARENTHESIS;
        return nextToken;
      }
      else if (c == '[') {
        currentState = 46;
        nextToken.type = SN;
        nextToken.tableIdx = OP_BRACKETS;
        return nextToken;
      }
      else if (c == ']') {
        currentState = 47;
        nextToken.type = SN;
        nextToken.tableIdx = CL_BRACKETS;
        return nextToken;
      }
      else if (c == '^') {
        currentState = 53;
        nextToken.type = SN;
        nextToken.tableIdx = CIRCUMFLEX;
        return nextToken;
      }
      else if (c == EOF) {
        nextToken.type = EOP;
        return nextToken;
      }
      else {
        error("Unexpected character");
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
        currentState = 56;
        nextToken.type = SN;
        nextToken.tableIdx = AMP;
        ungetc(c, file);
        return nextToken;
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
    else if (currentState == 22) {
      if (c == '=') {
        currentState = 25;
        nextToken.type = SN;
        nextToken.tableIdx = DOUBLE_EQ;
        return nextToken;
      } else {
        currentState = 33;
        nextToken.type = SN;
        nextToken.tableIdx = EQ;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 23) {
      if (c == '=') {
        currentState = 26;
        nextToken.type = SN;
        nextToken.tableIdx = LESS_EQ;
        return nextToken;
      } else {
        currentState = 34;
        nextToken.type = SN;
        nextToken.tableIdx = LESS;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 24) {
      if (c == '=') {
        currentState = 32;
        nextToken.type = SN;
        nextToken.tableIdx = GREATER_EQ;
        return nextToken;
      } else {
        currentState = 35;
        nextToken.type = SN;
        nextToken.tableIdx = GREATER;
        ungetc(c, file);
        return nextToken;
      }
    }
    else if (currentState == 11) {
      if (c == '\\') {
        currentState = 48;
        ch = c;
      } else if (isprint(c)) {
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
    else if (currentState == 48) {
      if (c == 'n') {
        currentState = 49;
        ch = c;
      } else if (c == '0') {
        currentState = 50;
        ch = c;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 49) {
      if (c == '\'') {
        currentState = 51;
        nextToken.type = CNL;
        return nextToken;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 50) {
      if (c == '\'') {
        currentState = 52;
        nextToken.type = CNULL;
        return nextToken;
      }
      else {
        error("Unexpected character");
      }
    }
    else if (currentState == 38) {
      if (c == ':') {
        currentState = 54;
        nextToken.type = SN;
        nextToken.tableIdx = DOUBLE_COLON;
        return nextToken;
      } else {
        currentState = 55;
        nextToken.type = SN;
        nextToken.tableIdx = COLON;
        ungetc(c, file);
        return nextToken;
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
  for (i = 1; i < (sizeof(kwTable)) / (sizeof(kwTable[0])); i++)
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
      printf("<ID, %s>", token.lexeme);
      break;

    case ICT:
      printf("<ICT, %d>", token.integerVal);
      break;

    case FCT:
      printf("<FCT, %f>", token.floatVal);
      break;

    case CCT:
      printf("<CCT, %c>", token.charVal);
      break;

    case LT:
      printf("<LT, %s>", literalTable[token.tableIdx]);
      break;

    case KW:
      printf("<KW, %s>", kwTable[token.tableIdx]);
      break;

    case SN:
      printf("<SN, %s>", signTable[token.tableIdx]);
      break;

    case CNULL:
      printf("<CNULL>");
      break;

    case CNL:
      printf("<CNL>");
      break;

    case EOP:
      printf("Fim de arquivo.");
      break;
  }
}
