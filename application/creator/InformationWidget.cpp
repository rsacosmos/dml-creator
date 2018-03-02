#include "InformationWidget.h"
#include "ui_InformationWidget.h"

static const QString DATE_FORMAT("yyyy-MM-dd");

InformationWidget::InformationWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _ui(new Ui::InformationWidget)
{
    _ui->setupUi(this);
}

InformationWidget::~InformationWidget()
{
    delete _ui;
}

void InformationWidget::updateTexts()
{
    _ui->retranslateUi(this);
}

void InformationWidget::reloadInformation(const dml::core::Information& information)
{
    // Producer
    _ui->producerValue->blockSignals(true);
    _ui->producerValue->setCurrentText(QString::fromStdString(information.producer()));
    _ui->producerValue->blockSignals(false);

    // Copyright
    _ui->copyrightValue->blockSignals(true);
    _ui->copyrightValue->setCurrentText(QString::fromStdString(information.copyright()));
    _ui->copyrightValue->blockSignals(false);

    // Description
    _ui->descriptionValue->blockSignals(true);
    _ui->descriptionValue->setPlainText(QString::fromStdString(information.description()));
    _ui->descriptionValue->blockSignals(false);

    // Publication date
    bool hasPublicationDate = information.hasPublicationDate();
    // Here we need _ui->publicationDate to emit the signal if it is toggled, to enable the date field
    _ui->publicationDate->setChecked(hasPublicationDate); 
    // qDebug() << "hasPublicationDate=" << hasPublicationDate; // left for debug______________________________________
    _ui->publicationDateValue->blockSignals(true);
    if (hasPublicationDate)
    {
        QString publicationDateString = QString::fromStdString(information.publicationDate());
        QDate publicationDate = QDate::fromString(publicationDateString, DATE_FORMAT);
        _ui->publicationDateValue->setDate(publicationDate);
        // qDebug() << "publicationDateString=" << publicationDateString; // left for debug____________________________
        // qDebug() << "publicationDate=" << publicationDate.toString(DATE_FORMAT); // left for debug__________________
        // qDebug() << "publicationDate=" << QString::fromStdString(show.publicationDate()); // left for debug_________
    }
    else _ui->publicationDateValue->setDate(QDate());
    _ui->publicationDateValue->blockSignals(false);

    // Emit signal "informationChanged()"
    emit informationChanged(information);
}

void InformationWidget::refreshInformation()
{
    dml::core::Information information;
    information.setDescription(_ui->descriptionValue->toPlainText().toStdString());
    if (_ui->publicationDate->isChecked()) information.setPublicationDate(_ui->publicationDateValue->date().toString(DATE_FORMAT).toStdString());
    information.setProducer(_ui->producerValue->currentText().toStdString());
    information.setCopyright(_ui->copyrightValue->currentText().toStdString());
    emit informationChanged(information);
}

QStringList InformationWidget::producerList() const
{
    QStringList items;
    for (int index=0; index<_ui->producerValue->count(); ++index) items << _ui->producerValue->itemText(index);
    items.sort();
    return items;
}

void InformationWidget::setProducerList(const QStringList& producerList)
{
    QString currentText = _ui->producerValue->currentText();
    foreach(const QString& producer, producerList)
    {
        if (!this->producerList().contains(producer))
        {
            _ui->producerValue->addItem(producer);
        }
    }
    _ui->producerValue->setCurrentText(currentText);
}

QStringList InformationWidget::copyrightList() const
{
    QStringList items;
    for (int index=0; index<_ui->copyrightValue->count(); ++index) items << _ui->copyrightValue->itemText(index);
    items.sort();
    return items;
}

void InformationWidget::setCopyrightList(const QStringList& copyrightList)
{
    QString currentText = _ui->copyrightValue->currentText();
    foreach(const QString& copyright, copyrightList)
    {
        if (!this->copyrightList().contains(copyright))
        {
            _ui->copyrightValue->addItems(copyrightList);
        }
    }
    _ui->copyrightValue->setCurrentText(currentText);
}
