#include "datamanagerdialog.h"

#include "waveformdata.h"
#include "spectrogramdata.h"
#include "curvesettingsdialog.h"
#include "interfaces.h"
#include "pluginviewtreewidget.h"
#include "datasourcetreewidget.h"

#include <QtDebug>
#include <QtWidgets>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QGridLayout>

DataManagerDialog::DataManagerDialog(QList<AbstractWaveform2WaveformMeasure*> *w2wPlugins, QList<AbstractWaveform2SpectrogramMeasure*> *w2sPlugins, QList<AbstractSpectrogram2WaveformMeasure*> *s2wPlugins, QList<AbstractSpectrogram2SpectrogramMeasure*> *s2sPlugins, QList<WaveformData*> *wfd, QList<SpectrogramData*> *spd, QWidget *parent) :
	QDialog(parent)
{
    setSizeGripEnabled(true);

    maWaveformData = wfd;
    maSpectrogramData = spd;

    this->mW2wPlugins = w2wPlugins;
    this->mW2sPlugins = w2sPlugins;
    this->mS2wPlugins = s2wPlugins;
    this->mS2sPlugins = s2sPlugins;

    QGridLayout *glayout = new QGridLayout(this);

    mWaveformTree = new DataSourceTreeWidget("waveform",this); mWaveformTree->setHeaderHidden(true);
    mSpectrogramTree = new DataSourceTreeWidget("spectrogram",this); mSpectrogramTree->setHeaderHidden(true);

    mW2wTree = new PluginViewTreeWidget("waveform"); mW2wTree->setHeaderHidden(true);
    mW2sTree = new PluginViewTreeWidget("waveform"); mW2sTree->setHeaderHidden(true);
    mS2wTree = new PluginViewTreeWidget("spectrogram"); mS2wTree->setHeaderHidden(true);
    mS2sTree = new PluginViewTreeWidget("spectrogram"); mS2sTree->setHeaderHidden(true);

    glayout->addWidget(new QLabel(tr("Waveforms")),0,0);
    glayout->addWidget(mWaveformTree,1,0);
    glayout->addWidget(new QLabel(tr("Spectrograms")),2,0);
    glayout->addWidget(mSpectrogramTree,3,0);

    glayout->addWidget(new QLabel(tr("Waveform-to-Waveform")),0,1);
    glayout->addWidget(mW2wTree,1,1);
    glayout->addWidget(new QLabel(tr("Waveform-to-Spectrogram")),0,2);
    glayout->addWidget(mW2sTree,1,2);

    glayout->addWidget(new QLabel(tr("Spectrogram-to-Waveform")),2,1);
    glayout->addWidget(mS2wTree,3,1);
    glayout->addWidget(new QLabel(tr("Spectrogram-to-Spectrogram")),2,2);
    glayout->addWidget(mS2sTree,3,2);

    populateWaveformTree();
    populateSpectrogramTree();

    populateW2wTree();
    populateW2sTree();
    populateS2wTree();
    populateS2sTree();

    mW2wTree->expandAll();
    mW2sTree->expandAll();
    mS2wTree->expandAll();
    mS2sTree->expandAll();

    connect(mW2wTree,SIGNAL(dropped(int,int,int)),this,SLOT(w2wDrop(int,int,int)));
    connect(mW2sTree,SIGNAL(dropped(int,int,int)),this,SLOT(w2sDrop(int,int,int)));
    connect(mS2wTree,SIGNAL(dropped(int,int,int)),this,SLOT(s2wDrop(int,int,int)));
    connect(mS2sTree,SIGNAL(dropped(int,int,int)),this,SLOT(s2sDrop(int,int,int)));

    connect(mWaveformTree,SIGNAL(removeItemSignal(int)),this,SIGNAL(removeWaveform(int)));
    connect(mWaveformTree,SIGNAL(removeItemSignal(int)),this,SLOT(populateWaveformTree())); // and redraw
    connect(mSpectrogramTree,SIGNAL(removeItemSignal(int)),this,SIGNAL(removeSpectrogram(int)));
    connect(mSpectrogramTree,SIGNAL(removeItemSignal(int)),this,SLOT(populateSpectrogramTree())); // and redraw

    connect(mWaveformTree,SIGNAL(renameItemSignal(int)),this,SLOT(renameWaveform(int)));
    connect(mSpectrogramTree,SIGNAL(renameItemSignal(int)),this,SLOT(renameSpectrogram(int)));

    this->setLayout(glayout);
    this->setWindowTitle("Acoustic Workspace Data Manager");

    //    qDebug() << "Constructor this" << this->windowTitle();
    //    w2wDrop(0,0,0);
}

void DataManagerDialog::populateW2wTree()
{
    mW2wTree->clear();
    for(int i=0; i<mW2wPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(mW2wPlugins->at(i)->name()));
	mW2wTree->addTopLevelItem(tmp);
	for(int j=0; j<mW2wPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(mW2wPlugins->at(i)->names().at(j))));
	if(mW2wPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateW2sTree()
{
    mW2sTree->clear();
    for(int i=0; i<mW2sPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(mW2sPlugins->at(i)->name()));
	mW2sTree->addTopLevelItem(tmp);
	for(int j=0; j<mW2sPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(mW2sPlugins->at(i)->names().at(j))));
	if(mW2sPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateS2wTree()
{
    mS2wTree->clear();
    for(int i=0; i<mS2wPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(mS2wPlugins->at(i)->name()));
	mS2wTree->addTopLevelItem(tmp);
	for(int j=0; j<mS2wPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(mS2wPlugins->at(i)->names().at(j))));
	if(mS2wPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateS2sTree()
{
    mS2sTree->clear();
    for(int i=0; i<mS2sPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(mS2sPlugins->at(i)->name()));
	mS2sTree->addTopLevelItem(tmp);
	for(int j=0; j<mS2sPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(mS2sPlugins->at(i)->names().at(j))));
	if(mS2sPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateWaveformTree()
{
    mWaveformTree->clear();
    for(int i=0; i<maWaveformData->count(); i++)
	mWaveformTree->addTopLevelItem(new QTreeWidgetItem(QStringList(maWaveformData->at(i)->name())));
}

void DataManagerDialog::populateSpectrogramTree()
{
    mSpectrogramTree->clear();
    for(int i=0; i<maSpectrogramData->count(); i++)
	mSpectrogramTree->addTopLevelItem(new QTreeWidgetItem(QStringList(maSpectrogramData->at(i)->name())));
}

void DataManagerDialog::w2wDrop(int from, int toPlugin, int toSubplugin)
{
    mW2wPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < mW2wPlugins->at(toPlugin)->names().length())
	mW2wPlugins->at(toPlugin)->calculate(toSubplugin,maWaveformData->at(from));
    else if(toSubplugin == mW2wPlugins->at(toPlugin)->names().length())
	for(int i=0; i<mW2wPlugins->at(toPlugin)->names().length(); i++)
	    mW2wPlugins->at(toPlugin)->calculate(i,maWaveformData->at(from));
    populateWaveformTree();
}

void DataManagerDialog::w2sDrop(int from, int toPlugin, int toSubplugin)
{
    mW2sPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < mW2sPlugins->at(toPlugin)->names().length())
	mW2sPlugins->at(toPlugin)->calculate(toSubplugin,maWaveformData->at(from));
    else if(toSubplugin == mW2sPlugins->at(toPlugin)->names().length())
	for(int i=0; i<mW2sPlugins->at(toPlugin)->names().length(); i++)
	    mW2sPlugins->at(toPlugin)->calculate(i,maWaveformData->at(from));
    populateSpectrogramTree();
}

void DataManagerDialog::s2wDrop(int from, int toPlugin, int toSubplugin)
{
    mS2wPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < mS2wPlugins->at(toPlugin)->names().length())
	mS2wPlugins->at(toPlugin)->calculate(toSubplugin,maSpectrogramData->at(from));
    else if(toSubplugin == mS2wPlugins->at(toPlugin)->names().length())
	for(int i=0; i<mS2wPlugins->at(toPlugin)->names().length(); i++)
	    mS2wPlugins->at(toPlugin)->calculate(i,maSpectrogramData->at(from));
    populateWaveformTree();
}

void DataManagerDialog::s2sDrop(int from, int toPlugin, int toSubplugin)
{
    mS2sPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < mS2sPlugins->at(toPlugin)->names().length())
	mS2sPlugins->at(toPlugin)->calculate(toSubplugin,maSpectrogramData->at(from));
    else if(toSubplugin == mS2sPlugins->at(toPlugin)->names().length())
	for(int i=0; i<mS2sPlugins->at(toPlugin)->names().length(); i++)
	    mS2sPlugins->at(toPlugin)->calculate(i,maSpectrogramData->at(from));
    populateSpectrogramTree();
}

/*
void DataManagerDialog::removeWaveform(int index)
{
    delete aWaveformData->takeAt(index);
    delete waveformTree->topLevelItem(index);
}

void DataManagerDialog::removeSpectrogram(int index)
{
    delete aSpectrogramData->takeAt(index);
    delete spectrogramTree->topLevelItem(index);
}
*/

void DataManagerDialog::renameWaveform(int index)
{
    bool ok;

    QString text = QInputDialog::getText(this, tr("Enter a new label"),
					 tr("Enter a new label"), QLineEdit::Normal,
					 maWaveformData->at(index)->name(), &ok);

    if (ok && !text.isEmpty())
    {
	maWaveformData->at(index)->setName(text);
	mWaveformTree->topLevelItem(index)->setText(0,text);
    }
}

void DataManagerDialog::renameSpectrogram(int index)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter a new label"),
					 tr("Enter a new label"), QLineEdit::Normal,
					 maSpectrogramData->at(index)->name(), &ok);
    if (ok && !text.isEmpty())
    {
	maSpectrogramData->at(index)->setName(text);
	mSpectrogramTree->topLevelItem(index)->setText(0,text);
    }
}
