#include "../include/anasem.h"

void validateDeclaration(Symbol sb) {
  char errorMessage[100];

  if (sb.stereotype == FIMP && sb.scope == GLOBAL_SCOPE) {
    snprintf(errorMessage, 100, "\nGlobal function implementation not allowed for %s", sb.name);
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
      snprintf(errorMessage, 100, "\nRedeclaration not allowed for %s", sb.name);
      error(errorMessage);
    }
  } else if (existInSymbolTable(sb.name, sb.scope) || existInTypeTableByScope(sb.name, sb.scope)) {
    snprintf(errorMessage, 100, "\nRedeclaration not allowed for %s", sb.name);
    error(errorMessage);
  }

  if (((sb.stereotype == VAR || sb.stereotype == ARG) && sb.type == SB_VOID) && sb.isPointer == 0) {
    snprintf(errorMessage, 100, "\nVoid type not allowed for variables (%s)", sb.name);
    error(errorMessage);
  }

  if ((sb.isArray == 1 && sb.isPointer == 1) && sb.type == SB_CLASS) {
    snprintf(errorMessage, 100, "\nPointer vector not allowed (%s)", sb.name);
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
      snprintf(errorMessage, 100, "\nFuntion signature not found for %s", sb.name);
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

    if (typeTable[j].stereotype == ARG || symbolTable[k].stereotype == ARG) {
      validArgs = 0;
    }

    if (original.isPointer != sb.isPointer || original.type != sb.type || stricmp(original.cType, sb.cType) != 0 || validArgs == 0) {
      snprintf(errorMessage, 100, "\nSignature mismatch for %s", sb.name);
      error(errorMessage);
    }
  }
}

void validateReference(char lexeme[]) {
  if (!existInSymbolTable(lexeme, LOCAL_SCOPE) && !existInSymbolTable(lexeme, GLOBAL_SCOPE)) {
    if (!existInTypeTableByScope(lexeme, LOCAL_SCOPE) && !existInTypeTableByScope(lexeme, GLOBAL_SCOPE)) {
      char errorMessage[100];
      snprintf(errorMessage, 100, "\nInvalid reference for %s", lexeme);
      error(errorMessage);
    }
  }
}

void validateReferenceInSymbolTable(char lexeme[]) {
  if (!existInSymbolTable(lexeme, LOCAL_SCOPE) && !existInSymbolTable(lexeme, GLOBAL_SCOPE)) {
    char errorMessage[100];
    snprintf(errorMessage, 100, "\nInvalid reference for %s", lexeme);
    error(errorMessage);
  }
}

void validateReferenceInTypeTable(char lexeme[]) {
  if (!existInTypeTableByScope(lexeme, LOCAL_SCOPE) && !existInTypeTableByScope(lexeme, GLOBAL_SCOPE)) {
    char errorMessage[100];
    snprintf(errorMessage, 100, "\nInvalid reference for %s", lexeme);
    error(errorMessage);
  }
}

void validateDeleteCommand(char lexeme[]) {
  Symbol *sb = findInSymbolTable(lexeme);
  if (sb->isPointer == 0) {
    char errorMessage[100];
    snprintf(errorMessage, 100, "\nInvalid usage for DELETE command (%s)", lexeme);
    error(errorMessage);
  }
  
}

void validateNewCommand(char target[], Token tk) {
  Symbol *sb = findInSymbolTable(target);
  if (sb->isPointer == 0 || sb->type != SB_VOID && stricmp(sb->class, tk.lexeme) != 0) {
    char errorMessage[100];
    snprintf(errorMessage, 100, "\nInvalid usage for NEW command (%s)", tk.lexeme);
    error(errorMessage);
  }
}

void validateArithmeticFactor(char lexeme[], int circumflexFound) {
  Symbol *sb = findInSymbolTable(lexeme);
  if (sb->isPointer == 1 && circumflexFound == 0) {
    char errorMessage[100];
    snprintf(errorMessage, 100, "\nInvalid arithmetic factor (%s)", lexeme);
    error(errorMessage);
  }
}

void validateScopeAccess(char baseId[], char fieldId[], char classScope[]) {
  char forbiddenAccessMessage[100];
  snprintf(forbiddenAccessMessage, 100, "\nCan't access field %s", fieldId);

  Symbol *sb = findInSymbolTable(fieldId);

  if (classScope == NULL) { // função global
    if (sb == NULL || (sb->stereotype != GFN || sb->stereotype != FIMP)) {
      error(forbiddenAccessMessage);
    }
  } else { //função de escopo
    sb = sb == NULL ? findInTypeTable(classScope, fieldId) : sb;

    if (sb == NULL) {
      Symbol *sb2 = findInSymbolTable(baseId);
      if (sb2 != NULL) {
        Symbol *sb3 = findInTypeTable(sb2->cType, fieldId);
        if (sb3 != NULL) {
          sb = sb3->stereotype == CFN ? sb3 : sb;
        }
      }
    }

    if (sb == NULL) {
      error(forbiddenAccessMessage);
    }
  }
}

int compareTypes(Symbol sb, Symbol sb2) {
  if (sb.type == SB_OBJ) {
    if (sb2.type != SB_OBJ) return 0;
    return (stricmp(sb.cType, sb2.cType) == 0) && (sb.isPointer == sb2.isPointer) && (sb.isArray == sb2.isArray);
  } else {
    if (sb2.type == SB_OBJ) return 0;
    return (sb.type == sb2.type) && (sb.isPointer == sb2.isPointer) && (sb.isArray == sb2.isArray);
  }
}
