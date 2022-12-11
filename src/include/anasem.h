#include "../include/core.h"

/**
 * As regras semanticas 13 e 14 são atendidas pela natureza do funcionamento
 * do motor de analise sintatica.
*/

/**
 * Atende as regras semanticas 1, 4, 5, 7, 8 e 16.
*/
void validateDeclaration(Symbol sb);

/**
 * Atende as regras semanticas 2 e 9.
*/
void validateReferenceInSymbolTable(char lexeme[]);
void validateReferenceInTypeTable(char lexeme[]);

/**
 * Atende a regra semantica 15
*/
void validateDeleteCommand(char lexeme[]);

/**
 * Atende a regra semantica 17
*/
void validateNewCommand(char lexeme[], Token tk);