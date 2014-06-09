#include "pluginviewtreewidget.h"

#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

PluginViewTreeWidget::PluginViewTreeWidget(QString mime, QWidget *parent) : QTreeWidget(parent)
{
    this->setDragDropMode(QAbstractItemView::DropOnly);
    this->setAcceptDrops(true);
    this->viewport()->setAcceptDrops(true);

    this->mime = mime;
}

QStringList PluginViewTreeWidget::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

void PluginViewTreeWidget::dropEvent(QDropEvent *event)
{
    QTreeWidgetItem *target = this->itemAt(event->pos());
    int index;
    if( (index = indexOfTopLevelItem(target)) == -1 ) // if it is a second-level item
    {
	index = indexOfTopLevelItem(target->parent());
    int subindex = target->parent()->indexOfChild(target);

	QByteArray encodedData = event->mimeData()->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QString text;
	stream >> text;

	QStringList bits = text.split("+");
	QVariant which(bits.at(1));
	if(bits.at(0) == mime && index!=-1 && subindex!=-1)
	{
//		qDebug() << which.toInt() << index << subindex;
	    emit dropped(which.toInt(), index, subindex);
	    event->accept();
	}
    }
    event->ignore();
}

void PluginViewTreeWidget::dragMoveEvent ( QDragMoveEvent *event )
{
    QTreeWidgetItem *target = this->itemAt(event->pos());
    if( indexOfTopLevelItem(target) == -1 ) // if it's a second-level item
    {
	QByteArray encodedData = event->mimeData()->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QString text;
	stream >> text;

	QStringList bits = text.split("+");
	QVariant which(bits.at(1));
	if(bits.at(0) == mime)
	{
	    QTreeWidget::dragMoveEvent(event);
	}
	else
	{
	    event->ignore();
	}

    }
    else
    {
	event->ignore();
    }
}
