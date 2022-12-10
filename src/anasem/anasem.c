#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  char errorMessage[24];
  if (sb.stereotype == SFN) {
    int i;
    for (i = getSymbolTableTop() - 1; i >= 0; i--) {      
      if (stricmp(symbolTable[i].name, sb.name) == 0 && stricmp(symbolTable[i].fnScope, sb.fnScope) == 0) {
        snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
        error(errorMessage);
      }
    }
  } else if (existInSymbolTable(sb.name, sb.scope) || existInTypeTable(sb.name, sb.scope)) {
    snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
    error(errorMessage);
  }

  if (sb.stereotype == FIMP && sb.scope == GLOBAL_SCOPE) {
    snprintf(errorMessage, 24, "\nImplementação de função global %s não permitida", sb.name);
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