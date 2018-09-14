#include<iostream>
#include<dlfcn.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
static void* (*sys_malloc)(size_t)=0; 
void init()
{
 		
sys_malloc=reinterpret_cast<void*(*)(size_t)>(dlsym(RTLD_NEXT,"malloc")); 
if(sys_malloc==0)
	cout<<"hehe"<<endl;
}
extern "C" void* malloc(size_t size)
{
   cout<<".....";
} 
int main()
{
  init(); 	
  void* p=malloc(1); 
  return 0; 
} 
