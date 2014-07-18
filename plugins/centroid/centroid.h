#ifndef CENTROID_H
#define CENTROID_H

#include <QObject>

#include <QStringList>
#include <QVariant>

#include "interfaces.h"

class WaveformData;
class SpectrogramData;

class CentroidPlugin : public AbstractSpectrogram2WaveformMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractSpectrogram2WaveformMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractspectrogram2waveformmeasure/1.0")

public:
    CentroidPlugin();
    ~CentroidPlugin();
    CentroidPlugin* copy() const;

public slots:
    QString name() const;
    QString scriptName() const;
    QStringList names() const;
    void settings(int i);
    void setParameter(QString label, QVariant value);
    void calculate(int index, SpectrogramData *data);
    void calculate(QString name, SpectrogramData *data);

private:
    QStringList pluginnames;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;

signals:
    void waveformCreated(WaveformData *data);
};

#endif
