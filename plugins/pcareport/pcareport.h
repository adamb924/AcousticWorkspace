#ifndef RMS_H
#define RMS_H

#include <QObject>
#include <QList>
#include <QVariant>

#include "interfaces.h"

class WaveformData;
class SpectrogramData;

class PcaReportPlugin : public AbstractSpectrogram2WaveformMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractSpectrogram2WaveformMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractspectrogram2waveformmeasure/1.0")

public:
    PcaReportPlugin();
    ~PcaReportPlugin() {}
    PcaReportPlugin* copy() const;

public slots:
    QString name() const;
    QStringList names() const;
    void settings(int i);
    void calculate(int index, SpectrogramData *data);
    void calculate(QString name, SpectrogramData *data);
    void setParameter(QString label, QVariant value);
    QString scriptName() const;

private:
    QStringList pluginnames;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;

signals:
    void waveformCreated(WaveformData *data);
};

#endif
