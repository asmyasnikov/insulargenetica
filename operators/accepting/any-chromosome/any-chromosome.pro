TEMPLATE = lib
TARGET = genetic-any-chromosome
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -linsulargenetica
# Input
HEADERS += CAnyChromosome.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/accepting
