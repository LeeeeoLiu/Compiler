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
            tr("Config Files (*.c *.h)"));
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
