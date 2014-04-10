//#include <unistd.h> for read and write?
#include <stdio.h>

int main()
{
  char c = getchar();
  while(c != EOF)
  {
    putchar(c);
    c = getchar();
  }   
}
