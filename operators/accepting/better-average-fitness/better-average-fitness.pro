TEMPLATE = lib
TARGET = genetic-better-average-fitness
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -lgenetic
# Input
HEADERS += CBetterAverageFitness.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/.ui
MOC_DIR = ./src.gen/.moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/accepting