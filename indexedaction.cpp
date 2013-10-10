#include "indexedaction.h"

IndexedAction::IndexedAction(QString text, int i, QObject *parent) : QAction(text,parent)
{
    index = i;
    connect(this, SIGNAL(triggered()), this, SLOT(mytrigger()) );
}

void IndexedAction::mytrigger()
{
    emit indexClicked(index);
}
