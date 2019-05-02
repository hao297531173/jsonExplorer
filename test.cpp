#include <cstdio>
#include <cstring>
#include <iostream>
#include "jsonExplorer.h"
using namespace std;

void get(){
    explorer e("test.json");
    e.dump();
    char a[2][100] = {"Tony", "course"};
    char b[2][100] = {"Mary", "age"};
    Value val;
    val = e.findElement(a, 2);
    if(val.type != ERROR){
        e.traverse(val);
    }
    val = e.findElement(b, 2);
    if(val.type != ERROR){
        e.traverse(val);
    }
}

int main(){
    get();
    return 0;
}