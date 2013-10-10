/*! \class PlotViewTreeWidget
    \ingroup SupportWidgets
    \brief A tree widget class for displaying plots and plot items hierarchically

    This dialog is a subclassed QTreeWidget. This is to enable it to provide MIME data when items are dragged from the widget, and to provide a context menu.

    The class is used in PlotManagerDialog.
  */

#ifndef PROSODYVIEWTREE_H
#define PROSODYVIEWTREE_H

#include <QDialog>
#include <QList>
#include <QTreeWidget>
#include <QtDebug>
#include <QAction>
#include <QMenu>

#include "datasourcetreewidget.h"
#include "plotviewwidget.h"
#include "plotviewtreewidget.h"

class SpectrogramData;
class PlotViewWidget;
class WaveformData;

class PlotViewTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PlotViewTreeWidget(QList<PlotViewWidget*> *pv, QWidget *parent);

    //! \brief Fill the tree with plots and their plot items
    void populate();

    //! \brief Indicates at the widget provides text/plain data. Reimplemented from QWidget
    QStringList mimeTypes() const;

    //! \brief Process drop events (see detailed description). Reimplemented from QWidget
    /*!
      If the drop event comes from outside of the widget, the origin is decoded from the MIME data, and the curve or spectrogram is added to the plot.

      If the drop event is an internal move, ... something remains to be implemented.
      */
    void dropEvent(QDropEvent *event);

    //! \brief Allows drag events unless the mouse is over a second-level item. Reimplemented from QWidget
    void dragMoveEvent ( QDragMoveEvent *event );

    //! \brief Provides a context menu and options
    void contextMenuEvent ( QContextMenuEvent * event );

    //! \brief Provides MIME data for internal moves
    /*!
      This seems as if it's yet to be implemented.
      */
    QMimeData * mimeData ( const QList<QTreeWidgetItem *> items ) const;

    //! \brief Returns true if the \a index-th second-level item in the \a plotIndex-th first-level item refers to spectrogram, false if it's a curve. If false, it changes \a itemIndex so that it a zero-indexed reference to the appropriate curve.
    bool isSpectrogram(int plotIndex, int &index);

    QAction *removeAction, *settingsAction, *secondaryAxisAction, *moveToOtherAxisAction;

private slots:
    //! \brief Launches a CurveSettingsDialog or SpectrogramSettingsDialog, or none, as appropriate.
    void doubleClick(QTreeWidgetItem * item, int column);

    //! \brief Determines which item is intended, and emits removePlot()
    void remove();

    //! \brief Determines which item is intended, and launches a CurveSettingsDialog or SpectrogramSettingsDialog
    void settings();

    //! \brief Determines which item is intended, and emits setSecondaryAxis()
    void secondary(bool does);

    //! \brief Determines which item is intended, and toggles the item between the primary and secondary axis
    void moveToOtherAxis();

signals:
    //! \brief Emitted to indicate the user wants to indicate that plot \a plotIndex has a secondary axis (or not)
    /*!
      Never used in the code.
      */
    void setSecondaryAxis(int plotIndex, bool hasSecondaryAxis);

    //! \brief Request to remove plot \a plotIndex
    void removePlot(int plotIndex);

    //! \brief Request to add waveform \a waveform to plot \a plot
    void addWaveform(int plot, int waveform);

    //! \brief Request to add spectrogram \a spectrogram to plot \a plot
    void addSpectrogram(int plot, int spectrogram);

private:
    QList<PlotViewWidget*> *aProsodyViews;
};

#endif // PROSODYVIEWTREE_H
