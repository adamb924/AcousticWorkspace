#ifndef CEPSTRUM_SPECTROGRAM_H
#define CEPSTRUM_SPECTROGRAM_H

#include <QObject>

#include <QStringList>
#include <QVariant>

#include "interfaces.h"

class SpectrogramData;

class CepstrumSpectrogramPlugin : public AbstractSpectrogram2SpectrogramMeasure
{
    Q_OBJECT
    Q_INTERFACES(AbstractSpectrogram2SpectrogramMeasure)
    Q_PLUGIN_METADATA(IID "acousticworkspace.qt.abstractspectrogram2spectrogrammeasure/1.0")

public:
    CepstrumSpectrogramPlugin();
    ~CepstrumSpectrogramPlugin() {}
    CepstrumSpectrogramPlugin* copy() const;

    QString name() const;
    QStringList names() const;
    void settings(int i);
    void calculate(int index, SpectrogramData *data);

    QString scriptName() const;
    void calculate(QString name, SpectrogramData *data);
    void setParameter(QString label, QVariant value);

private:
    QStringList pluginnames;

    QStringList settingsLabels;
    QList<QVariant> settingsValues;

signals:
    void spectrogramCreated(SpectrogramData *data);
};

#endif
