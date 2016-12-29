#ifndef COMPILE_H
#define COMPILE_H

#pragma once

#include "global.h"

extern vector<string> keywords;	//保留字表
extern vector<string> Id;			//符号表code=0
extern vector<string> ConstNum;	//常数表code=3
extern vector<string> ConstString;	//字符串常量表code=2
extern vector<string> ConstChar;		//字符常量表code=1

extern vector<quadruple> inter_pro;	//四元式序列

extern vector<Token> sym_list_stack;//存变量标识符的栈
extern vector<typel> TYPEL;
extern vector<synbl> SYNBL;//符号表
extern int VALL_pointer;  //表示活动记录当前指向
extern int temp_num;//临时变量ti指针，从零开始，code = -2

struct targe{
    int flag;//有几个操作数
    string cw;
    string arg1;
    string arg2;
    string label = "";
    targe* next;
};
struct arr_list{
    string name;
    int size;
};


void data_new_node(targe* temp);
string itos(int num);
void DSEG();
void CSEG();
void compilization();
void cout_compile();
#endif // COMPILE_H
