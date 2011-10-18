qmake -project -t app -r -after "LIBS += -lGL -lGLU -lGLEW -lglut -L../common/lib -lcommon" "OBJECTS_DIR = obj" "INCLUDEPATH += ../common/include"
