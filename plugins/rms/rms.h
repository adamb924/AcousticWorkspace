#ifndef RMS_H
#define RMS_H

#include <QObject>

#include <QStringList>
#include <QVariant>

#include "interfaces.h"

class WaveformData;

class RmsPlugin : public AbstractWaveform2WaveformMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractWaveform2WaveformMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractwaveform2waveformmeasure/1.0")

public:
    RmsPlugin();
    ~RmsPlugin()
    {
	if(times) { free(times); }
	if(values) { free(values); }
    }
    RmsPlugin* copy() const;

public slots:
    QString name() const;
    QStringList names() const;
    void settings(int i);
    void calculate(int i, WaveformData *data);

    void calculate(QString name, WaveformData *data);
    void setParameter(QString label, QVariant value);
    QString scriptName() const;

private:
    QStringList pluginnames;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;

    size_t nframes;
    double *times;
    double *values;
};

#endif
