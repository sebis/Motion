qmake -project -t app -recursive -after "LIBS += -lGL -lGLEW -lglut -lSDL -L../common/lib -lcommon" "OBJECTS_DIR = obj" "INCLUDEPATH += ../common/include"
