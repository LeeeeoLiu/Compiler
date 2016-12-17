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

struct GCToken
{
    char content[100];//内容
    char describe[30];//描述
    int type;//种别码
    int addr;//地址
};


void dcqk();
int duqu();
int duAsm();
int zfleixing(char zi);
int fhleixing(char* dc);
int quzi();
void huaci();
void zfprint(int zfsh);
void shuchu();
void keyinit();
void zhuanma();
void print_token();
int cifa_main();
int _tmain();
void test();


#endif // CIFA_H
