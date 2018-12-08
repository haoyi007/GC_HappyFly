#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T16:37:06
#
#-------------------------------------------------

QT       += core gui webkitwidgets webkit
QT       += serialport
QT += printsupport
QT += multimedia

CONFIG  += qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HAOYI20180328
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        thread.cpp \
    dataprocessor.cpp \
    radiatrueshow.cpp\
    qcustomplot.cpp \
    myspeed.cpp\
    about.cpp \
    readthread.cpp
HEADERS  += mainwindow.h\
        thread.h \
   dataprocessor.h \
    radiatrueshow.h\
    qcustomplot.h \
    myspeed.h\
    about.h \
    global.h \
    readthread.h

FORMS    += mainwindow.ui \
    radiatrueshow.ui \
    about.ui
RC_FILE = myapp.rc  //图标添加网址：http://blog.sina.com.cn/s/blog_71fa0df501011xrk.html

RESOURCES += \
    pic.qrc
