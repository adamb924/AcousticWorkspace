#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QString>
#include <QList>

class QXmlStreamReader;

class WaveformData;
class SpectrogramData;
class RegressionModel;
class IntervalAnnotation;

class Sound : public QObject
{
    Q_OBJECT
public:
    enum ReadState { Success, Error, NoAttempt };

    Sound(const QString & filename, QObject * parent = 0);
    Sound(WaveformData *sound, QObject * parent = 0);
    ~Sound();

    Sound::ReadState readState() const;
    QString name() const;

    QList<WaveformData*> * waveformData();
    QList<SpectrogramData*> * spectrogramData();

    const QList<WaveformData*> * waveformData() const;
    const QList<SpectrogramData*> * spectrogramData() const;

    const QList<IntervalAnnotation*> * intervals() const;

    void writeProjectToFile(const QString &filename);
    void readTextGridFromFile(const QString &fileName);

public slots:

    //! \brief Adds the spectrogram to the project, if the project is in a focused window.
    void addSpectrogram(SpectrogramData *data);

    //! \brief Adds the waveform to the project, if the project is in a focused window.
    void addWaveform(WaveformData *data);

    //! \brief Adds \regression to the project
    void addRegression(RegressionModel *regression);


signals:
    //! \brief This signal indicates that data pertinent to the scripting environment has changed
    void scriptDataChanged();

private:
    QString mFilename;
    Sound::ReadState mReadState;
    QList<WaveformData*> maWaveformData;
    QList<SpectrogramData*> maSpectrogramData;
    QList<RegressionModel*> maRegressions;
    QList<IntervalAnnotation*> maIntervalAnnotations;

    void readFromFile(const QString & filename);
    QString readXmlElement(QXmlStreamReader &reader, QString elementname);
};

#endif // SOUND_H
