DEFINES += MUTATE_CHILDS
DEFINES += USE_STOHASTIC_MUTATION
#DEFINES += DONT_USE_MUTATION

debug_and_release {
    CONFIG -= debug_and_release
}
debug_and_release_target {
    CONFIG -= debug_and_release_target
}
release {
    debug {
        CONFIG -= debug
    }
}
debug {
    release {
        CONFIG -= release
    }
}
TEMPLATE = lib
TARGET = insulargenetica
debug {
    TARGET = $$join(TARGET,,,d)
}
LANGUAGE = C++
QT -= gui
CONFIG += plugin
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
CONFIG -= asp
asp {
    DESTDIR = ../../ASPDBEditor/build
}else{
    DESTDIR = ./build
}
DEPENDPATH += . \
    include \
    kernel \
    operators\accepting \
    operators\grouping \
    operators\mutation \
    operators\reproduction \
    operators\selection
INCLUDEPATH += .
HEADERS += include/CChromosome.h \
    include/CFitnessHelper.h \
    include/CGeneticController.h \
    include/CPopulation.h \
    include/export.h \
    kernel/CFitnessSafeThreadFunction.h \
    kernel/CGeneticAlgorithm.h \
    operators/accepting/CAnyChromosome.h \
    operators/accepting/CBestFitness.h \
    operators/accepting/CBetterAverageFitness.h \
    operators/grouping/CAllWithAll.h \
    operators/grouping/CBestWithAll.h \
    operators/grouping/CBestWithBest.h \
    operators/grouping/CInBreeding.h \
    operators/grouping/COutBreeding.h \
    operators/grouping/CRandomGrouping.h \
    operators/mutation/CFullMutation.h \
    operators/mutation/COneDotMutation.h \
    operators/mutation/CRandomMutation.h \
    operators/mutation/CTwoDotMutation.h \
    operators/reproduction/COneDotCrossover.h \
    operators/reproduction/CTwoDotCrossover.h \
    operators/reproduction/CUnifiedCrossover.h \
    operators/selection/CElitarSelection.h \
    operators/selection/CRandomSelection.h \
    operators/selection/CRouletteSelection.h
SOURCES += kernel/CChromosome.cpp \
    kernel/CFitnessHelper.cpp \
    kernel/CGeneticAlgorithm.cpp \
    kernel/CGeneticController.cpp \
    kernel/CPopulation.cpp \
    operators/selection/CElitarSelection.cpp \
    operators/mutation/CRandomMutation.cpp
