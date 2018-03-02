#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

#include <scan/MainScan.h>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

    void setStep(dml::scan::MainScan::ScanStep step);

signals:
    void cancel();

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
