TEMPLATE = lib
TARGET = genetic-random-grouping
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -linsulargenetica
# Input
HEADERS += CRandomGrouping.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/grouping
