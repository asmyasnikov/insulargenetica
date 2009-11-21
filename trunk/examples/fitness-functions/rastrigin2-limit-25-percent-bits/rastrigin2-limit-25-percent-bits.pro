TEMPLATE = lib
TARGET = rastrigin2-limit-25-precent-bits
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
FITNESSCOMMONLIB = fitness-common
debug {
    TARGET = $$join(TARGET,,,d)
    FITNESSCOMMONLIB = $$join(FITNESSCOMMONLIB,,,d)
}
LIBS += -L../../build
LIBS += -l$$FITNESSCOMMONLIB
DEPENDPATH += . 
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += plugin

UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../../build/fitness

# Input
SOURCES += main.cpp
LIBS += -L../../../build -lfitness-common
