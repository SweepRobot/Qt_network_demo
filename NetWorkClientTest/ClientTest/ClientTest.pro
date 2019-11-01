 #-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T16:36:13
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientTest
TEMPLATE = app


SOURCES += main.cpp\
        sockettcpclient.cpp

HEADERS  += sockettcpclient.h

FORMS    += sockettcpclient.ui
