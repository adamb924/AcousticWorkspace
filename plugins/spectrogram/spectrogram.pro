TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_spectrogram)
DESTDIR = ..
LIBS += -lm \
    -L./ \
    -lfftw3

HEADERS += \
    ../../interfaces.h \
    ../../waveformdata.h \
    ../../spectrogramdata.h \
    ../../dataentrydialog.h \
    spectrogram.h

SOURCES += \
    ../../waveformdata.cpp \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    spectrogram.cpp
