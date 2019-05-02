/**
 * 这个文件写一些保留token
 * */
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

typedef enum{
    TOKEN_UNKNOWN,  //未知类型
    
    TOKEN_LEFT_BRACE,   //左花括号
    TOKEN_RIGHT_BRACE,  //右花括号

    TOKEN_LEFT_SQUARE,  //左方括号
    TOKEN_RIGHT_SQUARE, //右方括号

    TOKEN_COMMA,        //逗号

    TOKEN_QUOTATION,   //双引号

    TOKEN_NULL,         //NULL

    TOKEN_FLAG, //斜杠
    TOKEN_RE_FLAG,   //反斜杠
    TOKEN_COLON,    //冒号
    TOKEN_EOF
}TokenType;

//保留字结构体
struct keywordToken{
    char* keyword;
    int length;
    TokenType token;
};

