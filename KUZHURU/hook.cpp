#include<iostream>
#include<malloc.h>
using namespace std;
static void* my_malloc_hook(size_t,const void*);//声明自己的malloc函数
static void my_free_hook(void*,const void*);//声明自己的free技术

void* (*old_malloc_hook)(size_t size,const void* caller);
void (*old_free_hook)(void* ptr,const void* caller);

static void my_init(void)//这一步的意义就是将自己的malloc设置在钩子上面
{
   old_malloc_hook=__malloc_hook;
   old_free_hook=__free_hook;
   __malloc_hook=my_malloc_hook;
   __free_hook=my_free_hook;
} 
static void* my_malloc_hook(size_t size,const void* caller)
{
   void* result;		
//已经进入到自己函数里面了，所以还原原来的malloc函数，在这里面调用系统的mallco函数
  __malloc_hook=old_malloc_hook;
  __free_hook=old_free_hook;
  result=malloc(size);//调用系统函数申请内存
  //申请完了以后再 将当前的保存下俩然后在
  std::cout<<result<<endl;
   old_malloc_hook=__malloc_hook;
   old_free_hook=__free_hook;
   __malloc_hook=my_malloc_hook;
   __free_hook=my_free_hook;
   return result;
}
static void my_free_hook(void* ptr,const void* caller)
{
   __malloc_hook=old_malloc_hook;
   __free_hook=old_free_hook;
   //free(ptr); 
   old_malloc_hook=__malloc_hook;
   old_free_hook=__free_hook;
   //cout<<"mmmmm"<<endl; 
   //cout<<ptr<<endl;
   printf("ptr=%p\n",ptr); 
   __malloc_hook=my_malloc_hook;
   __free_hook=my_free_hook;
} 
int main()
{
  my_init(); 		
  int *p=(int*)malloc(sizeof(int));//调用malloc
  *p=10;
  cout<<p;
  free(p); //调用free
  return 0;
} 
