TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_centroid)
DESTDIR = ..
LIBS += -lm \
    -L./ 

HEADERS += \
    ../../interfaces.h \
    ../../waveformdata.h \
    ../../spectrogramdata.h \
    ../../dataentrydialog.h \
    centroid.h

SOURCES += \
    ../../waveformdata.cpp \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    centroid.cpp
