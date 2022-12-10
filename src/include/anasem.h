#include "../include/core.h"

/**
 * Atende as regras semanticas 1, 4 e 5.
*/
void validateDeclaration(Symbol sb);

/**
 * Atende a regra semantica 2.
*/
void validateReferenceInSymbolTable(char lexeme[]);
void validateReferenceInTypeTable(char lexeme[]);
