#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#pragma once

#include "global.h"

extern vector<string> keywords;	//保留字表
extern vector<string> Id;			//符号表code=0
extern vector<string> ConstNum;	//常数表code=3
extern vector<string> ConstString;	//字符串常量表code=2
extern vector<string> ConstChar;		//字符常量表code=1
extern int InsertConstNum(string strToken);

extern vector<quadruple> inter_pro;	//四元式序列


string ftos(float num);
void basic_block();
void DAG_optimaize(int front, int end);
int find_first_cal(int start, int end);
void creat_quad(int start_pointer, int end_pointer);
void output_inter_pro(int front, int end);
void optimization();

#endif // OPTIMIZE_H
