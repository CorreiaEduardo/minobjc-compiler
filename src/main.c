#include "include/analex.h"

static const char INPUT_FILE[] = "../examples/analex.moc";

int main(int argc, char const *argv[])
{
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
  system("pause");
  return 0;
}
