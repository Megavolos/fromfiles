#-------------------------------------------------
#
# Project created by QtCreator 2017-05-02T09:53:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fromfiles
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/debug/ -lqwt
else:unix:!macx: LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
DEPENDPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
