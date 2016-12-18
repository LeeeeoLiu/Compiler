/********************************************************************************
** Form generated from reading UI file 'help.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Help
{
public:
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *keysTab;
    QVBoxLayout *verticalLayout;
    QLabel *keysLabel;
    QWidget *tab_2;

    void setupUi(QWidget *Help)
    {
        if (Help->objectName().isEmpty())
            Help->setObjectName(QStringLiteral("Help"));
        Help->resize(362, 284);
        horizontalLayout_2 = new QHBoxLayout(Help);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget = new QTabWidget(Help);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        keysTab = new QWidget();
        keysTab->setObjectName(QStringLiteral("keysTab"));
        verticalLayout = new QVBoxLayout(keysTab);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        keysLabel = new QLabel(keysTab);
        keysLabel->setObjectName(QStringLiteral("keysLabel"));

        verticalLayout->addWidget(keysLabel);

        tabWidget->addTab(keysTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget);


        retranslateUi(Help);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Help);
    } // setupUi

    void retranslateUi(QWidget *Help)
    {
        Help->setWindowTitle(QApplication::translate("Help", "Form", Q_NULLPTR));
        keysLabel->setText(QApplication::translate("Help", "TextLabel", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(keysTab), QApplication::translate("Help", "Keys", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Help", "Tab 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Help: public Ui_Help {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
