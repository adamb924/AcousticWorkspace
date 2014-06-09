/*!
  \class SoundWidget
  \ingroup GUI
  \brief This widget is the immediate subchild of the MDI subwindows of the application that display projects.

  This is the widget for displaying projects. It is the child of the MDI subwindows to display projects. It holds all of the data from the project. The actual plot windows are not here, but in the PlotManager widget. This widget has the menus. It also provides the scripting environment.
*/

#ifndef SOUNDWIDGET_H
#define SOUNDWIDGET_H

#include <QMetaType>
#include <QWidget>
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

//Q_DECLARE_METATYPE(QList<WaveformData*>)
//Q_DECLARE_METATYPE(QList<SpectrogramData*>)

class SoundWidget : public QWidget
{
    Q_OBJECT

    friend class ComparisonWidget;

public:
    explicit SoundWidget(QWidget *parent = 0, MainWindow *wnd = 0);
    ~SoundWidget();

signals:
    //! \brief Emitted whenever the horizontal scale (e.g., sliders & buttons) changes
    void horizontalScaleUpdated(double left, double right);

    //! \brief This signal is for scripts to invoke, to allow them to get fresh data
    void requestScriptDataRefresh();

    //! \brief This signal indicates that data pertinent to the scripting environment has changed
    void scriptDataChanged();

public slots:
    //! \brief Reads a project file into memory
    void readFromFile(QString filename);

    //! \brief Reads a Praat TextGrid file into memory
    void readTextGridFromFile(QString fileName);

    //! \brief Closes the current sound
    void close();

    //! \brief Loads a sound into the project
    void loadSound(QString fileName);

    //! \brief Write the project to the current filename, or prompt the user for a filename if none exists.
    void save();

    //! \brief Write the project to \a filename
    void writeProjectToFile(QString filename);

    //! \brief Remove the \a index-th waveform, if \a index is a valid index
    void removeWaveform(int index);

    //! \brief Remove the \a index-th spectrogram, if \a index is a valid index
    void removeSpectrogram(int index);

private slots:
    //! \brief Prompts the user to select a file to be opened by loadSound()
    void importSound();

    //! \brief Launches a DataManagerDialog
    void launchDataManager();

    //! \brief Launches a RegressionDialog
    void newRegression();

    //! \brief Prompt the user to select a project to open, and calls readFromFile to read the project
    void openProject();

    //! \brief Launches a PlotManagerDialog
    void launchPlotManager();

    //! \brief Prompts the user for a new filename, and calls writeProjectToFile
    void saveAs();

    //! \brief Prompts the user to select a TextGrid file, and calls readTextGridFromFile
    void importTextGrid();

    //! \brief Adds the appropriate submenu to the annotations menu
    void addAnnotationMenu(IntervalAnnotation *annotation);

    //! \brief Adds \regression to the project
    void addRegression(RegressionModel *regression);

    //! \brief Adds the spectrogram to the project, if the project is in a focused window.
    void addSpectrogram(SpectrogramData *data);

    //! \brief Adds the waveform to the project, if the project is in a focused window.
    void addWaveform(WaveformData *data);

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
    MainWindow *mMainWnd;
    PlotDisplayAreaWidget *mPlotDisplay;

    QScriptEngine* mScriptEngine;

    QMenuBar *mMenuBar;

    QString mCurrentFilename;

    QMenu *mFileMenu, *mOptionsMenu, *mVisibilityMenu, *mRegressionMenu, *mAnnotationMenu, *mScriptingMenu;
    QAction *mImportSoundAction, *mCloseAction, *mOpenProjectAction, *mSaveProjectAction, *mSaveProjectAsAction, *mNewSoundAction;
    QAction *mNewRegressionAction, *mImportTextGridAction;
    QAction *mPlotManagerAction, *mDataManagerAction, *mRegressionAction;
    QList<QAction*> mVisibilityActions;

    QList<WaveformData*> maWaveformData;
    QList<SpectrogramData*> maSpectrogramData;
    QList<RegressionModel*> maRegressions;
    QList<IntervalAnnotation*> maIntervalAnnotations;
    QList<QMenu*> maRegressionMenus;
    QList<QMenu*> maAnnotationMenus;

    //! \brief Return the inner text of the next element named \a elementname in \a reader, or an empty string if there is none
    QString readXmlElement(QXmlStreamReader &reader, QString elementname);

    //! \brief Add actions to the menus
    void setupActions();

    //! \brief Set up menus for the widget
    void setupMenus();
};


#endif // SOUNDWIDGET_H
