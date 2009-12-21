TEMPLATE = lib
TARGET = cyrcle
debug_and_release:CONFIG -= debug_and_release
debug_and_release_target:CONFIG -= debug_and_release_target
release:debug:CONFIG -= debug
debug:release:CONFIG -= release
INSULARGENETICALIB = insulargenetica
debug {
    TARGET = $$join(TARGET,,,d)
    INSULARGENETICALIB = $$join(INSULARGENETICALIB,,,d)
}
LIBS += -L../../build
LIBS += -l$$INSULARGENETICALIB
DEPENDPATH += .
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += plugin
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../../build

# Input
HEADERS += CCyrcle.h
SOURCES += main.cpp \
    CCyrcle.cpp
LIBS += -L../../../build \
    -lfitness-common
