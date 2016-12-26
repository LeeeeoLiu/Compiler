#include <iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include <stdlib.h>
using namespace std;


class Token{
public:
	int code;	//关键字的类
	int value;	//在对应表中的位置
	Token(){ code = 0; value = 0; }
	Token(int a, int b){ code = a; value = b; }
	bool operator== (const Token& t1){
		if (code == t1.code && value == t1.value)    return true;
		else      return false;
	}
};

/*struct typel {
	string name;	//类型名称
	int lenth;	//类型应该分配的长度
};

class synbl{
public:
	Token name;
	int type;		//指向类型表，内容应该不变
	int cat;		//种类，1是函数，2是变量，3是赋值形参，4是域名
	int addr;		//指向1函数表,0表示不指
};*/

Token tok;
vector<Token> token;
//vector<synbl> SYNBL;
//vector<typel> TYPEL;

vector<string> keywords;	//保留字表
vector<string> ConstNum;	//常数表code=3
vector<string> ConstString;	//字符串常量表code=2
vector<string> ConstChar;		//字符常量表code=1
vector<string> Id;			//符号表code=0


//保留字  “main”~"struct"4~10,"void"~"return"29~31,"cout"~"cin"34~35,“long”~”Imaginary”38~65
char* KT[100] = { "main", "while", "if", "char", "int", "float", "struct", "void", "else", "return", "cout", "cin"
"long", "short", "bool", "double", "typedef", "unsigned", "static", "enum", "for", "do", "continue", "signed",
"extern", "inline", "const", "default", "case", "break", "switch", "sizeof", "union", "auto", "volatile",
"register", "goto", "restrict", "Complex", "Imaginary" };