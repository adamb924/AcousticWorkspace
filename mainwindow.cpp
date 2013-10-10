#include "mainwindow.h"

#include "mdiarea.h"
#include <QDebug>
#include <QPluginLoader>
#include <QApplication>
#include "interfaces.h"
#include <QAction>
#include <QMenuBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadPlugins();

    mdiWindow = new MdiArea(this);

    setupMenus();

    this->setWindowTitle(tr("Acoustic Workspace"));
    this->setCentralWidget(mdiWindow);
}


MainWindow::~MainWindow()
{

}

void MainWindow::setupMenus()
{
    QAction *newAction = new QAction(tr("New sound window"),this);
    newAction->setShortcut(QKeySequence("Ctrl+Z"));
    connect(newAction,SIGNAL(triggered()),mdiWindow,SLOT(newSoundWindow()));

    QAction *newComparison = new QAction(tr("New comparison window"),this);
    newComparison->setShortcut(QKeySequence("Ctrl+C"));
    connect(newComparison,SIGNAL(triggered()),mdiWindow,SLOT(newComparisonWindow()));

    menuBar()->addAction(newAction);
    menuBar()->addAction(newComparison);

    QMenu *windows = new QMenu(tr("Windows"));
    menuBar()->addMenu(windows);
    windows->addAction(tr("Tile Windows"),mdiWindow,SLOT(tileSubWindows()));
    windows->addAction(tr("Cascade Windows"),mdiWindow,SLOT(cascadeSubWindows()));
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir;
    QStringList pluginFileNames;

    foreach (QObject *plugin, QPluginLoader::staticInstances())
	loadPlugin(plugin);

    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
	pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
	pluginsDir.cdUp();
	pluginsDir.cdUp();
	pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
	QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
	/*
    QLibrary *tmp = new QLibrary(pluginsDir.absoluteFilePath(fileName));
    qDebug() << tmp->load();
    qDebug() << tmp->errorString();
*/
	QObject *plugin = loader.instance();
	if (plugin) {
	    loadPlugin(plugin);
	    pluginFileNames += fileName;
	}
/*	else
	{
	    qDebug() << fileName << loader.errorString();
	}
*/
    }
}

void MainWindow::loadPlugin(QObject *plugin)
{
    AbstractWaveform2WaveformMeasure *wm = qobject_cast<AbstractWaveform2WaveformMeasure*>(plugin);
    if (wm)
	w2wPlugins << wm;

    AbstractWaveform2SpectrogramMeasure *sm = qobject_cast<AbstractWaveform2SpectrogramMeasure*>(plugin);
    if (sm)
	w2sPlugins << sm;

    AbstractSpectrogram2WaveformMeasure *sw = qobject_cast<AbstractSpectrogram2WaveformMeasure*>(plugin);
    if (sw)
	s2wPlugins << sw;

    AbstractSpectrogram2SpectrogramMeasure *ss = qobject_cast<AbstractSpectrogram2SpectrogramMeasure*>(plugin);
    if (ss)
	s2sPlugins << ss;
}
