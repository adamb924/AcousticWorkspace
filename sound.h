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
    ~Sound();

    Sound::ReadState readState() const;

signals:
    //! \brief This signal indicates that data pertinent to the scripting environment has changed
    void scriptDataChanged();

private:
    QString mCurrentFilename;
    Sound::ReadState mReadState;
    QList<WaveformData*> maWaveformData;
    QList<SpectrogramData*> maSpectrogramData;
    QList<RegressionModel*> maRegressions;
    QList<IntervalAnnotation*> maIntervalAnnotations;

    void readFromFile(const QString & filename);
    QString readXmlElement(QXmlStreamReader &reader, QString elementname);
    void addRegression(RegressionModel *regression);
};

#endif // SOUND_H
