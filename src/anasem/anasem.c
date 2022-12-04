#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  if (existInSymbolTable(sb.name, sb.scope) || existInTypeTable(sb.name, sb.scope)) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
    error(errorMessage);
  }
}

void validateReferenceInSymbolTable(char lexeme[]) {
  if (!existInSymbolTable(lexeme, LOCAL_SCOPE) && !existInSymbolTable(lexeme, GLOBAL_SCOPE)) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nReferencia inválida para %s", lexeme);
    error(errorMessage);
  }
}

void validateReferenceInTypeTable(char lexeme[]) {
  if (!existInTypeTable(lexeme, LOCAL_SCOPE) && !existInTypeTable(lexeme, GLOBAL_SCOPE)) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nReferencia inválida para %s", lexeme);
    error(errorMessage);
  }
}