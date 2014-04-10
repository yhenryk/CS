#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const char* original, const char* temp2, int size, int maxsize)
{
  int g;
  if(maxsize != 0)
  {
    g = maxsize/size;
  }
  else
  {
    g = 0;
  }
  int d = 0;
  int same = 0;
  int range = maxsize;
  for(d = 0; d < g; d++)
  {
    range = size*d;
    same = memcmp((original+range), temp2, size);
    if(same == 0)
    {
      return 0;
    }
  } 
  return 1;
}
int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "Error, Please supply only one argument.\n");
    exit (1);
  }
  int i = strtoul(argv[1], NULL, 0);
  if( i < 0)
  { 
    fprintf(stderr, "Error, Argument must be positive\n");
    exit (1);
  }
  char *record;
  record = malloc(i);
  if(record == NULL)
  {
    fprintf(stderr, "Error, not enough memory\n");
    exit (1);
  }
  char *temp = malloc(i);
  if(temp == NULL)
  {
    fprintf(stderr, "Error, not enough memory\n");
    free(record);
    exit (1);
  }
  char ch = getchar();
  int n = 0;
  int size = 0;
  int maxsize = i;
  while(ch != EOF)
  {
    if(size >= maxsize)
    {
      char *old = record;
      maxsize += i;
      record = realloc(record, maxsize);
      if(record == NULL)
      {
        fprintf(stderr, "Error, not enough memory\n");
        free(old);
        free(temp);
        exit(1); 
      }
    }
    if(n < i)
    {
      temp[n] = ch;
      n++;
    }
    else
    {
      int cmp = compare(record, temp, i , size);
      if(cmp != 0)
      {
        strcat(record, temp);
        size = size + n;
      }
     n = 0;
     temp[n] = ch;
     n++;
    }
    ch = getchar();
  }
  int add = i - n;
  while(add > 0)
  {
    temp[n] = '\0';
    n++;
    add --;
  }
  strcat(record, temp);
  int flag = 1;
  while(flag == 1)
  {
    flag = 0;
    int count = 0;
    int next = i;
    while(count < maxsize)
   {
     if(next < maxsize)
     {
       int compare = memcmp((record+count),(record+next), i);
       if(compare == 1)
       {
         flag = 1;
         int number = 0;
         for(number = 0; number < i; number++)
         {
           char temp = record[number+count];
           record[number+count] = record[number+next];
           record[number+next] = temp;
         }
       }
     }
    count = count + i;
    next = next + i;
   }
  }
  fprintf(stdout, "%s", record);
  free(record);
  free(temp);   
}
