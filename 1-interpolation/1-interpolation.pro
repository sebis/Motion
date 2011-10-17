######################################################################
# Automatically generated by qmake (2.01a) Mon Oct 17 05:08:08 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . include src
INCLUDEPATH += . include

# Input
HEADERS += include/ControlPoints.h \
           include/Interpolator.h \
           include/KeyframeAnimator.h \
           include/MainApplication.h \
           include/MeshObject.h \
           include/SplineRenderer.h \
           include/TransformRenderer.h
SOURCES += src/KeyframeAnimator.cpp \
           src/Main.cpp \
           src/MainApplication.cpp \
           src/MeshObject.cpp \
           src/SplineRenderer.cpp \
           src/TransformRenderer.cpp
LIBS += -lGL -lGLEW -lglut -lSDL -L../common/lib -lcommon
OBJECTS_DIR = obj
INCLUDEPATH += ../common/include
