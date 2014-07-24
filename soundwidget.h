/*!
  \class SoundWidget
  \ingroup GUI
  \brief This widget is the immediate subchild of the MDI subwindows of the application that display projects.

  This is the widget for displaying projects. It is the child of the MDI subwindows to display projects. It holds all of the data from the project. The actual plot windows are not here, but in the PlotManager widget. This widget has the menus. It also provides the scripting environment.
*/

#ifndef SOUNDWIDGET_H
#define SOUNDWIDGET_H

#include <QMainWindow>
#include <qwt_slider.h>

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
class AbstractWaveform2WaveformMeasure;
class AbstractWaveform2SpectrogramMeasure;
class AbstractSpectrogram2WaveformMeasure;
class AbstractSpectrogram2SpectrogramMeasure;
class QXmlStreamReader;
class QMenu;
class QMenuBar;
class MainWindow;
class PlotDisplayAreaWidget;
class RegressionModel;
class IntervalAnnotation;
class QScriptEngine;
class Sound;

namespace Ui {
    class SoundWidget;
}

class SoundWidget : public QMainWindow
{
    Q_OBJECT

public:
    SoundWidget(Sound * snd, QList<AbstractWaveform2WaveformMeasure*> *w2w, QList<AbstractWaveform2SpectrogramMeasure*> *w2s, QList<AbstractSpectrogram2WaveformMeasure*> *s2w, QList<AbstractSpectrogram2SpectrogramMeasure*> *s2s, QWidget *parent = 0);

    ~SoundWidget();

    Sound * sound();

signals:
    //! \brief Emitted whenever the horizontal scale (e.g., sliders & buttons) changes
    void horizontalScaleUpdated(double left, double right);

    //! \brief This signal is for scripts to invoke, to allow them to get fresh data
    void requestScriptDataRefresh();

    //! \brief This signal indicates that data pertinent to the scripting environment has changed
    void scriptDataChanged();

public slots:
    //! \brief Remove the \a index-th waveform, if \a index is a valid index
    void removeWaveform(int index);

    //! \brief Remove the \a index-th spectrogram, if \a index is a valid index
    void removeSpectrogram(int index);

private slots:
    //! \brief Launches a DataManagerDialog
    void launchDataManager();

    //! \brief Launches a RegressionDialog
    void newRegression();

    //! \brief Launches a PlotManagerDialog
    void launchPlotManager();

    //! \brief Prompts the user to select a TextGrid file, and calls readTextGridFromFile
    void importTextGrid();

    //! \brief Adds the appropriate submenu to the annotations menu
    void addAnnotationMenu(IntervalAnnotation *annotation);

    //! \brief Processes \action depending on the action's data, to produce appropriate behavior items in the regression menu
    void regressionMenuAction(QAction *action);

    //! \brief Processes \action depending on the action's data, to produce appropriate behavior items in the annotation menu
    void annotationMenuAction(QAction *action);

    //! \brief Prompts the user to select a script file to execute, and send the contents of the file to executeCode
    void runScript();

    //! \brief Sets up the scripting context and executes the code
    void executeCode(QString code);

    //! \brief Sets up the scripting environment variables
    void setupScriptEnvironment();

    //! \brief Set up the necessary scripting variables and signal/slot connections
    void setupScripting();

private:
    Ui::SoundWidget *ui;

    Sound * mSound;

    QScriptEngine* mScriptEngine;

    QMenuBar *mMenuBar;

    QMenu *mFileMenu, *mOptionsMenu, *mVisibilityMenu, *mRegressionMenu, *mScriptingMenu;
    QAction *mImportSoundAction, *mCloseAction, *mOpenProjectAction, *mSaveProjectAction, *mSaveProjectAsAction, *mNewSoundAction;
    QAction *mNewRegressionAction, *mImportTextGridAction;
    QAction *mPlotManagerAction, *mDataManagerAction, *mRegressionAction;
    QList<QAction*> mVisibilityActions;

    QList<AbstractWaveform2WaveformMeasure*> *mW2wPlugins;
    QList<AbstractWaveform2SpectrogramMeasure*> *mW2sPlugins;
    QList<AbstractSpectrogram2WaveformMeasure*> *mS2wPlugins;
    QList<AbstractSpectrogram2SpectrogramMeasure*> *mS2sPlugins;

    QList<QMenu*> maRegressionMenus;
    QList<QMenu*> maAnnotationMenus;

    void setupLayout();

    //! \brief Add actions to the menus
    void setupActions();
};


#endif // SOUNDWIDGET_H
