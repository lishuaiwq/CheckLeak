#include"Checkleak.h"
int main()
{
//在这里使用的new是宏定义的
int i=0;
  for(;i<1000000;i++) 
  {int *a=new int(0);
  int *b=new int[20000000];
  char *c=new char;
  }
  //delete [] b;
   return 0;
} 
