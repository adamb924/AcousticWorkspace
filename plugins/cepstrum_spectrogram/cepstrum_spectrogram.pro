TEMPLATE = lib
CONFIG += plugin qwt
INCLUDEPATH += ../..
TARGET = $$qtLibraryTarget(aw_cepstrum_spectrogram)
DESTDIR = ..
LIBS += -lm \
    -L./ \
    -lfftw3

HEADERS += \
    ../../interfaces.h \
    ../../spectrogramdata.h \
    ../../dataentrydialog.h \
    cepstrum_spectrogram.h

SOURCES += \
    ../../spectrogramdata.cpp \
    ../../dataentrydialog.cpp \
    cepstrum_spectrogram.cpp
