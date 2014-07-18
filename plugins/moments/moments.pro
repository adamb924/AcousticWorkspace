TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_moments)
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
    moments.h

SOURCES += \
    ../../waveformdata.cpp \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    moments.cpp
