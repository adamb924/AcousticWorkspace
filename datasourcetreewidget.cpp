#include "datasourcetreewidget.h"

#include <QMimeData>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QtDebug>

DataSourceTreeWidget::DataSourceTreeWidget(QString mime, QWidget *parent) : QTreeWidget(parent)
{
    mMimeIdString = mime;
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragOnly);

    mRemoveAction = new QAction(tr("Remove"),this);
    connect(mRemoveAction,SIGNAL(triggered()),this,SLOT(remove()));
    mRenameAction = new QAction(tr("Rename"),this);
    connect(mRenameAction,SIGNAL(triggered()),this,SLOT(rename()));
}

QStringList DataSourceTreeWidget::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData * DataSourceTreeWidget::mimeData ( const QList<QTreeWidgetItem *> items ) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);


    for(int i=0; i<items.count(); i++)
    {
	stream << mMimeIdString + "+" + QString::number(indexOfTopLevelItem(items.at(i)));
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

void DataSourceTreeWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);
    menu.addAction(mRenameAction);
    menu.addAction(mRemoveAction);
    menu.exec(event->globalPos());
}

void DataSourceTreeWidget::remove()
{
    QTreeWidgetItem *target = this->currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) != -1 )
    {
	emit removeItemSignal(index);
    }
}

void DataSourceTreeWidget::rename()
{
    QTreeWidgetItem *target = this->currentItem();
    if(target==0) { return; }

    int index;
    if( (index = indexOfTopLevelItem(target)) != -1 )
    {
	emit renameItemSignal(index);
    }
}

