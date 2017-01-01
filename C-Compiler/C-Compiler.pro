#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T13:58:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = C-Compiler
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += main.cpp\
        mainwindow.cpp \
    cifa.cpp \
    yufa_analysis.cpp \
    compile.cpp \
    optimize.cpp \
    help.cpp \
    view.cpp \
    src/codeeditor.cpp \
    src/myhighlighter.cpp

HEADERS  += mainwindow.h \
    global.h \
    cifa.h \
    yufa_analysis.h \
    compile.h \
    optimize.h \
    help.h \
    view.h \
    src/codeeditor.h \
    src/myhighlighter.h \
    src/typedef.h

FORMS    += mainwindow.ui \
    help.ui

DISTFILES += \
    wenben.txt

mac{
    LIBS += -framework opengl -framework glut
}
