# -------------------------------------------------
# Project created by QtCreator 2010-12-21T10:29:00
# -------------------------------------------------
TARGET = AcousticWorkspace
TEMPLATE = app
QT += script
SOURCES += main.cpp \
    mainwindow.cpp \
    dataentrywidget.cpp \
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
    mdiarea.cpp \
    datasourcetreewidget.cpp \
    indexedaction.cpp
HEADERS += mainwindow.h \
    interfaces.h \
    dataentrywidget.h \
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
    mdiarea.h
INCLUDEPATH += /Qwt-5.2.1/include
LIBS += -L./ \
    -lqwt5 \
    -llibsndfile-1 \
    -llibfftw3-3 \
    -lm \
    -lgsl \
    -lgslcblas
