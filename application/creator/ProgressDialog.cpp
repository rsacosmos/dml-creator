#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    connect(ui->CancelButton, &QPushButton::clicked, this, &ProgressDialog::cancel);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}


void ProgressDialog::setStep(dml::scan::MainScan::ScanStep step)
{
    switch(step)
    {
    case dml::scan::MainScan::ScanPaths:
        ui->StepLabel->setText(tr("Scan all paths..."));
        break;
    case dml::scan::MainScan::ScanFiles:
        ui->StepLabel->setText(tr("Scan all files..."));
        break;
    case dml::scan::MainScan::ScanSubtitles:
        ui->StepLabel->setText(tr("Scan all subtitles..."));
        break;
    case dml::scan::MainScan::ScanPreview:
        ui->StepLabel->setText(tr("Scan all previews..."));
        break;
    case dml::scan::MainScan::ScanAudio:
        ui->StepLabel->setText(tr("Scan all audios..."));
        break;
    case dml::scan::MainScan::ScanVideo:
        ui->StepLabel->setText(tr("Scan all videos..."));
        break;
    }

    ui->ProgressBar->setValue(step);
}