#include "../include/core.h"

int symbolTableTop = 0;
int typeTableTop = 0;

int isIdDefined(Token tk) {
  if (tk.type != ID) return 0;
  
  int i;
  for (i = 1; i < (sizeof(typeTable)) / (sizeof(typeTable[0])) - 1; i++)
  {
    if (stricmp(tk.lexeme, typeTable[i].name) == 0) return 1;
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

void error(char msg[]) { 
  printf("%s\n", msg);
  system("pause");
  exit(1); 
} 