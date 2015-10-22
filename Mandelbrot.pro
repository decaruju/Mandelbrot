#-------------------------------------------------
#
# Project created by QtCreator 2015-10-22T15:52:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mandelbrot
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -lgmp

SOURCES += main.cpp\
        mainwindow.cpp \
    mandel.cpp \
    startdialog.cpp

HEADERS  += mainwindow.h \
    bitmap_image.hpp \
    startdialog.h

FORMS    += mainwindow.ui \
    startdialog.ui
