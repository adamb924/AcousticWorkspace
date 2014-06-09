/*!
  \class PlotManagerDialog
  \ingroup MajorDialog
  \brief A dialog for managing the plots of the project

  The dialog provides a user interface so that the user can create or delete plots, and associate curve and/or spectogram data with those plots.
*/

#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include <QDialog>
#include <QList>
#include <QTreeWidget>
#include <QDropEvent>
#include <QtDebug>
#include <QAction>
#include <QMenu>

#include "datasourcetreewidget.h"
#include "plotviewwidget.h"
#include "plotviewtreewidget.h"

class SpectrogramData;
class PlotViewWidget;
class WaveformData;

class PlotManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlotManagerDialog(QList<PlotViewWidget*> *pv, const QList<WaveformData*> *wfd, const QList<SpectrogramData*> *spd, QWidget *parent = 0);

signals:
    //! \brief Emitted when the dialog is requesting that a new prosody be added
    void addProsody(PlotViewWidget *w, QString label);

public slots:
    //! \brief Add the \a waveform-th waveform to plot \a plot
    void addWaveform(int plot, int waveform);

    //! \brief Add the \a spectrogram-th spectrogram to plot \a plot
    void addSpectrogram(int plot, int spectrogram);

    //! \brief Prompt the user to enter a name for a new plot, and emit addProsody()
    void addPlot();

private:
    PlotViewTreeWidget *mPvt;
    DataSourceTreeWidget *mWaveformTree, *mSpectrogramTree;

    QList<PlotViewWidget*> *maProsodyViews;
    const QList<WaveformData*> *maWaveformData;
    const QList<SpectrogramData*> *maSpectrogramData;
};

#endif // PLOTMANAGER_H
