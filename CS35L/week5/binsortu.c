#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void delete(char** array, int size)
{
  int i = 0;
  for(i = 0; i < size; i++)
  {
    free(array[i]);
  }
}
int cstring_cmp(const void *a, const void *b)
{
  const char **ia = (const char**)a;
  const char **ib = (const char**)b;
  return strcmp(*ia, *ib);
}
int eq(char** array, char * array2, int size, int n)
{
  int number;
  int comparison;
  if(size == 0)
  {
    return 1;
  }
  for(number = 0; number < size; number++)
  {
    comparison = memcmp(array[number], array2, n);
    //printf("comparison :%d\n", comparison);
    if(comparison == 0)
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
    fprintf(stderr, "Error, please supply only one argument\n");
    exit(1);
  }
  int argument = strtoul(argv[1], NULL, 0);
  if(argument < 0)
  {
    fprintf(stderr, "Error, Argument must be a positive number\n"); 
    exit(1);
  }
  char **record = malloc(1*sizeof(char*));
  if(record == NULL)
  {
    fprintf(stderr, "Error, not enough memory\n");
    exit(1); 
  }
  char *temp = malloc(argument);
  if(temp == NULL)
  {
    fprintf(stderr, "Error, not enough memory\n");
    free(record);
    exit(1);
  }
  char ch = getchar();
  int n = 0;
  int size = 0;
  int maxsize = 1;
  while(ch != EOF) //read(0, &ch, 1)
  {
    if(size >= maxsize)
    {
      char** old = record;
      maxsize = maxsize + 1;
      record = (char**)realloc(record, maxsize*(sizeof(char*)));
      if(record == NULL)
      {
        fprintf(stderr, "Error, not enough memory\n");
        delete(old, maxsize);
        free(old);
        free(temp);
        exit(1);
      }
    }
   if( n  < argument)
   {
     temp[n] = ch;
     n++;
   }
  else
  {
    int cmp = eq(record, temp, size, argument);
    if(cmp != 0)
    {
      record[size] = temp;
      size = size + 1;
      temp = malloc(argument);
      n = 0;
      temp[n] = ch;
      n++;
    }
    else
    {
      n = 0;
      temp[n] = ch;
      n++;
    }
  }
  ch = getchar(); 
  }
  if(n != 0)
  {
    int add = argument - n;
    int o;
    for(o = 0; o < add; o++)
    {
      temp[n] = '\0';
    }
    record[size] = temp;
    size = size + 1;
  } 
  qsort(record, size, sizeof(char *), cstring_cmp);
  int f;
  int g;
  for(f = 0; f < size; f++)
  {
    for(g = 0; g < argument; g++)
    {
      printf("%c", record[f][g]);
    }
  }
  exit(0);
}
