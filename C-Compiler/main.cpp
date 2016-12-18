#include "mainwindow.h"
#include <QApplication>
#include "cifa.h"

using namespace std;

int main(int argc, char *argv[])
{
//    _tmain();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    cout<<"test"<<endl;
    return a.exec();
//    return 0;
}
