
#include "yufa_analysis.h"

string errorMessage;
/**
 语法分析结果中，所有的错误集合
 如果缺少了一个符号，程序会直接向下分析，而不会停止。但是如果当处符号错误，就不能继续分析了。
 */
MJErrorContainer sharedErrorContainer = MJErrorContainer();

/**
 存储变量标识符的栈
 */
vector<Token> sym_list_stack;
/**
 类型应该分配的长度
 **/
vector<typel> TYPEL = { { "char", 2 }, { "inta", 2 }, { "float", 2 } ,{"struct",},{"#define",}};
/**
 符号表
 */
vector<synbl> SYNBL;
/**
 数组表
*/
vector<ainfl> AINFL;
/**
  数组表标号
  */
int arrIndex=0;
int tempArrIndex=0;

/**
 结构体表
*/
vector<rinfl> RINFL;
/**
 结构体标号
*/
int structIndex;
int tempStructIndex;
int structOFF=0;
bool isArr=false;
bool isStruct=false;
/**
 活动记录当前指向
 */
int VALL_pointer ;
/**
 函数表
 */
vector<funtion_list>Funtion_list;
/**
  函数表元素临时变量,为方便各函数调用，应设为全局变量
  **/
funtion_list funtion_temp;
/**
 四元式序列
 */
vector<quadruple> inter_pro;
/**
 当前四元式是否被指入
 */
bool in_flag = false;

/**
 Token序列
 */
extern vector<Token> token;
/**
 关键字表
 */
extern vector<string> keywords;
/**
 临时变量ti指针，从零开始，code = -2
 */
int temp_num = 0;
/**
 操作数栈
 */
vector<Token> sem;
vector<Token> tokenStash;


Token currentToken;
int token_pointer;
Token currentFunctionToken;

bool funcstart=false;

//标识是否是返回语句
bool isReturn=false;

//标识函数块
bool funcArea=false;

//标识 if 块
bool ifArea=false;

//标识 else 块
bool elseArea=false;

//标识 while 块
bool whileArea=false;

int mainStartId;

/**
 语法分析是否成功。成功为true，失败为false。
 */
bool isGrammarCorrect;

/**
 生成错误信息，让语法分析器继续向后分析。
 在这种情况下，如果仅仅缺少了一个符号，就可以继续向后分析。但如果符号错了，或者是多了一个符号，就不好办了，后面的会报一大堆错。
 */
void errorHappenedWithMessage(string message) {
    isGrammarCorrect = false;
    MJSyntaxError newError = MJSyntaxError(message, token_pointer);
    if (!sharedErrorContainer.addError(newError)) {
        //如果同一个位置错误超过一个，就不能继续识别下一个错误了。
        cout<<"同一个位置错误超过两个不能继续分析"<<endl;
        exit(0);
    }
}

/**
 出口时，读取下一个字符
 @return 如果能读取下一个，`true`；否则，`false`。
 */
bool next() {
    if (token_pointer < token.size()) {
        currentToken = token[token_pointer];
        token_pointer++;
        return true;
    }
    else {
        return false;
    }
}

int isSynblExist(){
    //先查一下符号表
    int i;
    for (i = 0; i < SYNBL.size(); i++)
    {
        if (currentToken.value == SYNBL[i].name.value)
        {
            break;
        }
    }
    if (i == SYNBL.size())
        return 0;
    else
        return 1;
}

int getSynblIndex(Token tempName){
    //先查一下符号表
    int i;
    for (i = 0; i < SYNBL.size(); i++)
    {
        if (tempName.value == SYNBL[i].name.value && tempName.code == SYNBL[i].name.code)
        {
            break;
        }
    }
    if (i == SYNBL.size())
    {
        return -1;
    }
    return i;
}

/**
 生成函数返回的四元式
 */

void retQuat(){
    quadruple temp;
    temp.op = Token(77,-1);
    temp.arg2 = Token(-1,-1);
    temp.arg1 = Token(-1,-1);
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.label=1;
    temp.pointer = -1;
    temp.res= Token(-1,-1);
    inter_pro.push_back(temp);
}

/**
 生成函数调用的四元式
 */

void callQuat(Token fun){
    quadruple temp;
    temp.op = Token(76,-1);
    temp.arg2 = Token(-1,-1);
    temp.arg1 = Token(-1,-1);
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.label=1;
    temp.pointer = SYNBL[getSynblIndex(fun)].addr;
    temp.res= fun;
    for(int i=temp.pointer;i<inter_pro.size();i++)
    {
        if(inter_pro[i].op==Token(77,-1))
        {
            inter_pro[i].pointer=inter_pro.size()+1;
            break;
        }
    }
    inter_pro.push_back(temp);
}

/**
 生成计算表达式的四元式
 */
void cal_QUAT(int op) {
    quadruple temp;
    temp.op = Token(op, -1);
    temp.arg2 = sem.back();		//sem[s]
    sem.pop_back();
    temp.arg1 = sem.back();		//sem[s-1]
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }

    temp.pointer = NULL;
    temp.res = Token(-2, temp_num);//赋值给ti
    sem.push_back(Token(-2, temp_num));
    inter_pro.push_back(temp);
    temp_num++;
}


void self_Quat1(int code)//++ -- (++,a,_,a)
{
    quadruple temp;
    temp.op=Token(code,-1);
    temp.arg1=sem.back();
    sem.pop_back();
    temp.arg2=Token(-1,-1);
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.res=sem.back();
    sem.pop_back();
    sem.push_back(temp.res);
    temp.pointer=NULL;
    temp.optimize_flag=1;
    inter_pro.push_back(temp);
    temp_num++;
}

/*
 * 生成 += -= *= /=的四元式(+=,a,6,a)
*/
void self_Quat2(int code){//80~83
    quadruple temp;
    temp.op=Token(code,-1);
    temp.arg2=sem.back();
    sem.pop_back();
    temp.arg1=sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.res=sem.back();
    sem.pop_back();
    temp.pointer=NULL;
    temp.optimize_flag=1;
    inter_pro.push_back(temp);
    temp_num++;
}


/**
 取数组值的四元式
 */
void arrGetQuat()
{
    quadruple temp;
    temp.op = Token(71,-1);
    temp.arg2=sem.back();
    sem.pop_back();
    temp.arg1=sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.pointer=NULL;
    temp.res = Token(-2, temp_num);//赋值给ti
    sem.push_back(Token(-2, temp_num));
    inter_pro.push_back(temp);
    temp_num++;
}

/**
 存数组值的四元式
 */
void arrStoreQuat()
{
    quadruple temp;
    temp.op = Token(72,-1);
    temp.arg1=sem.back();
    sem.pop_back();
    temp.arg2=sem.back();
    sem.pop_back();
    temp.res=sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.pointer=NULL;
    inter_pro.push_back(temp);
}

/**
 取结构体成员变量的四元式
 */
void struGetQuat()
{
    quadruple temp;
    temp.op = Token(75,-1);
    temp.arg2=sem.back();
    sem.pop_back();
    temp.arg1=sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.pointer=NULL;
    temp.res = Token(-2, temp_num);//赋值给ti
    sem.push_back(Token(-2, temp_num));
    inter_pro.push_back(temp);
    temp_num++;
}

/**
 存结构体成员变量的四元式
 */

void struStoreQuat()
{
    quadruple temp;
    temp.op = Token(74,-1);
    temp.arg1=sem.back();
    sem.pop_back();
    temp.arg2=sem.back();
    sem.pop_back();
    temp.res=sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    temp.pointer=NULL;
    inter_pro.push_back(temp);
}

/**
 生成赋值表达式的四元组
 */
void equa_QUAT(int op) {
    quadruple temp;
    temp.op = Token(op, -1);
    temp.arg2 = Token(-1,-1);
    temp.arg1 = sem.back();
    sem.pop_back();
    if (in_flag) {
        temp.label = 2;
        in_flag = 0;
    }
    else {
        temp.label = 0;
    }
    if(op==85)
        temp.optimize_flag=1;
    else
        temp.optimize_flag=0;
    temp.pointer = NULL;
    temp.res = sem.back();//赋值给=左边的符号
    sem.pop_back();
    inter_pro.push_back(temp);//四元式生成插入
    if(isArr==true&&isStruct==true){
        sem.push_back(temp.res);
        struStoreQuat();
        arrStoreQuat();
        isArr=false;
        isStruct=false;
    }
    if(isStruct==true){
        sem.push_back(temp.res);
        struStoreQuat();
        isStruct=false;
    }
    if(isArr==true){
        sem.push_back(temp.res);
        arrStoreQuat();
        isArr=false;
    }
}

void F() {
    if (currentToken.code == 3 || currentToken.code == 0) {	//i为变量(标示符0)或常量（数字常量）
        //先查一下符号表
        int i;
        if(currentToken.code==0)
        {
            for (i = 0; i < SYNBL.size(); i++)
            {
                if (currentToken.value == SYNBL[i].name.value)
                {
                    break;
                }
            }
            if (i == SYNBL.size())
            {
                token_pointer--;
                errorHappenedWithMessage("未定义的标识符");
                next();
            }
        }
        Token preToken=currentToken;
        next();
        if(currentToken.code == 19){       // [
            next();
            int tempArrIndex2=stoi(ConstNum[currentToken.value]);
            if (SYNBL[i].cat==5)
                if(AINFL[SYNBL[i].addr].up>=tempArrIndex2)
                {
                    sem.push_back(preToken);
                    sem.push_back(currentToken);
                    arrGetQuat();
                    next();
                    next();
                }else
                {
                    token_pointer--;
                    errorHappenedWithMessage("数组标号超过指定范围");
                    next();
                }
            else
            {   token_pointer--;
                token_pointer--;
                errorHappenedWithMessage("该标识符不是数组");
                next();
                next();
            }
        }
        else if(currentToken.code == 73)           //.
        {
            if(SYNBL[i].cat==7)
            {
                next();
                int tempStructIndex=SYNBL[i].addr;
                for(int j=0;j<RINFL.size();j++)
                {
                    if(RINFL[j].num==tempStructIndex)
                    {
                        if(RINFL[j].name.value==currentToken.value){
                            sem.push_back(preToken);
                            sem.push_back(currentToken);
                            struGetQuat();
                            next();
                            break;
                        }
                    }
                }
            }
        }
        else if(currentToken.code == 24)       // (
        {
            int i=getSynblIndex(preToken);
            int parmNum=SYNBL[i].type;
            next();
            while(parmNum--)
            {
                sem.push_back(SYNBL[++i].name);
                sem.push_back(currentToken);
                equa_QUAT(17);
                next();
                if(currentToken.code!=18)   //,
                    break;
                else
                    next();

            }
            if (currentToken.code == 25)        // )
                next();
            else
                cout<<"函数调用错误"<<endl;
            callQuat(preToken);
        }
        else if(currentToken.code == 78||currentToken.code == 79)//a=a++ --
        {
            sem.push_back(preToken);
            equa_QUAT(17);
            sem.push_back(preToken);
            sem.push_back(preToken);
            self_Quat1(currentToken.code);
            sem.pop_back();
            next();
        }
        else{//；
            sem.push_back(preToken);    //入操作数栈
        }
        return;
    }
    else if(currentToken.code==1||currentToken.code==2)// 字符常量或字符串常量
    {
        sem.push_back(currentToken);//++和--压进去的是符号！！
        next();
    }
    else if(currentToken.code==78||currentToken.code==79)//a++和a=++b
    {
        Token preToken=currentToken;
        next();
        if(currentToken.code==0){
            sem.push_back(currentToken);
            sem.push_back(currentToken);
            self_Quat1(preToken.code);
            next();
        }
    }
    else if(currentToken.code==80||currentToken.code==81||currentToken.code==82||currentToken.code==83)// += -= *= /=
    {
        next();
        sem.push_back(currentToken);
        next();
    }
    else  {
        if (currentToken.code == 24) {//(
            next();
            E();
            if (currentToken.code == 25) {//)
                next();
                return;
            }
            else {
                token_pointer-=2;//kk
                errorHappenedWithMessage("算术表达式缺少右括号)");
                next();
                next();
            }
        }
        else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("算术表达式缺少括号(");
            next();
            next();
        }
    }
}

void T() {
    F();
    while (currentToken.code == 13 || currentToken.code == 14) {
        int flag = 13;
        if (currentToken.code == 14) {
            flag = 14;
        }
        next();
        F();
        cal_QUAT(flag);
    }
}

/**
 算数表达式的递归下降子程序识别。做过无数次了。
 */
void E() {

    T();   
    while (currentToken.code == 11 || currentToken.code == 12) {
        int flag = 11;
        if (currentToken.code == 12)
            flag = 12;		//表示是后者
        next();
        T();
        cal_QUAT(flag);
    }
}
/*
 * 算数表达式扩展，增加> < >= <=
*/
void A() {
    E();
    while (currentToken.code == 67 || currentToken.code == 69|| currentToken.code == 68|| currentToken.code == 70) {//> < >= <=
        int flag = currentToken.code;//表示是后者
        next();
        E();
        cal_QUAT(flag);
    }
}

void type_list() {	//类型
    if (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9)
        next();
    else {
        errorHappenedWithMessage("不是数据类型");
    }
}

/*
 Old contents:

 识别标识符，并将新标识符压入符号表。
void symbol_list(int type) {		//标识符表
    if (currentToken.code == 0) {		//标识符
        synbl temp;
        temp.name = currentToken;
        temp.type = type;
        temp.cat = 2;
        temp.addr = VALL_pointer;
        VALL_pointer = VALL_pointer + TYPEL[type].lenth;
        SYNBL.push_back(temp);		//压入符号表
        next();
    } else {
        errorHappenedWithMessage("缺少标识符");
    }

    while (currentToken.code == 18) {		//,
        next();
        if (currentToken.code == 0) {		//标识符
            synbl temp;
            temp.name = currentToken;
            temp.type = type;
            temp.cat = 2;
            temp.addr = VALL_pointer;
            VALL_pointer = VALL_pointer + TYPEL[type].lenth;
            SYNBL.push_back(temp);
            next();
        }
        else {
            errorHappenedWithMessage("相邻的标识符之间，缺失了逗号(,)");
        }
    }
}
*/

/**
 初始化赋值表
 */
void symbolList_init(int type) { //preToken=currentToken,type=currentToken.code-7,next(),char 0,int 1,float 2
    if (currentToken.code == 0) {    //标识符
        synbl temp;
        temp.name = currentToken;
        temp.type = type;
        temp.cat = 2;
        temp.addr = VALL_pointer;
        VALL_pointer = VALL_pointer + TYPEL[type].lenth;
        SYNBL.push_back(temp);    //压入符号表
        Token previousToken = currentToken;
        next();

        if(currentToken.code == 19){      //[
                    next();
                    ainfl tempArr;
                    tempArr.low=0;
                    tempArr.up=stoi(ConstNum[currentToken.value])-1;
                    switch(type){
                    case 0:
                        tempArr.ctp=ConstChar.size();
                        for(int i=0;i<stoi(ConstNum[currentToken.value]);i++){
                            ConstChar.push_back("0");
                        }
                        break;
                    case 1:
                    case 2:
                        tempArr.ctp=ConstNum.size();
                        for(int i=0;i<stoi(ConstNum[currentToken.value]);i++)
                            ConstNum.push_back("0");
                        break;
                    default:cout<<"WRONG ARRY TYPE"<<endl;break;
                    }
                    tempArr.clen=TYPEL[type].lenth;
                    AINFL.push_back(tempArr);
                    temp.cat=5;
                    temp.addr=arrIndex;
                    SYNBL.pop_back();
                    SYNBL.push_back(temp);
                    arrIndex++;
                    next();
                    if (currentToken.code==20){         //]
                        next();
                    }
        }
        else if (currentToken.code == 18) {    //,
            next();
            symbolList_init(type);
        }
        else if (currentToken.code == 17) {    //=
            sem.push_back(previousToken);
            next();
            E();
            equa_QUAT(17);
            // next();
            if (currentToken.code == 18) {    //,
                next();
                symbolList_init(type);
            }
        }
    }
}

/**
 识别变量是否为结构体标识符
 */
int isStructSys()
{
    int i;
    for (i = 0; i < SYNBL.size(); i++)
    {
        if (currentToken.value == SYNBL[i].name.value)
        {
            break;
        }
    }
    if (i == SYNBL.size())      //未定义标识符
    {
        token_pointer--;
        errorHappenedWithMessage("未定义的标识符");
        next();
        return 0;
    }else{
        if(SYNBL[i].cat==6)     //结构体定义
            return 1;
        else
            return 0;
    }
}

/**
 识别变量是否为函数标识符变量
 */
int isFuncVar()
{
    int i;
    for (i = 0; i < SYNBL.size(); i++)
    {
        if (currentToken.value == SYNBL[i].name.value)
        {
            break;
        }
    }
    if (i != SYNBL.size())      //未定义标识符
    {
        if(SYNBL[i].cat==1)     //结构体定义
            return 1;
        else
            return 0;
    }
}


/**
 识别变量是否为结构体变量
 */
int isStructVar()
{
    int i;
    for (i = 0; i < SYNBL.size(); i++)
    {
        if (currentToken.value == SYNBL[i].name.value)
        {
            break;
        }
    }
    if (i != SYNBL.size())      //未定义标识符
    {
        if(SYNBL[i].cat==7||SYNBL[i].cat==8)     //结构体定义
            return 1;
        else
            return 0;
    }
}

/**
 识别变量声明文法
 */
void var_declaration() {
    while (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9 || currentToken.code == 21 || currentToken.code == 3 ||(currentToken.code ==0 && isStructSys() )) {	//char|int|float
        if(currentToken.code == 0){     //是否是已经声明的结构体
            int i;
            for (i = 0; i < SYNBL.size(); i++)
            {
                if (currentToken.value == SYNBL[i].name.value)
                {
                    break;
                }
            }
            if (i == SYNBL.size())      //未定义标识符
            {
                token_pointer--;
                errorHappenedWithMessage("未定义的标识符");
                next();
            }else{
                Token preToken=currentToken;
                next();
                if(SYNBL[i].cat==6)     //结构体定义
                {
                    synbl sysTemp;
                    sysTemp=SYNBL[i];
                    sysTemp.cat=7;
                    sysTemp.type=3;
                    sysTemp.name=currentToken;
                    sysTemp.addr=structIndex;
                    SYNBL.push_back(sysTemp);
                    int structIndexTemp=SYNBL[i].addr;
                    int clength=0;
                    for (int j = 0; j < RINFL.size(); j++)
                    {
                        if(RINFL[j].num==structIndexTemp)
                        {
                            rinfl structTemp=RINFL[j];
                            structTemp.num=structIndex;
                            switch (structTemp.type) {
                            case 0:
                                structTemp.tp=ConstChar.size();
                                ConstChar.push_back("0");
                                break;
                            case 1:
                            case 2:
                                structTemp.tp=ConstNum.size();
                                ConstNum.push_back("0");
                                break;
                            default:break;
                            }
                            RINFL.push_back(structTemp);
                            clength+=structTemp.off;
                        }
                    }
                    next();
                    if(currentToken.code==19){ //[  结构体数组
                        next();
                        ainfl tempArr;
                        tempArr.low=0;
                        tempArr.up=(stoi(ConstNum[currentToken.value])-1);
                        tempArr.ctp=structIndex;
                        tempArr.clen=clength;
                        AINFL.push_back(tempArr);
                        sysTemp.cat=8;
                        sysTemp.type=3;
                        sysTemp.addr=arrIndex;
                        SYNBL.pop_back();
                        SYNBL.push_back(sysTemp);
                        arrIndex++;
                        for(int i=0;i<tempArr.up;i++){
                            for (int j = 0; j < RINFL.size(); j++)
                            {
                                if(RINFL[j].num==structIndexTemp)
                                {
                                    rinfl structTemp=RINFL[j];
                                    structTemp.num=structIndex;
                                    switch (structTemp.type) {
                                    case 0:
                                        structTemp.tp=ConstChar.size();
                                        ConstChar.push_back("0");
                                        break;
                                    case 1:
                                    case 2:
                                    default:
                                        structTemp.tp=ConstNum.size();
                                        ConstNum.push_back("0");
                                        break;
                                    }
                                    RINFL.push_back(structTemp);
                                }
                            }
                        }
                        next();
                        if (currentToken.code==20){         //]
                            next();
                        }
                    }
                    structIndex++;
                    if (currentToken.code == 21) {	//;
                        next();
                    } else {
                        token_pointer-=2;
                        errorHappenedWithMessage("变量声明后缺少分号(;)");
                        next();
                        next();
                    }
                }
                else
                {
                    token_pointer--;
                    next();
                }
            }
        }else{
            if (currentToken.code == 21 || currentToken.code == 3) // ;号，数字
            {
                if (currentToken.code == 21)
                {
                    errorHappenedWithMessage("前多(;)");
                    next();
                    continue;
                }
                else{
                    errorHappenedWithMessage("前多数字");
                    next();
                    continue;
                }
            }
            int type;
            type = currentToken.code - 7;//类型表中是哪个
            next();
            int i;
            for (i = 0; i < SYNBL.size(); i++)
            {
                if (currentToken.value == SYNBL[i].name.value)
                {
                    break;
                }
            }
            if (i == SYNBL.size())      //未定义标识符
            {
                symbolList_init(type);    //识别接下来的标识符
            }else{
                cout<<"标识符已定义"<<endl;
                next();
            }

            if (currentToken.code == 21) {	//;
                next();
            } else {
                token_pointer-=2;
                errorHappenedWithMessage("变量声明后缺少分号(;)");
                next();
                next();
            }
        }
    }
}

/**
 结构体声明
 */
void var_declarationStruct() {
    while (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9 || currentToken.code == 21 || currentToken.code == 3) {	//char|int|float
        if (currentToken.code == 21 || currentToken.code == 3) // ;号，数字
        {
            if (currentToken.code == 21)
            {
                errorHappenedWithMessage("前多(;)");
                next();
                continue;
            }
            else{
                errorHappenedWithMessage("前多数字");
                next();
                continue;
            }
        }
        int tempType=currentToken.code-7;
        next();
        rinfl temp;
        temp.num=structIndex;
        temp.name=currentToken;
        temp.type=tempType;
        temp.off=structOFF+(tempType==1)?1:4;
        temp.tp=0;               //声明不开辟空间
        RINFL.push_back(temp);
        next();

        if (currentToken.code == 21) {	//;
            next();
        } else {
            token_pointer-=2;
            errorHappenedWithMessage("变量声明后缺少分号(;)");
            next();
            next();
        }
    }
    structOFF=0;
    structIndex++;
}

/**
 识别语句表
 */
void senten_list() {
    while (currentToken.code == 0 || currentToken.code == 5 || currentToken.code == 6 || currentToken.code == 21 || currentToken.code == 3||currentToken.code==34||currentToken.code==31||(currentToken.code>=78)&&(currentToken.code<=83)) {  //可以识别标识符赋值语句、while及if
        if(currentToken.code == 0 && isStructVar()){     //是否是已经声明的结构体
            int i;
            for (i = 0; i < SYNBL.size(); i++)
            {
                if (currentToken.value == SYNBL[i].name.value)
                {
                    break;
                }
            }
            if (i == SYNBL.size())      //未定义标识符
            {
                token_pointer--;
                errorHappenedWithMessage("未定义的标识符");
                next();
            }
            else{
                Token preToken=currentToken;
                if(SYNBL[i].cat==7)     //结构体定义
                {
                    next();
                    if (currentToken.code == 73){   //.
                        isStruct=true;
                        next();
                        int tempStructIndex = SYNBL[i].addr;
                        for(int j=0;j<RINFL.size();j++)
                        {
                            if(RINFL[j].num==tempStructIndex)
                            {
                                if(RINFL[j].name.value==currentToken.value){
                                    sem.push_back(preToken);
                                    sem.push_back(currentToken);
                                    sem.push_back(preToken);
                                    sem.push_back(currentToken);
                                    struGetQuat();
                                    next();
                                }
                            }
                        }
                    }
                }
                else if(SYNBL[i].cat==8)  //结构体数组定义
                {
                    next();
                    if(currentToken.code==19)//[
                    {
                        isArr=true;
                        next();
                        int tempArrIndex2=stoi(ConstNum[currentToken.value]);
                        if(AINFL[SYNBL[i].addr].up>tempArrIndex2)
                        {
                            sem.push_back(preToken);
                            sem.push_back(currentToken);
                            sem.push_back(preToken);
                            sem.push_back(currentToken);
                            arrGetQuat();
                        }
                        else{
                            errorHappenedWithMessage("数组使用错误");
                        }
                        next();
                        if(currentToken.code==20)//]
                        {
                            next();
                            if(currentToken.code==73)//.
                            {
                                isStruct=true;
                                next();
                                int tempStructIndex = AINFL[SYNBL[i].addr].ctp;
                                Token tempToken=sem.back();
                                for(int j=0;j<RINFL.size();j++)
                                {
                                    if(RINFL[j].num==tempStructIndex)
                                    {
                                        if(RINFL[j].name.value==currentToken.value){
                                            sem.push_back(tempToken);
                                            sem.push_back(currentToken);
                                            sem.push_back(tempToken);
                                            sem.push_back(currentToken);
                                            struGetQuat();
                                            next();
                                        }
                                    }
                                }

                            }
                        }
                    }
                }
                else{
                    sem.push_back(preToken);
                }
                if (currentToken.code == 17) //  =
                    next();
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("标识符赋值语句缺少等号");
                    next();
                    next();
                }
                E();    //然后，识别算术表达式
                if (currentToken.code == 21) {    //;    赋值语句最后必须有分号
                //   if(!funcstart)
                //   SYNBL[getSynblIndex(currentFunctionToken)].addr =inter_pro.size()-1;
                    equa_QUAT(17);
                    next();
                    if(currentToken.code==16 &&funcArea==true &&whileArea==false&&ifArea==false&&elseArea==false)   //}
                    {
                        retQuat();
                        funcArea=false;
                        currentFunctionToken=Token(-1,-1);
                    }
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("标识符赋值语句缺少分号");
                    next();
                    next();
                }
            }
        }
        else if(currentToken.code == 0 && isFuncVar())  //是否是已经声明的函数标识符
        {
            int i;
            for (i = 0; i < SYNBL.size(); i++)
            {
                if (currentToken.value == SYNBL[i].name.value)
                {
                    break;
                }
            }
            if (i == SYNBL.size())      //未定义标识符
            {
                token_pointer--;
                errorHappenedWithMessage("未定义的标识符");
                next();
            }else{
                Token preToken=currentToken;
                if(SYNBL[i].cat==1)     //函数
                {
                    next();
                    if (currentToken.code == 24){   // (
                        next();
                        if(currentToken.code == 25){   //)
                            callQuat(preToken);
                            next();
                        }
                    }
                }
                if (currentToken.code == 21) {    //;    赋值语句最后必须有分号
                    next();
                    if(currentToken.code==16 &&funcArea==true &&whileArea==false&&ifArea==false&&elseArea==false)   //}
                    {
                        retQuat();
                        currentFunctionToken=Token(-1,-1);
                    }
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("标识符赋值语句缺少分号");
                    next();
                    next();
                }
            }
        }
        else if(currentToken.code == 31){      //识别 return 语句
            next();
            sem.push_back(currentToken);
            retQuat();
            next();
            if(currentToken.code == 21)
                next();
            else
                cout<<"return 语句出错"<<endl;
            isReturn=true;
        }
        else
        {
            if (currentToken.code == 21 || currentToken.code == 3) // ;号，数字
            {
                if (currentToken.code == 21)
                {
                    errorHappenedWithMessage("前多(;)");
                    next();
                    continue;
                }
                else{
                    errorHappenedWithMessage("前多数字");
                    next();
                    continue;
                }
            }

            if (currentToken.code == 0) {	//标识符赋值语句
                //先查一下符号表
                int i;
                for (i = 0; i < SYNBL.size(); i++)
                {
                    if (currentToken.value == SYNBL[i].name.value)
                    {
                        break;
                    }
                }
                if (i == SYNBL.size())
                {
                    token_pointer--;
                    errorHappenedWithMessage("未定义的标识符");
                    next();
                }
                Token preToken=currentToken;
                int aaaaa=0;//识别a=a++
                next();
                if(currentToken.code == 19){       // [
                    isArr=true;
                    next();
                    tempArrIndex=stoi(ConstNum[currentToken.value]);
                    if (SYNBL[i].cat==5)
                        if(AINFL[SYNBL[i].addr].up>=tempArrIndex)
                        {
                            sem.push_back(preToken);
                            sem.push_back(currentToken);
                            sem.push_back(preToken);
                            sem.push_back(currentToken);
                            arrGetQuat();
                            next();
                            next();
                        }else
                        {
                            token_pointer--;
                            errorHappenedWithMessage("数组下标不在允许范围之内");
                            next();
                        }
                    else
                    {
                        token_pointer--;
                        token_pointer--;
                        errorHappenedWithMessage("该标识符不是数组");
                        next();
                        next();
                    }

                }else{
                    sem.push_back(preToken);    //入操作数栈
                }
                if (currentToken.code == 17)    //  =
                {
                    next();
                    if(currentToken.code==0){
                        next();
                        if(currentToken.code==78||currentToken.code==79)
                            aaaaa=121;
                        token_pointer-=2;
                        next();
                    }
                }
                else if((currentToken.code>=78)&&(currentToken.code<=83))// a++ -- += -= *= /=
                {
                    sem.push_back(preToken);
                    preToken=currentToken;
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("标识符赋值语句缺少等号");
                    next();
                    next();
                }
                E();    //然后，识别算术表达式
                if (currentToken.code == 21) {    //;    赋值语句最后必须有分号
//                    if(!funcstart)
//                        SYNBL[getSynblIndex(currentFunctionToken)].addr =inter_pro.size()-1;
                    if((preToken.code>=80)&&(preToken.code<=83)){
                        self_Quat2(preToken.code);
                    }
                    else if(preToken.code==78||preToken.code==79){
                        self_Quat1(preToken.code);
                        sem.pop_back();
                    }
                    else if(aaaaa==121){
                        aaaaa=0;
                    }
                    else equa_QUAT(17);
                    next();
                    if(currentToken.code==16 &&funcArea==true &&whileArea==false&&ifArea==false&&elseArea==false)   //}
                    {
                        retQuat();
                        funcArea==false;
                        currentFunctionToken=Token(-1,-1);
                    }
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("标识符赋值语句缺少分号");
                    next();
                    next();
                }
            } else if (currentToken.code == 5) {		//while
                int mark1,mark2;
                mark1 = inter_pro.size();    //记录while开始位置
                next();
                whileArea=true;
                if (currentToken.code == 24)	//(
                    next();
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("while语句后面没有括号(");
                    next();
                    next();
                }

                //识别一下算术表达式
                A();
                if (currentToken.code == 25) {	//)
                    quadruple temp;
                    temp.op = Token(5, -1);//while
                    temp.arg2 = Token(-1, -1);
                    temp.res = Token(-1, -1);
                    temp.arg1 = sem.back();//res(E)
                    sem.pop_back();

                    temp.label = 1;//跳出
                    temp.pointer = NULL;//未知
                    inter_pro.push_back(temp);//四元式生成插入
                    mark2 = inter_pro.size() - 1;//记录跳出的四元式位置
                    next();
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("while语句后面没有右括号)");
                    next();
                    next();
                }

                if (currentToken.code == 15)	// {
                    next();
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("while语句后面没有大括号{");
                    next();
                    next();
                }

                senten_list();
                if (currentToken.code == 16) {	//}
                    quadruple temp;
                    temp.op = Token(5, -1);//while
                    temp.arg2 = Token(-1, -1);
                    temp.res = Token(-1, -1);
                    temp.arg1 = Token(-1, -1);

                    temp.label = 1;//跳出
                    temp.pointer = mark1;//跳到while开始
                    inter_pro[mark1].label = 2;		//被跳入位置重新标记
                    inter_pro.push_back(temp);//四元式生成插入
                    inter_pro[mark2].pointer = inter_pro.size();//地址回填，指向while结尾的下一个四元式
                    in_flag = true;

                    next();
                    whileArea=false;
                }
                else {
                    errorHappenedWithMessage("while语句没有右大括号}结束");
                }
            } else if (currentToken.code == 6)
            {		//if
                ifArea=true;
                int mark;
                next();
                if (currentToken.code == 24)		//(
                    next();
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("if后面没有括号(");
                    next();
                    next();
                }

                //识别一下算术表达式
                A();
                if (currentToken.code == 25) {		//)
                    quadruple temp;
                    temp.op = Token(6, -1);//if
                    temp.arg2 = Token(-1, -1);
                    temp.res = Token(-1, -1);
                    temp.arg1 = sem.back();//res(E)
                    sem.pop_back();

                    temp.label = 1;//跳出
                    temp.pointer = NULL;//未知
                    inter_pro.push_back(temp);//四元式生成插入
                    mark = inter_pro.size() - 1;//记录跳出的四元式位置
                    next();
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("if后面没有右括号)");
                    next();
                    next();
                }
                if (currentToken.code == 15)	//{
                    next();
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("if后面没有大括号{");
                    next();
                    next();
                }
                //识别一下if后面的block
                senten_list();
                if (currentToken.code == 16) {	//}
                    next();
                    ifArea=false;
                }
                else {
                    errorHappenedWithMessage("if后面没有右大括号}");
                }

                //下面识别else，有可能没有
                if (currentToken.code == 30) {    //else
                    next();
                    elseArea=true;
                    if (currentToken.code == 15) {    //{
                        next();
                    }
                    else {
                        token_pointer -= 2;//kk
                        errorHappenedWithMessage("else后面没有大括号{");
                        next();
                        next();
                    }
                    //识别else接的block
                    senten_list();
                    if (currentToken.code == 16) {    //}
                        inter_pro[mark].pointer = inter_pro.size();//指向下一条
                        in_flag = true;
                        next();
                        elseArea=false;
                    } else {
                        errorHappenedWithMessage("else后面没有右大括号}");
                    }
                } else {    //没有else
                    inter_pro[mark].pointer = inter_pro.size();//指向下一条
                    in_flag = true;
                    continue;
                }
            }else if(currentToken.code == 34){ //识别cout
                next();
                if(currentToken.code==36)
                {
                    next();
                     E();
                    if(currentToken.code==21)//语句逗号
                        {
                           quadruple temp;
                           temp.op = Token(34, -1);//cout
                           temp.arg2 = Token(-1, -1);
                           temp.res = Token(-1, -1);
                           temp.arg1 = sem.back();//res(E)
                           sem.pop_back();
                           if (in_flag) {
                               temp.label = 2;
                               in_flag = 0;
                           }
                           else {
                               temp.label = 0;
                           }
                           temp.optimize_flag=1;
                           temp.pointer = NULL;//未知
                           inter_pro.push_back(temp);//四元式生成插入
                           next();
                           if(currentToken.code==16 &&funcArea==true &&whileArea==false&&ifArea==false&&elseArea==false)   //}
                           {
                               retQuat();
                               funcArea=false;
                               currentFunctionToken=Token(-1,-1);
                           }
                        }
                    else
                        {
                           token_pointer -= 2;//kk
                           errorHappenedWithMessage("cout后面没有逗号\n");
                           next();
                           next();
                        }
                }
                else{
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("cout后面没有<<\n");
                    next();
                    next();
                    }
            }
            else if(currentToken.code==78||currentToken.code==79)//识别++a --a
            {
                Token preToken=currentToken;
                next();
                sem.push_back(currentToken);
                sem.push_back(currentToken);
                next();
                if(currentToken.code==21){
                    self_Quat1(preToken.code);
                    next();
                    if(currentToken.code==16 &&funcArea==true &&whileArea==false&&ifArea==false&&elseArea==false)   //}
                    {
                        retQuat();
                        currentFunctionToken=Token(-1,-1);
                    }
                }
            }
        }
    }
}

void param_list() {		//参数表
    int paramNum=0;
    int type=0;
    if (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9)	{//int|float|char
        type=currentToken.code-7;
        next();
    }
    else {    //如果没有参数，就直接return完事
        return;
    }

    if (currentToken.code == 0) {    //标识符
        if(isSynblExist()==0)
        {
            synbl temp;
            temp.name = currentToken;
            temp.type = type;
            temp.cat = 2;
            temp.addr = VALL_pointer;
            VALL_pointer = VALL_pointer + TYPEL[type].lenth;
            SYNBL.push_back(temp);    //压入符号表
            paramNum++;
        }
        next();
    }
    else {
        token_pointer -= 2;//kk
        errorHappenedWithMessage("函数参数表中缺少标识符（参数名称）");
        next();
        next();
    }

    while (currentToken.code == 18) {	//,
        next();
        if (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9)	{//int|float|char
            type=currentToken.code-7;
            next();
        }
        if (currentToken.code == 0) {   //标识符
            if(isSynblExist()==0)
            {
                synbl temp;
                temp.name = currentToken;
                temp.type = type;
                temp.cat = 2;
                temp.addr = VALL_pointer;
                VALL_pointer = VALL_pointer + TYPEL[type].lenth;
                SYNBL.push_back(temp);    //压入符号表
                paramNum++;
            }
            next();
        }else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("函数参数表中缺少标识符（参数名称）");
            next();
            next();
        }
    }
    SYNBL[getSynblIndex(currentFunctionToken)].type=paramNum;
}

void compound_sen() {	//复合语句
        var_declaration();	//变量说明

        senten_list();		//语句表


}

/**
 结构体的识别
 */
void structure() {
    while (currentToken.code == 10) {	//struct
        next();
        if (currentToken.code == 0)	//标识符
        {
            tokenStash.push_back(currentToken);
            next();
        }else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("结构体struct标记后没有接标识符");
            next();
            next();
        }

        if (currentToken.code == 15)	//{
        {
            synbl temp;
            temp.name = tokenStash.back();
            tokenStash.pop_back();
            temp.type = 3;
            temp.cat = 6;
            temp.addr = structIndex;
            SYNBL.push_back(temp);    //压入符号表
            next();
        }else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("结构体没有大括号");
            next();
            next();
        }

        //识别变量声明，注意结构体里面只能声明，不可以赋值什么的！
        var_declarationStruct();

        if (currentToken.code == 16)	//}
            next();
        else {
            errorHappenedWithMessage("结构体定义后没有大右括号");
        }

        if (currentToken.code == 21)	//;
            next();
        else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("结构体定义后没有分号");
            next();
            next();
        }
    }
}

void programStartSymbol() {  
    while(currentToken.code==84){//宏定义 #
        next();
        if(currentToken.code==85){//define
            if(currentToken.code == 85 && isSynblExist()==0)
            {
                synbl temp;
                temp.name = currentToken;
                temp.type = 1;
                temp.cat = 9;
                temp.addr = inter_pro.size();
                SYNBL.push_back(temp);    //压入符号表
            }
            next();
            synbl temp;
            temp.name = currentToken;
            temp.type = 2;
            temp.cat = 8;
            sem.push_back(currentToken);
            next();
            temp.addr = ConstNum.size();
            SYNBL.push_back(temp);    //压入符号表
            ConstNum.push_back(ConstNum[currentToken.value]);
            sem.push_back(currentToken);
            equa_QUAT(85);
            next();
        }
    }
    //下面先识别结构体
    structure();
    //识别函数，这里可以识别多个函数
        while (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9 || currentToken.code == 29) {	//int|float|char|void，他们都是函数的返回值
            Token returnToken=currentToken;
            next();
            if (currentToken.code == 0 || currentToken.code == 4)	//标识符|main
            {
                if(currentToken.code == 0 && isSynblExist()==0)
                {
                    synbl temp;
                    temp.name = currentToken;
                    temp.type = 0;
                    temp.cat = 1;
                    temp.addr = inter_pro.size();
                    SYNBL.push_back(temp);    //压入符号表
                    funcArea=true;
                    currentFunctionToken=currentToken;
                }else if(currentToken.code==4)
                    mainStartId=inter_pro.size();
                next();
            }
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("函数返回值类型后面缺少标识符");
                next();
                next();
            }
            if (currentToken.code == 24)		//(
            {
                next();
            }
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("函数参数后面缺少左括号(");
                next();
                next();
            }

            param_list();			//参数表
            if (currentToken.code == 25)		//)
                next();
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("函数参数后面缺少右括号)");
                next();
                next();
            }
            if (currentToken.code == 15)		//{
                next();
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("函数参数后面缺少大括号({)");
                next();
                next();
            }

            compound_sen();			//复合语句

            if (returnToken.code!=29 &&isReturn==false){
                if (currentToken.code == 31) {    //return

                    next();
                    if (currentToken.code == 21) {    //;
                        //直接一个return语句，没有返回值
                        next();
                    }
                    else if (currentToken.code == 0 || currentToken.code == 1 || currentToken.code == 2 || currentToken.code == 3) {
                        next();
                        if (currentToken.code == 21) {    //;
                            next();
                        }
                        else {
                            token_pointer -= 2;//kk
                            errorHappenedWithMessage("函数返回语句缺少分号");
                            next();
                            next();
                        }
                    }
                    else {
                        errorHappenedWithMessage("函数返回return语句非法");
                    }
                }
                else {
                    token_pointer -= 2;//kk
                    errorHappenedWithMessage("函数末尾没有return语句");
                    next();
                    next();
                }
            }
            if (currentToken.code == 16) {		//}
                if (!next()) {
                quadruple temp;
                temp.arg1 = Token(-1, -1);
                temp.arg2 = Token(-1, -1);
                temp.label = 0;
                temp.op = Token(-1, -1);;
                temp.pointer = NULL;
                temp.res = Token(-1, -1);
                inter_pro.push_back(temp);
                    return;
                }
            }
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("函数块(dispatch_block_t)之后缺少右大括号(})");
                next();
                next();
            }


    }
}

/**
 @return If the token chain is grammartically right, `true`. Else, `false`.
 */
bool syntax_analysis() {
    token_pointer = 0;
    VALL_pointer = 0;
    isGrammarCorrect = true;
    next();

    programStartSymbol();

    if (!isGrammarCorrect) {    //如果文法有误，就打印出错误
        sharedErrorContainer.printErrors();
        sharedErrorContainer.clearErrors();
    }
    return isGrammarCorrect;
}
