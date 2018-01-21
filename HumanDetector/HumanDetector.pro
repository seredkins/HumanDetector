#-------------------------------------------------
#
# Project created by QtCreator 2017-12-02T17:53:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HumanDetector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aqimage.cpp

HEADERS  += mainwindow.h \
    aqimage.h

FORMS    += mainwindow.ui

SOURCES += \
    model.cpp \
    liblinear-1.8/tron.cpp \
    liblinear-1.8/linear.cpp \
    liblinear-1.8/blas/dscal.c \
    liblinear-1.8/blas/dnrm2.c \
    liblinear-1.8/blas/ddot.c \
    liblinear-1.8/blas/daxpy.c

HEADERS += \
    model.h \
    liblinear-1.8/tron.h \
    liblinear-1.8/linear.h \
    liblinear-1.8/blas/blasp.h \
    liblinear-1.8/blas/blas.h
