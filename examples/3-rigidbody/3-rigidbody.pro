######################################################################
# Automatically generated by qmake (2.01a) Sat Oct 29 17:13:05 2011
######################################################################
include(../../engine/engine.pri)

LIBS += -L../../engine/lib
LIBS += -lGLEW -lglut -lengine
OBJECTS_DIR = obj
TEMPLATE = app
TARGET = 3-rigidbody
DEPENDPATH += . src
INCLUDEPATH += . src

# Input
HEADERS += src/MainApplication.h
SOURCES += src/Main.cpp src/MainApplication.cpp
