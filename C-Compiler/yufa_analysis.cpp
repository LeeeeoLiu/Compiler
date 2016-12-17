#include "yufa_analysis.h"


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
vector<typel> TYPEL = { { "char", 2 }, { "inta", 2 }, { "float", 2 } };
/**
 符号表
 */
vector<synbl> SYNBL;
/**
 活动记录当前指向
 */
int VALL_pointer ;

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


Token currentToken;
int token_pointer;


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

    temp.pointer = NULL;
    temp.res = sem.back();//赋值给=左边的符号
    sem.pop_back();
    inter_pro.push_back(temp);//四元式生成插入
}

void F() {
    if (currentToken.code == 3 || currentToken.code == 0) {	//i为变量(标示符0)或常量（数字常量）
        sem.push_back(currentToken);
        next();
        return;
    }
    else  {
        if (currentToken.code == 24) {
            next();
            E();
            if (currentToken.code == 25) {
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
void symbolList_init(int type) {
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

        if (currentToken.code == 18) {    //,
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
 识别变量声明文法
 */
void var_declaration() {
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
        int type;
        type = currentToken.code - 7;//类型表中是哪个
        next();
        symbolList_init(type);    //识别接下来的标识符

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
/**
 识别语句表
 */
void senten_list() {

    while (currentToken.code == 0 || currentToken.code == 5 || currentToken.code == 6 || currentToken.code == 21 || currentToken.code == 3) {    //可以识别标识符赋值语句、while及if

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
 //               errorHappenedWithMessage("未定义的标识符");
                next();
            }

            sem.push_back(currentToken);    //入操作数栈
            next();
            if (currentToken.code == 17)    //等号
                next();
            else {
                token_pointer -= 2;//kk
//                errorHappenedWithMessage("标识符赋值语句缺少等号");
                next();
                next();
            }
            E();    //然后，识别算术表达式
            if (currentToken.code == 21) {    //赋值语句最后必须有分号
                equa_QUAT(17);
                next();
            }
            else {
                token_pointer -= 2;//kk
 //               errorHappenedWithMessage("标识符赋值语句缺少分号");
                next();
                next();
            }
        } else if (currentToken.code == 5) {		//while
            int mark1,mark2;
            mark1 = inter_pro.size();    //记录while开始位置
            next();
            if (currentToken.code == 24)	//(
                next();
            else {
                token_pointer -= 2;//kk
                errorHappenedWithMessage("while语句后面没有括号(");
                next();
                next();
            }

            //识别一下算术表达式
            E();
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
            }
            else {
                errorHappenedWithMessage("while语句没有右大括号}结束");
            }
        } else if (currentToken.code == 6) {		//if
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
            E();
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
            }
            else {
                errorHappenedWithMessage("if后面没有右大括号}");
            }

            //下面识别else，有可能没有
            if (currentToken.code == 30) {    //else
                next();
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
                } else {
                    errorHappenedWithMessage("else后面没有右大括号}");
                }
            } else {    //没有else
                inter_pro[mark].pointer = inter_pro.size();//指向下一条
                in_flag = true;
                continue;
            }
        }
    }
}

void param_list() {		//参数表
    if (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9)	//int|float|char
        next();
    else {    //如果没有参数，就直接return完事
        return;
    }

    if (currentToken.code == 0) {    //标识符
        next();
    }
    else {
        token_pointer -= 2;//kk
        //errorHappenedWithMessage("函数参数表中缺少标识符（参数名称）");
        next();
        next();
    }

    while (currentToken.code == 18) {	//,
        next();
        type_list();
        if (currentToken.code == 0)    //标识符
            next();
        else {
            token_pointer -= 2;//kk
            //errorHappenedWithMessage("函数参数表中缺少标识符（参数名称）");
            next();
            next();
        }
    }
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
            next();
        else {
            token_pointer -= 2;//kk
            //errorHappenedWithMessage("结构体struct标记后没有接标识符");
            next();
            next();
        }

        if (currentToken.code == 15)	//{
            next();
        else {
            token_pointer -= 2;//kk
            errorHappenedWithMessage("结构体没有大括号");
            next();
            next();
        }

        //识别变量声明，注意结构体里面只能声明，不可以赋值什么的！
        var_declaration();

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
    //下面先识别结构体

    structure();

    //识别函数，这里可以识别多个函数
        while (currentToken.code == 7 || currentToken.code == 8 || currentToken.code == 9 || currentToken.code == 29) {	//int|float|char|void，他们都是函数的返回值
            next();
            if (currentToken.code == 0 || currentToken.code == 4)	//标识符|main
                next();
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
    }
    return isGrammarCorrect;
}
