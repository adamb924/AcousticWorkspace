/*!
  \class PlotDisplayAreaWidget
  \ingroup GUI
  \brief A widget that manages multiple PlotViewWidget objects.

  The class manages several PlotViewWidget objects, which are kept in \a aPlotViewWidgets. From a UI perspective, the class also has the layout code, and also manages the time slider and time-navigation buttons.
*/

#ifndef PLOTDISPLAYWIDGET_H
#define PLOTDISPLAYWIDGET_H

#include <QWidget>

class PlotViewWidget;
class QVBoxLayout;
class QwtSlider;
class IntervalDisplayWidget;

class PlotDisplayAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotDisplayAreaWidget(QWidget *parent = 0);
    ~PlotDisplayAreaWidget();

    //! \brief Return a pointer to the list of PlotViewWidget objects
    QList<PlotViewWidget*>* plotViews() { return &maPlotViewWidgets; }

    //! \brief Return a pointer to the list of annotation display (IntervalDisplayWidget) objects
    QList<IntervalDisplayWidget*>* annotations() { return &maAnnotations; }

    //! \brief Set the minimum and maximum times of the sound to \a min and \a max, respectively.
    /*!
      Note that this does not just set the left and right boundaries of the display, but sets the minimum and maximum values for the entire sound (for display purposes; the underlying data are not changed). When a user clicks "All" the view zooms to these values.
      */
    void setTimeMinMax(double min, double max);

    //! \brief Set the \a left and \a right edges of the time scale.
    void setTimeAxes(double left, double right);

    //! \brief Sets the position of the slider, based on the current time-zoom.
    void setSliderFromWindow();

    //! \brief Return the maximum time of the project
    double getTMax() { return mTMax; }

    //! \brief Return the minimum time of the project
    double getTMin() { return mTMin; }

    //! \brief Return the time at the left edge of the display
    double getLeftPos() { return mLeftPos; }

    //! \brief Return the time at the right edge of the display
    double getRightPos() { return mRightPos; }


public slots:
    //! \brief Adds \a pr to the list of plot views, establishes signal/slot connections, and calls drawVerticalLayout() to update the layout
    void addPlotView(PlotViewWidget *pr, QString name = QString());

    //! \brief Adds \a annotation to the project, and calls drawVerticalLayout() to update the layout
    void addAnnotation(IntervalDisplayWidget *annotation);

    //! \brief (Re)creates the vertical layout containing the plots
    void drawVerticalLayout();

private:
    QVBoxLayout *mVerticalLayout;
    QWidget *mScrollAreaWidget;
    QwtSlider *mSlider;
    int mProsodyHeight, mProsodySpacing, mBorder;
    double mTMin, mTMax, mLeftPos, mRightPos, mWindowWidth;

    QStringList maProsodyNames;
    QList<IntervalDisplayWidget*> maAnnotations;
    QList<PlotViewWidget*> maPlotViewWidgets;

    //! \brief Updates the geometry of the plot area when the widget is resized. Reimplemented from QWidget.
    void resizeEvent(QResizeEvent *event);

signals:
    //! \brief Emitted when the \a left and \a right values of the time axis are changed
    void timeAxisChanged(double left, double right);

private slots:
    //! \brief Zoom to display the entire time range.
    void all();

    //! \brief Zoom in to an area half the width of the current view.
    void in();

    //! \brief Zoom out to an area twice the width of the current view.
    void out();

    //! \brief Zoom to the selection. Currently not implemented.
    void sel();

    //! \brief Changes the time scale based on the new slider position \a value
    void updateTimeScaleFromSlider(double value);

};

#endif // PLOTDISPLAYWIDGET_H
