#ifndef CIFA_H
#define CIFA_H

#include "global.h"
#include "yufa_analysis.h"
#include "compile.h"
#include "optimize.h"
#include "mainwindow.h"

extern bool syntax_analysis();//词法分析及四元式生成
extern void optimization();	//目标代码优化
extern string compilization();//汇编生成
extern vector<synbl> SYNBL;
extern vector<typel> TYPEL;
extern vector<Token> sem;
extern vector<string> check_list;
extern vector<rinfl> RINFL;
extern vector<ainfl> AINFL;
extern int inter_pro_pointer;//中间代码

void Psynbl();
int InsertConstNum(string strToken);

int readFile(string path);
void keyinit();
void initl();
void scaner();
void print_token();
int cifa_main(string path);
int _tmain();
void test();
int duAsm();

#endif // CIFA_H


