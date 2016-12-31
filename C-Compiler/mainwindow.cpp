#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;

#include "help.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->codeArea->show();

    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(Help_actionHelp_Slot()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(File_actionOpen_Slot()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(File_actionSave_Slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Help_actionHelp_Slot()
{
    Help *windowHelp = new Help();
    windowHelp->show();

}

void MainWindow::File_actionOpen_Slot()
{
    path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Code Files(*.c)"));
            if(path.length() == 0) {
                    QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
            } /*else {
                    QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
            }*/

     QFile file(path);
     QString code;
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             qDebug()<<"Can't open the file!"<<endl;
         }
         QTextStream in(&file);
         while( !in.atEnd()){
             QString line = in.readLine();
             code =code+line+"\n";
         }
     ui->codeArea->setText(code);
}

void MainWindow::File_actionSave_Slot()
{
    if(path.length() == 0) {
        path = QFileDialog::getSaveFileName(this,
            tr("Open Config"),
            "",
            tr("Config Files (*.c *.h )"));
    }
    if (!path.isNull())
    {
        QFile f(path);
             if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
             {
                  QMessageBox::information(NULL, tr("Path"), tr("Open failed."));
             }
             QTextStream txtOutput(&f);
             QString codeSave =ui->codeArea->toPlainText();
             txtOutput << codeSave;
             f.close();
     }
}

void MainWindow::initAll()
{
    codeRecord="";
    outputRecord("Init the program......");
    ui->tableViewSYNBL->clearSpans();
    Id.clear();          //符号表的序列项表
    ConstNum.clear();    //常数表的表
    ConstString.clear(); //字符串常量表
    ConstChar.clear();   //字符常量表
    SYNBL.clear();
    keywords.clear();
    inter_pro.clear();
    token.clear();
    sem.clear();
    ww=' ';
    temp_num=0;
    check_list.clear();
    inter_pro_pointer=0;
    TYPEL.clear();
    sym_list_stack.clear();
    VALL_pointer=0;  //表示活动记录当前指向
    temp_num=0;//临时变量ti指针，从零开始，code = -2
    row=0;
    mainStartId=0; //记录 main 开始的位置
    tp=cnum=front=0; //cnum记录文本字符总数;
    row=1;

}

void MainWindow::outputRecord(QString tempMess)
{
    codeRecord=codeRecord+tempMess+"\n";
    ui->statusArea->setText(codeRecord);
}

void MainWindow::on_btnCompile_clicked()
{
    initAll();
//    ui->btnCompile->setEnabled(false);
    File_actionSave_Slot();
    outputRecord("Start Cifa Analysising......");
    cifa_main(path.toStdString());
    outputRecord("Cifa Analysis Succeed!We've put the token list into the left table.");
    displayToken();
    outputRecord("Start Yufa Analysising......");
    if (syntax_analysis())
    {
        outputRecord("Yufa Analysis Succeed!We've put the SYNBL information into the left table.");
        displaySYNBL();
        outputRecord("Ready to make Quats......");
        displayQuat(0, inter_pro.size() - 1);
        outputRecord("Try to get asm code......");
        asmCode=QString::fromStdString(compilization());
        outputRecord(asmCode);
        displayRINFL();
        displayAINFL();
        displayConstNum();
    }
    else
    {
        outputRecord("Yufa Analysis Failed!");
        outputRecord("Yufa Analysis Failed!");
        outputRecord("Yufa Analysis Failed!");
    }

}


void MainWindow::displayQuat(int front, int end)
{

    int vers=0;
    QStandardItemModel  *model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Quats"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Quats after optimization"));
    ui->tableViewQuat->setModel(model);
    while (1) {
        for (int i = front; i < end; i++){
            QString tempQuat="";
            if (inter_pro[i].op.code != -1){
                tempQuat="("+QString::fromStdString(keywords[inter_pro[i].op.code])+",";
                switch (inter_pro[i].arg1.code){
                case 0:tempQuat=tempQuat+QString::fromStdString(Id[inter_pro[i].arg1.value])+","; break;//当为变量时
                case 3:tempQuat=tempQuat+QString::fromStdString(ConstNum[inter_pro[i].arg1.value])+","; break;//常数
                case -2:tempQuat=tempQuat+"t"+QString::number(inter_pro[i].arg1.value)+","; break;
                case -1:tempQuat=tempQuat+"_,";
                }
                switch (inter_pro[i].arg2.code){
                case 0:tempQuat=tempQuat+QString::fromStdString(Id[inter_pro[i].arg2.value]) +","; break;//当为变量时
                case 3:tempQuat=tempQuat+QString::fromStdString(ConstNum[inter_pro[i].arg2.value])+","; break;//常数
                case -2:tempQuat=tempQuat+"t"+QString::number(inter_pro[i].arg2.value)+","; break;
                case -1:tempQuat=tempQuat+"_,";
                }
                switch (inter_pro[i].label){
                case 0:case 2:{
                           switch (inter_pro[i].res.code){
                           case 0:tempQuat=tempQuat+QString::fromStdString(Id[inter_pro[i].res.value]) ; break;//当为变量时
                           case 3:tempQuat=tempQuat+QString::fromStdString(ConstNum[inter_pro[i].res.value]) ; break;//常数
                           case -2:tempQuat=tempQuat+"t"+QString::number(inter_pro[i].res.value ); break;
                           case -1:tempQuat=tempQuat+"_,";
                           }
                           break;
                }
                case 1:{
                           tempQuat=tempQuat+QString::number(inter_pro[i].pointer);break;
                }
                case 3:{
                                       switch (inter_pro[i].res.code){
                                       case 0:tempQuat=tempQuat+QString::fromStdString(Id[inter_pro[i].res.value]) ; break;//当为变量时
                                       case 3:tempQuat=tempQuat+QString::fromStdString(ConstNum[inter_pro[i].res.value] ); break;//常数
                                       case -2:tempQuat=tempQuat+"t"+QString::number(inter_pro[i].res.value) ; break;
                                       case -1:tempQuat=tempQuat+"_,";
                                       }
                                       break;
                            }
                }
                tempQuat=tempQuat+")";
            }
            model->setItem(i,vers,new QStandardItem(tempQuat));
            model->item(i,vers)->setTextAlignment(Qt::AlignCenter);
        }
        if(vers)
            break;
        outputRecord("Quats all ready!");
        optimization();
        front=0;
        end=inter_pro.size() - 1;
        outputRecord("Try to optimize the Quats......");
        vers=1;
    }
    ui->tableViewQuat->horizontalHeader()->setStretchLastSection(true);
    outputRecord("Quats optimization successed!");
}

void MainWindow::displayToken()
{
    QStandardItemModel  *model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Token"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Word"));
    ui->tableViewToken->setModel(model);
    for (int i = 0; i < token.size(); i++)
    {
        QString tempToken="("+ QString::number(token[i].code, 10)+","+QString::number(token[i].value, 10)+")";
        model->setItem(i,0,new QStandardItem(tempToken));
        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        QString tempChar="";
        switch (token[i].code)
        {
        case 0:tempChar="<"+QString::fromStdString(Id[token[i].value])+">"; break;
        case 1:tempChar="<"+QString::fromStdString(ConstChar[token[i].value])+">"; break;
        case 2:tempChar="<"+QString::fromStdString(ConstString[token[i].value])+">"; break;
        case 3:tempChar="<"+QString::fromStdString(ConstNum[token[i].value])+">"; break;
        case -1:tempChar="Input not allowed !Please review your code in row "+QString::number(row)+"."; break;//不允许输入
        case 555:tempChar="ERROR CONST CHAR ! Please review your const char in row "+QString::number(row)+"."; break;//字符常量出错
        case 666:tempChar="ERROR CONST FLOATING NUMBER ! Please review your floating number in row "+QString::number(row)+"."; break;//浮点数出错
        default:tempChar="<"+QString::fromStdString(keywords[(token[i].code)])+">"; break;
        }//switch
        model->setItem(i,1,new QStandardItem(tempChar));
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);

    }
    ui->tableViewToken->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::displaySYNBL()
{
    QStandardItemModel  *model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("NAME"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("TYP"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("CAT"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("ADDR"));
    ui->tableViewSYNBL->setModel(model);
    for (int i = 0; i < SYNBL.size(); i++)
    {
        QString tempName="";
        switch (SYNBL[i].name.code)
        {
        case 0:tempName=QString::fromStdString(Id[SYNBL[i].name.value]); break;
        case 1:tempName=QString::fromStdString(ConstChar[SYNBL[i].name.value]); break;
        case 2:tempName=QString::fromStdString(ConstString[SYNBL[i].name.value]); break;
        case 3:tempName=QString::fromStdString(ConstNum[SYNBL[i].name.value]); break;
        default:tempName=QString::fromStdString(keywords[SYNBL[i].name.code]);
        }
        model->setItem(i,0,new QStandardItem(tempName));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,1,new QStandardItem(QString::fromStdString(TYPEL[SYNBL[i].type].name)));
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        QString tempCat="";
        switch (SYNBL[i].cat)
        {
        case 1:model->setItem(i,2,new QStandardItem("FUNC")); break;
        case 2:model->setItem(i,2,new QStandardItem("VARI")); break;
        case 3:model->setItem(i,2,new QStandardItem("PARA")); break;
        case 4:model->setItem(i,2,new QStandardItem("DOMA")); break;
        case 5:model->setItem(i,2,new QStandardItem("ARRAY")); break;
        case 6:model->setItem(i,2,new QStandardItem("STRUCTDEC")); break;
        case 7:model->setItem(i,2,new QStandardItem("STRUCTDEF")); break;
        default:model->setItem(i,2,new QStandardItem("ERROR")); break;
        }
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,3,new QStandardItem(QString::number(SYNBL[i].addr)));
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewSYNBL->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::displayRINFL()
{

}

void MainWindow::displayAINFL()
{

}

void MainWindow::displayConstNum()
{

}



void MainWindow::on_saveAsm_clicked()
{
    if(asmPath.length() == 0) {
        asmPath = QFileDialog::getSaveFileName(this,
            tr("Open Config"),
            "",
            tr("Config Files (*.asm )"));
    }
    if (!asmPath.isNull())
    {
        QFile f(asmPath);
             if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
             {
                  QMessageBox::information(NULL, tr("Path"), tr("Open failed."));
             }
             QTextStream txtOutput(&f);
             QString codeSave =asmCode;
             txtOutput << codeSave;
             f.close();
     }
}
