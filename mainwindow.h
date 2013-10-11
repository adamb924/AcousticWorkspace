/*!
  \class MainWindow
  \ingroup GUI
  \brief A main window of the application area.

  This class provides the main window for the application, and also the interface for accessing plugins.

From the UI perspective, the important member of the MainWindow class is the MdiArea object, which provides the the multiple document interface.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class QVBoxLayout;
class ProsodyViewWidget;
class QAction;
class Sound;
class QwtPlot;
class QStringList;
class QResizeEvent;
class SpectrogramData;
class PlotViewWidget;
class WaveformData;
class AbstractWaveformMeasure;
class AbstractWaveform2WaveformMeasure;
class AbstractWaveform2SpectrogramMeasure;
class AbstractSpectrogram2WaveformMeasure;
class AbstractSpectrogram2SpectrogramMeasure;
class QXmlStreamReader;


#include <QList>
#include <QDir>

class MdiArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //! \brief Returns a pointer to the MdiArea object (i.e., the MDI area)
    inline MdiArea* mdi() const { return mdiWindow; }

    //! \brief Sets up the menus for the window
    void setupMenus();

    //! \brief Returns a pointer to a list of pointers to the plugins that make waveforms from waveforms
    inline QList<AbstractWaveform2WaveformMeasure*>* w2w() { return &w2wPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make spectrograms from waveforms
    inline QList<AbstractWaveform2SpectrogramMeasure*>* w2s() { return &w2sPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make waveforms from spectrograms
    inline QList<AbstractSpectrogram2WaveformMeasure*>* s2w() { return &s2wPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make spectrograms from spectrograms
    inline QList<AbstractSpectrogram2SpectrogramMeasure*>* s2s() { return &s2sPlugins; }

private:
    //! \brief Reads the plugins from the application's plugins folder, invoking loadPlugin for each one
    void loadPlugins();

    //! \brief Adds the passed plugin to the appropriate array, depending on the successful outcome of qobject_cast
    void loadPlugin(QObject *plugin);

    QList<AbstractWaveform2WaveformMeasure*> w2wPlugins;
    QList<AbstractWaveform2SpectrogramMeasure*> w2sPlugins;
    QList<AbstractSpectrogram2WaveformMeasure*> s2wPlugins;
    QList<AbstractSpectrogram2SpectrogramMeasure*> s2sPlugins;

    QList<QAction*> pluginOptionActions;
    MdiArea *mdiWindow;
};

#endif // MAINWINDOW_H
