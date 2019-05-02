#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <stack>
using namespace std;

//将数据类型封装
/*
    type 0表示null
         1表示数字量（整型和浮点型）
         2表示字符串
         3表示list列表
         4表示嵌套的json数据对象
         5表示保留字
         6表示bool值
*/

typedef enum{
    null,
    NUMBER,
    STRING,
    LIST,
    OBJECT,
    BOOL,
    ERROR
} ValueType;



struct Value{
    ValueType type = null;   //初始化为null   
    string str=""; //用来记录字符量，整型常数和浮点型常数
    vector<Value> list; //用来记录列表
    map<string, Value> object;  //用来记录嵌套的json对象
    int count = 0;  //记录列表或者json对象数量
};

/*
stack就三个接口函数，push(),top(),pop()。

push()会将一个元素放入stack中。
top()会返回stack中的栈顶元素，返回的是reference，可以就地修改值。
pop()移除栈顶元素，无返回值。
size()返回stack长度。
empty()返回stack是否为空。
*/