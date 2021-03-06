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
    src/messageedit.cpp \
    src/groupwidget.cpp \
    src/qtmd_version.cpp \
    src/htmlviewer.cpp \
    src/loginwindow.cpp \
    src/registerwindow.cpp

win32: SOURCES += src/tamandua/message_buffer.cpp \
    src/tamandua/message_composer.cpp \
    src/tamandua/version.cpp

HEADERS  += headers/qtmdmain.hpp \
    headers/sslcertificate.hpp \
    headers/sslerrors.hpp \
    headers/messageedit.hpp \
    headers/groupwidget.hpp \
    headers/qtmd_version.hpp \
    headers/htmlviewer.hpp \
    headers/loginwindow.hpp \
    headers/registerwindow.hpp

win32: HEADERS += headers/tamandua/message.hpp \
    headers/tamandua/message_buffer.hpp \
    headers/tamandua/message_composer.hpp \
    headers/tamandua/message_header.hpp \
    headers/tamandua/types.hpp \
    headers/tamandua/version.hpp \
    headers/tamandua/version_config.hpp

FORMS    += forms/qtmdmain.ui \
    forms/sslcertificate.ui \
    forms/sslerrors.ui \
    forms/htmlviewer.ui \
    forms/loginwindow.ui \
    forms/registerwindow.ui

unix: LIBS += -ltamandua_msg -lssl -lcrypto
win32: LIBS += -L"C:/OpenSSL-Win32/lib" -llibeay32


RESOURCES += \
    resources/resources.qrc
