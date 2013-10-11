#include "plotviewtreewidget.h"

#include <qwt/qwt_plot.h>

#include "curvesettingsdialog.h"
#include "spectrogramsettingsdialog.h"

#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

PlotViewTreeWidget::PlotViewTreeWidget(QList<PlotViewWidget*> *pv, QWidget *parent = 0) : QTreeWidget(parent)
{
    this->aProsodyViews = pv;

    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setAcceptDrops(true);
    this->viewport()->setAcceptDrops(true);

    removeAction = new QAction(tr("Remove"),this);
    connect(removeAction,SIGNAL(triggered()),this,SLOT(remove()));

    settingsAction = new QAction(tr("Edit plot settings"),this);
    connect(settingsAction,SIGNAL(triggered()),this,SLOT(settings()));

    secondaryAxisAction = new QAction(tr("Has secondary axis"),this);
    secondaryAxisAction->setCheckable(true);
    connect(secondaryAxisAction,SIGNAL(toggled(bool)),this,SLOT(secondary(bool)));

    moveToOtherAxisAction = new QAction(tr("Move to other axis"),this);
    connect(moveToOtherAxisAction,SIGNAL(triggered()),this,SLOT(moveToOtherAxis()));

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(doubleClick(QTreeWidgetItem*,int)));
}

void PlotViewTreeWidget::populate()
{
    this->clear();
    for(int i=0; i<aProsodyViews->count(); i++)
    {
	QTreeWidgetItem *top = new QTreeWidgetItem(QStringList(aProsodyViews->at(i)->name()),i);
	this->addTopLevelItem(top);

	for(int j=0; j<aProsodyViews->at(i)->spectrograms()->count(); j++)
	{
	    QTreeWidgetItem *child = new QTreeWidgetItem(QStringList(aProsodyViews->at(i)->spectrogramData(j)->name()),i);
	    top->addChild(child);
	}

	for(int j=0; j<aProsodyViews->at(i)->curves()->count(); j++)
	{
	    QTreeWidgetItem *child = new QTreeWidgetItem(QStringList(aProsodyViews->at(i)->curveData(j)->name()),i);
	    top->addChild(child);
	}
    }
    this->expandAll();
}

QStringList PlotViewTreeWidget::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

void PlotViewTreeWidget::dropEvent(QDropEvent *event)
{
    if(event->source() != this)
    {
	QTreeWidgetItem *target = this->itemAt(event->pos());
	int index;
	if( (index = indexOfTopLevelItem(target)) == -1 )
	{
	    event->ignore();
	    return;
	}

	QByteArray encodedData = event->mimeData()->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QString text;
	stream >> text;

	QStringList bits = text.split("+");
	QVariant which(bits.at(1));
	if(bits.at(0) == "waveform")
	{
	    emit addWaveform(index, which.toInt() );
	}
	else if(bits.at(0) == "spectrogram")
	{
	    emit addSpectrogram(index, which.toInt() );
	}
	else
	{
	    event->ignore();
	    return;
	}
	event->accept();
    }
    else // internal move
    {
	qDebug() << "internal move";
	QByteArray encodedData = event->mimeData()->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QString text;
	stream >> text;
	qDebug() << text;

	QStringList bits = text.split("+");
	if(bits.length()==1) // moving a top-level item
	{
	    if(event->proposedAction() == Qt::MoveAction)
	    {
		qDebug() << "move a top-level item";
	    }
	}
	else if(bits.length()==2)
	{
	    qDebug() << "move a second-level item" << event->proposedAction();
	}
	else
	{
	    event->ignore();
	    return;
	}

    }
}

void PlotViewTreeWidget::dragMoveEvent ( QDragMoveEvent *event )
{
    QTreeWidgetItem *target = this->itemAt(event->pos());
    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // if it's a second-level item
    {
	event->ignore();
	return;
    }
    QTreeWidget::dragMoveEvent(event);
}

bool PlotViewTreeWidget::isSpectrogram(int plotIndex, int &index)
{
    if( index < aProsodyViews->at(plotIndex)->spectrograms()->count() )
    {
	return true;
    }
    else // must be a curve
    {
	index -= aProsodyViews->at(plotIndex)->spectrograms()->count();
	return false;
    }
}

void PlotViewTreeWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QTreeWidgetItem *target = this->itemAt(event->pos());
    if(target == 0 ) { return; }
    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // if it's a second-level item
    {
	int plotIndex = indexOfTopLevelItem( target->parent() );
	if(plotIndex == -1 ) { return; }
	index = target->parent()->indexOfChild(target);
	if(index == -1 ) { return; }

	QMenu menu(this);
	menu.addAction(settingsAction);
	if(!isSpectrogram(plotIndex,index))
	{
	    if(aProsodyViews->at(plotIndex)->curves()->at(index)->yAxis() == QwtPlot::yLeft)
		moveToOtherAxisAction->setText(tr("Move to secondary axis"));
	    else
		moveToOtherAxisAction->setText(tr("Move to primary axis"));
	    menu.addAction(moveToOtherAxisAction);
	}
	menu.addAction(removeAction);
	menu.exec(event->globalPos());
    }
    else // if it's a top-level item
    {
	QMenu menu(this);
	menu.addAction(removeAction);
	menu.addAction(secondaryAxisAction);
	secondaryAxisAction->setChecked(aProsodyViews->at(index)->hasSecondaryAxis());
	menu.exec(event->globalPos());
    }
}

QMimeData * PlotViewTreeWidget::mimeData ( const QList<QTreeWidgetItem *> items ) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    int index;
    for(int i=0; i<items.count(); i++)
    {
	if( (index = indexOfTopLevelItem(items.at(i))) != -1 ) // top-level item
	{
	    stream << QString::number(index);
	}
	else // second-level item
	{
	    index = indexOfTopLevelItem(items.at(i)->parent());
	    stream << QString::number(index) + "+" + QString::number( this->topLevelItem(index)->indexOfChild(items.at(i)) );
	}
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

void PlotViewTreeWidget::doubleClick(QTreeWidgetItem * item, int column)
{
    QTreeWidgetItem *target = item;
    if(target==0) { return; }

    int index, plotIndex;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
	plotIndex = indexOfTopLevelItem(target->parent());
	index = target->parent()->indexOfChild(target);

	if(isSpectrogram(plotIndex,index))
	{
	    SpectrogramSettingsDialog ss(aProsodyViews->at(plotIndex)->spectrograms()->at(index), aProsodyViews->at(plotIndex)->plot(), this);
	    ss.exec();
	}
	else
	{
	    CurveSettingsDialog cs(aProsodyViews->at(plotIndex)->curves()->at(index) ,aProsodyViews->at(plotIndex)->plot(),this);
	    cs.exec();
	}
    }
}

void PlotViewTreeWidget::remove()
{
    QTreeWidgetItem *target = this->currentItem();
    if(target==0) { return; }

    int index, plotIndex;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
	plotIndex = indexOfTopLevelItem(target->parent());
	index = target->parent()->indexOfChild(target);
	//	emit removeSource(plotIndex,index);

	aProsodyViews->at(plotIndex)->removeItemAt(index);
	aProsodyViews->at(plotIndex)->plot()->replot();
	populate();
    }
    else // it's a top-level item
    {
	emit removePlot(index);
    }
}

void PlotViewTreeWidget::settings()
{
    QTreeWidgetItem *target = this->currentItem();
    if(target==0) { return; }

    int index, plotIndex;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
	plotIndex = indexOfTopLevelItem(target->parent());
	index = target->parent()->indexOfChild(target);

	// spectrograms are always added to the list first
	if( isSpectrogram(plotIndex,index) )
	{
	    SpectrogramSettingsDialog ss(aProsodyViews->at(plotIndex)->spectrograms()->at(index), aProsodyViews->at(plotIndex)->plot(), this);
	    ss.exec();
	}
	else // must be a curve
	{
	    CurveSettingsDialog cs(aProsodyViews->at(plotIndex)->curves()->at(index) ,aProsodyViews->at(plotIndex)->plot(),this);
	    cs.exec();
	}
    }
}

void PlotViewTreeWidget::secondary(bool does)
{
    QTreeWidgetItem *target = this->currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) != -1 )
    {
	aProsodyViews->at(index)->setHasSecondaryAxis(does);
    }
}

void PlotViewTreeWidget::moveToOtherAxis()
{
    QTreeWidgetItem *target = this->currentItem();
    if(target == 0 ) { return; }
    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // if it's a second-level item
    {
	int plotIndex = indexOfTopLevelItem( target->parent() );
	if(plotIndex == -1 ) { return; }
	index = target->parent()->indexOfChild(target);
	if(index == -1 ) { return; }

	if( !isSpectrogram(plotIndex,index) )
	{
	    if(aProsodyViews->at(plotIndex)->curves()->at(index)->yAxis() == QwtPlot::yLeft)
		aProsodyViews->at(plotIndex)->setHasSecondaryAxis(true);

	    aProsodyViews->at(plotIndex)->toggleCurveAxisAssociation(index);
	}
    }
}
