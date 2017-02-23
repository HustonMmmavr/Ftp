QT += core
QT -= gui

CONFIG += c++11

TARGET = untitled1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    thread.cpp

DISTFILES += \
    untitled1.pro.user \
    ReadMe.txt

HEADERS += \
    convert.h \
    Dir.h \
    exeption.h \
    file.h \
    ftpServer.h \
    Ifile.h \
    Isocket.h \
    lightarray.h \
    socket.h \
    strings.h \
    unixdir.h \
    unixfile.h \
    unixsocket.h \
    windir.h \
    windowssocket.h \
    winfile.h \
    thread.h
