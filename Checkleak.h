
#ifndef CHECKLEAK_H__
#define CHECKLEAK_H__

#include<iostream>
#include<stdlib.h>
#include<cstring>
void* operator new(size_t size,const char *file,size_t line);
void* operator new[](size_t size,const char *file,size_t line);  

void operator delete(void *ptr); 
void operator delete[](void *ptr);  		
		
#ifndef NOT_USE_NEW//如果没有这个宏定义就定义这个宏
#define new new(__FILE__,__LINE__)//获取泄漏的文件和行

#endif

class CheckLeak
{
public:
  static int flag;	
   CheckLeak()
   {
      flag++;//创建一个对象,调用一个flag
   } 
   ~CheckLeak()
   {
      if(--flag==0)//只调用一些检测函数就可以检测出来哪个地方泄漏
	  {
	     check();//检测函数 
	  } 
   } 
private:
  void check();//检测内存泄漏检测  				  
}; 

static CheckLeak obj;//创建一个全局的静态对象，其声明周期随其定义的源文件


#endif 
