TEMPLATE = lib
TARGET = genetic
DEPENDPATH += . src ../include
INCLUDEPATH += .
LANGUAGE = C++
QT -= gui
CONFIG += plugin release
# Input
HEADERS += src/CGeneticAlgorithm.h
HEADERS += src/CFitnessSafeThreadFunction.h
HEADERS += ../include/CGeneticController.h
HEADERS += ../include/CChromosome.h
HEADERS += ../include/CPopulation.h
HEADERS += ../include/CFitnessHelper.h
HEADERS += ../include/CElitarSelection.h
HEADERS += ../include/CRandomMutation.h
SOURCES += src/CGeneticAlgorithm.cpp
SOURCES += src/CGeneticController.cpp
SOURCES += src/CChromosome.cpp
SOURCES += src/CPopulation.cpp
SOURCES += src/CFitnessHelper.cpp
SOURCES += src/CElitarSelection.cpp
SOURCES += src/CRandomMutation.cpp

UI_DIR = ./src.gen/.ui
MOC_DIR = ./src.gen/.moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../build
