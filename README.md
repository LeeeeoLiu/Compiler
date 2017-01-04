# Compiler
### 任务分工
| Task |Performer|
|--------|--------|
| 词法分析生成 Token 序列、符号表子表|     Sakura  |
| 语法分析,符号表总表,测试,合并代码|Leeeeo|
| 中间代码生成及优化|Loke|
| 目标代码生成|Koala|

## 开发环境
```
MacOS:

Qt Creator 4.2.0 (4.2.0)
Based on Qt 5.7.1 (Clang 7.0 (Apple), 64 bit)

Built on Dec 9 2016 12:24:11

From revision 5071dd80cb

Copyright 2008-2016 The Qt Company Ltd. All rights reserved.

The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
```
```
Windows:

Qt Creator 4.2.0 (4.2.0)
Based on Qt 5.7.1 (MSVC 2015, 32 bit)

Built on Dec 9 2016 12:30:00

From revision 5071dd80cb

Copyright 2008-2016 The Qt Company Ltd. All rights reserved.

The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
```
## 版本记录

### Todolist
- [ ] 完善语义分析报错功能(可能需要重写该模块)
- [ ] 添加快捷键
- [ ] 添加 switch case 语句
- [ ] 添加 for 语句
- [ ] 添加 多维数组
- [ ] 添加 += -= *= /=

### Version 3.0 (修复之前的 bug 继续增加功能)

- [ ] 修复已经出现的bugs [0] (详细见 [issue](https://github.com/LeeeeoLiu/Compiler/issues))
	- [x] #16 函数跳转问题 没有函数的时候不应该启用funcstart变量 [Leeeeo]
	- [ ] #14 目标代码 JZ 没有标号 [Loke]
	- [x] #12 变量重定义		[Leeeeo]
	- [x] #11 初始化所有变量		[Leeeeo]
	- [x] #6  数组中间代码优化问题	[Loke]
	- [ ] #5  中间定义新变量问题		[Loke]
- [ ] 数组支持char float struct
	- [x] 四元式生成[Sakura]
	- [ ] 目标代码生成[Loke]
- [x] 添加函数嵌套使用	[Leeeeo]
- [x] 添加显示结构体表,数组表,常数表 [Leeeeo]
- [x] 语义分析报错信息,明确到行并显示到屏幕上 [Leeeeo]
- [x] 宏定义
	- [x] 四元式生成 [Sakura]
	- [x] 目标代码生成 [Loke]
- [x] 自增自减
	- [x] 四元式生成 [Sakura]
	- [x] 目标代码生成 [Loke]
- [x] 代码高亮	[Leeeeo]
- [x] 代码编辑区添加行号	[Leeeeo]

### Version 2.0 (增加一些功能)
- [x] 添加 cout
	- [x] cout 四元式     [Loke]
	- [x] cout 目标代码    [Koala]
- [x] 添加数组
	- [x] 添加数组四元式      [Leeeeo]
	- [x] 数组目标代码生成 		[Koala]
- [x] 添加结构体
	- [x] 添加结构体四元式  [Leeeeo]
	- [x] 结构体目标代码生成  	[Loke]
- [x] 添加函数
	- [x] void 函数 call ret四元式生成 	[Leeeeo]
	- [x] 带返回值函数call ret四元式生成 	[Leeeeo]
	- [x] 函数目标代码生成	[Leeeeo]
- [x] 图形界面实现  [Leeeeo]
- [x] 行注释,块注释 [Sakura]
- [x] 添加> < >= <=
	- [x] > < >= <= 四元式 [Sakura]
	- [x] > < >= <= 目标代码生成 	[Koala]
- [x] DAG中间代码优化	[Loke]
- [x] 词法扫描器升级2.0 [Sakura]
- [x] 补充文法		[Leeeeo]

### Version 1.0 (黑框框,使用简单文法走一遍整体流程)
- [x] 添加 赋值语句		[Sakura]
- [x] 添加 if 语句			[Leeeeo]
- [x] 添加 while 语句		[Loke]
- [x] 简单文法词法分析		[Sakura]
- [x] 简单文法语法分析		[Leeeeo]
- [x] 简单文法中间代码生成	[Loke]
- [x] 简单文法目标代码生成	[Koala]
- [x] 文法定义				[Leeeeo]


## 函数调用四元式 [Leeeeo]
调用函数 test();
```
(call,_,_,0)
```
返回 return;
```
(ret,_,_,7)
```

## 结构体四元式 [Leeeeo]
```
struct hh
{
        int a;
        char c;
};
```
存数 test.a=2;

```
(=.,test,a,t3)
(=,2,_,t3)
(.=,t3,a,test)
```

取数 e=test.a;

```
(=.,test,a,t4)
(=,t4,_,e)
```
## 数组四元式 [Leeeeo]
存数 bb[4]=t0;
```
([]=,t0,4,bb)
```
取数 t6=bb[5];
```
(=[],bb,5,t6)
```
## cout 四元式 [Loke]
输出变量
```
(cout,g,_,_)
```
输出表达式
```
(cout,t0,_,_)
```
## 词法分析器的设计 [Sakura]
输入:源代码
输出: Token 序列
### 词法定义
备注：code是种别码，value是相应表中的地址

| 描述 | code |value|
|--------|--------|-------|
|自定义标识符|	0|	|
|字符常量	|1	||
|字符串常量|	2	||
|数字	|3	||
|main	|4	|0|
|while	|5|	0|
|if|	6	|0|
|char|	7	|0|
|int	|8|	0|
|float	|9|	0|
|struct|	10|	0|
|+|	11	|0|
|-|	12	|0
|*|	13	|0
|/|	14||	
|{|	15|	0
|}|	16|	0
|=|	17|	0
|,|	18|	0
|[|	19|	0
|]|	20|	0
|;|	21|	0
|\"|22||
|'|	23|	|
|(|	24|	|
|)|	25|	|
|&|	26|	|
|&#124;|27||	
|!|	28||
|void|	29||	
|else|	30	||
|return|	31||	
|&&|	32	||
|&#124;&#124;|	33	||
|cout|	34	||
|cin|	35	||
|<<|	36	||
|>>|	37	||
|long|	38	||
|short|	39	||
|bool|	40	||
|double|	41	||
|typedef|	42	||
|unsigned|	43	||
|static|	44	||
|enum|	45	||
|for|	46	||
|do|	47	||
|continue|	48	||
|signed|	49	||
|extern|	50	||
|inline|	51	||
|const|	52	||
|default|	53	||
|case|	54	||
|break|	55	||
|switch|	56	||
|sizeof|	57	||
|union|	58	||
|auto|	59	||
|volatile|	60	||
|register|	61	||
|goto|	62	||
|restrict|	63	||
|Complex|	64	||
|Imaginary|	65	||
|==|	66	||
|>|	67	||
|>=|	68	||
|<|	69	||
|<=|	70	||
|=[] |71	||
|[]= |72	||
|.|73	||
|.= |74	| |
|=.|75	| |
|call|76	| |
|ret|77	| |
|++ |78	| |
|-- |79	| |
|+= |80	| |
|-= |81	| |
|*= |82	| |
|/= |83	| |
|# |84	| |
|define|85	| |
| |86	| |
| |87	| | |

## 语法分析器的设计 [Leeeeo]
### 功能
#### 输入
词法分析生成的TOKEN序列
#### 输出
在TOKEN序列有语法错误时输出相应的错误信息，否则返回isGrammarCorrect 
#### 主要思想及原理
检查词法分析生成的Token序列是否有语法错误。如果有错误，就将其存储起来，并继续扫描后面的错误。为了便于四元式的生成，本程序采用递归下降子程序法。本程序采用的文法是LL(1)文法，可以保证递归下降子程序的正确运行。在语法分析的过程当中，能够将遇到的语法分析通过的变量标识符填入定义的符号表中。
#### 文法定义
>  程序 -> 多个结构体定义 多个函数定义

>  多个结构体定义 -> {结构体}

>  结构体 -> struct 标识符 { 多个变量声明 } ;


>  多个变量声明 -> {单个变量声明}


> 单个变量声明 -> 变量类型 变量 ;| 变量类型 [长度];


> 变量 -> 标识符


>  变量类型 -> int|float|char


>  多个函数定义 -> {函数定义}


> 函数定义 -> 函数返回值类型 函数名称标识符 ( 参数列表 ) { 复合语句 返回语句 }


> 函数返回值类型 -> 变量类型|void


> 返回语句 -> return 算数表达式E ;|return ;|return 常量 ;


> 函数名称标识符 -> 标识符|main


> 参数列表 -> 变量类型 标识符 , 参数列表|空


>  复合语句 -> 多个变量声明 多个语句


>  多个语句 -> { 语句 }


>  语句 -> 初始化语句|赋值语句|while语句|if语句|cout语句


>  初始化语句 -> 变量类型 初始化赋值表


>  初始化赋值表 -> 标识符 , 初始化赋值表|标识符 = 算数表达式E , 初始化赋值表|;


>  赋值语句 -> 标识符 = 算数表达式E ;


>  算数表达式


>  > E -> T E1


>  > E1 -> w0 T E1|空


>  > T -> F T1


>  > T1 -> w1 F T1|空


>  > F -> I|( E )

>  while语句 -> while ( 算数表达式 ) { 多个语句 }


>  if语句 -> if ( 算数表达式 ) { 多个语句 } 否则语句


>  否则语句 -> else { 多个语句 }|空

>  cout语句->cout

>  cout  -><< 变量;|算术表达式;

#### 数据结构
1.	单个语法错误MJSyntaxError类
a)	构造函数MJSyntaxError(string errorMessage, int afterIndex)
i.	Parameter errorMessage: 错误的信息
ii.	Parameter afterIndex: 错误在token串中的位置
2.	语法错误集合MJErrorContainer类
a)	添加一个错误bool addError(MJSyntaxError newError)
i.	Returns 如果在token串的同一个位置的错误多于一个，就代表发生了不能忽略的错误，这时，返回false，停止语法分析。
b)	在屏幕上输出所有的错误void printErrors()
3.	存储变量标识符的栈vector<Token> sym_list_stack
4.	符号表vector<synbl> SYNBL
5.	四元式序列vector<quadruple> inter_pro
6.	操作数栈vector<Token> sem
7. 关键字表 vector<string> keywords
8. TOKEN序列vector<Token> token

#### 主要函数：
```
bool syntax_analysis()//主函数，判断是否有语法错误
void errorHappenedWithMessage(string message)//生成错误信息
bool next()  //读取下一字符
int isSynblExist()//查符号表
void retQuat()//生成函数返回的四元式
void callQuat(Token fun)//生成函数调用的四元式
void cal_QUAT(int op)//生成计算表达式的四元式
void arrGetQuat()//生成取值的四元式
void arrStoreQuat()//生成存数组的四元式
void struGetQuat()//生成取结构体成员变量的四元式
void struStoreQuat()//生成存结构体成员变量的四元式
void equa_QUAT(int op)//生成赋值表达式的四元式
void F(),void T(),void E()//算术表达式的递归下降子程序识别
void A()//>  <　>=  <=的识别
void type_list()//判断是否为数据类型
void symbol_list(int type)//识别标识符并将新标识符压入符号表
void symbolList_init(int type)//初始化赋值表
int isStructSys()//识别变量是否为结构体标识符
int isFuncVar()//识别变量是否为函数标识符
int isStructVar()//识别变量是否为结构体
void var_declaration()//识别变量声明
void var_declarationStruct()//识别结构体声明
void senten_list()//识别语句表
void param_list()//参数表
void structure()//识别结构体
compound_sen()//复合语句
```
#### 优点及创新：
模块化设计，便于函数的调用以及错误的修改，函数复用性很高，功能强大，能够识别多种语法错误并详细的指明错误的具体位置及相关信息，有利于源程序语法错误的修改

## 中间代码生成及优化 [Loke]

##### 功能
生成四元式，填写符号表，并对产生的四元式进行优化处理。
##### 数据结构
- 符号表
	- Synbl类：
	```
    class synbl
	{
	public:
		Token name;
		int type;		//指向类型表，内容应该不变
		int cat;		//种类，1是函数，2是变量，3是赋值形参，4是域名
		int addr;		//指向1函数表,0表示不指
	};
    ```
    符号表为Synbl类的vector容器：      vector<synbl> SYNBL;//符号表 
定义变量记录当前符号表中最后一个变量的位置：int VALL_pointer ;//
- 四元式
	- 四元式结构体：
	```
    struct quadruple{		//四元式定义
		int label;		//是否跳转,0表示不是跳转，1表示跳到别处，2表示跳到此处
		Token op;		//op中code == -1 表示叶子结点，运算为空
		Token arg1;		//如果没有内容code内容选-1
		Token arg2;
		Token res;
		int pointer;	//跳转指向
	};
	```
	其中每个四元式单元都记录该四元式跳转需求，操作符，两个操作数，结果单元，跳转地址
四元式序列定义为vector<quadruple> inter_pro;	//四元式序列
- 操作数栈
	- 操作数栈定义为vector<Token> sem;//操作数栈
	- 每一个单元都存储token串的结构，在token结构中又已经存储过类型以及在各类表中的位置，因此可以很容易得到token串的值及其类型，便于四元式的生成。
- 基本块
	- 基本块的结构体
	```
    struct flow_graph{
		int front;
		int end;
	};  每一个基本块都只需记录开头和结尾两个四元式序列号即可
    ```
    - 基本块的vector容器类
    ```
	vector<flow_graph> flowGraph;	//记录基本块划分状况  ，入口和出口的地址
	```
- DAG图
	- DAG节点数据结构定义：
	```
	struct vex_node{
		Token op;	//在token序列中的运算符在关键字表的编号
		vector<Token> labels;	//主标记是第一个，附加标记
		arc_node arc;	//指向操作数
	};   需要记录每个节点的运算符的标号，主标记和附标记，还有指向的两个两个操作数
	```
	- DAG图的vector容器类
	```
	vector<vex_node> DAG;	//DAG图
	```
- 优化后中间代码序列
	```
    vector<quadruple> new_inter;	//新生成的中间代码
	```

##### 算法
在语义分析和中间代码生成阶段，采用语法制导翻译法。在语法分析时插入相应的语义动作，本课设为生成四元式。语法制导翻译法完成对语法成分的翻译工作，即在语法分析过程中，依随分析的过程，根据每个产生式所对应的语义子程序(或语义规则描述的语义处理的加工动作)进行翻译。生成if条件语句、while循环语句、算术表达式（包含赋值语句）的四元式。
优化部分。首先优化分为两种类型，一种为与机器无关的优化，一种为与机器有关的优化。在这里我选择的优化方式为局部优化，属于与机器无关的优化，在中间代码级别上执行，针对本程序情况即为优化四元式。在程序里想要实现的也就是:常值表达式节省、公共子表达式节省和删除无用赋值。局部优化分为两个步骤：首先要划分基本块，找出一个入口和出口之间的代码段。其次就是利用基于DAG的局部优化方法。这里又分为两步，依次为：画出DAG图，重组出优化的四元式。

## 目标代码生成 [Koala]
##### 输入
优化后的四元式集合
##### 输出
目标代码(8086汇编)
##### 主要思想及原理
针对给定的四元式，生成相应的目标代码，通过compilization()函数实现此功能，依据符号表提供的信息对存储空间进行分配，这里使用CSEG()函数实现，然后对于相应的四元式进行读取，根据四元式中操作码的性质的不同对四元式中的操作码进行不同的运算，并将其转化为相应的目标代码
if语句块、while语句块牵涉到地址回填，这里，借助stack工具，通过把当前或者当前下一条的地址入站，已经生成的类汇编代码进行回填，或者所需回填代码还未生成，则等到遇到时生成。Cout语句块涉及到机内二进制代码转化为十进制代码输出问题，这里使用cout_compile()函数通过除十取余的方法使这一问题得以解决。
##### 创新点：
生成的目标代码不仅详尽地显示了整个程序的存取运行流程，更显示了符号表中各个数据存储的信息，可以非常方便的了解到数据存储位置

| 四元式 | 汇编代码 |作用|
|--------|--------|--------|
|```（+，a, b, t0）```|```MOV  AX, a```<br>```MOV  BX, b```<br>```ADD  AX,BX```<br>```MOV  t0,AX ```|两个操作数相加|
|``` （-，a,  b, t0）```|```MOV  AX, a```<br>```MOV  BX, b```<br>```SUB   AX,BX```<br>```MOV t0,AX ```|两个操作数相减|
|```（*， a,  b,  t0）```|```MOV  AX, a```<br>```MOV  BX, b```<br>```MUL  AX,BX```<br>```MOV  t0,AX ```|两个操作数相乘|
|```（/,  a,  b,  t0）```|```MOV  AX, a```<br>```MOV  BX, b```<br>```DIV  AX,BX```<br>```MOV  t0,AX ```|两个操作数相除|
|```（=， a,  _,  t0）```|```MOV  AX,a```<br>```MOV  t0,AX ```|赋值|
|```（>,  a,  b,  t0） ```|```MOV  AX,a```<br>```MOV  BX,b```<br>```CMP  AX,BX```<br>```MOV  t0,AX ```|比较|
|```（<,  a,  b,  t0） ```|```MOV  AX,a```<br>```MOV  BX,b```<br>```CMP  AX,BX```<br>```MOV  t0,AX ```|比较|
|```（if,  t0,  _,  t1） ```|```MOV  AX,t0```<br>```AND  AX,AX```<br>```JZ    t1 ```|条件跳转|
|``` （while,t0,  _,  t1）```|```MOV  AX,t0```<br>```AND  AX,AX```<br>```JZ     t1 ```|条件跳转|
|```（while,  _,  _, t0） ```|``` JMP   t0```|直接跳转|
|  ```(cout,t0,_,_) ```    |  ```MOV  AX,t0```<br>```CALL  COUT```     |输出|
|```（[]=,  a,  b,  t0） ```|```MOV  AX, a[b]```<br>```MOV  t0 , AX ```|数组取数|
|```（=[],  a,  b ,  t0） ```|```MOV  AX, t0```<br>```MOV  a[b],AX ```|数组存数|

##### 数据结构
```
struct targe{
    int flag;			//有几个操作数
    string cw;			//类型表中的名称或四元式中的运算符
    string arg1;		//类型表中的字长或四元式的一个操作数
    string arg2;		//类型表中的空间或四元式的一个操作数
    string label = "";	//四元式的结果
    targe* next;		//指向下一个结点
};
```
##### 主要函数
```
void data_new_node();    //建立新节点
string itos();             //将int类型的数据转化为string类型
void DSEG();            // data segment数据段完成data链表
void CSEG();            // code segment 代码段完成code链表
void compilization();     //主函数，完成汇编代码
void cout_compile();     //cout语句中将二进制转化为十进制输出
```

