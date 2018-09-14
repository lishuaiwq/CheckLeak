#include<iostream>
using namespace std;
extern "C" void* malloc(size_t size)
{
  cout<<"my malloc"<<endl;
}
extern "C" void free(void *ptr)
{
  cout<<"my free"<<endl;
} 
