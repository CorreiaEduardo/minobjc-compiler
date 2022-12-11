#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  char errorMessage[24];
  if (sb.stereotype == SFN) {
    int i;
    for (i = getSymbolTableTop() - 1; i >= 0; i--) {      
      if (stricmp(symbolTable[i].name, sb.name) == 0 && stricmp(symbolTable[i].class, sb.class) == 0) {
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

  if (((sb.stereotype == VAR || sb.stereotype == ARG) && sb.type == SB_VOID) && sb.isPointer == 0) {
    snprintf(errorMessage, 24, "\nUso do tipo void em variáveis e argumentos não é permitido", sb.name);
    error(errorMessage);
  }

  if ((sb.isArray == 1 && sb.isPointer == 1) && sb.type == SB_CLASS) {
    snprintf(errorMessage, 24, "\nVetor de apontador não permitido para %s", sb.name);
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

void validateDeleteCommand(char lexeme[]) {
  Symbol *sb = findInSymbolTable(lexeme);
  if (sb->isPointer == 0) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nO comando delete só pode ser utilizado com apontadores %s", lexeme);
    error(errorMessage);
  }
  
}

void validateNewCommand(char target[], Token tk) {
  Symbol *sb = findInSymbolTable(target);
  if (sb->isPointer == 0 || sb->type != SB_VOID && stricmp(sb->class, tk.lexeme) != 0) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nUtilização inválida do comando new para %s", tk.lexeme);
    error(errorMessage);
  }
}
