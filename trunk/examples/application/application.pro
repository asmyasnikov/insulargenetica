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
TARGET = insulargenetica
INSULARGENETICALIB = insulargenetica
debug {
    TARGET = $$join(TARGET,,,d)
    INSULARGENETICALIB = $$join(INSULARGENETICALIB,,,d)
}
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += console
LIBS += -L../../build
LIBS += -l$$INSULARGENETICALIB
# Input
SOURCES += main.cpp
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
