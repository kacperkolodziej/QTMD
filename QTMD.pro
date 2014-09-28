#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T20:37:44
#
#-------------------------------------------------

QT       += core gui network

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTMD
TEMPLATE = app

INCLUDEPATH += headers
DEPENDPATH += headers

SOURCES += main.cpp\
        src/qtmdmain.cpp \
    src/MessageEdit.cpp \
    src/sslcertificate.cpp \
    src/sslerrors.cpp

HEADERS  += headers/qtmdmain.hpp \
    headers/MessageEdit.hpp \
    headers/sslcertificate.hpp \
    headers/sslerrors.hpp

FORMS    += qtmdmain.ui \
    sslcertificate.ui \
    sslerrors.ui

unix|win32: LIBS += -ltamandua_msg -lssl -lcrypto

RESOURCES += \
    resources/resources.qrc
