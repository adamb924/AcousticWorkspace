/*!
  \class IntervalDisplayWidget
  \ingroup Annotation
  \brief A widget for displaying an interval annotation tier.

  This class provides the main window for the application, and also the interface for accessing plugins.
*/

#ifndef INTERVALDISPLAYWIDGET_H
#define INTERVALDISPLAYWIDGET_H

#include <QWidget>

class IntervalAnnotation;
class PlotViewWidget;

class IntervalDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Initializes the widget, given the interval tier \a annotation and a prosody interface (display widget) \a prosody.
    IntervalDisplayWidget(const IntervalAnnotation *annotation, PlotViewWidget *prosody, QWidget *parent = 0);

    //! \brief The obligatory overloaded functio
    QSize sizeHint() const;
protected:
    //! \brief Draws the interval tier, based on the time scale in the prosody interface pointed to by the member \a prosody.
    void paintEvent(QPaintEvent *event);

    const IntervalAnnotation *mAnnotation;
    /// @todo The only reason this variable is here is so that it can get the position of some associated PlotViewWidget. There is surely a better way to do this.
    PlotViewWidget *mProsody;
};

#endif // INTERVALDISPLAYWIDGET_H
