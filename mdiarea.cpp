#include "mainwindow.h"
#include "mdiarea.h"

#include "soundwidget.h"
#include "comparisonwidget.h"

#include <QtDebug>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QInputDialog>

MdiArea::MdiArea(QWidget *parent) :
    QMdiArea(parent)
{
    mMainWnd = qobject_cast<MainWindow*>(parent);

    newSoundWindow();
}

void MdiArea::newSoundWindow()
{
    SoundWidget *tmp = new SoundWidget(this,mMainWnd);
    addSubWindow(tmp);
    subWindowList().last()->setAttribute(Qt::WA_DeleteOnClose);
    tmp->show();
}

void MdiArea::newComparisonWindow()
{
    QList<SoundWidget*> *sounds = soundWindows();
    if( sounds->count() < 2 )
    {
	QMessageBox::critical(mMainWnd,tr("Error"),tr("You need at least two different sounds to make a comparison."));
	return;
    }

    QStringList items;
    for(int i=0; i< sounds->count(); i++)
	items << sounds->at(i)->windowTitle();

    bool ok;
    QString item = QInputDialog::getItem(mMainWnd, tr("Acoustic Workspace"),tr("Choose the primary sound for the comparison"), items, 0, false, &ok);
    if(!ok) { return; }
    int first = items.indexOf(item);


    ComparisonWidget *tmp = new ComparisonWidget(sounds->at(first), sounds);
    addSubWindow(tmp);
    subWindowList().last()->setAttribute(Qt::WA_DeleteOnClose);
    tmp->show();
}

QList<SoundWidget*>* MdiArea::soundWindows()
{
    QList<SoundWidget*> *sounds = new QList<SoundWidget*>;
    for(int i=0; i< subWindowList().count(); i++)
    {
	SoundWidget* tmp = qobject_cast<SoundWidget*>(subWindowList().at(i)->widget());
	if( tmp != 0 && subWindowList().at(i)->windowTitle() != tr("Acoustic Workspace") ) // this is a cheap way to tell if the window is uninitialized
	    *sounds << tmp;
    }
    return sounds;
}
