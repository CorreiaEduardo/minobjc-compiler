#include "include/analex.h"

int main(int argc, char const *argv[])
{
  FILE *fd;
  Token token;

  if ((fd = fopen("../examples/analex.moc", "r")) == NULL)
    error("Não foi possivel ler o código fonte.");

  while (token.type != EOP)
  {
    token = nextToken(fd);
    printToken(token);
  }

  fclose(fd);
  system("pause");
  return 0;
}
