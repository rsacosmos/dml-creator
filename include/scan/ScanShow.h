#ifndef __DML_SCANSHOW_H__
#define __DML_SCANSHOW_H__

#include "scan/Export.h"

#include <QObject>
#include <QMutex>
#include "core/Show.h"


namespace dml {
namespace scan {

class SCAN_EXPORT ScanShow : public QObject, public dml::core::Show
{
    Q_OBJECT
public:
    ScanShow(QObject* parent = NULL);
    ScanShow(const dml::core::Show& show, QObject* parent = NULL);
    ScanShow(const dml::scan::ScanShow& show, QObject* parent = NULL);

    ScanShow& operator=(const ScanShow& scanShow);

    void addAudio(const dml::core::Audio& audio);
    void addSubtitles(const dml::core::Subtitles &subtitles);
    void addVideo(const dml::core::Video& video);

    bool modified() const { return _modified; }
    void setModified(const bool& modified) { _modified = modified; }

    void ScanShow::refreshGUI();

    QMutex* locker() { return &_locker; }

signals:
    void notifyReload();

private:
    QMutex _locker;
    bool _modified;

}; // class ScanShow

} // namespace scan
} // namespace dml

#endif // __DML_SCANSHOW_H__
