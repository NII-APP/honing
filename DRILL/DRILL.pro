QT       += core gui opengl widgets

TARGET = DRILL
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

HEADERS += \
    CGL/cvertexes.h \
    CGL/cparse.h \
    CGL/cparallelepiped.h \
    CGL/clabel.h \
    CGL/cinterval.h \
    CGL/cglwidgetmenu.h \
    CGL/cglwidget.h \
    CGL/cgl.h \
    CGL/cexpression.h \
    CGL/cdimension.h \
    CGL/cchartdata.h \
    CGL/cchart3d.h \
    CGL/cchart.h

SOURCES += \
    main.cpp \
    CGL/cvertexes.cpp \
    CGL/cparse.cpp \
    CGL/cparallelepiped.cpp \
    CGL/clabel.cpp \
    CGL/cinterval.cpp \
    CGL/cglwidgetmenu.cpp \
    CGL/cglwidget.cpp \
    CGL/cgl.cpp \
    CGL/cexpression.cpp \
    CGL/cdimension.cpp \
    CGL/cchartdata.cpp \
    CGL/cchart3d.cpp \
    CGL/cchart.cpp
