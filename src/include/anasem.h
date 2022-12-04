#include "../include/core.h"

/**
 * Atende a regra semantica 1.
*/
void validateDeclaration(Symbol sb);

/**
 * Atende a regra semantica 2.
*/
void validateReferenceInSymbolTable(char lexeme[]);
void validateReferenceInTypeTable(char lexeme[]);

void semanticError(char message[]);