TEMPLATE = app
TARGET = soccer_basics

include(/DIST/lehre/lab_roso/tech/usr/include/settings.pri)
#include(/DIST/lehre/lab_roso/tech/usr_sim/include/settings.pri)

DEPENDPATH += src
INCLUDEPATH += src

DESTDIR = bin
MOC_DIR = .moc
OBJECTS_DIR = .obj
#QT += core
#QT -= gui
CONFIG +=  debug

# Input
HEADERS += \ 
    src/drive_to_pos.h \
    src/constants.h \
    src/soccer_tests.h
SOURCES += \
    src/soccer_basics.cpp \
    src/drive_to_pos.cpp \
    src/soccer_tests.cpp
		
		
##############
## Documentation
##############
# custom target 'doc' in *.pro file
dox.target = doc
dox.commands = doxygen Doxyfile
dox.depends = FORCE

# somewhere else in the *.pro file
QMAKE_EXTRA_TARGETS += dox











