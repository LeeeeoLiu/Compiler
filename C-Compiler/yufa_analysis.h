#ifndef YUFA_ANALYSIS_H
#define YUFA_ANALYSIS_H


#include "global.h"

extern vector<Token> token;
extern vector<string> keywords;	//保留字表
extern vector<string> Id;			//符号表code=0
extern vector<string> ConstNum;	//常数表code=3
extern vector<string> ConstString;	//字符串常量表code=2
extern vector<string> ConstChar;		//字符常量表code=1

/**
 Syntax error indicator.
 */
class MJSyntaxError {
public:
    /**
     The position in the token array behind which the error happens.
     */
    int indexInTokenArrayAfter;
    string message;
    /**
     @param errorMessage the error message.
     @param afterIndex the position of the token behind which an error happens.
     */
    MJSyntaxError(string errorMessage, int afterIndex) {
        message = errorMessage;
        indexInTokenArrayAfter = afterIndex;
    }

    bool operator== (const MJSyntaxError& anotherError) {
        if ((message == anotherError.message) && (indexInTokenArrayAfter == anotherError.indexInTokenArrayAfter)) {
            return true;
        } else {
            return false;
        }
    }
};

class MJErrorContainer {
private:
    vector<MJSyntaxError> errors;
public:
    MJErrorContainer() {
        errors = { };
    }

    /**
     Adds an error to the error container.
     If the container has too many errors at the same position, then the token chain afterwards is not analysable any longer. The whole analysis process needs to be aborted. If this happens, only the FIRST error at that specific position counts, and will be returned.
     @return If there are too many errors at the same position, *false*, meaning the whole analysis needs to get aborted. Else, everything's normal about that error, *true*.
     */
    bool addError(MJSyntaxError newError) {
        bool duplicate = false;
        for (int number = 0; number < errors.size(); number++) {
            if (errors[number].indexInTokenArrayAfter == newError.indexInTokenArrayAfter) {
                if (!duplicate) {    //如果先前的元素都没有重复，就置重复
                    duplicate = true;
                } else {    //如果第二次重复，就出错了，不能向下分析
                    return false;    //返回出错
                }
            }
        }
        errors.push_back(newError);
        return true;
    }

    /**
     Print all the errors on the screen. If multiple errors happens at the same position of the token chain, only the first error will be printed.
     */
    void printErrors() {
        for (int number = 0; number < errors.size(); number++) {
            cout << errors[number].indexInTokenArrayAfter << ".";
            switch (token[errors[number].indexInTokenArrayAfter].code)
            {    //先输出错误位置的token内容
            case 0:cout << Id[token[errors[number].indexInTokenArrayAfter].value] << " "; break;
            case 1:cout << ConstChar[token[errors[number].indexInTokenArrayAfter].value] << " "; break;
            case 2:cout << ConstString[token[errors[number].indexInTokenArrayAfter].value] << " "; break;
            case 3:cout << ConstNum[token[errors[number].indexInTokenArrayAfter].value] << " "; break;
            default:cout << keywords[token[errors[number].indexInTokenArrayAfter].code] << " ";
            }
            cout<< errors[number].message << endl;    //输出错误信息
        }
    }
};






//函数声明部分
void E();
void errorHappenedWithMessage(string message);
bool next();
void cal_QUAT(int op);
void equa_QUAT(int op);
void F();
void T();
void E();
void type_list();
void symbolList_init(int type);
void var_declaration();
void senten_list();
void param_list();
void compound_sen();
void structure();
void programStartSymbol();
bool syntax_analysis();


#endif // YUFA_ANALYSIS_H
