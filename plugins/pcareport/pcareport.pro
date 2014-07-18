TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_pcareport)
DESTDIR = ..
LIBS += -lm \
    -L./ \
    -lgsl \
    -lgslcblas

HEADERS += \
    ../../interfaces.h \
    ../../waveformdata.h \
    ../../spectrogramdata.h \
    ../../dataentrydialog.h \
    pcareport.h

SOURCES += \
    ../../waveformdata.cpp \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    pcareport.cpp
