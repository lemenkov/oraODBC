#include "mem_functions.h"
#include <stdio.h>

void* ora_malloc(size_t size)
{
  void * ptr = malloc(size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr;
}

void* ora_realloc(void *ptr, size_t size)
{  
  void * ptr2 = realloc(ptr,size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr2;
}
