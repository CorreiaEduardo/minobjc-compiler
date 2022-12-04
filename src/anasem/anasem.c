#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  if (existInSymbolTable(sb.name, sb.scope) || existInTypeTable(sb.name, sb.scope)) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
    error(errorMessage);
  }
}
