TEMPLATE = lib
TARGET = genetic-best-with-best
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -lgenetic
# Input
HEADERS += CBestWithBest.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/grouping