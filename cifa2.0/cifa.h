#include <iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include <stdlib.h>
using namespace std;


class Token{
public:
	int code;	//�ؼ��ֵ���
	int value;	//�ڶ�Ӧ���е�λ��
	Token(){ code = 0; value = 0; }
	Token(int a, int b){ code = a; value = b; }
	bool operator== (const Token& t1){
		if (code == t1.code && value == t1.value)    return true;
		else      return false;
	}
};

/*struct typel {
	string name;	//��������
	int lenth;	//����Ӧ�÷���ĳ���
};

class synbl{
public:
	Token name;
	int type;		//ָ�����ͱ�����Ӧ�ò���
	int cat;		//���࣬1�Ǻ�����2�Ǳ�����3�Ǹ�ֵ�βΣ�4������
	int addr;		//ָ��1������,0��ʾ��ָ
};*/

Token tok;
vector<Token> token;
//vector<synbl> SYNBL;
//vector<typel> TYPEL;

vector<string> keywords;	//�����ֱ�
vector<string> ConstNum;	//������code=3
vector<string> ConstString;	//�ַ���������code=2
vector<string> ConstChar;		//�ַ�������code=1
vector<string> Id;			//���ű�code=0


//������  ��main��~"struct"4~10,"void"~"return"29~31,"cout"~"cin"34~35,��long��~��Imaginary��38~65
char* KT[100] = { "main", "while", "if", "char", "int", "float", "struct", "void", "else", "return", "cout", "cin"
"long", "short", "bool", "double", "typedef", "unsigned", "static", "enum", "for", "do", "continue", "signed",
"extern", "inline", "const", "default", "case", "break", "switch", "sizeof", "union", "auto", "volatile",
"register", "goto", "restrict", "Complex", "Imaginary" };