#include "compile.h"


vector<string> check_list;//给出原来四元式的位置即可找到应该跳转的标记

targe data;//汇编语言序列
targe code;
targe* data_pointer;
targe* data_end;//链表最后一个元素的指针

stack<targe*> backfilling;
int jump_label = 0;
string the_first_data_label;

void data_new_node(targe* temp){
    data_end->next = temp;
    data_end = temp;
}

string itos(int num){
    ostringstream ss;
    ss << num;
    string result(ss.str());
    return result;
}

void DSEG(){
    int int_num, float_num, char_num;
    int_num = 0;
    float_num = 0;
    char_num = 0;
    for (int i = 0; i < SYNBL.size(); i++){
        switch (SYNBL[i].type){
        case 0:{char_num++; break; }
        case 1:{int_num++; break; }
        case 2:{float_num++; break; }
        }
    }
    targe* last_data;
    last_data = &data;
    for (int i = 0;i < TYPEL.size(); i++){
        data_pointer = new targe;
        data_pointer->cw = TYPEL[i].name;
        switch (TYPEL[i].lenth){
        case 1:data_pointer->arg1 = "DB"; break;
        case 2:data_pointer->arg1 = "DW"; break;
        }
        switch (i){
        case 0:data_pointer->arg2 = itos(char_num) + " DUP(0)"; break;
        case 1:data_pointer->arg2 = itos(int_num) + " DUP(0)"; break;
        case 2:data_pointer->arg2 = itos(float_num) + " DUP(0)"; break;
        }
        if (i == 0){
            the_first_data_label = data_pointer->cw;
        }
        data_pointer->flag = 2;
        data_pointer->next = NULL;
        last_data->next = data_pointer;
        last_data = data_pointer;
    }
    data_pointer = new targe;
    data_pointer->cw = "TEMP";
    data_pointer->arg1 = "DW";
    data_pointer->arg2 = itos(temp_num) + " DUP(0)";
    data_pointer->flag = 2;
    data_pointer->next = NULL;
    last_data->next = data_pointer;
    last_data = data_pointer;

    data_pointer = new targe;
    data_pointer->cw = "CONST";
    data_pointer->arg1 = "DW";
    data_pointer->arg2 = ConstNum[0];
    for (int i = 1; i < ConstNum.size(); i++){
        data_pointer->arg2 = data_pointer->arg2 + "," + ConstNum[i];
    }
    data_pointer->flag = 2;
    data_pointer->next = NULL;
    last_data->next = data_pointer;
    last_data = data_pointer;

}

void CSEG(){
    targe* code_pointer;
    targe* code_last;
    int inter_pro_pointer;//中间代码
    inter_pro_pointer = 0;
    code_last = &code;
    for (;inter_pro_pointer < inter_pro.size(); inter_pro_pointer++){

        if (inter_pro[inter_pro_pointer].label != -1){
            if (inter_pro[inter_pro_pointer].op.code == 11 ||		//是算术运算
                inter_pro[inter_pro_pointer].op.code == 12 ||
                inter_pro[inter_pro_pointer].op.code == 13 ||
                inter_pro[inter_pro_pointer].op.code == 14){


                //读入第一个操作数
                code_pointer = new targe;

                if (inter_pro[inter_pro_pointer].label == 2){
                    code_pointer->label = check_list[inter_pro_pointer];
                }

                switch (inter_pro[inter_pro_pointer].arg1.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov ax,int|char|float[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].arg1.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg1.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov ax,const[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = "CONST[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                           break;
                }
                case -2:{
                            code_pointer->cw = "MOV";		//mov ax,temp[i]
                            code_pointer->arg1 = "AX";
                            code_pointer->arg2 = "TEMP[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;


                //读入第二个操作数
                code_pointer = new targe;
                switch (inter_pro[inter_pro_pointer].arg2.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov Bx,int|char|float[i]
                           code_pointer->arg1 = "BX";
                           //TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg2.value].type].name
                           code_pointer->arg2 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].arg2.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg2.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov bx,const[i]
                           code_pointer->arg1 = "BX";
                           code_pointer->arg2 = "CONST[" + itos(inter_pro[inter_pro_pointer].arg2.value * 2) + "]";
                           break;
                }
                case -2:{
                            code_pointer->cw = "MOV";		//mov bx,temp[i]
                            code_pointer->arg1 = "BX";
                            code_pointer->arg2 = "TEMP[" + itos(inter_pro[inter_pro_pointer].arg2.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

                //进行计算
                code_pointer = new targe;
                switch (inter_pro[inter_pro_pointer].op.code){
                case 11:{
                            code_pointer->cw = "ADD";
                            code_pointer->arg1 = "AX";
                            code_pointer->arg2 = "BX";
                            code_pointer->flag = 2;
                            code_pointer->next = NULL;
                            code_last->next = code_pointer;
                            code_last = code_pointer;
                            break;
                }
                case 12:{
                            code_pointer->cw = "SUB";
                            code_pointer->arg1 = "AX";
                            code_pointer->arg2 = "BX";
                            code_pointer->flag = 2;
                            code_pointer->next = NULL;
                            code_last->next = code_pointer;
                            code_last = code_pointer;
                            break;
                }
                case 13:{
                            code_pointer->cw = "MUL";
                            code_pointer->arg1 = "BX";
                            code_pointer->arg2 = "";
                            code_pointer->flag = 1;
                            code_pointer->next = NULL;
                            code_last->next = code_pointer;
                            code_last = code_pointer;
                            break;
                }
                case 14:{
                            code_pointer->cw = "XOR";
                            code_pointer->arg1 = "DX";
                            code_pointer->arg2 = "DX";
                            code_pointer->flag = 2;
                            code_pointer->next = NULL;
                            code_last->next = code_pointer;
                            code_last = code_pointer;

                            code_pointer = new targe;
                            code_pointer->cw = "DIV";
                            code_pointer->arg1 = "BX";
                            code_pointer->arg2 = "";
                            code_pointer->flag = 1;
                            code_pointer->next = NULL;
                            code_last->next = code_pointer;
                            code_last = code_pointer;
                            break;
                }
                }

                //结果存到res
                code_pointer = new targe;
                switch (inter_pro[inter_pro_pointer].res.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov int|char|float[i],ax
                           code_pointer->arg2 = "AX";
                           code_pointer->arg1 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].res.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].res.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov const[i],ax
                           code_pointer->arg2 = "AX";
                           code_pointer->arg1 = "CONST[" + itos(inter_pro[inter_pro_pointer].res.value * 2) + "]";
                           break;
                }
                case -2:{	//反正也不应该有
                            code_pointer->cw = "MOV";		//mov temp[i],ax
                            code_pointer->arg2 = "AX";
                            code_pointer->arg1 = "TEMP[" + itos(inter_pro[inter_pro_pointer].res.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;
            }




            //赋值表达式
            if (inter_pro[inter_pro_pointer].op.code == 17){
                //读出到AX
                code_pointer = new targe;

                if (inter_pro[inter_pro_pointer].label == 2){
                    code_pointer->label = check_list[inter_pro_pointer];
                }

                switch (inter_pro[inter_pro_pointer].arg1.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov ax,int|char|float[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].arg1.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg1.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov ax,const[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = "CONST[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                           break;
                }
                case -2:{
                            code_pointer->cw = "MOV";		//mov ax,temp[i]
                            code_pointer->arg1 = "AX";
                            code_pointer->arg2 = "TEMP[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

                //结果存到res
                code_pointer = new targe;
                switch (inter_pro[inter_pro_pointer].res.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov int|char|float[i],ax
                           code_pointer->arg2 = "AX";
                           code_pointer->arg1 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].res.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].res.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov const[i],ax
                           code_pointer->arg2 = "AX";
                           code_pointer->arg1 = "CONST[" + itos(inter_pro[inter_pro_pointer].res.value * 2) + "]";
                           break;
                }
                case -2:{	//反正也不应该有
                            code_pointer->cw = "MOV";		//mov temp[i],ax
                            code_pointer->arg2 = "AX";
                            code_pointer->arg1 = "TEMP[" + itos(inter_pro[inter_pro_pointer].res.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

            }





            //四元式是if
            if (inter_pro[inter_pro_pointer].op.code == 6){
                //读出到AX
                code_pointer = new targe;
                switch (inter_pro[inter_pro_pointer].arg1.code){
                case 0:{
                           code_pointer->cw = "MOV";		//mov ax,int|char|float[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = the_first_data_label + "[" +
                               itos(inter_pro[inter_pro_pointer].arg1.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg1.value].type].lenth) + "]";
                           break;
                }
                case 3:{
                           code_pointer->cw = "MOV";		//mov ax,const[i]
                           code_pointer->arg1 = "AX";
                           code_pointer->arg2 = "CONST[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                           break;
                }
                case -2:{
                            code_pointer->cw = "MOV";		//mov ax,temp[i]
                            code_pointer->arg1 = "AX";
                            code_pointer->arg2 = "TEMP[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                            break;
                }
                }
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

                code_pointer = new targe;
                code_pointer->cw = "AND";
                code_pointer->arg1 = "AX";
                code_pointer->arg2 = "AX";
                code_pointer->flag = 2;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

                code_pointer = new targe;
                code_pointer->cw = "JZ";
                code_pointer->arg1 = check_list[inter_pro[inter_pro_pointer].pointer];
                code_pointer->arg2 = "";
                code_pointer->flag = 1;
                code_pointer->next = NULL;
                code_last->next = code_pointer;
                code_last = code_pointer;

            }


            //四元式是while
            if (inter_pro[inter_pro_pointer].op.code == 5){
                if (inter_pro[inter_pro_pointer].arg1.code != -1){//条件跳出部分
                    code_pointer = new targe;
                    switch (inter_pro[inter_pro_pointer].arg1.code){
                    case 0:{
                               code_pointer->cw = "MOV";		//mov ax,int|char|float[i]
                               code_pointer->arg1 = "AX";
                               code_pointer->arg2 = the_first_data_label + "[" +
                                   itos(inter_pro[inter_pro_pointer].arg1.value * TYPEL[SYNBL[inter_pro[inter_pro_pointer].arg1.value].type].lenth) + "]";
                               break;
                    }
                    case 3:{
                               code_pointer->cw = "MOV";		//mov ax,const[i]
                               code_pointer->arg1 = "AX";
                               code_pointer->arg2 = "CONST[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                               break;
                    }
                    case -2:{
                                code_pointer->cw = "MOV";		//mov ax,temp[i]
                                code_pointer->arg1 = "AX";
                                code_pointer->arg2 = "TEMP[" + itos(inter_pro[inter_pro_pointer].arg1.value * 2) + "]";
                                break;
                    }
                    }
                    code_pointer->flag = 2;
                    code_pointer->next = NULL;
                    code_last->next = code_pointer;
                    code_last = code_pointer;

                    code_pointer = new targe;
                    code_pointer->cw = "AND";
                    code_pointer->arg1 = "AX";
                    code_pointer->arg2 = "AX";
                    code_pointer->flag = 2;
                    code_pointer->next = NULL;
                    code_last->next = code_pointer;
                    code_last = code_pointer;

                    code_pointer = new targe;
                    code_pointer->cw = "JZ";
                    code_pointer->arg1 = check_list[inter_pro[inter_pro_pointer].pointer];
                    code_pointer->arg2 = "";
                    code_pointer->flag = 1;
                    code_pointer->next = NULL;
                    code_last->next = code_pointer;
                    code_last = code_pointer;
                }
                else {
                    code_pointer = new targe;
                    code_pointer->cw = "JMP";
                    code_pointer->arg1 = check_list[inter_pro[inter_pro_pointer].pointer];
                    code_pointer->arg2 = "";
                    code_pointer->flag = 1;
                    code_pointer->next = NULL;
                    code_last->next = code_pointer;
                    code_last = code_pointer;
                }
            }
        }
    }
}


void compilization(){
    check_list.resize(inter_pro.size());
    for (int i = 0; i < inter_pro.size(); i++){
        if (inter_pro[i].label == 2){
            check_list[i] = "t" + itos(jump_label);
            jump_label++;
        }
    }
    DSEG();
    CSEG();

    ofstream ofile("D:\\QT"
                   "/Code/Compiler/C-Compiler/test.asm",ios::out);

    targe * front,* back;
    ofile << "DSEG\tSEGMENT" << endl;

    back = &data;
    while (back->next != NULL){
        back = back->next;
        ofile << "\t" + back->cw + "\t" + back->arg1 + "\t" + back->arg2 << endl;

    }

    ofile << "DSEG\tENDS" << endl;
    ofile << "CSEG\tSEGMENT\n\tASSUME\tCS:CSEG,DS:DSEG" << endl;
    ofile << "start:\tMOV\tAX,DSEG\n\tMOV\tDS,AX" << endl;

    front = &code;
    while (front->next != NULL){
        front = front->next;
        if (front->label.size() != 0){
            ofile << front->label << ":";
        }
        switch (front->flag){
        case 0:ofile << "\t" + front->cw << endl; break;
        case 1:ofile << "\t" + front->cw + "\t" + front->arg1 << endl; break;
        case 2:ofile << "\t" + front->cw + "\t" + front->arg1 << "," << front->arg2 << endl; break;
        }
    }
    ofile << "CSEG\tENDS\n\tEND\tSTART" << endl;


 }
