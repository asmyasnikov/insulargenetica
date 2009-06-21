TEMPLATE = app
TARGET = insulargenetica
DEPENDPATH += .
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += console release
LIBS += -L../../build -linsulargenetica
# Input
SOURCES += main.cpp
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
