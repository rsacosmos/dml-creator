#ifndef __DML_CREATOR_SHOWPROPERTIESWIDGET_H__
#define __DML_CREATOR_SHOWPROPERTIESWIDGET_H__

#include <QWidget>
#include <core/Show.h>
#include <core/Information.h>

namespace Ui { class InformationWidget; }

class InformationWidget: public QWidget
{
    Q_OBJECT

public:
    InformationWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~InformationWidget();

    void reloadInformation(const dml::core::Information&);
    
    void updateTexts(); /* to be renamed in retranslateUi() */

    QStringList producerList() const;
    void setProducerList(const QStringList&);

    QStringList copyrightList() const;
    void setCopyrightList(const QStringList&);

signals:
    void informationChanged(const dml::core::Information&);

private slots:
    void refreshInformation();

private:
    // Main window reference.
    Ui::InformationWidget *_ui;
};

#endif // __DML_CREATOR_SHOWPROPERTIESWIDGET_H__
