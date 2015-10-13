#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T21:02:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGTalk
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatarea.cpp

HEADERS  += mainwindow.h \
    chatarea.h

FORMS    += mainwindow.ui \
    chatarea.ui

QMAKE_CXXFLAGS += -std=c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../gloox/lib/release/ -lgloox
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../gloox/lib/debug/ -lgloox
else:unix: LIBS += -L$$PWD/../gloox/lib/ -lgloox

INCLUDEPATH += $$PWD/../gloox/include
DEPENDPATH += $$PWD/../gloox/include
