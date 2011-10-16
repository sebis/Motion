qmake -project -t lib -recursive -after "CONFIG += staticlib" "LIBS += -Llib -lGL -lGLEW -lglut" "OBJECTS_DIR = obj" "DESTDIR = lib"
