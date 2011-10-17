qmake -project -t app -recursive -after "LIBS += -lGL -lGLEW -lSDL -lglut -L../common/lib -lcommon" "OBJECTS_DIR = obj" "INCLUDEPATH += ../common/include"
