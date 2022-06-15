#-------------------------------------------------
#
# Project created by QtCreator 2020-11-09T15:29:38
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h
FORMS    += mainwindow.ui

INCLUDEPATH += D:\lc\Application\opencv\opencv-build\install\include

LIBS += D:\lc\Application\opencv\opencv-build\lib\libopencv_*.a

SUBDIRS += \
    MainWindow.pro

RC_ICONS = picture.ico
