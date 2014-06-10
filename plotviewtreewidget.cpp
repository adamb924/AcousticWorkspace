#include "plotviewtreewidget.h"

#include <qwt_plot.h>

#include "curvesettingsdialog.h"
#include "spectrogramsettingsdialog.h"

#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

PlotViewTreeWidget::PlotViewTreeWidget(QList<PlotViewWidget*> *pv, QWidget *parent = 0) : QTreeWidget(parent), maProsodyViews(pv)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);

    mRemoveAction = new QAction(tr("Remove"),this);
    connect(mRemoveAction,SIGNAL(triggered()),this,SLOT(remove()));

    mSettingsAction = new QAction(tr("Edit plot settings"),this);
    connect(mSettingsAction,SIGNAL(triggered()),this,SLOT(settings()));

    mSecondaryAxisAction = new QAction(tr("Has secondary axis"),this);
    mSecondaryAxisAction->setCheckable(true);
    connect(mSecondaryAxisAction,SIGNAL(toggled(bool)),this,SLOT(secondary(bool)));

    mMoveToOtherAxisAction = new QAction(tr("Move to other axis"),this);
    connect(mMoveToOtherAxisAction,SIGNAL(triggered()),this,SLOT(moveToOtherAxis()));

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(doubleClick(QTreeWidgetItem*,int)));
}

void PlotViewTreeWidget::populate()
{
    clear();
    for(int i=0; i<maProsodyViews->count(); i++)
    {
    QTreeWidgetItem *top = new QTreeWidgetItem(QStringList(maProsodyViews->at(i)->name()),i);
    addTopLevelItem(top);

    for(int j=0; j<maProsodyViews->at(i)->spectrograms()->count(); j++)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList(maProsodyViews->at(i)->spectrogramData(j)->name()),i);
        top->addChild(child);
    }

    for(int j=0; j<maProsodyViews->at(i)->curves()->count(); j++)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList(maProsodyViews->at(i)->curveData(j)->name()),i);
        top->addChild(child);
    }
    }
    expandAll();
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
    QTreeWidgetItem *target = itemAt(event->pos());
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
    QTreeWidgetItem *target = itemAt(event->pos());
    if( indexOfTopLevelItem(target) == -1 ) // if it's a second-level item
    {
    event->ignore();
    return;
    }
    QTreeWidget::dragMoveEvent(event);
}

bool PlotViewTreeWidget::isSpectrogram(int plotIndex, int &index)
{
    if( index < maProsodyViews->at(plotIndex)->spectrograms()->count() )
    {
    return true;
    }
    else // must be a curve
    {
    index -= maProsodyViews->at(plotIndex)->spectrograms()->count();
    return false;
    }
}

void PlotViewTreeWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QTreeWidgetItem *target = itemAt(event->pos());
    if(target == 0 ) { return; }
    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // if it's a second-level item
    {
    int plotIndex = indexOfTopLevelItem( target->parent() );
    if(plotIndex == -1 ) { return; }
    index = target->parent()->indexOfChild(target);
    if(index == -1 ) { return; }

    QMenu menu(this);
    menu.addAction(mSettingsAction);
    if(!isSpectrogram(plotIndex,index))
    {
        if(maProsodyViews->at(plotIndex)->curves()->at(index)->yAxis() == QwtPlot::yLeft)
        mMoveToOtherAxisAction->setText(tr("Move to secondary axis"));
        else
        mMoveToOtherAxisAction->setText(tr("Move to primary axis"));
        menu.addAction(mMoveToOtherAxisAction);
    }
    menu.addAction(mRemoveAction);
    menu.exec(event->globalPos());
    }
    else // if it's a top-level item
    {
    QMenu menu(this);
    menu.addAction(mRemoveAction);
    menu.addAction(mSecondaryAxisAction);
    mSecondaryAxisAction->setChecked(maProsodyViews->at(index)->hasSecondaryAxis());
    menu.exec(event->globalPos());
    }
}

QMimeData * PlotViewTreeWidget::mimeData ( const QList<QTreeWidgetItem *> items ) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for(int i=0; i<items.count(); i++)
    {
        int index;
        if( (index = indexOfTopLevelItem(items.at(i))) != -1 ) // top-level item
        {
            stream << QString::number(index);
        }
        else // second-level item
        {
            index = indexOfTopLevelItem(items.at(i)->parent());
            stream << QString::number(index) + "+" + QString::number( topLevelItem(index)->indexOfChild(items.at(i)) );
        }
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

void PlotViewTreeWidget::doubleClick(QTreeWidgetItem * item, int column)
{
    Q_UNUSED(column);

    QTreeWidgetItem *target = item;
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
    int plotIndex = indexOfTopLevelItem(target->parent());
    index = target->parent()->indexOfChild(target);

    if(isSpectrogram(plotIndex,index))
    {
        SpectrogramSettingsDialog ss(maProsodyViews->at(plotIndex)->spectrograms()->at(index), maProsodyViews->at(plotIndex)->plot(), this);
        ss.exec();
    }
    else
    {
        CurveSettingsDialog cs(maProsodyViews->at(plotIndex)->curves()->at(index) ,maProsodyViews->at(plotIndex)->plot(),this);
        cs.exec();
    }
    }
}

void PlotViewTreeWidget::remove()
{
    QTreeWidgetItem *target = currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
    int plotIndex = indexOfTopLevelItem(target->parent());
    index = target->parent()->indexOfChild(target);
    //	emit removeSource(plotIndex,index);

    maProsodyViews->at(plotIndex)->removeItemAt(index);
    maProsodyViews->at(plotIndex)->plot()->replot();
    populate();
    }
    else // it's a top-level item
    {
    emit removePlot(index);
    }
}

void PlotViewTreeWidget::settings()
{
    QTreeWidgetItem *target = currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // then it's not a top-level item, which is what we want
    {
    int plotIndex = indexOfTopLevelItem(target->parent());
    index = target->parent()->indexOfChild(target);

    // spectrograms are always added to the list first
    if( isSpectrogram(plotIndex,index) )
    {
        SpectrogramSettingsDialog ss(maProsodyViews->at(plotIndex)->spectrograms()->at(index), maProsodyViews->at(plotIndex)->plot(), this);
        ss.exec();
    }
    else // must be a curve
    {
        CurveSettingsDialog cs(maProsodyViews->at(plotIndex)->curves()->at(index) ,maProsodyViews->at(plotIndex)->plot(),this);
        cs.exec();
    }
    }
}

void PlotViewTreeWidget::secondary(bool does)
{
    QTreeWidgetItem *target = currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) != -1 )
    {
    maProsodyViews->at(index)->setHasSecondaryAxis(does);
    }
}

void PlotViewTreeWidget::moveToOtherAxis()
{
    QTreeWidgetItem *target = currentItem();
    if(target == 0 ) { return; }
    int index = indexOfTopLevelItem(target);
    if( index == -1 ) // if it's a second-level item
    {
    int plotIndex = indexOfTopLevelItem( target->parent() );
    if(plotIndex == -1 ) { return; }
    index = target->parent()->indexOfChild(target);
    if(index == -1 ) { return; }

    if( !isSpectrogram(plotIndex,index) )
    {
        if(maProsodyViews->at(plotIndex)->curves()->at(index)->yAxis() == QwtPlot::yLeft)
        maProsodyViews->at(plotIndex)->setHasSecondaryAxis(true);

        maProsodyViews->at(plotIndex)->toggleCurveAxisAssociation(index);
    }
    }
}
