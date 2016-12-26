#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtOpenGL>
/* for mac */
#ifdef Q_OS_MAC
    #include <GLUT/glut.h>
#endif
/* for windows */
#ifdef Q_OS_WIN32
    #include <GL/glut.h>
#endif

#include <QMainWindow>
#include <QGLWidget>
#include "cifa.h"

extern int row;
extern char ww;
extern int tp;
extern int cnum;
extern int front;
extern char filec[10000];

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void outputRecord(QString tempMess);
    void displayToken();
    void initAll();
    void displaySYNBL();
    void displayQuat(int front, int end);

private slots:
    void Help_actionHelp_Slot();
    void File_actionOpen_Slot();
    void File_actionSave_Slot();
    void on_btnCompile_clicked();
    void on_saveAsm_clicked();

private:
    Ui::MainWindow *ui;
    QString path;
    QString codeRecord;
    QString asmCode;
    QString asmPath;
};

#endif // MAINWINDOW_H
