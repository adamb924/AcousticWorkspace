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
class Sound;


#include <QList>
#include <QDir>

class SoundWidget;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //! \brief Returns a pointer to a list of pointers to the plugins that make waveforms from waveforms
    inline QList<AbstractWaveform2WaveformMeasure*>* w2w() { return &mW2wPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make spectrograms from waveforms
    inline QList<AbstractWaveform2SpectrogramMeasure*>* w2s() { return &mW2sPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make waveforms from spectrograms
    inline QList<AbstractSpectrogram2WaveformMeasure*>* s2w() { return &mS2wPlugins; }

    //! \brief Returns a pointer to a list of pointers to the plugins that make spectrograms from spectrograms
    inline QList<AbstractSpectrogram2SpectrogramMeasure*>* s2s() { return &mS2sPlugins; }

private slots:
    void openSound();

    void importSoundFile();

    //! \brief Create a new sound-display child window
    void newSoundWindow(Sound * snd);

    //! \brief Create a new sound-comparison child window
    void newComparisonWindow();

private:
    //! \brief Return a pointer to a list of pointers to SoundWidget objects.
    QList<SoundWidget*>* soundWindows();

    //! \brief Create a Sound from a wav file
    void loadSound(const QString & fileName);

    //! \brief Reads the plugins from the application's plugins folder, invoking loadPlugin for each one
    void loadPlugins();

    //! \brief Adds the passed plugin to the appropriate array, depending on the successful outcome of qobject_cast
    void loadPlugin(QObject *plugin);

    QList<AbstractWaveform2WaveformMeasure*> mW2wPlugins;
    QList<AbstractWaveform2SpectrogramMeasure*> mW2sPlugins;
    QList<AbstractSpectrogram2WaveformMeasure*> mS2wPlugins;
    QList<AbstractSpectrogram2SpectrogramMeasure*> mS2sPlugins;

    Ui::MainWindow *ui;

    QList<QAction*> mPluginOptionActions;
    QList<Sound*> mSounds;
};

#endif // MAINWINDOW_H
