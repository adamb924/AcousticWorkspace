/*! \class IndexedAction
    \ingroup SupportWidgets
    \brief A custom QAction action that emits an index when activated.

    Currently this class is used only in PlotViewWidget for its context menu. The constructor takes an index value; when the action is activated, the signal indexClicked(int index) is emitted, which provides the index back again.

    This is a generic class because different groups of action objects can be associated with different handler slots, as they're created.
  */

#ifndef INDEXEDACTION_H
#define INDEXEDACTION_H

#include <QAction>

class IndexedAction : public QAction
{
    Q_OBJECT
public:
    //! \brief Construct the object, storing \a i
    IndexedAction(QString text, int i, QObject *parent);
    int mIndex;
public slots:
    //! \brief Connected to the triggered() signal, this emits the indexClicked signal with the value of \a index
    void mytrigger();
signals:
    void indexClicked(int index);
};

#endif // INDEXEDACTION_H
