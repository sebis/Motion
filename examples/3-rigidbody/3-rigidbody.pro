######################################################################
# Automatically generated by qmake (2.01a) Mon Dec 19 21:02:33 2011
######################################################################
include(../../engine/engine.pri)

LIBS += -L../../engine/lib
LIBS += -lGL -lGLU -lGLEW -lglut -lengine
OBJECTS_DIR = obj
TEMPLATE = app
TARGET = 3-rigidbody
DEPENDPATH += . src
INCLUDEPATH += . src

# Input
HEADERS += src/MainApplication.h
SOURCES += src/Main.cpp src/MainApplication.cpp
