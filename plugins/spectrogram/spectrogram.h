#ifndef RMS_H
#define RMS_H

#include <QObject>
#include <QStringList>
#include <QVariant>

#include "interfaces.h"

class WaveformData;

class SpectrogramPlugin : public AbstractWaveform2SpectrogramMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractWaveform2SpectrogramMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractwaveform2spectrogrammeasure/1.0")

public:
    SpectrogramPlugin();
    ~SpectrogramPlugin()
    {
	if(window) { free(window); }
    }
    SpectrogramPlugin* copy() const;

public slots:
    QString name() const;
    QString scriptName() const;
    QStringList names() const;
    void settings(int i);
    void setParameter(QString label, QVariant value);
    void calculate(int index, WaveformData *data);
    void calculate(QString name, WaveformData *data);

private:
    QStringList pluginNames;
    double *window;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;
};

#endif
