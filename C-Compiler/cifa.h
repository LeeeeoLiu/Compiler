#ifndef CIFA_H
#define CIFA_H

#include "global.h"
#include "yufa_analysis.h"
#include "compile.h"
#include "optimize.h"

extern bool syntax_analysis();//词法分析及四元式生成
extern void optimization();	//目标代码优化
extern void compilization();//汇编生成
extern vector<synbl> SYNBL;
extern vector<typel> TYPEL;

void Psynbl();
int InsertConstNum(string strToken);

int readFile();
void keyinit();
void initl();
void scaner();
void print_token();
int cifa_main();
int _tmain();
void test();

#endif // CIFA_H


