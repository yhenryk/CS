#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int counter = 0;
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
  counter++;
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
    write(2, "Error, please supply only one argument\n", 40);
    exit(1);
  }
  int argument = strtoul(argv[1], NULL, 0);
  if(argument < 0)
  {
    write(2, "Error, Argument must be a positive number\n", 44); 
    exit(1);
  }
  struct stat file;
  fstat(0, &file);
  int filesize = file.st_size;
  filesize = (filesize / argument)+1;
  //fprintf(stderr,"file =%d", filesize);
  char **record = malloc(filesize*sizeof(char*));
  if(record == NULL)
  {
    write(2, "Error, not enough memory\n", 27);
    exit(1); 
  }
  char *temp = malloc(argument);
  if(temp == NULL)
  {
    write(2, "Error, not enough memory\n", 27);
    free(record);
    exit(1);
  }
  char ch;
  int n = 0;
  int size = 0;
  int maxsize = filesize;
  while(read(0, &ch, 1)) //read(0, &ch, 1)
  {
    if(size >= maxsize)
    {
      char** old = record;
      maxsize = maxsize + 1;
      record = (char**)realloc(record, maxsize*(sizeof(char*)));
      if(record == NULL)
      {
        write(2, "Error, not enough memory\n", 27);
        delete(old, maxsize);
        free(old);
        free(temp);
        exit(1);
      }
    }
   if( n  < argument)
   {
     temp[n] = ch;
     //fprintf(stdout, "%s\n", temp);
     n++;
   }
  else
  {
    int cmp = eq(record, temp, size, argument);
    //printf("%d\n", cmp);
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
  }
  //fprintf(stderr,"n =%d\n", n);
  if(n != 0)
  {
    int add = argument - n;
    int o;
    for(o = 0; o < add; o++)
    {
      temp[n] = '\0';
      n++;
    }
    //fprintf(stderr, "size = %d\n", size);
    record[size] = temp;
    size = size + 1;
  } 
  qsort(record, size, sizeof(char *), cstring_cmp);
  int f;
  int g;
  //printf("Record: ");
  //for(f = 0; f < size; f++)
  //{
    //for(g = 0; g < argument; g++)
    //{
      //write(1, record[f][g], 1);
    //}
   //printf("|");
  //}
  for(f = 0; f < size; f++)
  {
    write(1, record[f], argument);
  }
  write(1, "\n", 2);
  fprintf(stderr,"Number of comparisons: %d\n", counter);
  exit(0);
}
