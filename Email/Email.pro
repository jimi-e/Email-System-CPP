#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T09:40:28
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Email
TEMPLATE = app


SOURCES += main.cpp\
    contentwidget.cpp \
        emaildialog.cpp \
    colorwidget.cpp \
    coloritem.cpp \
    form.cpp \
    infomation.cpp \
    loginwidget.cpp \
    recvwidget.cpp \
    signupdialog.cpp

HEADERS  += emaildialog.h \
    client_net.h \
    colorwidget.h \
    coloritem.h \
    contentwidget.h \
    form.h \
    identify.h \
    infomation.h \
    loginwidget.h \
    mail.h \
    recvwidget.h \
    signupdialog.h

FORMS    += \
    contentwidget.ui \
    emaildialog.ui \
    form.ui \
    infomation.ui \
    loginwidget.ui \
    recvwidget.ui \
    signupdialog.ui

RESOURCES += \
    email.qrc \
    resource.qrc

DISTFILES += \
