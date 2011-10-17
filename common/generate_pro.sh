qmake -project -t lib -r -after "CONFIG += staticlib" "LIBS += -Llib -lGL -lGLU -lGLEW -lglut" "OBJECTS_DIR = obj" "DESTDIR = lib"
