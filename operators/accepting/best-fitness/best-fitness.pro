TEMPLATE = lib
TARGET = genetic-best-fitness
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -linsulargenetica
# Input
HEADERS += CBestFitness.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/accepting
