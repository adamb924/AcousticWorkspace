# -------------------------------------------------
# Project created by QtCreator 2010-12-21T10:29:00
# -------------------------------------------------
TARGET = AcousticWorkspace
TEMPLATE = app
QT += script widgets core gui
CONFIG += qwt
SOURCES += main.cpp \
    mainwindow.cpp \
    plotmanagerdialog.cpp \
    plotviewtreewidget.cpp \
    spectrogramsettingsdialog.cpp \
    soundwidget.cpp \
    pluginviewtreewidget.cpp \
    comparisonwidget.cpp \
    plotdisplayareawidget.cpp \
    regressiondialog.cpp \
    regression.cpp \
    regressionitems.cpp \
    intervalannotation.cpp \
    intervaldisplaywidget.cpp \
    plotviewwidget.cpp \
    spectrogramdata.cpp \
    waveformdata.cpp \
    curvesettingsdialog.cpp \
    datamanagerdialog.cpp \
    datasourcetreewidget.cpp \
    indexedaction.cpp \
    dataentrydialog.cpp \
    sound.cpp
HEADERS += mainwindow.h \
    interfaces.h \
    plotmanagerdialog.h \
    plotviewtreewidget.h \
    textdisplaydialog.h \
    spectrogramsettingsdialog.h \
    soundwidget.h \
    pluginviewtreewidget.h \
    comparisonwidget.h \
    plotdisplayareawidget.h \
    regressiondialog.h \
    regression.h \
    intervalannotation.h \
    intervaldisplaywidget.h \
    plotviewwidget.h \
    indexedaction.h \
    spectrogramdata.h \
    waveformdata.h \
    datasourcetreewidget.h \
    curvesettingsdialog.h \
    datamanagerdialog.h \
    dataentrydialog.h \
    sound.h
LIBS += -L./ \
    -llibsndfile-1 \
    -lfftw3 \
    -lm \
    -lgsl

FORMS += \
    mainwindow.ui \
    soundwidget.ui
