#define NOT_USE_NEW
#include"Checkleak.h"


//在这里实现内存泄漏的核心代码
//
//我们用一个双向循环链表来维持我们的

int CheckLeak::flag=0;//初始化成员变量
static int sum=0;//用来统计用户总共申请的东西

struct List
{
   struct List* prev;
   struct List* next;
   size_t line;//指向行号
   char *file;//指向文件名
   bool isNew;//区分new/new[]
   size_t size;//记录用户存储的内存
}; 

//typedef struct List* List;
static struct List listhead={&listhead,&listhead,0,NULL,true,0}; //需要一个头结点


void* RequestMem(size_t size,const char *file,size_t line,bool isNew)//申请内存
{
    size_t totalsize=size+sizeof(struct List);//给存储的结点也需要开辟结点
    struct List* total=(struct List*)malloc(totalsize);//申请空间
	//将其插入到链表中去，采取头插
	total->prev=&listhead;//指向头结点
	total->next=listhead.next;//指向头结点指向的结点
    listhead.next->prev=total;//头结点的下一个结点的pre指向当前的新创建的结点
	listhead.next=total;//头结点的next指向当前创建的结点
    
	total->size=size;//保存用户申请的空间
	total->line=line;//保存行号
     if(file!=NULL)//给过来的文件信息不等于null 
	 { 
		total->file=(char*)malloc(strlen(file)+1);//给文件申请空间 
		strcpy(total->file,file);//将文件信息保存
	 	//std::cout<<total->file<<std::endl;
     }
	total->isNew=isNew;//保存是否为
    sum+=size;//记录用户申请的内存
   return (char*)total+sizeof(struct List);//返回去指针往后加才是用户申请的空间  
   //sizeof();的结果为char 
} 
void DeleteMem(void* ptr,bool isNew) //释放内存
{
      struct List* dele=(struct List*)((char*)ptr-sizeof(struct List));//直接转到当初删除的地址
      if(dele->isNew!=isNew)
			  return;
	  //从双向循环链表中删除当前的结点
      dele->prev->next=dele->next;
	  dele->next->prev=dele->prev;
      if(dele->file!=NULL)
	  {
	      free(dele->file); 
	  } 
    sum-=dele->size;
    free(dele); 
} 
void* operator new(size_t size,const char *file,size_t line)
{
//你只需要给new多少内存就行了		
   return RequestMem(size,file,line,true); 
} 
void* operator new[](size_t size,const char *file,size_t line)
{
   return RequestMem(size,file,line,false);//不使用new 使用的是new[]  
} 
void operator delete(void *ptr)
{
  DeleteMem(ptr,true); 
} 
void operator delete[](void *ptr)
{
  DeleteMem(ptr,false); 
}

void CheckLeak::check() //检查是否泄漏
{
   //判断内存泄漏只需要看sum==0?就可以了
   if(sum==0)
   {
		   std::cout<<"恭喜你，不存在内存泄漏"<<std::endl;
		   return;
   }
   struct List* cur=listhead.next;//指向下一个结点
   std::cout<<"发生内存泄漏:"<<std::endl;
   int i=0;
   int count=0;
   while(cur!=NULL&&&listhead!=cur)//遍历双向循环链表
   {
		   if(cur->isNew==true)//使用new 
		   {
				   std::cout<<"new发生泄漏,泄漏位置:"<<std::endl;
		   }else
		   {
				   std::cout<<"new[]发生泄漏,泄漏位置:"<<std::endl;
		   } 
		   if(cur->file!=NULL) 
		   {
				   std::cout<<cur->file<<" 第:"<<cur->line<<"行"<<" 泄漏大小:"<<cur->size<<std::endl;
		   }
		   i++;
		   count+=cur->size;
		   cur=cur->next;
   }
   std::cout<<"共计泄漏"<<i<<" 处,泄漏内存总大小"<<count<<"字节"<<std::endl; 
} 

