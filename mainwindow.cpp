#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPluginLoader>
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>

#include "sound.h"
#include "soundwidget.h"
#include "comparisonwidget.h"
#include "interfaces.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadPlugins();

    connect(ui->actionOpen_Sound, SIGNAL(triggered()), this, SLOT(openSound()));
}


MainWindow::~MainWindow()
{
    qDeleteAll(mSounds);
}

void MainWindow::openSound()
{
    QString fileName;
    fileName= QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("Project files (*.xml)"));
    if(!fileName.isNull())
    {
        Sound * newSound = new Sound(fileName);
        if( newSound->readState() == Sound::Success )
        {
            mSounds.append( newSound );
        }
    }
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

    QObject *plugin = loader.instance();
	if (plugin) {
	    loadPlugin(plugin);
	    pluginFileNames += fileName;
	}
//    else
//    {
//        if( fileName.right(3) == "dll" )
//        qDebug() << fileName << loader.errorString();
//    }
    }
}

void MainWindow::loadPlugin(QObject *plugin)
{
    AbstractWaveform2WaveformMeasure *wm = qobject_cast<AbstractWaveform2WaveformMeasure*>(plugin);
    if (wm)
	mW2wPlugins << wm;

    AbstractWaveform2SpectrogramMeasure *sm = qobject_cast<AbstractWaveform2SpectrogramMeasure*>(plugin);
    if (sm)
	mW2sPlugins << sm;

    AbstractSpectrogram2WaveformMeasure *sw = qobject_cast<AbstractSpectrogram2WaveformMeasure*>(plugin);
    if (sw)
	mS2wPlugins << sw;

    AbstractSpectrogram2SpectrogramMeasure *ss = qobject_cast<AbstractSpectrogram2SpectrogramMeasure*>(plugin);
    if (ss)
	mS2sPlugins << ss;
}

void MainWindow::newSoundWindow(Sound *snd)
{
    SoundWidget *tmp = new SoundWidget(snd, &mW2wPlugins,&mW2sPlugins,&mS2wPlugins,&mS2sPlugins,this);
    ui->mdiArea->addSubWindow(tmp);
    ui->mdiArea->subWindowList().last()->setAttribute(Qt::WA_DeleteOnClose);
    tmp->show();
}

void MainWindow::newComparisonWindow()
{
    QList<SoundWidget*> *sounds = soundWindows();
    if( sounds->count() < 2 )
    {
    QMessageBox::critical(this,tr("Error"),tr("You need at least two different sounds to make a comparison."));
    return;
    }

    QStringList items;
    for(int i=0; i< sounds->count(); i++)
    items << sounds->at(i)->windowTitle();

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Acoustic Workspace"),tr("Choose the primary sound for the comparison"), items, 0, false, &ok);
    if(!ok) { return; }
    int first = items.indexOf(item);


    ComparisonWidget *tmp = new ComparisonWidget(sounds->at(first), sounds);
    ui->mdiArea->addSubWindow(tmp);
    ui->mdiArea->subWindowList().last()->setAttribute(Qt::WA_DeleteOnClose);
    tmp->show();
}

QList<SoundWidget*>* MainWindow::soundWindows()
{
    QList<SoundWidget*> *sounds = new QList<SoundWidget*>;
    for(int i=0; i< ui->mdiArea->subWindowList().count(); i++)
    {
    SoundWidget* tmp = qobject_cast<SoundWidget*>(ui->mdiArea->subWindowList().at(i)->widget());
    if( tmp != 0 && ui->mdiArea->subWindowList().at(i)->windowTitle() != tr("Acoustic Workspace") ) // this is a cheap way to tell if the window is uninitialized
        *sounds << tmp;
    }
    return sounds;
}
