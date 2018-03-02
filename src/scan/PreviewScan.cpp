#include <scan/PreviewScan.h>

#include <scan/MainScan.h>

#include <QRegExp>

using namespace dml::scan;


bool PreviewScan::isPreview(const dml::conf::Configuration& configuration, const QString &filename)
{
    QRegExp exp(configuration.trailerExtensionPattern());
    return exp.exactMatch(filename);
}

QString PreviewScan::qStringRoot(const QString &a, const QString &b)
{
    QString str;

    for(int i = 0; i < a.length() && i < b.length(); ++i)
    {
        if(a.at(i) == b.at(i))
        {
            str.push_back(a.at(i));
        }
        else
        {
            break;
        }
    }
    return str;
}

void PreviewScan::insertPreviewInExistingVideo(dml::scan::ScanShow& show, QStringList& previewPaths)
{
    dml::core::VideoList videoList = show.videoList();
    for(int i = 0; i < videoList.size(); ++i)
    {
        dml::core::Video video = videoList.at(i);
        if(QString(video.preview().c_str()).isEmpty())
        {
            double bestPreview = 0.;
            QString preview;

            QString videoRelativePath(video.videoName().c_str());

            QString videoFileName = MainScan::fileNameFromFilePath(videoRelativePath);
            QString videoFilePath = MainScan::folderPathFromFilePath(videoRelativePath);
            QStringList videoFoldersPath = videoFilePath.split("/");

            foreach(const QString& previewPath, previewPaths)
            {
                QString previewFileName = MainScan::fileNameFromFilePath(previewPath);
                QString previewFilePath = MainScan::folderPathFromFilePath(previewPath);
                QStringList previewFoldersPath = previewFilePath.split("/");

                double common = 0.;
                for(int i = 0 ; i < videoFoldersPath.size() && i < previewFoldersPath.size() ; ++i)
                {
                    if(videoFoldersPath.at(i) == previewFoldersPath.at(i))
                    {
                        ++common;
                    }
                    else
                    {
                        --common;
                    }
                }
                common-= std::abs(videoFoldersPath.size() - previewFoldersPath.size());

                QString rootVideoAndPreviewFileName = qStringRoot(videoFileName, previewFileName);
                common += (rootVideoAndPreviewFileName.size() / (double)videoFileName.size());

                if(common > 0. && common > bestPreview)
                {
                    bestPreview = common;
                    preview = previewPath;
                }
            }
            if(bestPreview > 0.)
            {
                previewPaths.removeOne(preview);
                video.setPreview(preview.toStdString());
                show.addVideo(video);
            }
        }
    }
}

