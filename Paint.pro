QT       += core gui\
            svg\
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paint
TEMPLATE = app


SOURCES += main.cpp\
        paintwindow.cpp \
    polygonitem.cpp \
    svgreader.cpp

HEADERS  += paintwindow.h \
    polygonitem.h \
    svgreader.h \
    includes.h

FORMS    += paintwindow.ui
