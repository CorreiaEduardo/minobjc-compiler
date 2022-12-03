#include "../include/core.h"

#ifndef ANALEX_H

#define ANALEX_H
#define NUMBER_MAX_LENGTH 1000
#define LT_MAX_LENGTH 1000

/*
* ALPHABET
* min_letter = a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
* up_letter = A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
* letter = min_letter | up_letter
* number = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
* escaped_char = \n, \0
* symbol = ; : _ , . {  } (  ) [  ] +  -  *  /  &&  || !  ==  !=  <=  <  >=  >
*/

enum SIGNS {
  SEMI_COLON = 1, COLON, UNDERSCORE, // ;, :, _
  S_QUOTE, D_QUOTE, // ', "
  COMMA, DOT,  // , e .
  OP_BRACES, CL_BRACES, // {, }
  OP_PARENTHESIS, CL_PARENTHESIS, // (, )
  OP_BRACKETS, CL_BRACKETS, // [, ]
  PLUS, MINUS, ASTERISK, SLASH, DOUBLE_AMP, DOUBLE_PIPE, CIRCUMFLEX, DOUBLE_COLON, // +, -, *, /, &&, ||, ^, ::
  EXCLAMATION_MARK, DOUBLE_EQ, NOT_EQ, LESS_EQ, LESS, GREATER_EQ, GREATER,  // !, ==, !=, <=, <, >=, >
  AMP, PIPE, EQ // & | =
};

static const char * const typeNames[] = {
  [ID] = "ID",
  [KW] = "KEYWORD",
  [SN] = "SIGN",
  [ICT] = "INTEGER_CT",
  [FCT] = "FLOAT_CT",
  [CCT] = "CHAR_CT",
  [LT] = "LITERAL",
  [EOP] = "EOP",
};

static const char * const kwTable[] = {
  [KW_CLASS] = "CLASS",
  [DATA] = "DATA",
  [CODE] = "CODE",
  [KW_CHAR] = "CHAR",
  [KW_INT] = "INT",
  [KW_FLOAT] = "FLOAT",
  [KW_BOOL] = "BOOL",
  [KW_VOID] = "VOID",
  [IF] = "IF",
  [ELSE] = "ELSE",
  [WHILE] = "WHILE",
  [FOR] = "FOR",
  [RETURN] = "RETURN",
  [DELETE] = "DELETE",
  [NEW] = "NEW",
  [INTERN] = "INTERN"
};

static const char * const signTable[] = {
  [SEMI_COLON] = "SEMI_COLON",
  [COLON] = "COLON",
  [UNDERSCORE] = "UNDERSCORE",
  [S_QUOTE] = "S_QUOTE",
  [D_QUOTE] = "D_QUOTE",
  [COMMA] = "COMMA",
  [DOT] = "DOT",
  [OP_BRACES] = "OP_BRACES",
  [CL_BRACES] = "CL_BRACES",
  [OP_PARENTHESIS] = "OP_PARENTHESIS",
  [CL_PARENTHESIS] = "CL_PARENTHESIS",
  [OP_BRACKETS] = "OP_BRACKETS",
  [CL_BRACKETS] = "CL_BRACKETS",
  [PLUS] = "PLUS",
  [MINUS] = "MINUS",
  [ASTERISK] = "ASTERISK",
  [SLASH] = "SLASH",
  [DOUBLE_AMP] = "DOUBLE_AMP",
  [DOUBLE_PIPE] = "DOUBLE_PIPE",
  [CIRCUMFLEX] = "CIRCUMFLEX",
  [DOUBLE_COLON] = "DOUBLE_COLON",
  [EXCLAMATION_MARK] = "EXCLAMATION_MARK",
  [DOUBLE_EQ] = "DOUBLE_EQ",
  [NOT_EQ] = "NOT_EQ",
  [LESS_EQ] = "LESS_EQ",
  [LESS] = "LESS",
  [GREATER_EQ] = "GREATER_EQ",
  [GREATER] = "GREATER",
  [AMP] = "AMP",
  [PIPE] = "PIPE",
  [EQ] = "EQ",
};

// GLOBALS
const char *literalTable[1000];

// FUNCTIONS
Token nextToken(FILE *file);
void printToken(Token token);

#endif /* ANALEX_H */