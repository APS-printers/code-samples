#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T13:39:24
#
#-------------------------------------------------

QT       += core

QT       += gui widgets

TARGET = hard-reset
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cxx \
    ../lib/windows/usbprinter.cxx

INCLUDEPATH += ../lib/windows/

HEADERS += \
    ../lib/windows/usbprinter.hxx
