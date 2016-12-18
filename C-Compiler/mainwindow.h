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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void Help_actionHelp_Slot();
    void File_actionOpen_Slot();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
