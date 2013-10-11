/*!
  \class SpectrogramData
  \ingroup Data
  \brief A data class for spectrogram-like data.

  This class subclasses QwtRasterData, offering added tools that are helpful for storing spectrogram-like data.

  The class is a subclass of QObject so that SpectrogramData objects can be used by the scripting interface.
*/

#ifndef SPECTROGRAMDATA_H
#define SPECTROGRAMDATA_H

#include <qwt/qwt_raster_data.h>

#include "fftw3.h"
#include <math.h>

#include <QtDebug>
#include <QTime>

class SpectrogramData: public QObject, public QwtRasterData
{
    Q_OBJECT
public:
    //! \brief A bare-bones constructor
    SpectrogramData();

    //! \brief Construct a SpectrogramData object from existing data sources
    /*!
      All pointers passed to the constructor must be filled with real data. There is no error checking.
      \param n Name of the spectrogram
      \param data Pointer to the spectrogram data
      \param times Pointer to a vector of \a nFrames time values
      \param nFrames Number of time steps in the spectrogram
      \param frequencies Pointer to a vector of \a nFreqBins frequency bin values
      \param nFreqBins Number of frequency bins in the spectrogram
      \param spec_min Minimum value of the spectrogram data
      \param spec_max Maximum value of the spectrogram data
      \param windowLength Window length of the spectrogram
      \param timeStep Time step of the spectrogram
    */
    SpectrogramData(QString n, double *data, double *times, size_t nFrames, double *frequencies, size_t nFreqBins, double spec_min, double spec_max , double windowLength, double timeStep);

    ~SpectrogramData();

public slots:
    //! \brief Return the bounding rectangle of the data. Reimplemented from QwtRasterData
    QRectF boundingRect() const;

    //! \brief Create a copy of the object. Performs a deep copy of the data structures
    virtual SpectrogramData* copy() const;

    //! \brief Return the value at at time \a t, frequency \a f
    double dataAt(quint32 t, quint32 f) const;

    //! \brief Return the value at \a i, where the data are treated as a 1D array
    double flatdata(quint32 i) const;

    //! \brief Return a pointer to the spectrogram data
    double* pdata() const;

    //! \brief Return a pointer to the frequency vector
    double* pfrequencies() const;

    //! \brief Check if \a t is within the time range of the spectrogram
    bool inTimeRange(double t) const;

    //! \brief Check if \a f is within the time range of the spectrogram
    bool inFrequencyRange(double f) const;

    //! \brief Return the index of the time step above \a t
    quint32 timeStepAbove(double t) const;

    //! \brief Return the index of the time step below \a t
    quint32 timeStepBelow(double t) const;

    //! \brief Return the index of the frequency bin above \a t
    quint32 frequencyBinAbove(double t) const;

    //! \brief Return the index of the frequency bin below \a t
    quint32 frequencyBinBelow(double t) const;

    //! \brief Return the interpolated value at time \a x, frequency \a y. Reimplemented from QwtRasterData
    virtual double value(double x, double y) const;

    //! \brief Return the bilinearly interpolated value at time \a x, frequency \a y
    double bilinearInterpolation(double x, double y) const;

    //! \brief Return the name of the spectrogram
    QString name() const;

    //! \brief Return the name of the spectrogram stripped of whitespace
    QString safeName() const;

    //! \brief Set the name of the spectrogram
    void setName(QString n);

    //! \brief Return the window length of the spectrogram
    double getWindowLength() const;

    //! \brief Return the time step of the spectrogram
    double getTimeStep() const;

    //! \brief Return the number of time steps in the spectrogram
    quint32 getNTimeSteps() const;

    //! \brief Return the number of frequency bins in the spectrogram
    quint32 getNFrequencyBins() const;

    //! \brief Return the time at time step \a i
    double getTimeFromIndex(int i) const;

    //! \brief Return the frequency at bin \a i
    double getFrequencyFromIndex(int i) const;

private:
    QString label;
    QString safeLabel;

    double *data;
    double *times;
    double *frequencies;

    double spec_min;
    double spec_max;
    double windowLength;
    double timeStep;
    quint32 windowLengthInSamples;
    quint32 timeStepInSamples;

    quint32 nFrames, nFreqBins;
};

#endif // SPECTROGRAMDATA_H
