#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QList>
#include <QStringList>
#include <QVariant>

#include <qwt_series_data.h>

class WaveformData;
class SpectrogramData;

/*! \class AbstractMeasurement
    \ingroup Plugin
    \brief Base class for other abstract measurement classes

    This class provides the pure virtual function \a fn settings, which is used by all measurement plugins.
  */
class AbstractMeasurement: public QObject
{
    Q_OBJECT
public:
    //! \brief Display the settings dialog box for measurement \a i
    virtual void settings(int i) = 0;
};

/*! \class AbstractWaveform2WaveformMeasure
    \ingroup Plugin
    \brief Base class for other plugins that create new waveforms from old waveforms
  */
class AbstractWaveform2WaveformMeasure : public AbstractMeasurement
{
    Q_OBJECT
public:
    AbstractWaveform2WaveformMeasure() {}
    virtual ~AbstractWaveform2WaveformMeasure() {}
    virtual AbstractWaveform2WaveformMeasure* 	copy () const = 0;

    //! \brief Return the name of the plugin library
    virtual QString	name() const = 0;

    //! \brief Return the name the plugin should have for use in scripting
    virtual QString	scriptName() const = 0;

    //! \brief Return the list of names of the measures defined by the plugin
    virtual QStringList	names() const = 0;

    //! \brief Display the settings dialog box for measurement \a i
    virtual void settings(int i) = 0;

    //! \brief Change the setting with label \a label to \a value
    virtual void setParameter(QString label, QVariant value) = 0;

    //! \brief Return a list of pointers to WaveformData objects calculated by measurement \a i, from \a data
    virtual void calculate(int i, WaveformData *data) = 0;

    //! \brief A convenience overload to allow measures to be called by name
    virtual void calculate(QString name, WaveformData *data) = 0;

signals:
    void waveformCreated(WaveformData *data);
};

/*! \class AbstractWaveform2SpectrogramMeasure
    \ingroup Plugin
    \brief Base class for other plugins that create spectrograms from waveforms
  */
class AbstractWaveform2SpectrogramMeasure : public AbstractMeasurement
{
    Q_OBJECT
public:
    AbstractWaveform2SpectrogramMeasure() {}
    virtual ~AbstractWaveform2SpectrogramMeasure() {}
    virtual AbstractWaveform2SpectrogramMeasure* 	copy () const = 0;

public slots:
    //! \brief Return the name of the plugin library
    virtual QString	name() const = 0;

    //! \brief Return the name the plugin should have for use in scripting
    virtual QString	scriptName() const = 0;

    //! \brief Return the list of names of the measures defined by the plugin
    virtual QStringList	names() const = 0;

    //! \brief Display the settings dialog box for measurement \a i
    virtual void settings(int i) = 0;

    //! \brief Change the setting with label \a label to \a value
    virtual void setParameter(QString label, QVariant value) = 0;

    //! \brief Return a list of pointers to SpectrogramData objects calculated by measurement \a i, from \a data
    virtual void calculate(int i, WaveformData *data) = 0;

    //! \brief A convenience overload to allow measures to be called by name
    virtual void calculate(QString name, WaveformData *data) = 0;

signals:
    void spectrogramCreated(SpectrogramData *data);
};

/*! \class AbstractSpectrogram2WaveformMeasure
    \ingroup Plugin
    \brief Base class for other plugins that create waveforms from spectrograms
  */
class AbstractSpectrogram2WaveformMeasure : public AbstractMeasurement
{
    Q_OBJECT
public:
    AbstractSpectrogram2WaveformMeasure() {}
    virtual ~AbstractSpectrogram2WaveformMeasure() {}
    virtual AbstractSpectrogram2WaveformMeasure* 	copy () const = 0;

    //! \brief Return the name of the plugin library
    virtual QString	name() const = 0;

    //! \brief Return the name the plugin should have for use in scripting
    virtual QString	scriptName() const = 0;

    //! \brief Return the list of names of the measures defined by the plugin
    virtual QStringList	names() const = 0;

    //! \brief Display the settings dialog box for measurement \a i
    virtual void settings(int i) = 0;

    //! \brief Change the setting with label \a label to \a value
    virtual void setParameter(QString label, QVariant value) = 0;

    //! \brief Return a list of pointers to WaveformData objects calculated by measurement \a i, from \a data
    virtual void calculate(int i, SpectrogramData *data) = 0;

    //! \brief A convenience overload to allow measures to be called by name
    virtual void calculate(QString name, SpectrogramData *data) = 0;

signals:
    void waveformCreated(WaveformData *data);
};

/*! \class AbstractSpectrogram2SpectrogramMeasure
    \ingroup Plugin
    \brief Base class for other plugins that create new spectrograms from old spectrograms
  */
class AbstractSpectrogram2SpectrogramMeasure : public AbstractMeasurement
{
    Q_OBJECT
public:
    AbstractSpectrogram2SpectrogramMeasure() {}
    virtual ~AbstractSpectrogram2SpectrogramMeasure() {}
    virtual AbstractSpectrogram2SpectrogramMeasure* 	copy () const = 0;

    //! \brief Return the name of the plugin library
    virtual QString	name() const = 0;

    //! \brief Return the name the plugin should have for use in scripting
    virtual QString	scriptName() const = 0;

    //! \brief Return the list of names of the measures defined by the plugin
    virtual QStringList	names() const = 0;

    //! \brief Display the settings dialog box for measurement \a i
    virtual void settings(int i) = 0;

    //! \brief Change the setting with label \a label to \a value
    virtual void setParameter(QString label, QVariant value) = 0;

    //! \brief Return a list of pointers to SpectrogramData objects calculated by measurement \a i, from \a data
    virtual void calculate(int i, SpectrogramData *data) = 0;

    //! \brief A convenience overload to allow measures to be called by name
    virtual void calculate(QString name, SpectrogramData *data) = 0;

signals:
    void spectrogramCreated(SpectrogramData *data);
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(AbstractWaveform2WaveformMeasure,"acousticworkspace.qt.abstractwaveform2waveformmeasure/1.0")
Q_DECLARE_INTERFACE(AbstractWaveform2SpectrogramMeasure,"acousticworkspace.qt.abstractwaveform2spectrogrammeasure/1.0")
Q_DECLARE_INTERFACE(AbstractSpectrogram2WaveformMeasure,"acousticworkspace.qt.abstractspectrogram2waveformmeasure/1.0")
Q_DECLARE_INTERFACE(AbstractSpectrogram2SpectrogramMeasure,"acousticworkspace.qt.abstractspectrogram2spectrogrammeasure/1.0")
QT_END_NAMESPACE

#endif // INTERFACES_H
