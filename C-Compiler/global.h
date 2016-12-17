#ifndef GLOBAL_H
#define GLOBAL_H

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <stdlib.h>


using namespace std;

class Token{		//在此文件中不能定义全局变量，全局变量定义在.cpp中让后用extern调用
public:
    int code;	//关键字的类
    int value;	//在对应表中的位置
    Token(){ code = 0; value = 0; }
    Token(int a, int b){ code = a; value = b; }
    bool operator== (const Token& t1){
        if (code == t1.code && value == t1.value)
            return true;
        else
            return false;
    }
};

class action{
public:
    string insert;
    bool move;
    action(){ insert = "null"; move = false; }	//对分析表的内容在创建时初始化压空内容，保持不动
    action(string s, bool m){
        insert = s;	//压栈内容
        m = move;	//是否读下一个
    }
};

class redu_table{			//用于简单优先文法使用
public:
    string content;	//识别出来的字符串
    string redu_key;		//归约符
    redu_table(){
        content = "";
        redu_key = "";
    }
};

struct quadruple{		//四元式定义
    int label;		//是否跳转,0表示不是跳转，1表示跳到别处，2表示跳到此处
    Token op;		//op中code == -1 表示叶子结点，运算为空
    Token arg1;		//如果没有内容code内容选-1
    Token arg2;
    Token res;
    int pointer;	//跳转指向
};

struct flow_graph{
    int front;
    int end;
};

struct arc_node{	//有向图边的定义,这里每个结点最多有两条出边，因为只有两个运算对象
    int adjvex1;		//指向的顶点，没有指向时值为-1
    int adjvex2;
};

struct vex_node{
    Token op;	//在token序列中的运算符在关键字表的编号
    vector<Token> labels;	//主标记是第一个，附加标记
    arc_node arc;	//指向操作数
};

struct typel {
    string name;	//类型名称
    int lenth;	//类型应该分配的长度
};

class synbl{
public:
    Token name;
    int type;		//指向类型表，内容应该不变
    int cat;		//种类，1是函数，2是变量，3是赋值形参，4是域名
    int addr;		//指向1函数表,0表示不指
};

#endif // GLOBAL_H
