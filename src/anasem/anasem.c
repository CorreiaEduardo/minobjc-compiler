#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  char errorMessage[24];

  if (sb.stereotype == FIMP && sb.scope == GLOBAL_SCOPE) {
    snprintf(errorMessage, 24, "\nImplementação de função global %s não permitida", sb.name);
    error(errorMessage);
  }

  if (sb.stereotype == SFN) {
    int i, count = 0;
    for (i = getSymbolTableTop() - 1; i >= 0; i--) {      
      if (stricmp(symbolTable[i].name, sb.name) == 0 && stricmp(symbolTable[i].class, sb.class) == 0) {
        count++;
      }
    }

    if (count > 1) {
      snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
      error(errorMessage);
    }
  } else if (existInSymbolTable(sb.name, sb.scope) || existInTypeTable(sb.name, sb.scope)) {
    snprintf(errorMessage, 24, "\nRedeclaração de %s não permitida", sb.name);
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

  // 6
  if (sb.stereotype == SFN) {
    int i, found = 0;

    for (i = 0; i < getTypeTableTop(); i++) {
      if (stricmp(typeTable[i].name, sb.name) == 0 && stricmp(typeTable[i].class, sb.class) == 0) {
        found = 1;
        break;
      }
    }

    if (found == 0) {
      snprintf(errorMessage, 24, "\nAssinatura não encontrada para a declaração: %s", sb.name);
      error(errorMessage);
    }

    Symbol original = typeTable[i];
    int k;

    for (k = getSymbolTableTop() - 1; i >= 0; k--) {
      if (stricmp(symbolTable[k].name, sb.name) == 0 && stricmp(symbolTable[k].class, sb.class) == 0) {
        break;
      }
    }

    int j = i, stop = 0, validArgs = 0;

    j++;
    k++;

    if (typeTable[j].stereotype != ARG && symbolTable[k].stereotype != ARG) {
      validArgs = 1;
    }

    while (typeTable[j].stereotype == ARG && symbolTable[k].stereotype == ARG) {
      validArgs = compareTypes(typeTable[j], symbolTable[k]);
      j++;
      k++;
    }

    if (original.isPointer != sb.isPointer || original.type != sb.type || stricmp(original.cType, sb.cType) != 0 || validArgs == 0) {
      snprintf(errorMessage, 24, "\nDeclaração não compatível com assinatura: %s", sb.name);
      error(errorMessage);
    }
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

void validateArithmeticFactor(char lexeme[], int circumflexFound) {
  Symbol *sb = findInSymbolTable(lexeme);
  if (sb->isPointer == 1 && circumflexFound == 0) {
    char errorMessage[24];
    snprintf(errorMessage, 24, "\nUtilização inválida de fator aritmético para %s", lexeme);
    error(errorMessage);
  }
}

int compareTypes(Symbol sb, Symbol sb2) {
  if (sb.type == SB_OBJ) {
    if (sb2.type != SB_OBJ) return 0;
    return stricmp(sb.cType, sb2.cType) == 0;
  } else {
    if (sb2.type == SB_OBJ) return 0;
    return sb.type == sb2.type;
  }
}
