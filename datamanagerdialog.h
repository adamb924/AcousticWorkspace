/*! \class DataManagerDialog
    \ingroup MajorDialog
    \brief A dialog class for managing project data.

    This dialog provides a user interface that allows users to rename or delete spectrograms and waveforms. Users can also create new spectrograms and waveforms by dragging existing ones onto plugins that will make new ones.
  */


#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QDialog>
#include <QList>
#include <QTreeWidget>
#include <QDropEvent>
#include <QtDebug>
#include <QAction>
#include <QMenu>

class SpectrogramData;
class PlotViewWidget;
class WaveformData;
class PluginViewTreeWidget;
class DataSourceTreeWidget;

class AbstractWaveform2WaveformMeasure;
class AbstractWaveform2SpectrogramMeasure;
class AbstractSpectrogram2WaveformMeasure;
class AbstractSpectrogram2SpectrogramMeasure;

class DataManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DataManagerDialog(QList<AbstractWaveform2WaveformMeasure*> *mW2wPlugins, QList<AbstractWaveform2SpectrogramMeasure*> *mW2sPlugins, QList<AbstractSpectrogram2WaveformMeasure*> *mS2wPlugins, QList<AbstractSpectrogram2SpectrogramMeasure*> *mS2sPlugins, QList<WaveformData*> *wfd, QList<SpectrogramData*> *spd, QWidget *parent = 0);

private slots:
    //! \brief Populates the tree that displays waveform-to-waveform plugins
    void populateW2wTree();

    //! \brief Populates the tree that displays waveform-to-spectrogram plugins
    void populateW2sTree();

    //! \brief Populates the tree that displays spectrogram-to-waveform plugins
    void populateS2wTree();

    //! \brief Populates the tree that displays spectrogram-to-spectrogram plugins
    void populateS2sTree();

    //! \brief Populates the tree that displays waveforms
    void populateWaveformTree();

    //! \brief Populates the tree that displays spectrograms
    void populateSpectrogramTree();

public slots:
    //! \brief Calls the \a calculate method of  the \a toSubplugin-th plugin of the \a toPlugin-th w2w plugin, passing the \a from-th waveform
    /*!
      If \a toSubplugin is equal to the number of sub-plugins (i.e., one higher than the index of the highest plugin), \a calculate is called for each of the sub-plugins.
      */
    void w2wDrop(int from, int toPlugin, int toSubplugin);

    //! \brief Calls the \a calculate method of  the \a toSubplugin-th plugin of the \a toPlugin-th w2s plugin, passing the \a from-th waveform
    /*!
      If \a toSubplugin is equal to the number of sub-plugins (i.e., one higher than the index of the highest plugin), \a calculate is called for each of the sub-plugins.
      */
    void w2sDrop(int from, int toPlugin, int toSubplugin);

    //! \brief Calls the \a calculate method of  the \a toSubplugin-th plugin of the \a toPlugin-th s2w plugin, passing the \a from-th spectrogram
    /*!
      If \a toSubplugin is equal to the number of sub-plugins (i.e., one higher than the index of the highest plugin), \a calculate is called for each of the sub-plugins.
      */
    void s2wDrop(int from, int toPlugin, int toSubplugin);

    //! \brief Calls the \a calculate method of  the \a toSubplugin-th plugin of the \a toPlugin-th s2s plugin, passing the \a from-th spectrogram
    /*!
      If \a toSubplugin is equal to the number of sub-plugins (i.e., one higher than the index of the highest plugin), \a calculate is called for each of the sub-plugins.
      */
    void s2sDrop(int from, int toPlugin, int toSubplugin);

    //! \brief Prompts the user to enter a new name for the \a index-th waveform, and renames the waveform
    void renameWaveform(int index);

    //! \brief Prompts the user to enter a new name for the \a index-th spectrogram, and renames the spectrogram
    void renameSpectrogram(int index);

signals:
    //! \brief Emitted when a user tried to delete a waveform using the context menu (in another widget)
    void removeWaveform(int index);
    //! \brief Emitted when a user tried to delete a spectrogram using the context menu (in another widget)
    void removeSpectrogram(int index);

private:
    PluginViewTreeWidget *mW2wTree, *mW2sTree, *mS2wTree, *mS2sTree;
    DataSourceTreeWidget *mWaveformTree, *mSpectrogramTree;
    void drawProsodyViewTree();

    QList<AbstractWaveform2WaveformMeasure*> *mW2wPlugins;
    QList<AbstractWaveform2SpectrogramMeasure*> *mW2sPlugins;
    QList<AbstractSpectrogram2WaveformMeasure*> *mS2wPlugins;
    QList<AbstractSpectrogram2SpectrogramMeasure*> *mS2sPlugins;

    QList<WaveformData*> *maWaveformData;
    QList<SpectrogramData*> *maSpectrogramData;
};

#endif // DATAMANAGER_H
