#include "../include/core.h"

/**
 * As regras semanticas 3, 13 e 14 são atendidas pela natureza do funcionamento
 * do motor de analise sintatica.
*/

/**
 * Atende as regras semanticas 1, 4, 5, 6, 7, 8 e 16.
*/
void validateDeclaration(Symbol sb);

/**
 * Atende as regras semanticas 2 e 9.
*/
void validateReference(char lexeme[]);
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

/**
 * Atende a regra semantica 18
*/
void validateArithmeticFactor(char lexeme[], int circumflexFound);

/**
 * Atende as regras semanticas 10, 11 e 12
*/
void validateScopeAccess(char baseId[], char fieldId[], char classScope[]);

/**
 * Atende a regra semantica 19
*/
void validateInternSection(Symbol originalClass);

/**
 * Utility
*/

int compareTypes(Symbol sb, Symbol sb2);