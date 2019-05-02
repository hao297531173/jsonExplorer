/**
 * 这个文件用来进行token解析
 * 2019-4-30 添加了token的行号，词法分析差不多就完成啦
 * 测试结果保存在output.txt里面了
 * 对外的接口就是pullToken() 获取下一个token
 * 提供无参数的构造函数，不过需要使用initParser()来提供待解析文件路径
 **/
#include "parser.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define MAXSIZE 100 //最大字符长度

struct Token{
    TokenType type; //token类型
    int length; //长度
    char token[MAXSIZE];
    int lineNo; //行号
};

//关键字查找表
struct keywordToken keywords[] = {
    {"{", 1, TOKEN_LEFT_BRACE},
    {"}", 1, TOKEN_RIGHT_BRACE},
    {"[", 1, TOKEN_LEFT_SQUARE},
    {"]", 1, TOKEN_RIGHT_SQUARE},
    {",", 1, TOKEN_COMMA},
    {"\"", 1, TOKEN_QUOTATION},
    {"null", 4, TOKEN_NULL},
    {"\\", 1, TOKEN_FLAG},
    {"/", 1, TOKEN_RE_FLAG},
    {":", 1, TOKEN_COLON},
    {"EOF", 1, TOKEN_EOF},
    {NULL, 0, TOKEN_UNKNOWN}
};

class parser{
    const char* file;   //文件路径
    FILE *fileWrite;  //写出的文件指针
    FILE *fp;   //文件指针
    Token result;
    int line;   //标识行
    char ch;
    int tokenCount = 0;
    

    //判断token是否是关键字
    TokenType isKeyword(char a[], int length);
    //跳过空格，运行完后fp指向不为空格或者回车的字符
    void skipBlanks();
    //获取下一个token
    void getNextToken();
    //显示token
    void showToken();
    //初始化result
    void initResult();
public:
    //解析主函数(用于测试)
    void parse();
    //发送token给别的程序(给别的程序调用)
    Token pullToken();
    //构造函数(一个无参数，一个有参数)
    parser(){}
    parser(char *file){
        this->file = file;
        this->fp = fopen(this->file, "r");  //打开文件
        this->fileWrite = fopen("output.txt", "w");
        this->line = 1;
        initResult();
        ch = fgetc(fp); //先读取第一个字符
        //printf("construct : %c\n", ch);
    }
    void initParser(char *file){
        this->file = file;
        this->fp = fopen(this->file, "r");  //打开文件
        this->fileWrite = fopen("output.txt", "w");
        this->line = 1;
        initResult();
        ch = fgetc(fp); //先读取第一个字符
    }
    //析构函数，主要用来关闭文件
    ~parser(){
        fclose(this->fileWrite);
        fclose(this->fp);
    }
};

void parser::parse(){
    while(this->result.type != TOKEN_EOF){
        skipBlanks();   //跳过空格和换行
        getNextToken(); //获取下一个Token
        showToken();    //显示
        this->tokenCount++;
    }
}

Token parser::pullToken(){
    skipBlanks();
    getNextToken();
    return this->result;
}

TokenType parser::isKeyword(char a[], int length){
    int idx = 0;
        while(keywords[idx].keyword != NULL){
            if(length == keywords[idx].length &&
                memcmp(keywords[idx].keyword, a, length) == 0){
                    return keywords[idx].token;
                }
            idx++;
        }
        return TOKEN_UNKNOWN;   //不是关键字
}

//这个函数运行完后ch为第一个不是空格或者换行的字符
void parser::skipBlanks(){
    while(ch == ' ' || ch == '\n'){
        if(ch == '\n'){
            this->line++;
        }
        ch = fgetc(fp);
        //printf("skip : %c\n", ch);
    }
}


void parser::getNextToken(){
    //这个时候ch已经指向了第一个字符
    initResult();
    //先判断是不是特殊字符
    if(ch == EOF){
        this->result.type = TOKEN_EOF;
        this->result.length = 1;
        strcpy(this->result.token, "EOF");
         this->result.lineNo = this->line;
        return;
    }
    if(ch == ','){
        this->result.type = TOKEN_COMMA;
        this->result.length = 1;
        strcpy(this->result.token, ",");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '"'){
        this->result.type = TOKEN_QUOTATION;
        this->result.length = 1;
        strcpy(this->result.token, "\"");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '{'){
        this->result.type = TOKEN_LEFT_BRACE;
        this->result.length = 1;
        strcpy(this->result.token, "{");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '}'){
        this->result.type = TOKEN_RIGHT_BRACE;
        this->result.length = 1;
        strcpy(this->result.token, "}");
        this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '['){
        this->result.type = TOKEN_LEFT_SQUARE;
        this->result.length = 1;
        strcpy(this->result.token, "[");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == ']'){
        this->result.type = TOKEN_RIGHT_SQUARE;
        this->result.length = 1;
        strcpy(this->result.token, "]");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '/'){
        this->result.type = TOKEN_RE_FLAG;
        this->result.length = 1;
        strcpy(this->result.token, "/");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == '\\'){
        this->result.type = TOKEN_FLAG;
        this->result.length = 1;
        strcpy(this->result.token, "\\");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    if(ch == ':'){
        this->result.type = TOKEN_COLON;
        this->result.length = 1;
        strcpy(this->result.token, ":");
         this->result.lineNo = this->line;
        ch = fgetc(fp);
        return;
    }
    //到这里就不会是上面几种符号了
    while(ch != ' ' && ch != '\n'){
        if(ch!=',' && ch!='\"' && ch!='/' && ch!='\\' && ch!=':' && ch!='{' &&
            ch!='}' && ch!='[' && ch!=']'){
        this->result.token[this->result.length] = ch;
        this->result.length++;
        ch = fgetc(fp);
        //printf("gettoken : %c\n", ch);
        } else {    //等于其中任意一种
            //this->result.token[this->result.length] = '\0';
            this->result.type = isKeyword(this->result.token, this->result.length);
            this->result.lineNo = this->line;
            return;
        }
    }
}

void parser::showToken(){
    printf("line %d : (%s, %d,",this->result.lineNo, this->result.token, this->result.length);
    #if OUT
    fprintf(this->fileWrite, "line %d : (%s, %d,",this->result.lineNo, this->result.token, this->result.length);
    #endif
    string str;
    switch(this->result.type){
        case 0:
            str = "TOKEN_UNKNOWN";
            break;
        case 1:
            str = "TOKEN_LEFT_BRACE";
            break;
        case 2:
            str = "TOKEN_RIGHT_BRACE";
            break;
        case 3:
            str = "TOKEN_LEFT_SQUARE";
            break;
        case 4:
            str = "TOKEN_RIGHT_SQUARE";
            break;
        case 5:
            str = "TOKEN_COMMA";
            break;
        case 6:
            str = "TOKEN_QUOTATION";
            break;
        case 7:
            str = "TOKEN_NULL";
            break;
        case 8:
            str = "TOKEN_FLAG";
            break;
        case 9:
            str = "TOKEN_RE_FLAG";
            break;
        case 10:
            str = "TOKEN_COLON";
            break;
        case 11:
            str = "TOKEN_EOF";
            break;
        default:
            break;
    }
    printf(" %s)\n", str.c_str());
    //编译的时候需要加上 -DOUT 才能写入文件
    #if OUT
    fprintf(this->fileWrite, " %s)\n", str.c_str());
    #endif
}

void parser::initResult(){
    this->result.type = TOKEN_UNKNOWN;
    this->result.length = 0;
    for(int i=0; i<MAXSIZE; i++){
        this->result.token[i] = '\0';   //初始化的时候写满结束符就行了，哈哈，我真机智
    }
    this->result.lineNo = this->line;
}

/*
int main(){
    parser p("test.json");
    p.parse();
    return 0;
}*/

