TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_spectralchange)
DESTDIR = ..
LIBS += -lm \
    -L./ 

HEADERS += \
    ../../interfaces.h \
    ../../waveformdata.h \
    ../../spectrogramdata.h \
    ../../dataentrydialog.h \
    spectralchange.h

SOURCES += \
    ../../waveformdata.cpp \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    spectralchange.cpp