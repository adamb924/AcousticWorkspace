#ifndef RMS_H
#define RMS_H

#include <QObject>

#include <QStringList>
#include <QVariant>

#include "interfaces.h"

class WaveformData;

class UnaryPlugin : public AbstractWaveform2WaveformMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractWaveform2WaveformMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractwaveform2spectrogrammeasure/1.0")

public:
    UnaryPlugin();
    ~UnaryPlugin();
    UnaryPlugin* copy() const;
public slots:
    QString name() const;
    QString scriptName() const;
    QStringList names() const;
    void settings(int i);
    void calculate(int index, WaveformData *data);
    void calculate(QString name, WaveformData *data);
    void setParameter(QString label, QVariant value);

private:
    WaveformData *sound;

    QStringList pluginnames;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;

signals:
    void waveformCreated(WaveformData *data);
};

#endif
