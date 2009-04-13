TEMPLATE = lib
TARGET = genetic-random-selection
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -lgenetic
# Input
HEADERS += CRandomSelection.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/.ui
MOC_DIR = ./src.gen/.moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/selection