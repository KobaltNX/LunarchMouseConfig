QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LunarchMouseConfig
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

# WinAPI для сенсы
win32:LIBS += -lUser32
win32:LIBS += -lSetupapi

