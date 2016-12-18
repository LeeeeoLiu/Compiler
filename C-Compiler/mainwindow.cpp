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
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Code Files(*.c)"));
            if(path.length() == 0) {
                    QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
            } else {
                    QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
            }

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
