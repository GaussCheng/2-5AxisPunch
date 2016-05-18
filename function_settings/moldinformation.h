#ifndef MOLDINFORMATION_H
#define MOLDINFORMATION_H

#include <QFrame>
#include <QModelIndex>
#include <QFileInfoList>
#include <QList>
#include "ichcsystemsettingsframe.h"

#include "icmold.h"

class QTableWidgetItem;

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

    bool IsStandProgram(const QString& name) const { return standPrograms_.contains(name);}

    ~MoldInformation();


protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *e);

signals:
    void ReadyToLoad(QString);
    void MoldChanged(QString);
//    void NewFileCreated(const QString & moldName);
//    void DeleteFile(const QString & moldName);
//    void LoadFileInfoButtonClicked(const QString & moldName);

private:
    bool CreateNewSourceFile(const QString & fileName);
    void AddNewInTableWidget(const QString & fileName, const QString & dateTime);
    bool CopySourceFile(const QString & originFileName, const QString & targetFileName);
    bool DeleteSourceFile(const QString & fileName);

    void UpdateInformationTable();
    void CreateFileHelper_(QList<ICMoldItem> &items, int axis, int servo, int pneumatic);
    void RefreshFileList();
    bool CheckIsUsbAttached() const ;

private:
    Ui::MoldInformation *ui;
    static MoldInformation * instance_;
    explicit MoldInformation(QWidget *parent = 0);
    QString recordFilePath_;
    QFileInfoList fileInfoList_;
    QStringList standPrograms_;
    QStringList selectedExportItemName_;
    QStringList selectedImportItemName_;
    QStringList acts_ ;
    QString getFileDir_ ;
//    ICHCSystemSettingsFrame ichcsystemsettingsframe_ ;
//    QList<QTableWidgetItem *>itemSelectedList ;

private slots:
//    void on_informationTableWidget_clicked(QModelIndex index);
    void on_deleteToolButton_clicked();
    void on_loadToolButton_clicked();
    void on_copyToolButton_clicked();
    void on_newToolButton_clicked();
    void on_informationTableWidget_itemSelectionChanged();
    void on_importToolButton_clicked();
    void on_exportToolButton_clicked();
    void switchPushButton();
    void on_allToolButton_clicked();
    void on_InverseToolButton_clicked();
    void on_unselectToolButton_clicked();

    void on_destinationFileLineEdit_textChanged(const QString &arg1);
};

#endif // MOLDINFORMATION_H
