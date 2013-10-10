/*!
  \class WaveformData
  \ingroup Data
  \brief A data class for periodic waveform data.

  This class subclasses QwtData, offering added tools that are helpful for storing spectrogram-like data.

  The class is a subclass of QObject so that WaveformData objects can be used by the scripting interface.
*/

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include <QObject>
#include <QtGlobal>
#include "qwt_data.h"
#include <QString>

class WaveformData : public QObject, public QwtData
{
    Q_OBJECT
public:
    //! \brief A bare-bones constructor. Needed for the copy-constructor
    WaveformData();

    //! \brief Construct the object from existing sources of data
    /*!
      \param name Name of the waveform
      \param x Pointer to the x-data
      \param y Pointer to the y-data
      \param nsam Number of samples in the waveform
      \param fs Sampling frequency of the waveform
    */
    WaveformData(QString name, double *x, double *y, size_t nsam, size_t fs);

    //! \brief Create a copy of the object. Performs a deep copy of the data structures
    WaveformData* copy() const;

public slots:
    //! \brief Return x-value at time index \a i. Reimplemented from QwtData
    double x(size_t i) const;

    //! \brief Return y-value at time index \a i. Reimplemented from QwtData
    double y(size_t i) const;

    //! \brief Return the number of samples in the data. Reimplemented from QwtData
    size_t size() const;

    //! \brief Return a pointer to the x-data
    double* xData() const;

    //! \brief Return a pointer to the y-data
    double* yData() const;

    //! \brief Set the pointer to the x-data
    /*!
      The pointer to the current x-data is deallocated if non-null.
      */
    void setXData(double *x);

    //! \brief Set the pointer to the y-data
    /*!
      The pointer to the current y-data is deallocated if non-null.
      */
    void setYData(double *y);

    //! \brief Set the x-data at time index \a index to \a x
    void setXDataAt(int index, double x);

    //! \brief Return the number of samples in the waveform
    size_t getNSamples() const;

    //! \brief Return the minimum time
    double tMin() const;

    //! \brief Return the maximum time
    double tMax() const;

    //! \brief Return the length of the waveform
    double length() const;

    //! \brief Return true if the start, end, and sampling frequency of this waveform matches that of \a other
    bool checkCongruentWith(const WaveformData *other) const;

    //! \brief Return the sampling frequency of the waveform
    double getSamplingFrequency() const;

    //! \brief Return the Nyquist frequency of the waveform
    double getNyquistFrequency() const;

    //! \brief Return the y-value at sample \a i
    double getYAtSample(quint32 i) const;

    //! \brief Return the (time) x-value at sample \a i
    double getTimeAtSample(quint32 i) const;

    //! \brief Return the sample index before \a time
    /*!
      If \a time is outside of the range, the first or last sample index is returned, as appropriate.
      */
    size_t getSampleFromTime(double time);

    //! \brief Return the name of the waveform
    QString name() const { return label; }

    //! \brief Return the name of the waveform stripped of whitespace
    QString safeName() const { return safeLabel; }

    //! \brief Set the name of the waveform
    void setName(QString n);

    //! \brief Return the bounding rectangle of the data. Reimplemented from QwtRasterData
    QwtDoubleRect boundingRect() const;

    //! \brief Calcuate minimum and maximum values of y-data
    void calculateMinMax();

private:
    QString label;
    QString safeLabel;
    size_t fs, nsam;
    double period;
    double len;
    double *data;
    double *times;
    double minimum;
    double maximum;
};

#endif // WAVEFORMDATA_H
