TEMPLATE = app
TARGET = soccer_team_a

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
    src/robot.h \
    src/timer.h \
    src/game.h \
    src/goalie.h \
    src/striker.h \
    src/opponent.h \
    src/strategy.h \
	src/vectorfield.h \
    src/pathfinder.h
SOURCES += \
    src/main.cpp \
    src/robot.cpp \
    src/game.cpp \
    src/goalie.cpp \
    src/striker.cpp \
    src/opponent.cpp \
    src/strategy.cpp \
	src/vectorfield.cpp \
    src/pathfinder.cpp
		
		
##############
## Documentation
##############
# custom target 'doc' in *.pro file
dox.target = doc
dox.commands = doxygen Doxyfile
dox.depends = FORCE

# somewhere else in the *.pro file
QMAKE_EXTRA_TARGETS += dox

OTHER_FILES += \
    src/legacy_code.txt
