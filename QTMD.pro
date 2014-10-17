#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T20:37:44
#
#-------------------------------------------------

QT       += core gui network multimedia

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTMD
TEMPLATE = app

INCLUDEPATH += headers \
    headers\tamandua
DEPENDPATH += headers

SOURCES += main.cpp\
        src/qtmdmain.cpp \
    src/sslcertificate.cpp \
    src/sslerrors.cpp \
    src/version.cpp \
    src/messageedit.cpp \
    src/groupwidget.cpp

win32: SOURCES += src/tamandua/message_buffer.cpp \
    src/tamandua/message_composer.cpp \
    src/tamandua/version.cpp

HEADERS  += headers/qtmdmain.hpp \
    headers/sslcertificate.hpp \
    headers/sslerrors.hpp \
    headers/version.hpp \
    headers/messageedit.hpp \
    headers/groupwidget.hpp

win32: HEADERS += headers/tamandua/message.hpp \
    headers/tamandua/message_buffer.hpp \
    headers/tamandua/message_composer.hpp \
    headers/tamandua/message_header.hpp \
    headers/tamandua/types.hpp \
    headers/tamandua/version.hpp \
    headers/tamandua/version_config.hpp

FORMS    += qtmdmain.ui \
    sslcertificate.ui \
    sslerrors.ui

unix: LIBS += -ltamandua_msg -lssl -lcrypto
win32: LIBS += -L"C:/OpenSSL-Win32/lib" -llibeay32


RESOURCES += \
    resources/resources.qrc
