qmake -project -t app -recursive -after "LIBS += -lGL -lGLEW -lSDL -L../common/lib -lcommon" "OBJECTS_DIR = obj" "INCLUDEPATH += ../common/include"
