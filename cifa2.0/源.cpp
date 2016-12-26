#include"cifa.h"

#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;

char filec[10000], word[60];
char ww;//wwָʾ��ǰ�ַ� 
int tp, row;//tp��filec��ָ�룬ָ��ǰ�ַ�   row��¼����
int key;//key���ֱ��룬sum�����ж��������
int cnum = 0; //wnum��¼�ı���������,cnum��¼�ı��ַ�����


bool readFile(){
	int i = 0;
	FILE *fp;
	if ((fopen_s(&fp, "D:/code/cifa2.0/wenben.txt", "r")) != 0){
		cout << "No such file exists!" << endl;
		fclose(fp);
		return false;
	}
	else{
		while ((ww = fgetc(fp)) != EOF){
			if (ww != '\t') filec[i++] = ww; 
			else filec[i++] = ' ';
			cnum++;
		}
		filec[i] = '\0';
		cout << "Read sucessfully! " << endl;
		fclose(fp);
	}
	return true;
}
void initl(){
	int i;
	switch (key)
	{
	case 0:{//�Զ����ʶ��
			   for (i = 0; i < Id.size(); i++){//�ַ����������
				   if (!Id[i].compare(word))
					   break;
			   }
			   if (i == Id.size()){
				   Id.push_back(word);
			   }
			   tok.code = 0;
			   tok.value = i;
			   token.push_back(tok);
			   break;
	}
	case 1:{//�ַ�����
			   for (i = 0; i < ConstChar.size(); i++){//�ַ����������
				   if (!ConstChar[i].compare(word))
					   break;
			   }
			   if (i == ConstChar.size()){
				   ConstChar.push_back(word);
			   }
			   tok.code = 1;
			   tok.value = i;
			   token.push_back(tok);
			   break;
	}
	case 2:{//�ַ�������
			   for (i = 0; i < ConstString.size(); i++){//�ַ����������
				   if (!ConstString[i].compare(word))
					   break;
			   }
			   if (i == ConstString.size()){
				   ConstString.push_back(word);
			   }
			   tok.code = 2;
			   tok.value = i;
			   token.push_back(tok);
			   break;
	}
	case 3:{//����
			   for (i = 0; i < ConstNum.size(); i++){//�ַ����������
				   if (!ConstNum[i].compare(word))
					   break;
			   }
			   if (i == ConstNum.size()){
				   ConstNum.push_back(word);
			   }
			   tok.code = 3;
			   tok.value = i;
			   token.push_back(tok);
			   break;
	}
	case -2:row++; break;//����
	default:{//�ؼ��ֻ��߷�����
				/*
				k = keywords.size();
				for (i = 0; i < k; i++){//�ַ����������
				if (!keywords[i].compare(word))
				break;
				}
				if (i == k){
				keywords.push_back(word);
				}
				tok.code = key;
				tok.value = 0;//??
				token.push_back(tok);
				break;
				*/
				tok.code = key;
				tok.value = 0;
				token.push_back(tok);
				break;
	}
	}
}

void keyinit()//!!!
{
	keywords.resize(73);
	keywords[4] = "main";
	keywords[5] = "while";
	keywords[6] = "if";
	keywords[7] = "char";
	keywords[8] = "int";
	keywords[9] = "float";
	keywords[10] = "struct";
	keywords[11] = "+";
	keywords[12] = "-"; //�ṩ���﷨����
	keywords[13] = "*";
	keywords[14] = "/";	//�ṩ���﷨����
	keywords[15] = "{";
	keywords[16] = "}";
	keywords[17] = "=";
	keywords[18] = ",";
	keywords[19] = "[";
	keywords[20] = "]";
	keywords[21] = ";";
	keywords[22] = "\"";
	keywords[23] = "\'";
	keywords[24] = "(";
	keywords[25] = ")";
	keywords[26] = "&";
	keywords[27] = "|";
	keywords[28] = "!";
	keywords[29] = "void";
	keywords[30] = "else";
	keywords[31] = "return";
	keywords[32] = "&&";
	keywords[33] = "||";
	keywords[34] = "cout";
	keywords[35] = "cin";
	keywords[36] = "<<";
	keywords[37] = ">>";
	keywords[38] = "long";
	keywords[39] = "short";
	keywords[40] = "bool";
	keywords[41] = "double";
	keywords[42] = "typedef";
	keywords[43] = "unsigned";
	keywords[44] = "static";
	keywords[45] = "enum";
	keywords[46] = "for";
	keywords[47] = "do";
	keywords[48] = "continue";
	keywords[49] = "signed";
	keywords[50] = "extern";
	keywords[51] = "inline";
	keywords[52] = "const";
	keywords[53] = "default";
	keywords[54] = "case";
	keywords[55] = "break";
	keywords[56] = "switch";
	keywords[57] = "sizeof";
	keywords[58] = "union";
	keywords[59] = "auto";
	keywords[60] = "volatile";
	keywords[61] = "register";
	keywords[62] = "goto";
	keywords[63] = "restrict";
	keywords[64] = "Complex";
	keywords[65] = "Imaginary";
	keywords[66] = "==";
	keywords[67] = ">";
	keywords[68] = ">=";
	keywords[69] = "<";
	keywords[70] = "<=";
	keywords[71] = "=[]";
	keywords[72] = "[]=";
}

void print_token(){
	keyinit();
	for (int i = 0; i < token.size(); i++)
	{
		cout << "<";
		if (i<10)printf("0");
		if (i<100)printf("0");
		cout << i << ">\t";
		cout << "(" << token[i].code << "," << token[i].value << ")\t";
		switch (token[i].code)
		{
		case 0:cout << "<" << Id[token[i].value] << "> " << endl; break;
		case 1:cout << "<" << ConstChar[token[i].value] << "> " << endl; break;
		case 2:cout << "<" << ConstString[token[i].value] << "> " << endl; break;
		case 3:cout << "<" << ConstNum[token[i].value] << "> " << endl; break;
		case -1:cout << "Input not allowed !Please review your code in row " << row << "." << endl; break;//����������
		case 555:cout << "ERROR CONST CHAR ! Please review your const char in row " << row << "." << endl; break;//�ַ���������
		case 666:cout << "ERROR CONST FLOATING NUMBER ! Please review your floating number in row " << row << "." << endl; break;//����������
		default:cout << "<" << keywords[token[i].code] << "> " << endl; break;
		}//switch
	}   //forѭ��
}

void scaner()
{
	/*
	��������ַ������ɨ�衣ȥ���ո񣬽����Ϊ����飬�ֱ��Ǳ�ʾ�������֡����š�
	iT��ʶ�� A,ch 00
	cT�ַ� 'c','a' 01
	sT�ַ��� "ABC" 02
	CT���� 123,12.5 03
	KT�ؼ��� int,main����04~
	PT��� >= 50  ����������ͷָ���

	*/
	int m,n;//������
	for (n = 0; n<60; n++) word[n] = NULL;
	ww = filec[tp++];
	while (ww == ' '){//�ӷ��㿪ʼʶ��
		ww = filec[tp];
		tp++;
	}
	if ((ww >= 'a'&&ww <= 'z') || (ww >= 'A'&&ww <= 'Z') || ww == '_'){ //����ĸ����ĸ��_�������Ǳ�ʶ��/������/����
		m = 0;
		while ((ww >= '0'&&ww <= '9') || (ww >= 'a'&&ww <= 'z') || (ww >= 'A'&&ww <= 'Z')){
			word[m++] = ww;
			ww = filec[tp++];
		}
		tp--;
		key = 0;
		for (n = 0; n<7; n++){
			if (strcmp(word, KT[n]) == 0)	{
				key = n + 4;//"main"~"struct"4~10
				break;
			}
		}
		if (n == 7){
			for (; n < 10; n++){
				if (strcmp(word, KT[n]) == 0)	{
					key = n + 22;//"void"~"return"29~31
					break;
				}
			}
		}
		if (n == 10){
			for (; n < 12; n++){
				if (strcmp(word, KT[n]) == 0)	{
					key = n + 24;//"cout"~"cin"34~35
					break;
				}
			}
		}
		if (n ==12){
			for (; n < 39; n++){
				if (strcmp(word, KT[n]) == 0)	{
					key = n + 26;//"long"~"Imaginary"38~65
					break;
				}
			}
		}
	}
	else if ((ww >= '0'&&ww <= '9')){
		m = 0;
		while ((ww >= '0') && (ww <= '9')){
			word[m++] = ww;
			ww = filec[tp++];
		}
		if (ww == '.'){
			word[m++] = ww;//С����Ҳռһλ
			ww = filec[tp++];
			if ((ww >= '0') && (ww <= '9')){
				while ((ww >= '0') && (ww <= '9')){
					word[m++] = ww;
					ww = filec[tp++];
				}
				if (ww == 'f' || ww == 'd'){
					word[m++] = ww;
				}
				else tp--;
				key = 3;
			}
			else {
				word[m] = '\0';
				tp--;
				key = 666;//����������
			}
		}
		else {
			key = 3;//�������������뿩
			tp--;
		}
		word[m] = '\0';
	}
	else{//!!!!�������߽��
		m = 0;
		word[m++] = ww;
		switch (ww){   //�����ַ�
		case'>': ww = filec[tp++];
			if (ww == '='){
				key = 68;
				word[m++] = ww;
			}
			else if (ww == '>'){
				key = 37;
				word[m++] = ww;
			}
			else{
				key = 67; tp--;
			}
			word[m] = '\0';
			break;
		case'<': ww = filec[tp++];
			if (ww == '='){
				key = 70;
				word[m++] = ww;
			}
			if (ww == '<'){
				key = 36;
				word[m++] = ww;
			}
			else {
				key = 69; tp--;
			}
			word[m] = '\0';
			break;
		case'=': ww = filec[tp++];
			if (ww == '='){
				key = 66;
				word[m++] = ww;
			}
			else if (ww == '['){
				word[m++] = ww;
				ww = filec[tp++];
				if (ww == ']'){
					word[m++] = ww;
					key = 71;
				}
				else {
					key = 777;//���Ŵ���
					tp--;
				}
			}
			else {
				key = 17; tp--;
			}
			word[m] = '\0';
			break;
		case'+':key = 11; word[m] = '\0'; break;
		case'-':key = 12; word[m] = '\0'; break;
		case'*':key = 13; word[m] = '\0'; break;
		case'/':key = 14; word[m] = '\0'; break;
		case'{':key = 15; word[m] = '\0'; break;
		case'}':key = 16; word[m] = '\0'; break;
		case',':key = 18; word[m] = '\0'; break;
		case'[':ww = filec[tp++];
			if (ww == ']'){
				word[m++] = ww;
				ww = filec[tp++];
				if (ww = '='){
					word[m++] = ww;
					key = 72;
				}
				else{
					key = 777;
					tp--;
				}
			}
			else{
				key = 19; tp--;
			}
			word[m] = '\0';
			break;
		case']':key = 20; word[m] = '\0'; break;
		case';':key = 21; word[m] = '\0'; break;
		case'"':{//�ַ�������
					ww = filec[tp++];
					while (ww != '"'){//����һ����Χ
						word[m++] = ww; 
						ww = filec[tp++];
					}
					word[m++] = ww;
					word[m] = '\0';
					key = 2;
		}
			//'"'key = 22; word[0] = ww; 
			break;
		case'\'':{//�ַ����� word[m] = '\0';
					 ww = filec[tp++]; word[m++] = ww; ww = filec[tp++];
					 if (ww == '\''){
						 word[m++] = ww;
						 key = 1;
					 }
					 else key = 555; tp--;//�ַ���������
					 word[m] = '\0';
					 // key = 23; word[0] = ww;���ֻ�ǵ����ķ����أ����޿���
					 break;
		}

		case'(':key = 24; word[m] = '\0'; break;
		case')':key = 25; word[m] = '\0'; break;

		case'&':ww = filec[tp++];
			if (ww == '&'){//'&&'
				key = 32;
				word[1] = ww;
			}
			else { key = 26; tp--; }//'&'
			word[m] = '\0';
			break;
		case'|':ww = filec[tp++];
			if (ww == '|'){//'||'
				key = 33;
				word[1] = ww;
			}
			else { key = 27; tp--; }//'|'
			word[m] = '\0';
			break;

		case'!':key = 28; word[m] = '\0'; break;
		case'\n':key = -2; word[m] = '\0'; break;//��������
		default: key = -1; word[m] = '\0'; break;
		}
	}
		
}

int main()
{
	if (readFile()){
		tp = 0;
		row = 1;
		Id.clear();          //���ű���������
		ConstNum.clear();    //������ı�
		ConstString.clear(); //�ַ���������
		ConstChar.clear();   //�ַ�������
		while (tp<(cnum-1)){
			scaner();//����ɨ�躯��scaner����token��key
			initl();
			/*�������
			switch (key){
			case -1:cout << "Input not allowed !Please review your code in row " << row << "." << endl; system("pause"); break;//����������
			case 555:cout << "ERROR CONST CHAR ! Please review your const char in row " << row << "." << endl; system("pause"); break;//�ַ���������
			case 666:cout << "ERROR CONST FLOATING NUMBER ! Please review your floating number in row " << row << "." << endl; system("pause"); break;//����������
			case -2: row++; break;
			default: cout << "(" << key << " ,  " << word << ")" << endl; system("pause"); break;
			}*/
		} 
		print_token();
	}
	system("pause");
	return 0;
}
