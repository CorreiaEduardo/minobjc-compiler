#include "include/anasint.h"

static const char INPUT_FILE[] = "../examples/anasint.moc";

void analex() {
  FILE *fd;
  Token token;

  if ((fd = fopen(INPUT_FILE, "r")) == NULL)
    error("Não foi possivel ler o código fonte.");

  while (token.type != EOP)
  {
    token = nextToken(fd);
    printToken(token);
  }

  fclose(fd);
  printf("\n");
  system("pause");
}

void anasint() {
  FILE *fd;

  if ((fd = fopen(INPUT_FILE, "r")) == NULL)
    error("Não foi possivel ler o código fonte.");

  executeSyntaxAnalysis(fd);

  fclose(fd);
  printf("\n");
  system("pause");
}

int main(int argc, char const *argv[])
{
  analex();
  anasint();

  return 0;
}
