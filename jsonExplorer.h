/**
 * jsonExplorer version 1.0 
 * 完成时间 2019-5-2
 * 主要数据结构：
 * typedef enum{
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

 * 主要对外接口：
 * parser类中
 * pullToken()返回下一个Token这是内部接口
 * explorer类中
 * 带参数的构造函数，参数是待解析的json文件路径
 * void dump() 
 * 没有参数没有返回值，作用是将文件进行解析并且存储到类内部的value中
 * Value findElement(char key[][MAXSIZE], int length);
 * 第一个参数是key的值，按照顺序逐个查询，第二个参数是key值的个数
 * 如果查询成功则返回查询的那个Value，否则返回ERROR类型的Value
 * void traverse();
 * void traverse(Value val);
 * 对Value进行遍历，不带参数的是遍历类中自带的value
 * void output(char *file,Value val);
 * void output(char *file);
 * 对Value进行输出，输出到文件名为file的文件中，第二个也是对类自身的value进行输出
 * 注意：只能写入json对象，也就是最外层必须是花括号
*/

#include "runner.cpp"
