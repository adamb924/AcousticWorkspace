/*!
  \class WaveformData
  \ingroup Data
  \brief A data class for periodic waveform data.

  This class subclasses QwtPointArrayData, offering added tools that are helpful for storing spectrogram-like data.

  The class is a subclass of QObject so that WaveformData objects can be used by the scripting interface.
*/

#ifndef WAVEFORMDATA_H
#define WAVEFORMDATA_H

#include <QObject>
#include <qwt/qwt_point_data.h>

class QString;

class WaveformData : public QObject, public QwtPointArrayData
{
    Q_OBJECT
public:
    //! \brief Construct the object from existing sources of data
    /*!
      \param name Name of the waveform
      \param x Pointer to the x-data
      \param y Pointer to the y-data
      \param nsam Number of samples in the waveform
      \param fs Sampling frequency of the waveform
    */
    WaveformData(QString name, double *x, double *y, size_t nsam, size_t fs);

    //! \brief Copy constructor. Performs a deep copy of the data structures
    WaveformData(const WaveformData& other);

public slots:
    QPointF sample (size_t i) const;

    //! \brief Return the number of samples in the data. Reimplemented from QwtPointArrayData
    size_t size() const;

    //! \brief Set the pointer to the x-data
    /*!
      The pointer to the current x-data is deallocated if non-null.
      */
    void setXData(double *x);

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
    QRectF boundingRect() const;

    //! \brief Calcuate minimum and maximum values of y-data
    void calculateMinMax();

private:
    QString label;
    QString safeLabel;
    size_t fs;
    double period;
    double len;
    double *times;
    double minimum;
    double maximum;
};

#endif // WAVEFORMDATA_H
