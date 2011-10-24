qmake -project -t lib -r -after "CONFIG += staticlib" "LIBS += -lGL -lGLU -lGLEW -lglut -L../common/lib -lcommon" "OBJECTS_DIR = obj" "INCLUDEPATH += ../common/include" "DESTDIR = lib"
