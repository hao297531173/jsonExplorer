/**
 * 这个文件用于写错误信息
 * */
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

//解析时遇到错误
void formatError_runner(Token token, char a){
    printf("ERROR : line %d:", token.lineNo);
    printf("'%s' format error, ", token.token);
    printf("expected '%c'\n", a);
}

void formatError_runner(Token token, char a, char b){
    printf("ERROR : line %d:", token.lineNo);
    printf("'%s' format error, ", token.token);
    printf("expected '%c' or '%c'\n", a, b);
}

void stringError(Token token){
    printf("ERROR : line %d:", token.lineNo);
    printf("'%s' format error, ", token.token);
    printf("expected a string as key\n");
}

//查找时错误
void findError(){
    printf("FIND ERROR: there is no such key-value\n");
}

//修改错
void changeError(){
    printf("CHANGE ERROR : there is no such key-value\n");
}

//Value类型错
void ValueTypeError(){
    printf("type of valut is not expected\n");
}

//遍历错
void traverseError(){
    printf("the type of value is ERROR\n");
}