#include "cifa.h"

Token tok;
vector<Token> token;
string str[]={"","","","","main","while","if","char","int","float","struct","+","-","*","/","{","}","=",",","[","]",";","\"",
              "'","(",")","&","|","!","void","else","return","&&","||","cout","cin","<<",">>","long","short","bool",
              "double","typedef","unsigned","static","enum","for","do","continue","signed","extern","inline","const",
              "default","case","break","switch","sizeof","union","auto","volatile","register","goto","restrict",
              "Complex","Imaginary","==",">",">=","<","<=","=[]","[]=",".",".=","=.","call","ret"};
vector<string> keywords(str, str+78);//保留字表
vector<string> ConstNum;	//常数表code=3
vector<string> ConstString;	//字符串常量表code=2
vector<string> ConstChar;		//字符常量表code=1
vector<string> Id;			//符号表code=0
char ww;//ww指示当前字符
char filec[10000], word[60];
int tp, row;//tp是filec的指针，指向当前字符   row记录行数
int key;//key是种别码，sum用于判断整数溢出
int cnum = 0; //wnum记录文本单词总数,cnum记录文本字符总数
static int front = 0;
//保留字  “main”~"struct"4~10,"void"~"return"29~31,"cout"~"cin"34~35,“long”~”Imaginary”38~65
char* KT[100] = { "main", "while", "if", "char", "int", "float", "struct", "void", "else", "return", "cout", "cin",
                  "long", "short", "bool", "double", "typedef", "unsigned", "static", "enum", "for", "do", "continue",
                  "signed","extern", "inline", "const", "default", "case", "break", "switch", "sizeof", "union", "auto",
                  "volatile","register", "goto", "restrict", "Complex", "Imaginary"};

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

int readFile(){
    int i = 0;
    FILE *fp=fopen("D:/Code/Compiler/C-Compiler/wenben.txt", "r");
    if (fp == NULL){
        cout << "No such file exists!" << endl;
        fclose(fp);
        return 0;
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
    return 1;
}
int duAsm()
{
    tp = 0;
    FILE *fp = fopen("D:/Code/Compiler/C-Compiler/test.asm", "r");
    if (fp == NULL){ printf("No File\n"); fclose(fp); return 0; }
    else
    {
        while ((ww = fgetc(fp)) != EOF)
        {
             filec[tp++] = ww;
        }
    }
    printf("Reading Finished\n");
    fclose(fp);
    return 1;
}

void initl(){
    int i;
    switch (key)
    {
    case 0:{//自定义标识符
               for (i = 0; i < Id.size(); i++){//字符常量表查重
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
    case 1:{//字符常量
               for (i = 0; i < ConstChar.size(); i++){//字符常量表查重
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
    case 2:{//字符串常量
               for (i = 0; i < ConstString.size(); i++){//字符常量表查重
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
    case 3:{//常数
               for (i = 0; i < ConstNum.size(); i++){//字符常量表查重
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
    case -2:row++; break;//换行
    case 111:break;
    default:{   //关键字或者符号了
                tok.code = key;
                tok.value = 0;
                token.push_back(tok);
                break;
    }
    }
}

void print_token(){
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
        case -1:cout << "Input not allowed !Please review your code in row " << row << "." << endl; break;//不允许输入
        case 555:cout << "ERROR CONST CHAR ! Please review your const char in row " << row << "." << endl; break;//字符常量出错
        case 666:cout << "ERROR CONST FLOATING NUMBER ! Please review your floating number in row " << row << "." << endl; break;//浮点数出错
        case 777:cout << "ERROR SYMBOL ! Please review your floating number in row " << row << "." << endl; break;//浮点数出错
        default:cout << "<" << keywords[(token[i].code)] << "> " << endl; break;
        }//switch
    }   //for循环
}

void scaner()
{
    /*
    对输入的字符串逐个扫描。去掉空格，将其分为三大块，分别是标示符、数字、符号。
    iT标识符 A,ch 00   cT字符 'c','a' 01   sT字符串 "ABC" 02  CT常数 123,12.5 03  KT关键字 int,main……04~   PT界符 >= 50  包括运算符和分隔符
    */
    int m,n;//计数器
    for (n = 0; n<60; n++) word[n] = NULL;
    ww = filec[tp++];
    while (ww == ' '){//从非零开始识别
        ww = filec[tp];
        tp++;
    }
    if (front == 2){
        m = 0;
        while (ww != '"'){
            word[m++] = ww;
            ww = filec[tp++];
        }
        word[m] = '\0';
        tp--;
        key = 02;
        front = 1;
        return;
    }
    if (front == 4){
        m = 0;
        while (ww != '\'')	{
            word[m++] = ww;
            ww = filec[tp++];
        }
        if (m==1) key = 1;
        else key = 555;//字符常量错误
        tp--;
        word[m] = '\0';
        front = 3;
        return;
    }
    if ((ww >= 'a'&&ww <= 'z') || (ww >= 'A'&&ww <= 'Z') || ww == '_'){ //首字母是字母或_，可能是标识符/变量名/保留
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
        return;
    }
    else if ((ww >= '0'&&ww <= '9')){
        m = 0;
        while ((ww >= '0') && (ww <= '9')){
            word[m++] = ww;
            ww = filec[tp++];
        }
        if (ww == '.'){
            word[m++] = ww;//小数点也占一位
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
                key = 666;//浮点数错误
            }
        }
        else {
            key = 3;//整数，数字输入咯
            tp--;
        }
        word[m] = '\0';
        return;
    }
    else{//!!!!运算符或边界符
        m = 0;
        word[m++] = ww;
        switch (ww){   //其他字符
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
            else if (ww == '<'){
                key = 36;
                word[m++] = ww;
            }
            else {
                key = 69; tp--;
            }
            word[m] = '\0';
            break;
        case'=': {
            ww = filec[tp++];
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
                    key = 777;//符号错误
                    tp--;
                }
            }
            else if(ww=='.'){
                key=75;
                word[m++]=ww;
            }
            else {
                key = 17; tp--;
            }
            word[m] = '\0';
            break;
        }
        case'+':key = 11; word[m] = '\0'; break;
        case'-':key = 12; word[m] = '\0'; break;
        case'*':{
            ww=filec[tp++];
            if(ww=='/'){                
                key=111;
            }
            else{
                key = 13;
                tp--;
            }
            word[m] = '\0';
            break;
        }
        case'/':{
            ww=filec[tp++];
            if(ww=='/'){
                key=111;
                while(ww!='\n'){
                    ww=filec[tp++];
                }
                tp--;
            }
            else if(ww=='*'){
                key=111;
                ww=filec[tp++];
                while(1){
                    while(ww!='*') {
                        if(ww=='\n') row++;
                        ww=filec[tp++];//ww='*'
                    }
                    ww=filec[tp++];
                    if(ww=='/') {
                        tp-=2;
                        break;
                    }
                }
            }
            else {
                key = 14;
                tp--;
            }
            word[m]='\0';
            break;
        }
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
        case'"':{
                    key = 22; word[0] = ww; word[1] = '\0';
                    if (front == 1) front = 0;//字符串常量读取完毕
                    else front = 2;
                    break;
        }
        case'\'':{//字符常量
                     key = 23; word[0] = ww;word[m] = '\0';
                     if (front == 3) front = 0;
                     else front = 4;
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
        case'.': {
            ww = filec[tp++];
            if (ww == '='){
                key = 74;
                word[m++] = ww;
            }
            else {
                key = 73; tp--;
            }
            word[m] = '\0';
            break;
        }
        case'!': key = 28; word[m] = '\0'; break;
        case'\r':
        case'\n':key = -2; word[m] = '\0'; break;//计算行数
        default: key = -1; word[m] = '\0'; break;
        }
    }
}

int cifa_main()
{
    cout<<"Start To Read A File."<<endl;
    if (readFile()){
        tp = 0;
        row = 1;
        Id.clear();          //符号表的序列项表
        ConstNum.clear();    //常数表的表
        ConstString.clear(); //字符串常量表
        ConstChar.clear();   //字符常量表
        cout<<"Start To Analysis Words"<<endl;
        while (tp<(cnum-1)){
            scaner();//调用扫描函数scaner返回token的key
            initl();
        }
        cout<<"Words Analysis Result\nToken:"<<endl;
        print_token();
        cout<<"Words Analysis Finished."<<endl;
    }
    system("pause");
    return 0;
}

int _tmain()
{
    cifa_main();
    cout << "Start Syntax Analysis:\n" << endl;
    //system("pause");
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
        for (int aa=0; aa < tp; aa++)
        {
            printf("%c", filec[aa]);
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
