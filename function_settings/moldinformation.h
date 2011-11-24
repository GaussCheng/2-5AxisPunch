#ifndef MOLDINFORMATION_H
#define MOLDINFORMATION_H

#include <QFrame>
#include <QModelIndex>
#include <QFileInfoList>
#include <QList>

#include "icmold.h"

namespace Ui {
    class MoldInformation;
}

class MoldInformation : public QFrame
{
    Q_OBJECT

public:
    static MoldInformation * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new MoldInformation;
        }
        return instance_;
    }

    ~MoldInformation();

protected:
    void changeEvent(QEvent *);

signals:
    void NewFileCreated(const QString & moldName);
    void DeleteFile(const QString & moldName);
    void LoadFileInfoButtonClicked(const QString & moldName);

private:
    bool CreateNewSourceFile(const QString & fileName);
    void AddNewInTableWidget(const QString & fileName, const QString & dateTime);
    bool CopySourceFile(const QString & originFileName, const QString & targetFileName);
    bool DeleteSourceFile(const QString & fileName);

    void UpdateInformationTable();
    void CreateFileHelper_(QList<ICMoldItem> &items, int axis, int servo, int pneumatic);

private:
    Ui::MoldInformation *ui;
    static MoldInformation * instance_;
    explicit MoldInformation(QWidget *parent = 0);
    QString recordFilePath_;
    QFileInfoList fileInfoList_;

private slots:
    void on_informationTableWidget_clicked(QModelIndex index);
    void on_deleteToolButton_clicked();
    void on_loadToolButton_clicked();
    void on_copyToolButton_clicked();
    void on_newToolButton_clicked();
};

#endif // MOLDINFORMATION_H
