######################################################################
# Automatically generated by qmake (2.01a) Mon Dec 19 20:49:03 2011
######################################################################
include(../../engine/engine.pri)

LIBS += -L../../engine/lib
LIBS += -lGL -lGLU -lGLEW -lglut -lengine
OBJECTS_DIR = obj
TEMPLATE = app
TARGET = 1-interpolation
DEPENDPATH += . src
INCLUDEPATH += . src

# Input
HEADERS += src/MainApplication.h
SOURCES += src/Main.cpp src/MainApplication.cpp
