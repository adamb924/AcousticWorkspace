/*!
  \class PlotViewWidget
  \ingroup GUI
  \brief A widget that displays individual plots.

  The class provides for multiple curves and multiple spectrograms. A context menu provides various functionality (accessing data, changing settings).
*/

#ifndef PROSODYINTERFACE_H
#define PROSODYINTERFACE_H

#include <QWidget>
#include <QList>

#include "qwt_plot_curve.h"
#include "qwt_plot_spectrogram.h"

#include "waveformdata.h"
#include "spectrogramdata.h"

class QMouseEvent;
class QHBoxLayout;

class PlotViewWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(Type)
public:
    PlotViewWidget(QString n, QWidget *parent = 0);
    ~PlotViewWidget();

    //! \brief Return the name of the plot
    QString name() const;

    //! \brief Set the name of the plot to \a n
    void setName(QString n);

    //! \brief Return a suggested size for the widget. Reimplemented from QWidget
    QSize sizeHint() const;

    //! \brief Return the list of curve plot items
    QList<QwtPlotCurve*>* curves();

    //! \brief Return the list of spectrogram plot items
    QList<QwtPlotSpectrogram*>* spectrograms();

    //! \brief Return a pointer to the data associated with the \a i-th curve
    WaveformData* curveData(int i);

    //! \brief Return a pointer to the data associated with the \a i-th spectrogram
    SpectrogramData* spectrogramData(int i);

    //! \brief Return a pointer to the plot object
    QwtPlot* plot();

    //! \brief Return true if the plot has a secondary axis, otherwise return false
    bool hasSecondaryAxis();

    //! \brief Set the height of the plot to \a height
    void setHeight(int height);

    //! \brief Switch the \a index-th curve between the primary and secondary axis
    void toggleCurveAxisAssociation(int index);

    //! \brief Add \a curveData to the current plot
    /*!
      \param curveData The curve data to be added to the plot
      \param secondary If true the curve is plotted on the secondary axis
      \param col The color for the plot curve
      */
    void addCurveData(WaveformData *curveData, bool secondary = false, QColor col = Qt::blue);

    //! \brief Add \a spectrogramData to the current plot
    void addSpectrogramData(SpectrogramData *spectrogramData);

    //! \brief Remove the \a i-th plot item (curve or spectrogram)
    void removeItemAt(int i);

    //! \brief Display a context menu. Reimplemented from QWidget
    void contextMenuEvent ( QContextMenuEvent * event );

    //! \brief Change the cursor to a crosshairs if it is over the plot area. Reimplemented from QWidget
    /*!
      The implementation also contains some defunct code for resizing the plots.
      */
    void mouseMoveEvent ( QMouseEvent * event );

    //! \brief Launch the CurveSettingsDialog for the first curve, or the SpectrogramSettingsDialog for the first spectrogram if there is no curve
    void mouseDoubleClickEvent ( QMouseEvent *event );

public slots:
    //! \brief Set the left and right bounds of the plot to \a left and \a right
    void setHorizontalAxis(double left, double right);

    //! \brief Set whether the plot has a secondary axis
    void setHasSecondaryAxis(bool hasSecondaryAxis);

    //! \brief Launch a TextDisplayDialog with the sample times of the \a index-th curve
    void showCurveTimes(int index);

    //! \brief Launch a TextDisplayDialog with the times and values of the \a index-th curve
    void showCurveTimesData(int index);

    //! \brief Launch a TextDisplayDialog with the sample times of the \a index-th spectrogram
    void showSpectrogramTimes(int index);

    //! \brief Launch a TextDisplayDialog with the frequency bins of the \a index-th spectrogram
    void showSpectrogramFrequencies(int index);

    //! \brief Launch a CurveSettingsDialog for the \a index-th curve
    void launchCurveSettings(int index);

    //! \brief Launch a SpectrogramSettingsDialog for the \a index-th spectrogram
    void launchSpectrogramSettings(int index);

protected:
    int widgetHeight;
    QHBoxLayout *hlayout;

    bool secondaryAxis;
    QString label;
    QwtPlot *qwtPlot;

    QList<WaveformData*> aWaveformData;
    QList<SpectrogramData*> aSpectrogramData;

    QList<QwtPlotCurve*> aCurves;
    QList<QwtPlotSpectrogram*> aSpectrograms;
};

#endif // PROSODYINTERFACE_H
