TEMPLATE = lib
TARGET = genetic-two-dot-crossover
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -linsulargenetica
# Input
HEADERS += CTwoDotCrossover.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/reproduction
