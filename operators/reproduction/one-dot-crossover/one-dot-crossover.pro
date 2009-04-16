TEMPLATE = lib
TARGET = genetic-one-dot-crossover
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -lgenetic
# Input
HEADERS += COneDotCrossover.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/reproduction