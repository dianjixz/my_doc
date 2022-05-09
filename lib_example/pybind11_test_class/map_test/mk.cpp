


#include <iostream>
#include<string>
#include<list>
using namespace std;
typedef list<string> LISTSTR;
int  main()
{
    LISTSTR test;

    test.push_back("back");                               //back
    test.push_front("middle");                       //middle back
    test.push_front("front");                           //front  middle back

    cout<<test.front()<<endl;                        //front
    cout<<*test.begin()<<endl;                       //front

    cout<<test.back()<<endl;                        //back
    cout<<*(test.rbegin())<<endl;                //back

    test.pop_front();                                   //middle back
    test.pop_back();                                    //middle

    cout<<test.front()<<endl;                    //middle

}