/**
 * 这个文件就是解释器的主要文件了，进行具体的操作
 * 2019-5-2 添加了用二维数组进行value值查询
 * */
#include "parser.cpp"
#include "runner.h"
#include "error.h"
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#define MAXSIZE 100

class explorer{
    Value value;    //用于存储json数据
    Value error;    //用于解析错误时返回
    Token token;    //用于存储当前token
    int num;        //用于控制每行缩进个数
    int depth;      //用于记录查询深度
    parser p;
    char *file;
    //解析花括号
    Value analysisBrace();
    //解析方括号
    Value analysisSquare();
    //递归遍历对象
    void traverseObject(Value val);
    //递归遍历列表
    void traverseList(Value val);
    /*将遍历结果输出到文件*/
    void outputObject(FILE *fp, Value val);
    void outputList(FILE *fp, Value val);
    /*通过key值递归查询*/
    Value findValue(string key,Value val);
    Value findValue(Value val, char a[][MAXSIZE], int length);
    /*修改value的值*/
    Value changeValue(string key, Value ch, Value &val);
public:
    //构造函数(a是文件名字符串)
    explorer(char *a){
        this->file = a;
        p.initParser(file);
        token = p.pullToken();  //获取第一个token
        error.type = ERROR;
        this->depth = 0;    //从0开始计数
    }
    ~explorer(){}
    //提供给外部的接口
    void dump();
    //写一个遍历函数
    void traverse();
    void traverse(Value val);
    //输出到文件
    void output(char *file,Value val);
    void output(char *file);
    //根据key值查value并且返回一个value变量
    Value findElement(string key);
    Value findElement(char key[][MAXSIZE], int length);
    //根据二维数组找Value值
    //Value findElement(char a[][]);
    //修改某个key中的value值
    //参数分别是key值，要改变的value，从那个value开始查找
    //第三个参数缺省的话就是改变类自己的value
    Value changeValueOfKey(string key, Value ch, Value &val);
    Value changeValueOfKey(string key, Value ch);
    
};

Value explorer::findValue(Value val, char a[][MAXSIZE], int length){
    this->depth++;
    map<string, Value>::iterator it = val.object.begin();
    for(;it != val.object.end(); it++){
        if(strcmp(it->first.c_str(), a[this->depth]) == 0){
            if(this->depth == (length-1)){//所有的条件都满足了
                return it->second;  //second就是要找的value
            } else {
                if(it->second.type == OBJECT){  //如果不是最终结果且不是对象类型的话报错
                    return findValue(it->second, a, length);    //否则继续往后找
                } else {
                    ValueTypeError();
                    return error;
                }
            }
        }
    }
}

Value explorer::findElement(char a[][MAXSIZE], int length){
    this->depth = -1;    //从0层开始
    Value ret = error;
    ret = findValue(this->value , a, length);
    if(ret.type == ERROR){
        findError();
    }
    return ret;
}


//修改成功返回被修改的value值，修改失败返回error
Value explorer::changeValue(string key, Value ch, Value &val){
    map<string, Value>::iterator it = val.object.begin();
    for(; it!= val.object.end(); it++){
        if(it->first == key){
            Value ret = it->second;
            val.object[it->first] = ch;
            return ret;
        } else if(it->second.type == OBJECT){
            changeValue(key, ch, val);
        } else {
            continue;
        }
    }
    return error;
}

Value explorer::changeValueOfKey(string key, Value ch){
    Value ret = changeValue(key, ch, this->value);
    if(ret.type == ERROR){
        changeError();
    }
    return ret;
}

Value explorer::changeValueOfKey(string key, Value ch, Value &val){
    if(val.type != OBJECT){ //第一个给定的value必须是object类型的
        ValueTypeError();
        return error;
    }
    Value ret = changeValue(key, ch, val);
    if(ret.type == ERROR){
        changeError();
    }
    return ret;
}

Value explorer::findElement(string key){
    Value ret = findValue(key, this->value);
    if(ret.type == ERROR){
        findError();
    }
    return ret;
}

//查询失败返回error类型的value
Value explorer::findValue(string key, Value val){
    Value ret = error;
    map<string, Value>::iterator it = val.object.begin();
     for(; it!=val.object.end(); it++){
         printf("it->first = %s\n", it->first.c_str());
         if(it->first == key){
             ret = it->second;
             return ret;
         } else if(it->second.type == OBJECT){
             return findValue(key, it->second);    //递归遍历
         } else {
             continue;
         }
     }
     return ret;  //查询没有结果
}

//这可以写自己的value
void explorer::output(char *file, Value val){
    this->num = 0;
    FILE *fp = fopen(file, "w");
    outputObject(fp, val);
    fclose(fp);
}
//这个是写系统的value
void explorer::output(char *file){
    this->num = 0;
    FILE *fp = fopen(file, "w");
    outputObject(fp, this->value);
    fclose(fp);
}

void explorer::outputObject(FILE *fp,Value val){
    if(val.type != OBJECT){
        printf("FILE ERROR : the type of Value is not OBJECT\n");
        return;
    }
    this->num++;
    fprintf(fp, "{\n");
    map<string, Value>::iterator it = val.object.begin();
    for(; it!= val.object.end(); ){
        //注意，it-first 是string类型的变量
        for(int i=0; i<this->num; i++){
            fprintf(fp, "    ");
        }
        fprintf(fp, "\"%s\" : ", it->first.c_str());
        if(it->second.type == null){
            fprintf(fp, "null");
        } else if(it->second.type == NUMBER){
            fprintf(fp, "%s", it->second.str.c_str());
        } else if(it->second.type == STRING){
            fprintf(fp, "\"%s\"", it->second.str.c_str());
        } else if(it->second.type == LIST){
            outputList(fp, it->second);
        } else if(it->second.type == OBJECT){
            outputObject(fp, it->second);
        } else if(it->second.type == BOOL){
            fprintf(fp, "%s", it->second.str.c_str());
        }
        it++;
        if(it != val.object.end()) {
            fprintf(fp, ",\n");
        } else {
            fprintf(fp, "\n");
        }
    }
    this->num--;
    for(int i=0; i<this->num; i++){
            fprintf(fp, "    ");
    }
    fprintf(fp, "}");
}

void explorer::outputList(FILE *fp, Value val){
    if(val.type != LIST){
        printf("FILE ERROR: the type of val is not LIST\n");
        return;
    }
    fprintf(fp, "[\n");
    this->num++;
    vector<Value>::iterator it = val.list.begin();
    for(int i=0; i<this->num; i++){
            fprintf(fp, "    ");
    }
    for(; it != val.list.end(); ){
        if(it->type == null){
            fprintf(fp, "null\n");
        } else if (it->type == NUMBER){
            fprintf(fp, "%s ", it->str.c_str());
        } else if(it->type == STRING){
            fprintf(fp, "\"%s\" ", it->str.c_str());
        } else if(it->type == LIST){
            outputList(fp, *it);
        } else if(it->type == OBJECT){
            outputObject(fp, *it);
        } else if(it->type == BOOL){
            fprintf(fp, "\"%s\" ", it->str.c_str());
        } 
        it++;
        if(it !=val.list.end()) {
            fprintf(fp, ",");
        }
    }
    this->num--;
    fprintf(fp, "\n");
    for(int i=0; i<this->num; i++){
        fprintf(fp, "    ");
    }
    fprintf(fp, "]");
}

void explorer::traverse(){
    //Value只有两种对象 json对象和列表
    this->num = 0; //每行缩进个数
    traverseObject(value);
}
//重载函数，可以遍历任意一个value
void explorer::traverse(Value val){
    this->num = 0;
    traverseObject(val);
}

void explorer::traverseObject(Value val){
    this->num++;
    if(val.type != OBJECT){
        if(val.type == null){
            printf("null\n");
        } else if(val.type == NUMBER){
            printf("(NUMBER):%s\n", val.str.c_str());
        } else if(val.type == STRING){
            printf("(STRING):\"%s\"\n", val.str.c_str());
        } else if(val.type == BOOL){
            printf("(BOOL):%s\n", val.str.c_str());
        } else if(val.type == ERROR){
            traverseError();
        } else if(val.type == LIST){
            traverseList(val);
        }
        return;
    }
    printf("{\n");
    map<string, Value>::iterator it = val.object.begin();
    for(; it!= val.object.end(); ){
        for(int i=0; i<this->num; i++){
            printf("    ");
        }
        //注意，it-first 是string类型的变量
        printf("\"%s\" : ", it->first.c_str());
        if(it->second.type == null){
            printf("null");
        } else if(it->second.type == NUMBER){
            printf("%s", it->second.str.c_str());
        } else if(it->second.type == STRING){
            printf("\"%s\"", it->second.str.c_str());
        } else if(it->second.type == LIST){
            traverseList(it->second);
        } else if(it->second.type == OBJECT){
            traverseObject(it->second);
        } else if(it->second.type == BOOL){
            printf("%s", it->second.str.c_str());
        }
        it++;
        if(it != val.object.end()) {
            printf(",\n");
        } else {
            printf("\n");
        }
    }
    this->num--;
    for(int i=0; i<this->num; i++){
        printf("    ");
    }
    printf("}");
    
}

void explorer::traverseList(Value val){
    if(val.type != LIST){
        printf("ERROR: the type of val is not LIST\n");
        return;
    }
    this->num++;
    printf("[\n");
    vector<Value>::iterator it = val.list.begin();
    for(int i=0; i<this->num; i++){
            printf("    ");
    }
    for(; it != val.list.end(); ){
        
        if(it->type == null){
            printf("null\n");
        } else if (it->type == NUMBER){
            printf("%s ", it->str.c_str());
        } else if(it->type == STRING){
            printf("\"%s\" ", it->str.c_str());
        } else if(it->type == LIST){
            traverseList(*it);
        } else if(it->type == OBJECT){
            traverseObject(*it);
        } else if(it->type == BOOL){
            printf("\"%s\" ", it->str.c_str());
        } 
        it++;
        if(it !=val.list.end()) {
            printf(",");
        }
    }
    this->num--;
    printf("\n");
    for(int i=0; i<this->num; i++){
        printf("    ");
    }
    printf("]");
    
}


void explorer::dump(){
    if(token.type == TOKEN_LEFT_BRACE){
        this->value = analysisBrace();
    } else {
        formatError_runner(this->token, '{');
    }
}

Value explorer::analysisBrace(){
    printf("object analysis\n");
    //只要不是左括号就继续解析
    Value v;
    v.type = OBJECT;    //json对象
    while(1){
        //key值必须是字符串
        this->token = this->p.pullToken();
        if(token.type != TOKEN_QUOTATION){
            formatError_runner(token, '"');
            return error;
        }
        this->token = this->p.pullToken();
        if(token.type != TOKEN_UNKNOWN){
            stringError(this->token);
            return this->error;
        }
        //到这里，就需要申请一个Value做为key值了
        string str;
        str += this->token.token;
        //还是解析双引号
        this->token = this->p.pullToken();
        if(this->token.type != TOKEN_QUOTATION){
            formatError_runner(token, '"');
           return this->error;
        }
        //解析冒号
       this->token = this->p.pullToken();
        if(this->token.type != TOKEN_COLON){
            formatError_runner(token, ':');
           return this->error;
        }
        //继续解析值
        Value  val;    //作为值
        this->token = this->p.pullToken();
        if(this->token.type == TOKEN_LEFT_BRACE){
            //如果是左括号就递归调用解析对象
            val = analysisBrace();  //递归调用
        } else if(this->token.type == TOKEN_LEFT_SQUARE){
            val = analysisSquare(); //解析列表值
        } else if(this->token.type == TOKEN_NULL){
            //是null值
            val.type = null;
            val.str += "null";
        } else if(this->token.type == TOKEN_UNKNOWN){
            //看看是不是布尔值
            if(strcmp(token.token, "true")==0 ||
                strcmp(token.token, "false")==0){
                    val.type = BOOL;
            }else {
                //认为是数字量
                val.type = NUMBER;
            }
            val.str += token.token;
        } else if(this->token.type == TOKEN_QUOTATION){
            //认为是字符量，注意，字符量可以有空格的
            this->token = this->p.pullToken();
            val.type = STRING;
            val.str += token.token;
            this->token = this->p.pullToken();
            while(this->token.type == TOKEN_UNKNOWN){
                val.str += " "; //两个字符量之间加一个空格
                val.str += token.token;
                this->token = this->p.pullToken();
            }
            //当不是字面量的时候需要一个双引号，否则报错
            if(token.type != TOKEN_QUOTATION){
                formatError_runner(token, '"');
               return this->error;
            }
        }
        //解析完后要将object insert
        v.object[str] = val;
        v.count++;

        //看看是不是逗号
        this->token = this->p.pullToken();
        
        if(this->token.type == TOKEN_COMMA){
            continue;   //如果是逗号就进入下一轮解析
        } else if(this->token.type == TOKEN_RIGHT_BRACE){
            break;//这一层解析结束
        } else {
            //报错，期望逗号或者左花括号
            formatError_runner(this->token, ',', '}');
            return this->error;
        }
    }
    return v;
}

Value explorer::analysisSquare(){
    printf("list analysis\n");
    Value v;    //用来当做返回值
    v.type = LIST;
    while(1){
        this->token = this->p.pullToken();
        if(this->token.type == TOKEN_QUOTATION){
            //解析STRING
            this->token = this->p.pullToken();
            Value val;
            val.type = STRING;
            val.str += this->token.token;
            val.count++;
            this->token = this->p.pullToken();
            while(this->token.type == TOKEN_UNKNOWN){
                val.str += " ";
                val.str += this->token.token;
                this->token = this->p.pullToken();
            }
            v.list.push_back(val);
            v.count++;
            //找到双引号
            if(this->token.type != TOKEN_QUOTATION){
                formatError_runner(this->token, '"');
                return this->error;
            }
        } else if(this->token.type == TOKEN_UNKNOWN){
            //解析bool值或者数字
            if(strcmp(this->token.token, "true") == 0||
               strcmp(this->token.token, "false") == 0){
                   Value val;
                   val.type = BOOL;
                   val.str += this->token.token;
                   v.list.push_back(val);
                   v.count++;
            } else {    //是数字
                Value val;
                val.type = NUMBER;
                val.str += this->token.token;
                v.list.push_back(val);
                v.count++;
            }
        } else if(this->token.type == TOKEN_LEFT_BRACE){
            Value val = analysisBrace();
            v.list.push_back(val);
            v.count++;
        } else if(this->token.type == TOKEN_LEFT_SQUARE){
            Value val = analysisSquare();
            v.list.push_back(val);
            v.count++;
        }
        this->token = this->p.pullToken();
        if(this->token.type == TOKEN_COMMA){
            continue;   //遇到逗号就继续解析
        } else if(this->token.type == TOKEN_RIGHT_SQUARE){
            break;  //解析结束
        } else {
            formatError_runner(this->token, ',', ']');
        }
    }
    return v;
}
/*
int main(){
    explorer e("test.json");
    e.dump();
    /*
    e.traverse();
    e.output("out.json");
    Value val;
    val = e.findElement("age");
    if(val.type != ERROR)
        e.traverse(val);
    printf("\n");
    Value ch;
    ch.type = NUMBER;
    ch.str = "123";
    val = e.changeValueOfKey("Mary", ch);
    e.traverse();
    char a[2][MAXSIZE] = {"Mary", "age"};
    Value val;
    val = e.findElement(a, 2);
    if(val.type != ERROR){
        e.traverse(val);
    }
    return 0;
}*/