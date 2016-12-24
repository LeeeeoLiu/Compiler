#include "cifa.h"

Token tok;
vector<Token> token;
char ch;			//当前字符
string strToken;
string programme;
vector<string> keywords;	//保留字表
vector<string> Id;			//符号表code=0
vector<string> ConstNum;	//常数表code=3
vector<string> ConstString;	//字符串常量表code=2
vector<string> ConstChar;		//字符常量表code=1
int file_pointer;

/**
Token串
*/
GCToken token_cifa[1000];
/**
Token串数组中的Token串数量
*/
int zongzishu;

/*char shuzi[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
char zimu[] = { '_', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F',
'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M' };
char bianbiefu[] = { '.', '\'', '"', ' ' };*/
char yunsuanfu[][4] = { "!", "&", "~", "^", "*", "/", "%", "+", "-", "<", ">", "=", "->", "++", "--", "<<",
">>", "<=", ">=", "==", "!=", "&&", "||", "+=", "-=", "*=", "/=", "^=", "&=", "~=", "%=", "|", "<<=", ">>=" }; // +61
char xianjiefu[][4] = { "(", ")", "[", "]", "{", "}", ".", "#", ",", ";", "'", "\"" }; //+100
char guanjianzi[][100] = { "auto", "break", "case", "char", "const", "continue", "default", "do",
"double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register",
"restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
"unsigned", "void", "volatile", "while", "bool", "Complex", "Imaginary","cin","cout"}; //+1

void Psynbl()
{
    cout << "Synbl List:\n";
    for (int i = 0; i < SYNBL.size(); i++)
    {
        cout << "Name: ";
        switch (SYNBL[i].name.code)
        {
        case 0:cout << Id[SYNBL[i].name.value] << "\t"; break;
        case 1:cout << ConstChar[SYNBL[i].name.value] << "\t"; break;
        case 2:cout << ConstString[SYNBL[i].name.value] << "\t"; break;
        case 3:cout << ConstNum[SYNBL[i].name.value] << "\t"; break;
        default:cout << keywords[SYNBL[i].name.code] << "\t";
        }
        cout << "TYPEL: " << TYPEL[SYNBL[i].type].name << "\t";
        switch (SYNBL[i].cat)
        {
        case 1:cout << "FUNC\n"; break;
        case 2:cout << "VARI\n"; break;
        case 3:cout << "PARA\n"; break;
        case 4:cout << "DOMA\n"; break;
        default:cout << "ERRO\n"; break;
        }

    }
}


int InsertConstNum(string strToken){
    ConstNum.push_back(strToken);
    return ConstNum.size() - 1;
}

char zf;    //字符
char dqdc[100];
char wenben[10000];
int wtp;//指向当前字符 替换wenbenxuhao
int zifushu;


void dcqk()
{
    for (int i = 0; i<100; i++){ dqdc[i] = ' '; }
}
void dcqw()
{
    for (int i = 0; i<100; i++)
    {
        if (dqdc[i] == ' ' || dqdc[i] == '\0'){ dqdc[--i] = '\0'; break; }
    }
}
int duqu()
{
    zifushu = 0;
    FILE *fp = fopen("D:\\QT/Code/Compiler/C-Compiler/wenben.txt", "r");
    if (fp == NULL){ printf("No File\n"); fclose(fp); return 0; }
    else
    {
        while ((zf = fgetc(fp)) != EOF)
        {
            if (zf != '\n'&&zf != '\t'){ wenben[zifushu++] = zf; }
            else { wenben[zifushu++] = ' '; }
        }
    }
    printf("Reading Finished\n");
    fclose(fp);
    return 1;
}
int duAsm()
{
    zifushu = 0;
    FILE *fp = fopen("D:\\QT/Code/Compiler/C-Compiler/test.asm", "r");
    if (fp == NULL){ printf("No File\n"); fclose(fp); return 0; }
    else
    {
        while ((zf = fgetc(fp)) != EOF)
        {
             wenben[zifushu++] = zf;
        }
    }
    printf("Reading Finished\n");
    fclose(fp);
    return 1;
}

int zfleixing(char w)
{
   /* for (int bijiao = 0; bijiao<53; bijiao++)
    {
        if (zi == zimu[bijiao])return 0;
    }
    for (int bijiao = 0; bijiao<10; bijiao++)
    {
        if (zi == shuzi[bijiao])return 52;
    }
    for (int bijiao = 0; bijiao<4; bijiao++)
    {
        if (zi == bianbiefu[bijiao])return bijiao + 55;
    }
    return 1;*/
    if((w>='a'&&w<='z')||(w>='A'&&w<='Z')||(w=='_'))return 0;//字母，包括下划线
    else if(w>='0'&&w<='9')return 52;//数字
    else if(w=='.') return 55;//以下为有特殊作用的符号，小数点区分整数和浮点数，单双引号区别字符常量字符串常量与自定义标识符
    else if(w=='\'')return 56;
    else if(w=='"')return 57;
    else if(w==' ')return 58;
    else return 1;
}

int fhleixing(char* dc)
{
    for (int bijiao = 0; bijiao<34; bijiao++)
    {
        if (strcmp(dc, yunsuanfu[bijiao]) == 0)return bijiao + 61;
    }
    for (int bijiao = 0; bijiao<12; bijiao++)
    {
        if (strcmp(dc, xianjiefu[bijiao]) == 0)return bijiao + 100;
    }
    if (strlen(dc) == 1)return 994; //无符号
    else return 0;
}


int quzi(){
    //按一定词法规则划分完整的单词
    dcqk();
    int i = 0;
    int biaoji = 999;
    while (wenben[wtp] == ' ')wtp++;//从非空开始读
    if (wtp<zifushu){
        while (wenben[wtp] != ' ')
        {
            biaoji = zfleixing(wenben[wtp]);
            dqdc[i++] = wenben[wtp++];
            if (biaoji == 0)//首字母是字母或_，可能是标识符/变量名/保留字
            {
                while (zfleixing(wenben[wtp]) == 0 || zfleixing(wenben[wtp]) == 52)//字母或_或数字，符合标识符定义规则
                {
                    dqdc[i++] = wenben[wtp++];
                }
                dqdc[i] = '\0'; return 0;
            }
            else if (biaoji == 56)//首字符是单引号 字符常量
            {
                dqdc[i++] = wenben[wtp++];
                if (zfleixing(wenben[wtp]) == 56)
                {
                    dqdc[i++] = wenben[wtp++];
                    dqdc[i] = '\0'; return 56;
                }
                else { dqdc[i] = '\0'; return 990; } //单字符错误
            }
            else if (biaoji == 57)//首字符是双引号 字符串常量
            {
                while (zfleixing(wenben[wtp]) != 57){ dqdc[i++] = wenben[wtp++]; }
                dqdc[i++] = wenben[wtp++];
                dqdc[i] = '\0'; return 57;
            }
            else if (biaoji == 52)
            {
                while (zfleixing(wenben[wtp]) == 52){ dqdc[i++] = wenben[wtp++]; }
                if (zfleixing(wenben[wtp]) == 55){ dqdc[i++] = wenben[wtp++]; }//小数点
                else { dqdc[i] = '\0'; return 51; }
                if (zfleixing(wenben[wtp]) == 52){ dqdc[i++] = wenben[wtp++]; }
                else { dqdc[i] = '\0'; return 991; } //数字错误
                while (zfleixing(wenben[wtp]) == 52){ dqdc[i++] = wenben[wtp++]; }
                if (wenben[wtp] == 'f'){ dqdc[i++] = wenben[wtp++]; dqdc[i] = '\0'; return 53; }
                else if (wenben[wtp] == 'd'){ dqdc[i++] = wenben[wtp++]; dqdc[i] = '\0'; return 54; }
                dqdc[i] = '\0'; return 53;
            }
            else if (biaoji == 1)
            {
                while (zfleixing(wenben[wtp]) == 1 && wenben[wtp] != ' ' && wtp < zifushu){ dqdc[i++] = wenben[wtp++]; }
                dqdc[i] = '\0'; return 1;
            }
            else { dqdc[i] = '\0'; return biaoji; }
        }
    }
    //下面的return肯定走不到，但是不写的话，编译器会报错，所以必须写。
    return 1000;
}

void huaci()
{
    int fuhaobiaoaddr = 0;
    zongzishu = 0;
    for (wtp = 0; wtp<zifushu;)
    {
        int leix = quzi();
        if (leix == 1000)
        {
            wtp = zifushu;
        }
        else if (leix == 0)
        {
            int bijiao;
            for (bijiao = 0; bijiao <= 38; bijiao++)
            {
                if (strcmp(dqdc, guanjianzi[bijiao]) == 0)//关键字表！！为什么不直接做成token序列！！！！
                {
                    strcpy(token_cifa[zongzishu].content, dqdc);
                    strcpy(token_cifa[zongzishu].describe, "KEY_DESC\0");
                    token_cifa[zongzishu].type = bijiao + 1;
                    //keyMap.push_back(make_pair(token_cifa[zongzishu].content,token_cifa[zongzishu].type));
                    zongzishu++;
                    break;
                }

            }
            if (bijiao == 39)//用户自定义标识符，放入符号表
            {
                strcpy(token_cifa[zongzishu].content, dqdc);
                strcpy(token_cifa[zongzishu].describe, "IDENTIFER_DESC\0");
                token_cifa[zongzishu].type = 40;
                token_cifa[zongzishu].addr = fuhaobiaoaddr++;//！！
                zongzishu++;
            }

        }
        else if (leix == 56)//字符常量
        {
            strcpy(token_cifa[zongzishu].content, dqdc);
            strcpy(token_cifa[zongzishu].describe, "ACHAR_DESC\0");
            token_cifa[zongzishu].type = leix;
            zongzishu++;
        }
        else if (leix == 57)//字符串常量
        {
            strcpy(token_cifa[zongzishu].content, dqdc);
            strcpy(token_cifa[zongzishu].describe, "ASTRING_DESC\0");
            token_cifa[zongzishu].type = leix;
            zongzishu++;
        }
        else if (leix <= 54 && leix >= 51)//数字常量
        {
            strcpy(token_cifa[zongzishu].content, dqdc);
            strcpy(token_cifa[zongzishu].describe, "CONSTANT_DESC\0");
            token_cifa[zongzishu].type = leix;
            zongzishu++;
        }
        else if (leix == 1)
        {
            for (;;)
            {
                int fhlx = fhleixing(dqdc);
                if (fhlx == 0)
                {
                    dcqw();
                    wtp--;
                }
                else if (fhlx == 994)//fhleixing返回值994 if (strlen(dc) == 1)return 994;
                {
                    strcpy(token_cifa[zongzishu].content, dqdc);
                    strcpy(token_cifa[zongzishu].describe, "ERROR_OPE\0");
                    token_cifa[zongzishu].type = fhlx;
                    zongzishu++;
                    break;
                }
                else
                {
                    strcpy(token_cifa[zongzishu].content, dqdc);
                    token_cifa[zongzishu].type = fhlx;
                    if (fhlx<99)//运算符
                    {
                        strcpy(token_cifa[zongzishu].describe, "OPE_DESC\0");
                        //operMap.push_back(make_pair(token_cifa[zongzishu].content,token_cifa[zongzishu].type));
                    }
                    else//边界符
                    {
                        strcpy(token_cifa[zongzishu].describe, "CLE_OPE_DESC\0");
                        //limitMap.push_back(make_pair(token_cifa[zongzishu].content,token_cifa[zongzishu].type));
                    }
                    zongzishu++;
                    break;
                }

            }

        }
        else
        {
            strcpy(token_cifa[zongzishu].content, dqdc);
            if (leix == 990)
            {
                strcpy(token_cifa[zongzishu].describe, "ERROR_ACHAR\0");
                token_cifa[zongzishu].type = leix;
            }
            else if (leix == 991)
            {
                strcpy(token_cifa[zongzishu].describe, "ERROR_CONSTANT\0");
                token_cifa[zongzishu].type = leix;
            }
            zongzishu++;
        }
    }
}

void zfprint(int zfsh)
{
    printf("<");
    printf("%s", token_cifa[zfsh].content);
    printf(">\t");
    printf("<");
    if (token_cifa[zfsh].type<9)printf("0");
    if (token_cifa[zfsh].type<99)printf("0");
    printf("%d", token_cifa[zfsh].type);
    printf(">\t");
    printf("<");
    printf("%s", token_cifa[zfsh].describe);
    printf(">\t");
    if (token_cifa[zfsh].addr != 0)
    {
        printf("<");
        if (token_cifa[zfsh].addr<9)printf("0");
        if (token_cifa[zfsh].addr<99)printf("0");
        printf("%d", token_cifa[zfsh].addr);
        printf(">");
    }
}

void shuchu()
{
    printf("序号\tContent\tType\tDescribe\t\tAddr\n");
    for (int wshu = 0; wshu<zongzishu; wshu++)
    {
        if (wshu<9)printf("0");
        if (wshu<99)printf("0");
        printf("%d", wshu + 1);
        printf("\t");
        zfprint(wshu);
        printf("\n");
    }
    /*
    for(int i=0;i<keyMap.size();i++)
    {
    printf("%s,%d\n",keyMap[i].first,keyMap[i].second);
    }
    for(int i=0;i<operMap.size();i++)
    {
    printf("%s,%d\n",operMap[i].first,operMap[i].second);
    }
    for(int i=0;i<limitMap.size();i++)
    {
    printf("%s,%d\n",limitMap[i].first,limitMap[i].second);
    }
    */
}

void keyinit()
{
    keywords.resize(38);
    keywords[4] = "main";
    keywords[5] = "while";
    keywords[6] = "if";
    keywords[7] = "char";
    keywords[8] = "int";
    keywords[9] = "float";
    keywords[10] = "struct";
    keywords[11] = "+";
    keywords[12] = "-"; //提供给语法分析
    keywords[13] = "*";
    keywords[14] = "/";	//提供给语法分析
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
    keywords[33] ="||";
    keywords[34] ="cout";
    keywords[35] ="cin";
    keywords[36] ="<<";
    keywords[37] =">>";
}

void zhuanma()
{
    Id.clear();          //符号表的序列项表
    ConstNum.clear();    //常数表的表
    ConstString.clear(); //字符串常量表
    ConstChar.clear();   //字符常量表
    for (int dangqiandanci = 0; dangqiandanci<zongzishu; dangqiandanci++)
    {
        switch (token_cifa[dangqiandanci].type)
        {
        case 40:
        {
                   if (strcmp(token_cifa[dangqiandanci].content, "main") == 0)
                   {
                       tok.code = 4;
                       tok.value = 0;
                       token.push_back(tok);
                   }
                   else
                   {
                       int i;
                       for (i = 0; i < Id.size(); i++) //查找表中是否已经存在
                       {
                           if (!Id[i].compare(token_cifa[dangqiandanci].content))
                               break;
                       }
                       if (i == Id.size())
                       {
                           Id.push_back(token_cifa[dangqiandanci].content);
                       }
                       tok.code = 0;
                       tok.value = i;
                       token.push_back(tok);
                   }
                   break;
        }
        case 56://字符常量
        {
                   int i;
                   for (i = 0; i < ConstChar.size(); i++)//字符常量表查重
                   {
                       if (!ConstChar[i].compare(token_cifa[dangqiandanci].content))
                           break;
                   }
                   if (i == ConstChar.size())
                       ConstChar.push_back(token_cifa[dangqiandanci].content); //插入字符串常量表并赋值

                   tok.value = i;
                   tok.code = 1;
                   token.push_back(tok);
                   break;
        }
        case 57://字符串常量
        {
                   int i;
                   for (i = 0; i < ConstString.size(); i++)//字符串常量表查重
                   {
                       if (!ConstString[i].compare(token_cifa[dangqiandanci].content))
                           break;
                   }
                   if (i == ConstString.size())
                       ConstString.push_back(token_cifa[dangqiandanci].content); //插入字符串常量表并赋值

                   tok.value = i;
                   tok.code = 2;
                   token.push_back(tok);
                   break;
        }
        case 51:
        case 53:
        case 54://常数表
        {
                   int i;
                   for (i = 0; i < ConstNum.size(); i++)//数字常量表查重
                   {
                       if (!ConstNum[i].compare(token_cifa[dangqiandanci].content))
                           break;
                   }
                   if (i == ConstNum.size())
                       ConstNum.push_back(token_cifa[dangqiandanci].content); //插入数字常量表并赋值

                   tok.value = i;
                   tok.code = 3;
                   token.push_back(tok);
                   break;
        }
        case 34:
        {
                   tok.code = 5;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 16:
        {
                   tok.code = 6;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 4:
        {
                  tok.code = 7;
                  tok.value = 0;
                  token.push_back(tok);
                  break;
        }
        case 18:
        {
                   tok.code = 8;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 13:
        {
                   tok.code = 9;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 27:
        {
                   tok.code = 10;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 68:
        {
                   tok.code = 11;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 69:
        {
                   tok.code = 12;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 65:
        {
                   tok.code = 13;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 66:
        {
                   tok.code = 14;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 104:
        {
                    tok.code = 15;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 105:
        {
                    tok.code = 16;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 72:
        {
                   tok.code = 17;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 108:
        {
                    tok.code = 18;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 102:
        {
                    tok.code = 19;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 103:
        {
                    tok.code = 20;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 109:
        {
                    tok.code = 21;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 111:
        {
                    tok.code = 22;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 110:
        {
                    tok.code = 23;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 100:
        {
                    tok.code = 24;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 101:
        {
                    tok.code = 25;
                    tok.value = 0;
                    token.push_back(tok);
                    break;
        }
        case 62:
        {
                   tok.code = 26;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 92:
        {
                   tok.code = 27;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 61:
        {
                   tok.code = 28;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 32:
        {
                   tok.code = 29;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 10:
        {
                   tok.code = 30;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 22:
        {
                   tok.code = 31;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 82:
        {
                   tok.code = 32;
                   tok.value = 0;
                   token.push_back(tok);
                   token.push_back(tok);
                   break;
        }
        case 83:
        {
                   tok.code = 33;
                   tok.value = 0;
                   token.push_back(tok);
                   token.push_back(tok);
                   break;
        }
        case 39:
        {
                   tok.code = 34;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 38:
        {
                   tok.code = 35;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 76:
        {
                   tok.code = 36;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        case 77:
        {
                   tok.code = 37;
                   tok.value = 0;
                   token.push_back(tok);
                   break;
        }
        default:;
        }
    }
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
        default:cout << "<" << keywords[token[i].code] << "> " << endl;
        }//switch
    }   //for循环
}

int cifa_main()
{
    printf("Start To Read A File\n");
    if (duqu())
    {
        printf("Start To Analysis Words\n");
        huaci();
        zhuanma();
        printf("Words Analysis\nToken:\n");
        print_token();
        //shuchu();
        printf("Words Analysis Finished\n");
    }
    system("pause");
    return 0;
}

int _tmain()
{
    cifa_main();
    cout << "Start Syntax Analysis:\n" << endl;
    system("pause");
    if (syntax_analysis())
    {
        cout << "Syntax Analysis Succeed\n" << endl;
        Psynbl();
        system("pause");
        optimization();
        system("pause");
        cout << "Start Assembly:\n" << endl;
        system("pause");
        compilization();

        duAsm();
        for (int aa=0; aa < zifushu; aa++)
        {
            printf("%c", wenben[aa]);
        }
    }
    else
    {
        cout << "Start Syntax Error And Over\n" << endl;
    }
    //simplePriority();
    //LL1();
    system("pause");
    return 0;
}

void test(){
    cout<<"test"<<endl;
}
