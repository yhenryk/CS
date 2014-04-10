#include <unistd.h>

int main()
{
  char ch;
  while(read(0, &ch, 1))
  {
   write(1, &ch, 1);
  }
}
