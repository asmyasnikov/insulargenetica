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
TARGET = gui-insulargenetica
INSULARGENETICALIB = insulargenetica
debug {
    TARGET = $$join(TARGET,,,d)
    INSULARGENETICALIB = $$join(INSULARGENETICALIB,,,d)
}
TEMPLATE = app
SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

DEPENDPATH += .
INCLUDEPATH += .
LANGUAGE = C++
LIBS += -L../../build
LIBS += -l$$INSULARGENETICALIB
UI_DIR = ./src.gen/ui
MOC_DIR = ./src.gen/moc
OBJECTS_DIR = ./src.gen/obj
DESTDIR = ../../build
