TEMPLATE = lib
TARGET = rastrigin2invert
DEPENDPATH += . 
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += plugin release

UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../../build/fitness

# Input
SOURCES += main.cpp
LIBS += -L../../../build -lfitness-common
