TARGET = gui-insulargenetica
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

DEPENDPATH += .
INCLUDEPATH += .
LANGUAGE = C++
CONFIG += release
LIBS += -L../../build -linsulargenetica
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
