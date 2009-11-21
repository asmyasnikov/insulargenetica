TEMPLATE = lib
TARGET = fitness-common
debug_and_release {
    CONFIG -= debug_and_release
}
debug_and_release_target {
    CONFIG -= debug_and_release_target
}
release {
    debug {
        CONFIG -= debug
    }
}
debug {
    release {
        CONFIG -= release
    }
}
INSULARGENETICALIB = insulargenetica
debug {
    TARGET = $$join(TARGET,,,d)
    INSULARGENETICALIB = $$join(INSULARGENETICALIB,,,d)
}
LIBS += -L../../build
LIBS += -l$$INSULARGENETICALIB
DEPENDPATH += . src
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += plugin
LIBS += -L../../build -linsulargenetica
# Input
HEADERS += ../include/CDeJong.h
HEADERS += ../include/CGriewank.h
HEADERS += ../include/CRastrigin.h
SOURCES += src/CDeJong.cpp
SOURCES += src/CGriewank.cpp
SOURCES += src/CRastrigin.cpp
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
