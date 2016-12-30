#include "optimize.h"

vector<vex_node> DAG;	//DAG图
vector<quadruple> new_inter;	//新生成的中间代码
vector<flow_graph> flowGraph;	//记录基本块划分状况

string ftos(float num){
    ostringstream ss;
    ss << num;
    string result(ss.str());
    return result;
}

void basic_block(){			//基本块划分，结果储存在flow_graph
    int inter_pointer = 0;
    flow_graph node;
    do{

        node.front = inter_pointer;
        for (; inter_pointer < inter_pro.size(); inter_pointer++){
            if (inter_pro[inter_pointer].label == 1){	//跳走
                //	node.end = inter_pointer;
                //	flowGraph.push_back(node);
                //	node.front = inter_pointer;
                break;
            }

            if (inter_pro[inter_pointer].label == 2){	//是被跳入的四元式
                if (node.front != inter_pointer)	//是否刚好也是入口
                    node.end = inter_pointer - 1;
                else
                    node.end = inter_pointer;
                flowGraph.push_back(node);
                node.front = inter_pointer;
            }
        }


        if (inter_pointer < inter_pro.size()){
            node.end = inter_pointer;
            flowGraph.push_back(node);
            inter_pointer++;
        }
        else //只有一块
       {
            node.end = inter_pointer-1;
            flowGraph.push_back(node);

        }
    } while (inter_pointer < inter_pro.size());

    if (flowGraph.size() != 0)
    {
    if (flowGraph[flowGraph.size() - 1].end != inter_pro.size() - 1){

        node.end = inter_pro.size() - 1;
        flowGraph.push_back(node);
    }
    }

}

void DAG_optimaize(int front, int end){
    int pointer;
    pointer = front;
    while (pointer <= end){
        if (inter_pro[pointer].op.code == 17 || inter_pro[pointer].op.code == 11 || inter_pro[pointer].op.code == 12 || inter_pro[pointer].op.code == 13 || inter_pro[pointer].op.code == 14
            ||inter_pro[pointer].op.code == 72 || inter_pro[pointer].op.code == 71||inter_pro[pointer].op.code == 67||inter_pro[pointer].op.code==69){	//除赋值表达式,[,],>,<,+ - * /外不参加优化
            //(1)
            if ((inter_pro[pointer].arg1.code == 0 || inter_pro[pointer].arg1.code == -2) && inter_pro[pointer].arg2.code == -1)	//(=,B,_,A)
            {
                int i, j, pointer_b = -1, pointer_a = -1, pointer_al = -1;	//用作标记是否找到,后面两个分别指向A的结点和label位置
                for (i = 0; i < DAG.size(); i++){	//从头往后找，把最新产生的找出来,找A B,规模不会太大所以全部走一遍
                    for (j = 0; j < DAG[i].labels.size(); j++){	//扫描标记
                        if (inter_pro[pointer].arg1 == DAG[i].labels[j]){
                            pointer_b = i;
                        }
                        if (inter_pro[pointer].res == DAG[i].labels[j]){
                            pointer_a = i;
                            pointer_al = j;
                        }
                    }
                }
                if (pointer_b == -1){		//还没有标记B
                    vex_node temp;
                    temp.op.code = -1;	//把结点的运算符加上
                    temp.labels.push_back(inter_pro[pointer].arg1);	//把B作为第一个标记
                    temp.arc = { -1, -1 };
                    DAG.push_back(temp);
                    pointer_b = DAG.size();
                    DAG[DAG.size() - 1].labels.push_back(inter_pro[pointer].res);
                }
                else{		//找到标记B

                    if (inter_pro[pointer].res.code == 0 && DAG[pointer_b].labels[0].code == -2){
                        DAG[pointer_b].labels.insert(DAG[pointer_b].labels.begin(), inter_pro[pointer].res);//A是用户定义，B是临时变量，A作为主标记
                    }
                    else{
                        DAG[pointer_b].labels.push_back(inter_pro[pointer].res);	//把A插到B所在labels的末尾
                    }
                }
                if (pointer_a != -1 && pointer_al != 0){	//A标记已经存在且不是label中的第一个main标记
                    vector<Token>::iterator it = DAG[pointer_a].labels.begin() + pointer_al;
                    DAG[pointer_a].labels.erase(it);		//删除A标记
                    if (DAG[pointer_a].labels.size() == 0){		//如果DAG结点的标记为空，则删除结点
                        vector<vex_node>::iterator it = DAG.begin() + pointer_a;
                        DAG.erase(it);
                    }
                }
            }



            //(2)
            else if (inter_pro[pointer].arg1.code == 3 && (inter_pro[pointer].arg2.code == 3 || inter_pro[pointer].arg2.code == -1)){//(w,C1,C2,A) 或 (=,C1,_,A)
                //指向活动记录中临时变量
                Token temp_token;
                string str;
                float constnum;
                if (inter_pro[pointer].arg2.code == -1)
                    str = ConstNum[inter_pro[pointer].arg1.value];	//只有C1，在下面的循环中会在constnum中找到对应的value值
                else{
                    if(inter_pro[pointer].op.code == 11)
                    constnum = atof(ConstNum[inter_pro[pointer].arg1.value].c_str())+atof(ConstNum[inter_pro[pointer].arg2.value].c_str());
                    if(inter_pro[pointer].op.code == 12)
                    constnum = atof(ConstNum[inter_pro[pointer].arg1.value].c_str())-atof(ConstNum[inter_pro[pointer].arg2.value].c_str());
                    if(inter_pro[pointer].op.code == 13)
                    constnum = atof(ConstNum[inter_pro[pointer].arg1.value].c_str())*atof(ConstNum[inter_pro[pointer].arg2.value].c_str());
                    if(inter_pro[pointer].op.code == 14)
                    constnum = atof(ConstNum[inter_pro[pointer].arg1.value].c_str())/atof(ConstNum[inter_pro[pointer].arg2.value].c_str());

                    char temp[10];
                    str = ftos(constnum);

                }
                int k;
                temp_token.code = 3;
                for (k = 0; k < ConstNum.size(); k++){
                    if (!ConstNum[k].compare(str)){
                        temp_token.value = k;
                        break;
                    }
                }
                if (k == ConstNum.size())	//上面的循环没找到
                    temp_token.value = InsertConstNum(str);	//在这里应该在常数表里插入一个由C1C2相乘得到的值,但这里只是在常量表里把两个相乘的数的string连接起来，再插到常数表尾部

                int i, j, pointer_const = -1, pointer_a = -1, pointer_al = -1;	//用作标记是否找到,后面两个分别指向A的结点和label位置
                for (i = 0; i < DAG.size(); i++){	//从前往后找,把最新生成的找出来,找A B,规模不会太大所以全部走一遍
                    for (j = 0; j < DAG[i].labels.size(); j++){	//扫描标记
                        if (temp_token == DAG[i].labels[j]){	//找到C1wC2 | C1
                            pointer_const = i;
                        }
                        if (inter_pro[pointer].res == DAG[i].labels[j]){	//找到A
                            pointer_a = i;
                            pointer_al = j;
                        }
                    }
                }

                if (pointer_const == -1){	//图里没有这个常数标记
                    vex_node temp;
                    temp.op.code = -1;	//常数没有运算符
                    temp.labels.push_back(temp_token);	//把常数作为第一个标记
                    temp.labels.push_back(inter_pro[pointer].res);//A作为第二个标记
                    temp.arc = { -1, -1 };		//边不指向任何结点
                    DAG.push_back(temp);
                }
                else{	//图中有这个常数标记,则附加上标记
                    DAG[pointer_const].labels.push_back(inter_pro[pointer].res);
                }
                if (pointer_a != -1 && pointer_al != 0){	//A标记已经存在且不是label中的第一个main标记
                    vector<Token>::iterator it = DAG[pointer_a].labels.begin() + pointer_al;
                    DAG[pointer_a].labels.erase(it);		//删除A标记
                    if (DAG[pointer_a].labels.size() == 0){		//如果DAG结点的标记为空，则删除结点
                        vector<vex_node>::iterator it = DAG.begin() + pointer_a;
                        DAG.erase(it);
                    }
                }
            }



            else{		//(w,B,C,A)
                int i, j, pointer_b = -1, pointer_bl = -1, pointer_c = -1, pointer_cl = -1, pointer_a = -1, pointer_al = -1;	//用作标记是否找到,后面两个分别指向A的结点和label位置
                for (i = 0; i < DAG.size(); i++){	//从尾部查找DAG表,找A B,规模不会太大所以全部走一遍
                    for (j = 0; j < DAG[i].labels.size(); j++){	//扫描标记
                        if (inter_pro[pointer].arg1 == DAG[i].labels[j]){	//找到B
                            pointer_b = i;
                            pointer_bl = j;
                        }
                        if (inter_pro[pointer].arg2 == DAG[i].labels[j]){	//找到C
                            pointer_c = i;
                            pointer_cl = j;
                        }
                        if (inter_pro[pointer].res == DAG[i].labels[j]){	//找到A
                            pointer_a = i;
                            pointer_al = j;
                        }
                    }
                }
                if (pointer_b == -1){		//B需要新建
                    vex_node vex_temp;
                    vex_temp.op.code = -1;		//新建的结点没有运算符
                    vex_temp.labels.push_back(inter_pro[pointer].arg1);	//把B作为第一个标记
                    vex_temp.arc = { -1, -1 };
                    DAG.push_back(vex_temp);
                    pointer_b = DAG.size() - 1;
                }
                if (pointer_c == -1){		//C需要新建
                    vex_node vex_temp;
                    vex_temp.op.code = -1;		//新建的结点没有运算符
                    vex_temp.labels.push_back(inter_pro[pointer].arg2);	//把B作为第一个标记
                    vex_temp.arc = { -1, -1 };
                    DAG.push_back(vex_temp);
                    pointer_c = DAG.size() - 1;
                }
                int k;
                for (k = 0; k < DAG.size(); k++){	//查找指向B C 的 w 结点
                    if (DAG[k].op == inter_pro[pointer].op){//结点的运算符相同
                        if ((DAG[k].arc.adjvex1 == pointer_b && DAG[k].arc.adjvex2 == pointer_c) ||	//且操作数相同
                            (DAG[k].arc.adjvex1 == pointer_c && DAG[k].arc.adjvex2 == pointer_b)){
                            //考虑标记的位置交换
                            if (inter_pro[pointer].res.code == 0 && DAG[k].labels[0].code == -2){
                                DAG[k].labels.insert(DAG[k].labels.begin(), inter_pro[pointer].res);//A是用户定义，B是临时变量，A作为主标记插入最前面
                            }
                            else{
                                //把A插到B所在labels的末尾
                                DAG[k].labels.push_back(inter_pro[pointer].res);
                            }

                            break;
                        }
                    }
                }
                if (k == DAG.size()){	//没有公共表达式
                    vex_node vex_temp;
                    vex_temp.op = inter_pro[pointer].op;		//加入运算符
                    vex_temp.labels.push_back(inter_pro[pointer].res);	//把A作为第一个标记
                    vex_temp.arc.adjvex1 = pointer_b;
                    vex_temp.arc.adjvex2 = pointer_c;
                    DAG.push_back(vex_temp);
                }
                if (pointer_a != -1 && pointer_al != 0){	//A标记已经存在且不是label中的第一个main标记
                    vector<Token>::iterator it = DAG[pointer_a].labels.begin() + pointer_al;
                    DAG[pointer_a].labels.erase(it);		//删除A标记
                    if (DAG[pointer_a].labels.size() == 0){		//如果DAG结点的标记为空，则删除结点
                        vector<vex_node>::iterator it = DAG.begin() + pointer_a;
                        DAG.erase(it);
                    }
                }
            }
        }
        pointer++;
    }
}

int find_first_cal(int start, int end){
    int i;
    for (i = start; i < end; i++){
        if (inter_pro[i].label == 0)
            break;
    }
    return i;
}

void creat_quad(int start_pointer, int end_pointer){
    int i;
    int quad_pointer;
    for (quad_pointer = start_pointer; quad_pointer < end_pointer; ){
        if (inter_pro[quad_pointer].op == Token(5, -1) || inter_pro[quad_pointer].op == Token(6, -1)||inter_pro[quad_pointer].op == Token(34, -1))
            quad_pointer++;
        else
            break;
    }
    for (i = 0; i < DAG.size(); i++){
        if (DAG[i].op.code != -1){ //结点有运算符
            quadruple temp;
            temp.arg1 = DAG[DAG[i].arc.adjvex1].labels[0];//把所指向的结点的主标记当做操作数
            temp.arg2 = DAG[DAG[i].arc.adjvex2].labels[0];//把所指向的结点的主标记当做操作数
            temp.op = DAG[i].op;
            temp.label = 0;
            temp.res = DAG[i].labels[0];//主标记作为赋值对象
            new_inter.push_back(temp);
           // cout<<"i="<<i<<endl;
           // cout<<DAG[i].op.code<<endl;
            int j;
            for (j = 1; j < DAG[i].labels.size(); j++){
                if (DAG[i].labels[j].code == 0){
                    quadruple temp;
                    temp.arg1 = DAG[i].labels[0];//把当前结点主标记当做操作数
                    temp.arg2.code = -1;	//没有操作数
                    temp.op = Token(17, 0);	// =
                    temp.label = 0;
                    temp.res = DAG[i].labels[j];//标记作为赋值对象
                    new_inter.push_back(temp);

                }
            }
        }
        else{	//结点没有运算符
            int j;
            for (j = 1; j < DAG[i].labels.size(); j++){
                if (DAG[i].labels[j].code == 0){	//有用户定义的标识符
                    quadruple temp;
                    temp.arg1 = DAG[i].labels[0];//把当前结点主标记当做操作数
                    temp.arg2.code = -1;	//没有操作数
                    temp.op = Token(17, 0);	// =
                    temp.res = DAG[i].labels[j];//标记作为赋值对象
                    temp.label = 0;
                    new_inter.push_back(temp);
                }
            }
        }
    }
  //  cout<<new_inter.size()<<endl;
    for (int i = 0; i < new_inter.size(); i++){
        int label = 0;
        if (inter_pro[quad_pointer + i].label != 0&&inter_pro[quad_pointer + i].label != 3){
            label = inter_pro[quad_pointer + i].label;
        }
        if(inter_pro[quad_pointer + i].label ==3)//特殊语句，即不跳入也不跳转
              quad_pointer++;
        inter_pro[quad_pointer + i] = new_inter[i];
        inter_pro[quad_pointer + i].label = label;
    }

    quad_pointer = quad_pointer + new_inter.size();
    for (; quad_pointer <= end_pointer; quad_pointer++){
        if (inter_pro[quad_pointer].label != 0)
            break;
        else{
            inter_pro[quad_pointer].op = Token(-1, -1);
            inter_pro[quad_pointer].arg1 = Token(-1, -1);
            inter_pro[quad_pointer].arg2 = Token(-1, -1);
            inter_pro[quad_pointer].pointer = NULL;
            inter_pro[quad_pointer].res = Token(-1, -1);
            inter_pro[quad_pointer].label = -1;
        }
    }
    new_inter.clear();
}

void output_inter_pro(int front, int end){
    int i = front;
    for (; i <= end; i++){
        if (inter_pro[i].op.code != -1){
            cout << "(" << keywords[inter_pro[i].op.code] << ",";
            switch (inter_pro[i].arg1.code){
            case 0:cout << Id[inter_pro[i].arg1.value] << ","; break;//当为变量时
            case 1:cout << ConstChar[inter_pro[i].arg1.value] << ","; break;
            case 3:cout << ConstNum[inter_pro[i].arg1.value] << ","; break;//常数
            case -2:cout << "t" << inter_pro[i].arg1.value << ","; break;
            case -1:cout << "_,";
            }
            switch (inter_pro[i].arg2.code){
            case 0:cout << Id[inter_pro[i].arg2.value] << ","; break;//当为变量时
            case 1:cout << ConstChar[inter_pro[i].arg1.value] << ","; break;
            case 3:cout << ConstNum[inter_pro[i].arg2.value] << ","; break;//常数
            case -2:cout << "t" << inter_pro[i].arg2.value << ","; break;
            case -1:cout << "_,";
            }
            switch (inter_pro[i].label){
            case 0:case 2:{
                       switch (inter_pro[i].res.code){
                       case 0:cout << Id[inter_pro[i].res.value] ; break;//当为变量时
                       case 3:cout << ConstNum[inter_pro[i].res.value] ; break;//常数
                       case -2:cout << "t" << inter_pro[i].res.value ; break;
                       case -1:cout << "_,";
                       }
                       break;
            }
            case 1:{
                       cout << inter_pro[i].pointer;break;
            }
            case 3:{
                                   switch (inter_pro[i].res.code){
                                   case 0:cout << Id[inter_pro[i].res.value] ; break;//当为变量时
                                   case 3:cout << ConstNum[inter_pro[i].res.value] ; break;//常数
                                   case -2:cout << "t" << inter_pro[i].res.value ; break;
                                   case -1:cout << "_,";
                                   }
                                   break;
                        }
            }
            cout << ")" << endl;
        }
    }
}

void optimization(){
    cout << "before optimize:" << endl;
    output_inter_pro(0, inter_pro.size() - 1);
    basic_block();
    for (int i = 0; i < flowGraph.size(); i++){
        DAG_optimaize(flowGraph[i].front, flowGraph[i].end);
       // cout<<i<<"nd"<<endl;
       //  output_inter_pro(flowGraph[i].front, flowGraph[i].end);
        creat_quad(flowGraph[i].front, flowGraph[i].end);
        DAG.clear();
    }
    system("pause");
    if (flowGraph.size() != 0)
    {

        cout << "after optimize:" << endl;
        output_inter_pro(0, inter_pro.size() - 1);
    }
    else {
        cout << "Can Not optimize" << endl;
    }

}

