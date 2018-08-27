#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T13:52:33
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DEFINES += QT_DLL QT_WIDGETS_LIB QT_XML_LIB QWT_DLL QT_SVG_LIB QT_OPENGL_LIB


TARGET = GraphImpl
TEMPLATE = app
DESTDIR = ./Debug

# Configuration for adding QWT plot
include(QwtConf.pri)


SOURCES += main.cpp\
        widget.cpp \
    graph/qwtgraphoperation.cpp

HEADERS  += widget.h \
    graph/GraphBaseStructs.h \
    graph/qwtgraphoperation.h

FORMS    += widget.ui
