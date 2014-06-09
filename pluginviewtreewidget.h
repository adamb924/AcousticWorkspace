/*! \class PluginViewTreeWidget
    \ingroup SupportWidgets
    \brief A tree widget class for displaying lists of plugins

    This dialog is a subclassed QTreeWidget. This is solely to enable it to provide MIME data when items are dragged from the widget. The class can be used for each of the four plugin types.

    The class is used in DataManagerDialog.
  */

#ifndef PLUGINVIEWTREE_H
#define PLUGINVIEWTREE_H

#include <QTreeWidget>


class PluginViewTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PluginViewTreeWidget(QString mime, QWidget *parent = 0);

    //! \brief Indicates that the widget provides text/plain data
    QStringList mimeTypes() const;

    //! \brief Picks the origin of the drop event from the MIME data, and emits the dropped() signal to indicate which plugin and sub-plugin are intended
    void dropEvent(QDropEvent *event);

    //! \brief Allows dragging events if the mouse is over a secondary item (i.e. the name of a sub-plugin) and if the item has appropriate MIME data
    void dragMoveEvent ( QDragMoveEvent *event );

signals:
    //! \brief Emitted to indicate that the user has requested that the data in \from to sent to plugin \a toPlugin, sub-plugin \a toSubplugin
    /*!
      There is no ambiguity in the interpretation of \a from because any type of plugin accepts only one kind of input data.
      */
    void dropped(int from, int toPlugin, int toSubplugin);

private:
    QString mMime;
};

#endif // PLUGINVIEWTREE_H
