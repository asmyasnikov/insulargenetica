TEMPLATE = lib
TARGET = fitness-common
DEPENDPATH += . src
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += release plugin
LIBS += -L../../build -lgenetic
# Input
HEADERS += ../include/CDeJong.h 
HEADERS += ../include/CGriewank.h 
HEADERS += ../include/CRastrigin.h
SOURCES += src/CDeJong.cpp 
SOURCES += src/CGriewank.cpp 
SOURCES += src/CRastrigin.cpp
UI_DIR = ./src.gen/.ui
MOC_DIR = ./src.gen/.moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
