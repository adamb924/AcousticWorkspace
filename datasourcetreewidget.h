/*! \class DataSourceTreeWidget
    \ingroup SupportWidgets
    \brief A tree widget class for displaying data sources (lists of spectrograms & waveforms).

    This dialog is a subclassed QTreeWidget. This is to enable it to provide MIME data when items are dragged from the widget, and to provide a simple context menu.

    The class is used in DataManagerDialog and PlotManagerDialog.
  */

#ifndef DATASOURCETREE_H
#define DATASOURCETREE_H

#include <QTreeWidget>

class DataSourceTreeWidget : public QTreeWidget
{
Q_OBJECT
public:
    //! \brief Sets up the widget for dragging and dropping
    /*!
      \param mime A string that will be prefixed to all of the MIME data
      */
    DataSourceTreeWidget(QString mime, QWidget *parent = 0);

    //! \brief Indicates that the widget provides text/plain data
    QStringList mimeTypes() const;

    //! \brief Creates the MIME data -- a string beginning with \a mimeIdString and ending with the index of the item being dragged
    QMimeData * mimeData ( const QList<QTreeWidgetItem *> items ) const;

    //! \brief Provides a context menu when an item is right-clicked
    void contextMenuEvent ( QContextMenuEvent * event );

signals:
    //! \brief Emitted when a user wants to remove the \a i-th item
    void removeItemSignal(int i);

    //! \brief Emitted when a user wants to rename the \a i-th item
    void renameItemSignal(int i);

private slots:
    //! \brief Detects which item is currently selected, emitting removeItemSignal if appropriate
    void remove();

    //! \brief Detects which item is currently selected, emitting renameItemSignal if appropriate
    void rename();

private:
    QString mMimeIdString;
    QAction *mRemoveAction, *mRenameAction;
};

#endif // DATASOURCETREE_H
