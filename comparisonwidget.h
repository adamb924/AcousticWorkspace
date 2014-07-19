/*!
  \class ComparisonWidget
  \ingroup Display
  \brief A widget that displays a comparison of two or more sound files.

  The class has a single \a primary WaveformData object, and a list of \a secondary objects. These are deep copies of data passed to the constructor. There is a single primary object, but there can be any number of secondary objects.

  The user interface is provided by a PlotDisplayWidget object.
*/

#ifndef COMPARISONWIDGET_H
#define COMPARISONWIDGET_H

#include <QWidget>

class Sound;
class SoundWidget;
class WaveformData;
class QContextMenuEvent;
class QwtPointArrayData;
class PlotDisplayAreaWidget;
class IntervalAnnotation;
class QColor;

class ComparisonWidget : public QWidget
{
    Q_OBJECT
public:
    //! \brief Prompts the user for input, and creates the window
    ComparisonWidget(const Sound *primary, const QList<Sound*> *sounds, QWidget *parent = 0);
    ~ComparisonWidget();

private:
    //! \brief  Stretch the samples of the primary curves linearly
    /*!
      In general it would not make sense to stretch the primary curves. This function stretches the time domain of the primary curves, based on the selected interval annotation, so that the locations of samples coincide with interval boundaries.
      */
    void warpPrimaryCurvesLinear();

    //! \brief  Stretch the samples of the secondary curves linearly
    /*!
      This function stretches the time domain of the secondary curves, based on the interval annotation of the primary curves and the interval annotation of the secondary curves, so that the secondary intervals are stretched to the primary intervals.
      */
    void warpSecondaryCurvesLinear(int index);

    //! \brief  Stretch the samples of the primary curves based on accumulated Furui 1986 change
    /*!
      Warps the time domain of the primary curves so that time (within an interval) corresponds to a constant accumulation of spectral change.
      */
    void warpPrimaryCurvesAccumulated();

    //! \brief  Stretch the samples of the secondary curves based on accumulated Furui 1986 change
    /*!
      Warps the time domain of the secondary curves so that time (within an interval) corresponds to a constant accumulation of spectral change. The final times are stretched to match the times of the primary interval.
      */
    void warpSecondaryCurvesAccumulated(int index);

    //! \brief Adds plots to the PlotDisplayWidget object for each comparison
    void drawCurves();

    //! \brief Overloaded function from QWidget. Provides a context menu to add a secondary sound.
    void contextMenuEvent(QContextMenuEvent *e);

    //! \brief Return a name for the comparison window
    QString createWindowTitle() const;

    const Sound *mPrimary;
    const QList<Sound*> *mSounds;

    QList<QColor> colors;

    int mWarpAlgorithm, mPrimaryChangeMetric;
    QList<int> mSecondaryChangeMetrics;

    PlotDisplayAreaWidget *mDisplayWidget;

    QList<WaveformData*> mPrimaryCurves;
    QList< QList<WaveformData*> > mSecondaryCurves;

    IntervalAnnotation *mPrimaryInterval;
    QList< IntervalAnnotation* > mSecondaryIntervals;


private slots:
    //! \brief Prompts user to add a secondary sound to the comparison.
    void addSecondary();

};

#endif // COMPARISONWIDGET_H
