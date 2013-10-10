#include "datamanagerdialog.h"

#include "waveformdata.h"
#include "spectrogramdata.h"
#include "curvesettingsdialog.h"
#include "interfaces.h"
#include "pluginviewtreewidget.h"
#include "datasourcetreewidget.h"

#include <QtDebug>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QGridLayout>

DataManagerDialog::DataManagerDialog(QList<AbstractWaveform2WaveformMeasure*> *w2wPlugins, QList<AbstractWaveform2SpectrogramMeasure*> *w2sPlugins, QList<AbstractSpectrogram2WaveformMeasure*> *s2wPlugins, QList<AbstractSpectrogram2SpectrogramMeasure*> *s2sPlugins, QList<WaveformData*> *wfd, QList<SpectrogramData*> *spd, QWidget *parent) :
	QDialog(parent)
{
    setSizeGripEnabled(true);

    aWaveformData = wfd;
    aSpectrogramData = spd;

    this->w2wPlugins = w2wPlugins;
    this->w2sPlugins = w2sPlugins;
    this->s2wPlugins = s2wPlugins;
    this->s2sPlugins = s2sPlugins;

    QGridLayout *glayout = new QGridLayout(this);

    waveformTree = new DataSourceTreeWidget("waveform",this); waveformTree->setHeaderHidden(true);
    spectrogramTree = new DataSourceTreeWidget("spectrogram",this); spectrogramTree->setHeaderHidden(true);

    w2wTree = new PluginViewTreeWidget("waveform"); w2wTree->setHeaderHidden(true);
    w2sTree = new PluginViewTreeWidget("waveform"); w2sTree->setHeaderHidden(true);
    s2wTree = new PluginViewTreeWidget("spectrogram"); s2wTree->setHeaderHidden(true);
    s2sTree = new PluginViewTreeWidget("spectrogram"); s2sTree->setHeaderHidden(true);

    glayout->addWidget(new QLabel(tr("Waveforms")),0,0);
    glayout->addWidget(waveformTree,1,0);
    glayout->addWidget(new QLabel(tr("Spectrograms")),2,0);
    glayout->addWidget(spectrogramTree,3,0);

    glayout->addWidget(new QLabel(tr("Waveform-to-Waveform")),0,1);
    glayout->addWidget(w2wTree,1,1);
    glayout->addWidget(new QLabel(tr("Waveform-to-Spectrogram")),0,2);
    glayout->addWidget(w2sTree,1,2);

    glayout->addWidget(new QLabel(tr("Spectrogram-to-Waveform")),2,1);
    glayout->addWidget(s2wTree,3,1);
    glayout->addWidget(new QLabel(tr("Spectrogram-to-Spectrogram")),2,2);
    glayout->addWidget(s2sTree,3,2);

    populateWaveformTree();
    populateSpectrogramTree();

    populateW2wTree();
    populateW2sTree();
    populateS2wTree();
    populateS2sTree();

    w2wTree->expandAll();
    w2sTree->expandAll();
    s2wTree->expandAll();
    s2sTree->expandAll();

    connect(w2wTree,SIGNAL(dropped(int,int,int)),this,SLOT(w2wDrop(int,int,int)));
    connect(w2sTree,SIGNAL(dropped(int,int,int)),this,SLOT(w2sDrop(int,int,int)));
    connect(s2wTree,SIGNAL(dropped(int,int,int)),this,SLOT(s2wDrop(int,int,int)));
    connect(s2sTree,SIGNAL(dropped(int,int,int)),this,SLOT(s2sDrop(int,int,int)));

    connect(waveformTree,SIGNAL(removeItemSignal(int)),this,SIGNAL(removeWaveform(int)));
    connect(waveformTree,SIGNAL(removeItemSignal(int)),this,SLOT(populateWaveformTree())); // and redraw
    connect(spectrogramTree,SIGNAL(removeItemSignal(int)),this,SIGNAL(removeSpectrogram(int)));
    connect(spectrogramTree,SIGNAL(removeItemSignal(int)),this,SLOT(populateSpectrogramTree())); // and redraw

    connect(waveformTree,SIGNAL(renameItemSignal(int)),this,SLOT(renameWaveform(int)));
    connect(spectrogramTree,SIGNAL(renameItemSignal(int)),this,SLOT(renameSpectrogram(int)));

    this->setLayout(glayout);
    this->setWindowTitle("Acoustic Workspace Data Manager");

    //    qDebug() << "Constructor this" << this->windowTitle();
    //    w2wDrop(0,0,0);
}

void DataManagerDialog::populateW2wTree()
{
    w2wTree->clear();
    for(int i=0; i<w2wPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(w2wPlugins->at(i)->name()));
	w2wTree->addTopLevelItem(tmp);
	for(int j=0; j<w2wPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(w2wPlugins->at(i)->names().at(j))));
	if(w2wPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateW2sTree()
{
    w2sTree->clear();
    for(int i=0; i<w2sPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(w2sPlugins->at(i)->name()));
	w2sTree->addTopLevelItem(tmp);
	for(int j=0; j<w2sPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(w2sPlugins->at(i)->names().at(j))));
	if(w2sPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateS2wTree()
{
    s2wTree->clear();
    for(int i=0; i<s2wPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(s2wPlugins->at(i)->name()));
	s2wTree->addTopLevelItem(tmp);
	for(int j=0; j<s2wPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(s2wPlugins->at(i)->names().at(j))));
	if(s2wPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateS2sTree()
{
    s2sTree->clear();
    for(int i=0; i<s2sPlugins->count(); i++)
    {
	QTreeWidgetItem *tmp = new QTreeWidgetItem(QStringList(s2sPlugins->at(i)->name()));
	s2sTree->addTopLevelItem(tmp);
	for(int j=0; j<s2sPlugins->at(i)->names().count(); j++)
	    tmp->addChild(new QTreeWidgetItem(QStringList(s2sPlugins->at(i)->names().at(j))));
	if(s2sPlugins->at(i)->names().count()>1)
	    tmp->addChild(new QTreeWidgetItem(QStringList(tr("<Run all>"))));
    }
}

void DataManagerDialog::populateWaveformTree()
{
    waveformTree->clear();
    for(int i=0; i<aWaveformData->count(); i++)
	waveformTree->addTopLevelItem(new QTreeWidgetItem(QStringList(aWaveformData->at(i)->name())));
}

void DataManagerDialog::populateSpectrogramTree()
{
    spectrogramTree->clear();
    for(int i=0; i<aSpectrogramData->count(); i++)
	spectrogramTree->addTopLevelItem(new QTreeWidgetItem(QStringList(aSpectrogramData->at(i)->name())));
}

void DataManagerDialog::w2wDrop(int from, int toPlugin, int toSubplugin)
{
    w2wPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < w2wPlugins->at(toPlugin)->names().length())
	w2wPlugins->at(toPlugin)->calculate(toSubplugin,aWaveformData->at(from));
    else if(toSubplugin == w2wPlugins->at(toPlugin)->names().length())
	for(int i=0; i<w2wPlugins->at(toPlugin)->names().length(); i++)
	    w2wPlugins->at(toPlugin)->calculate(i,aWaveformData->at(from));
    populateWaveformTree();
}

void DataManagerDialog::w2sDrop(int from, int toPlugin, int toSubplugin)
{
    w2sPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < w2sPlugins->at(toPlugin)->names().length())
	w2sPlugins->at(toPlugin)->calculate(toSubplugin,aWaveformData->at(from));
    else if(toSubplugin == w2sPlugins->at(toPlugin)->names().length())
	for(int i=0; i<w2sPlugins->at(toPlugin)->names().length(); i++)
	    w2sPlugins->at(toPlugin)->calculate(i,aWaveformData->at(from));
    populateSpectrogramTree();
}

void DataManagerDialog::s2wDrop(int from, int toPlugin, int toSubplugin)
{
    s2wPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < s2wPlugins->at(toPlugin)->names().length())
	s2wPlugins->at(toPlugin)->calculate(toSubplugin,aSpectrogramData->at(from));
    else if(toSubplugin == s2wPlugins->at(toPlugin)->names().length())
	for(int i=0; i<s2wPlugins->at(toPlugin)->names().length(); i++)
	    s2wPlugins->at(toPlugin)->calculate(i,aSpectrogramData->at(from));
    populateWaveformTree();
}

void DataManagerDialog::s2sDrop(int from, int toPlugin, int toSubplugin)
{
    s2sPlugins->at(toPlugin)->settings(toSubplugin);

    if(toSubplugin < s2sPlugins->at(toPlugin)->names().length())
	s2sPlugins->at(toPlugin)->calculate(toSubplugin,aSpectrogramData->at(from));
    else if(toSubplugin == s2sPlugins->at(toPlugin)->names().length())
	for(int i=0; i<s2sPlugins->at(toPlugin)->names().length(); i++)
	    s2sPlugins->at(toPlugin)->calculate(i,aSpectrogramData->at(from));
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
					 aWaveformData->at(index)->name(), &ok);

    if (ok && !text.isEmpty())
    {
	aWaveformData->at(index)->setName(text);
	waveformTree->topLevelItem(index)->setText(0,text);
    }
}

void DataManagerDialog::renameSpectrogram(int index)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter a new label"),
					 tr("Enter a new label"), QLineEdit::Normal,
					 aSpectrogramData->at(index)->name(), &ok);
    if (ok && !text.isEmpty())
    {
	aSpectrogramData->at(index)->setName(text);
	spectrogramTree->topLevelItem(index)->setText(0,text);
    }
}
