TEMPLATE = lib
TARGET = genetic-outbreeding
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
LIBS += -L../../../build -lgenetic
# Input
HEADERS += COutBriding.h
SOURCES += main.cpp
CONFIG += plugin release
UI_DIR = ./src.gen/.ui
MOC_DIR = ./src.gen/.moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ./../../../build/grouping