//文件名：whjy.h
#include <string>  
  
using namespace std;  
  
struct student{  
    string name;  
    int Chinese;  
    int Mathematics;  
    int English;  
    int total;  
  
    student(string n){  
    this->name = n;  
    }  
  
    void setName(string stuName){  
        this->name = stuName;  
    }  
};  
  
void calc(struct student&);
