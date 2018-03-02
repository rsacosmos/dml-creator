#include "AudioWidget.h"
#include "ui_AudioWidget.h"

#include <conf/ConfigurationManager.h>
#include <scan/MainScan.h>

#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QSignalMapper>
#include <QHeaderView>

AudioWidget::AudioWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _ui(new Ui::AudioWidget)
    , _speakerSignalMapper(new QSignalMapper(this))
{
    _ui->setupUi(this);

    QHeaderView *headerView = new QHeaderView(Qt::Horizontal, _ui->channelsTableWidget);
    _ui->channelsTableWidget->setHorizontalHeader(headerView);
    headerView->setSectionResizeMode(0, QHeaderView::Fixed);
    headerView->resizeSection(0, 210);
    headerView->setSectionResizeMode(1, QHeaderView::Stretch);
    headerView->setSectionResizeMode(2, QHeaderView::Fixed);
    headerView->resizeSection(2, 25);

    connect(_ui->listWidget, &QListWidget::currentRowChanged, this, &AudioWidget::setCurrentAudioIndex);
}

AudioWidget::~AudioWidget()
{
    delete _ui;
    delete _speakerSignalMapper;
}

void AudioWidget::setAudioList(const dml::core::AudioList& audioList)
{
    if (audioList==_audioList) return;

    // Should we rebuild the listWidget?
    bool rebuild = false;
    if (_audioList.size()!=audioList.size())
    {
        rebuild = true;
    }
    else
    {
        for (unsigned int audioIndex=0; audioIndex<audioList.size(); audioIndex++)
        {
            if (audioList.at(audioIndex).audioName()!=_audioList.at(audioIndex).audioName())
            {
                rebuild = true;
                break;
            }
        }
    }

    // Save the current row for later
    int currentRow = _ui->listWidget->currentRow();

    // Rebuild the listWidget if needed
    if (rebuild)
    {
        _blockSignals(true);
        _ui->listWidget->clear();
        for (unsigned int audioIndex=0 ; audioIndex<audioList.size(); audioIndex++)
            _ui->listWidget->addItem(QString::fromStdString(audioList.at(audioIndex).audioName()));
        _blockSignals(false);
    }
    else
    {
        // Check names
        for (unsigned int audioIndex=0 ; audioIndex<audioList.size(); audioIndex++)
        {
            QString name = QString::fromStdString(audioList.at(audioIndex).audioName());
            QListWidgetItem *item = _ui->listWidget->item(audioIndex);
            Q_ASSERT(item);
            QString currentItemText = item->text();
            if (currentItemText!=name)
            {
                item->setText(name);
            }
        }
    }

    // Save the changes
    _audioList = audioList;

    // Update the right part of the widget now
    if (0==audioList.size()) currentRow = -1;
    else if (currentRow>=audioList.size()) currentRow = 0;
    _blockSignals(true);
    _ui->listWidget->setCurrentRow(currentRow);
    _blockSignals(false);
    setCurrentAudioIndex(currentRow); // manual call, because 'setCurrentRow' will not emit 'changed' if the row is the same (in the case of only the currentVideo contents changed)

    emit audioListChanged(_audioList);
}

void AudioWidget::setCurrentAudioIndex(int currentIndex)
{
    using namespace dml::core;

    static const int speakerCount = Audio::SpeakerCount;

    bool noAudio = (currentIndex < 0);
    _ui->groupBox->setDisabled(noAudio);
    _ui->removePushButton->setDisabled(noAudio);

    if (noAudio)
    {
        _blockSignals(true);
        _ui->audioNameLineEdit->clear();
        _ui->pictureFrameRateSpinBox->setValue(0);
        _ui->languageLineEdit->clear();
        _ui->channelsTableWidget->setRowCount(0);
        _blockSignals(false);
        return;
    }

    if(currentIndex < _audioList.size())
    {
        const Audio& audio = _audioList.at(currentIndex);

        _blockSignals(true);

        // Audio name
        QString audioName = QString::fromStdString(audio.audioName());
        if (_ui->audioNameLineEdit->text() != audioName) _ui->audioNameLineEdit->setText(audioName);
    
        // Picture framerate
        double pictureFrameRate = audio.pictureFrameRate();
        if (_ui->pictureFrameRateSpinBox->value() != pictureFrameRate) _ui->pictureFrameRateSpinBox->setValue(pictureFrameRate);

        // Language
        QString language = QString::fromStdString(audio.language());
        if (_ui->languageLineEdit->text() != language) _ui->languageLineEdit->setText(language);

        // Speakers
        Audio::ChannelMap stlChannelMap = audio.channelMap();
        QMap<Audio::Speaker, std::string> qtChannelList(stlChannelMap);

        _speakerSignalMapper->deleteLater();
        _speakerSignalMapper = new QSignalMapper;
        connect(_speakerSignalMapper, SIGNAL(mapped(int)), this, SLOT(onSpeakerBrowseButtonClicked(int)));
        _ui->channelsTableWidget->clearContents();
        _ui->channelsTableWidget->setRowCount(speakerCount);
        for (int speakerIndex=0; speakerIndex<speakerCount; speakerIndex++)
        {
            Audio::Speaker speaker = static_cast<Audio::Speaker>(speakerIndex);
            QString speakerName = QString::fromStdString(dml::conf::Configuration::speakerToString(speaker));

            QString speakerPath;
            if (qtChannelList.contains(speaker)) speakerPath=QString::fromStdString(qtChannelList[speaker]);

            QTableWidgetItem *itemName = new QTableWidgetItem(speakerName);
            itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);
            QTableWidgetItem *itemPath = new QTableWidgetItem(speakerPath);
            QPushButton *button = new QPushButton;
            button->setText("...");
            button->setFixedWidth(25);
            connect(button, SIGNAL(clicked()), _speakerSignalMapper, SLOT(map()));
            _speakerSignalMapper->setMapping(button, speakerIndex);

            _ui->channelsTableWidget->setItem(speakerIndex, 0, itemName);
            _ui->channelsTableWidget->setItem(speakerIndex, 1, itemPath);
            _ui->channelsTableWidget->setCellWidget(speakerIndex, 2, button);
        }
        _blockSignals(false); 
    }
}

void AudioWidget::_blockSignals(bool blocked)
{
    _ui->listWidget->blockSignals(blocked);
    _ui->audioNameLineEdit->blockSignals(blocked);
    _ui->pictureFrameRateSpinBox->blockSignals(blocked);
    _ui->languageLineEdit->blockSignals(blocked);
    _ui->channelsTableWidget->blockSignals(blocked);
}

void AudioWidget::refreshAudio()
{
    using namespace dml::core;

    int currentIndex = _ui->listWidget->currentRow();
    if (-1==currentIndex) return;

    Audio audio;
    audio.setAudioName(_ui->audioNameLineEdit->text().toStdString());
    audio.setPictureFrameRate(_ui->pictureFrameRateSpinBox->value());
    audio.setLanguage(_ui->languageLineEdit->text().toStdString());

    Audio::ChannelMap channelMap;
    for (int speakerIndex=0; speakerIndex<_ui->channelsTableWidget->rowCount(); speakerIndex++)
    {
        QTableWidgetItem *item = _ui->channelsTableWidget->item(speakerIndex, 1);
        if (!item)
            return;
        Audio::Speaker speakerName = static_cast<Audio::Speaker>(speakerIndex);
        QString speakerPath = item->text();
        channelMap[speakerName] = speakerPath.toStdString();
    }
    audio.setChannelMap(channelMap);

    AudioList audioList(_audioList);
    if (audio!=audioList.at(currentIndex))
    {
        audioList.at(currentIndex) = audio;
        setAudioList(audioList);
    }
}

void AudioWidget::on_addPushButton_clicked()
{
    using namespace dml::core;
    Audio audio;
    audio.setAudioName(tr("Unnamed audio").toStdString());

    AudioList audioList(_audioList);
    audioList.push_back(audio);
    setAudioList(audioList);
}

void AudioWidget::on_removePushButton_clicked()
{
    QList<QListWidgetItem *> selectedItems = _ui->listWidget->selectedItems();

    if (QMessageBox::No == QMessageBox::question(this, 
        tr("Audio deletion"),
        tr("Audio will be deleted. Are you sure?")))
    {
        return;
    }

    QList<int> indexes;
    dml::core::AudioList audioList(_audioList);
    foreach(QListWidgetItem* item, selectedItems)
    {
        indexes <<_ui->listWidget->row(item);
    }
    qSort(indexes);

    int offset = 0;
    foreach(const int& index, indexes)
    {
        audioList.erase(audioList.begin()+index - offset);
        ++offset;
    }
    setAudioList(audioList);
}


void AudioWidget::onSpeakerBrowseButtonClicked(int speakerIndex)
{
    int currentAudioIndex = _ui->listWidget->currentRow();
    if (currentAudioIndex < 0)
    {
        return;
    }
    if (speakerIndex < 0)
    {
        return;
    }
    dml::core::Audio audio = _audioList.at(currentAudioIndex);
    dml::core::Audio::ChannelMap channelMap = audio.channelMap();

    std::string path;
    if(channelMap.count((dml::core::Audio::Speaker)speakerIndex) == 1)
    {
        path = channelMap.at((dml::core::Audio::Speaker)speakerIndex);
    }

    QString projectPath;
    if(_currentShow != NULL)
        projectPath = QString(_currentShow->rootPath().c_str());

    dml::conf::Configuration configuration = dml::conf::ConfigurationManager::instance()->configuration();
    QString filename = QFileDialog::getOpenFileName(0, tr("Choose preview file"), dml::scan::MainScan::toAbsolutePath(projectPath, QString(path.c_str())), configuration.audioExtensionSupported());
    if (filename.isEmpty())
        return;

    QString relativePath = dml::scan::MainScan::toLocalPath(projectPath, filename);
    QString language = dml::scan::MainScan::getLanguage(configuration, relativePath);
    if(language.isEmpty())
    {
        language = configuration.defaultSubtitlesLanguage().toLower();
    }
    double framerate = dml::scan::MainScan::getFramerate(configuration, relativePath);

    // Update the QLineEdit and ask for a refresh (let refreshVideo do the job ^^)
    blockSignals(true);
    QTableWidgetItem *itemPath = new QTableWidgetItem(relativePath);
    _ui->channelsTableWidget->setItem(speakerIndex, 1, itemPath);
    _ui->languageLineEdit->setText(language);
    _ui->pictureFrameRateSpinBox->setValue(framerate);
    blockSignals(false);
    refreshAudio(); // manual call to refreshAudio()
}
