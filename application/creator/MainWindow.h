#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <QMainWindow>

#include "ui_MainWindow.h"
#include "ProgressDialog.h"

#include <core/Video.h>
#include <core/Information.h>

#include <scan/ScanShow.h>
#include <scan/MainScan.h>

namespace Ui { class MainWindow; }

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    
    void openShow(const QString &filename);
    
    void closeShow();
    
    void reloadShow();

public slots:
    void focusObjectChanged(QObject *focusObject);
    void import(const QString& path);

protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void onNewShowRequested();
    void onOpenShowRequested();
    void onSaveShowRequested();
    void onSaveAsShowRequested();
    void onImportShowRequested();
    void onQuitRequested();
    void onEditConfigFileRequested();
    void onSelectConfigFileRequested();
    void onSelectAFDISchemaFileRequested();
    void onHelpContentRequested();
    void onAboutRequested();
    void onAboutQtRequested();
    
    void onInformationChanged(const dml::core::Information&);
    void onVideoListChanged(const dml::core::VideoList&);
    void onAudioListChanged(const dml::core::AudioList&);
    void onSubtitlesListChanged(const dml::core::SubtitlesList&);

    void onNotifyReloadShow();
    void onNotifyProgress(dml::scan::MainScan::ScanStep step);
    void onNotifyFinished();

    void onAbortScan();

private:
    void loadStoredData();
    void storeData();
    void updateTexts();
    bool userConfirmClosingCurrentShow();
    
    // Current show.
    dml::scan::ScanShow _currentShow;
    QString     _currentShowFilename;

    // Main window.
    Ui::MainWindow   _ui;

    // Scan.
    QString             _configFilename;
    QString             _schemaFilename;
    dml::scan::MainScan* _scan;
    ProgressDialog      _scanDialog;
};

#endif
