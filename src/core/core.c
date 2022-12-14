#include "../include/core.h"

int symbolTableTop = 0;
int typeTableTop = 0;

int isLocalScopeDelimiter(Symbol sb);
int isFunction(Symbol sb);

int isIdDefined(Token tk) {
  if (tk.type != ID) return 0;
  
  int i;
  for (i = 0; i < getTypeTableTop(); i++) {
    if (stricmp(tk.lexeme, typeTable[i].name) == 0 && typeTable[i].stereotype == STR_CLASS) {
      return 1;
    }
  }

  return 0;
}

void pushToSymbolTable(Symbol sb) {
  printf("\n------------ DEBUG: PUSHING TO SYMBOL TABLE [%s]: %s - %s", symbolTypeNames[sb.type], symbolStereotypeNames[sb.stereotype], sb.name);
  symbolTable[symbolTableTop] = sb;
  symbolTableTop++;
}

Symbol peekSymbolTable() {
  return symbolTable[symbolTableTop];
}

void popSymbolTable() {
  symbolTableTop--;
}

void printSymbolTable() {
  int i;
  for (i = symbolTableTop - 1; i >= 0; i--) {
    printf("\n--- DEBUG: SYMBOL TABLE [%s] = %s - %s", symbolTypeNames[symbolTable[i].type], symbolStereotypeNames[symbolTable[i].stereotype], symbolTable[i].name);
  }
}

Symbol *findInSymbolTable(char lexeme[]) {
  int i;
  for (i = symbolTableTop - 1; i >= 0; i--) {
    if (stricmp(symbolTable[i].name, lexeme) == 0) return &symbolTable[i];
  }

  return NULL;
}

int existInSymbolTable(char lexeme[], int scope) {
  int i, count = 0;

  for (i = symbolTableTop - 1; i >= 0; i--) {
    if (scope == LOCAL_SCOPE && isLocalScopeDelimiter(symbolTable[i])) break;
    if ((stricmp(symbolTable[i].name, lexeme) == 0) && symbolTable[i].scope == scope) count++;
  }

  return count > 0;
}

void pushToTypeTable(Symbol sb) {
  printf("\n------------ DEBUG: PUSHING TO TYPE TABLE [%s]: %s - %s", symbolTypeNames[sb.type], symbolStereotypeNames[sb.stereotype], sb.name);
  typeTable[typeTableTop] = sb;
  typeTableTop++;
}

void printTypeTable() {
  int i;
  for (i = typeTableTop - 1; i >= 0; i--) {
    printf("\n--- DEBUG: TYPE TABLE [%s] = %s - %s", symbolTypeNames[typeTable[i].type], symbolStereotypeNames[typeTable[i].stereotype], typeTable[i].name);
  }
}

int existInTypeTableByScope(char lexeme[], int scope) {
  int i, count = 0;

  for (i = typeTableTop - 1; i >= 0; i--) {    
    if (scope == LOCAL_SCOPE && isLocalScopeDelimiter(typeTable[i])) break;
    if ((stricmp(typeTable[i].name, lexeme) == 0) && typeTable[i].scope == scope) count++;
  }

  return count > 0;
}

int existInTypeTable(char lexeme[]) {
  int i, count = 0;

  for (i = typeTableTop - 1; i >= 0; i--) {    
    if (stricmp(typeTable[i].name, lexeme) == 0) count++;
  }

  return count > 0;
}

Symbol *findInTypeTable(char class[], char lexeme[]) {
  int i;
  for (i = typeTableTop - 1; i >= 0; i--) {
    if (stricmp(typeTable[i].name, lexeme) == 0 && stricmp(typeTable[i].class, class) == 0) return &typeTable[i];
  }

  return NULL;
}

int isLocalScopeDelimiter(Symbol sb) {
  return sb.stereotype == STR_CLASS || isFunction(sb);
}

int isFunction(Symbol sb) {
  return sb.stereotype == FIMP || sb.stereotype == GFN || sb.stereotype == CFN || sb.stereotype == IFN || sb.stereotype == SFN;
}

int getSymbolTableTop() {
  return symbolTableTop;
}

int getTypeTableTop() {
  return typeTableTop;
}

void error(char msg[]) { 
  printf("%s\n", msg);
  system("pause");
  exit(1); 
} 