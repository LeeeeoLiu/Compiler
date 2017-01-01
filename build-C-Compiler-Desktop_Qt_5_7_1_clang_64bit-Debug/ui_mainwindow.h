/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionKeyboard_Info;
    QAction *actionXZ_Plane;
    QAction *actionHelp;
    QAction *actionOpen;
    QAction *actionSave;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab_8;
    QGridLayout *gridLayout_8;
    QTableView *tableViewToken;
    QWidget *tab_5;
    QGridLayout *gridLayout_7;
    QTableView *tableViewSYNBL;
    QWidget *tab_9;
    QGridLayout *gridLayout_9;
    QTableView *tableViewQuat;
    QWidget *tab_6;
    QGridLayout *gridLayout_6;
    QTableView *tableViewRINFL;
    QWidget *tab_7;
    QGridLayout *gridLayout_5;
    QTableView *tableViewAINFL;
    QWidget *tab;
    QGridLayout *gridLayout_19;
    QTableView *tableViewConstNum;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_20;
    QLabel *label;
    QPushButton *btnCompile;
    QGridLayout *codeArea;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QGridLayout *gridLayout_4;
    QLabel *label_2;
    QTextBrowser *statusArea;
    QPushButton *saveAsm;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOperations;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(820, 556);
        actionKeyboard_Info = new QAction(MainWindow);
        actionKeyboard_Info->setObjectName(QStringLiteral("actionKeyboard_Info"));
        actionXZ_Plane = new QAction(MainWindow);
        actionXZ_Plane->setObjectName(QStringLiteral("actionXZ_Plane"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 391, 474));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        tabWidget = new QTabWidget(scrollAreaWidgetContents_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        gridLayout_8 = new QGridLayout(tab_8);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        tableViewToken = new QTableView(tab_8);
        tableViewToken->setObjectName(QStringLiteral("tableViewToken"));

        gridLayout_8->addWidget(tableViewToken, 0, 0, 1, 1);

        tabWidget->addTab(tab_8, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        gridLayout_7 = new QGridLayout(tab_5);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        tableViewSYNBL = new QTableView(tab_5);
        tableViewSYNBL->setObjectName(QStringLiteral("tableViewSYNBL"));

        gridLayout_7->addWidget(tableViewSYNBL, 0, 0, 1, 1);

        tabWidget->addTab(tab_5, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QStringLiteral("tab_9"));
        gridLayout_9 = new QGridLayout(tab_9);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        tableViewQuat = new QTableView(tab_9);
        tableViewQuat->setObjectName(QStringLiteral("tableViewQuat"));

        gridLayout_9->addWidget(tableViewQuat, 0, 0, 1, 1);

        tabWidget->addTab(tab_9, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        gridLayout_6 = new QGridLayout(tab_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        tableViewRINFL = new QTableView(tab_6);
        tableViewRINFL->setObjectName(QStringLiteral("tableViewRINFL"));

        gridLayout_6->addWidget(tableViewRINFL, 0, 0, 1, 1);

        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        gridLayout_5 = new QGridLayout(tab_7);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        tableViewAINFL = new QTableView(tab_7);
        tableViewAINFL->setObjectName(QStringLiteral("tableViewAINFL"));

        gridLayout_5->addWidget(tableViewAINFL, 0, 0, 1, 1);

        tabWidget->addTab(tab_7, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_19 = new QGridLayout(tab);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        tableViewConstNum = new QTableView(tab);
        tableViewConstNum->setObjectName(QStringLiteral("tableViewConstNum"));

        gridLayout_19->addWidget(tableViewConstNum, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea_2, 0, 0, 2, 1);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 391, 231));
        scrollAreaWidgetContents->setAutoFillBackground(true);
        gridLayout_20 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_20->setSpacing(6);
        gridLayout_20->setContentsMargins(11, 11, 11, 11);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_20->addWidget(label, 0, 0, 1, 1);

        btnCompile = new QPushButton(scrollAreaWidgetContents);
        btnCompile->setObjectName(QStringLiteral("btnCompile"));

        gridLayout_20->addWidget(btnCompile, 0, 2, 1, 1);

        codeArea = new QGridLayout();
        codeArea->setSpacing(6);
        codeArea->setObjectName(QStringLiteral("codeArea"));

        gridLayout_20->addLayout(codeArea, 1, 0, 1, 3);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 1, 1, 1);

        scrollArea_3 = new QScrollArea(centralWidget);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 391, 231));
        scrollAreaWidgetContents_3->setAutoFillBackground(true);
        gridLayout_4 = new QGridLayout(scrollAreaWidgetContents_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_2 = new QLabel(scrollAreaWidgetContents_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_4->addWidget(label_2, 0, 0, 1, 1);

        statusArea = new QTextBrowser(scrollAreaWidgetContents_3);
        statusArea->setObjectName(QStringLiteral("statusArea"));

        gridLayout_4->addWidget(statusArea, 1, 0, 1, 2);

        saveAsm = new QPushButton(scrollAreaWidgetContents_3);
        saveAsm->setObjectName(QStringLiteral("saveAsm"));

        gridLayout_4->addWidget(saveAsm, 0, 1, 1, 1);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        gridLayout->addWidget(scrollArea_3, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        scrollArea_2->raise();
        scrollArea->raise();
        scrollArea_3->raise();
        saveAsm->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 820, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOperations = new QMenu(menuBar);
        menuOperations->setObjectName(QStringLiteral("menuOperations"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOperations->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuHelp->addAction(actionHelp);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionKeyboard_Info->setText(QApplication::translate("MainWindow", "Keyboard Info", Q_NULLPTR));
        actionXZ_Plane->setText(QApplication::translate("MainWindow", "XZ Plane", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("MainWindow", "Token", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "\347\254\246\345\217\267\350\241\250", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_9), QApplication::translate("MainWindow", " \345\233\233\345\205\203\345\274\217", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("MainWindow", "\347\273\223\346\236\204\344\275\223\350\241\250", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("MainWindow", "\346\225\260\347\273\204\350\241\250", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\345\270\270\346\225\260\350\241\250", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\272\220\347\240\201\345\214\272", Q_NULLPTR));
        btnCompile->setText(QApplication::translate("MainWindow", "\347\274\226\350\257\221", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\347\273\223\346\236\234\346\230\276\347\244\272\345\214\272", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        statusArea->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        saveAsm->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\261\207\347\274\226\344\273\243\347\240\201", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuOperations->setTitle(QApplication::translate("MainWindow", "Operations", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
