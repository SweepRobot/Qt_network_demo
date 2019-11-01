#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T15:04:03
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTNetWork
TEMPLATE = app


SOURCES += main.cpp\
        sockettcpserver.cpp

HEADERS  += sockettcpserver.h

FORMS    += sockettcpserver.ui
