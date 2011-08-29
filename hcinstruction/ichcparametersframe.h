#ifndef ICHCPARAMETERSFRAME_H
#define ICHCPARAMETERSFRAME_H

#include <QFrame>
#include <QDir>
#include "icmold.h"

namespace Ui {
    class ICHCParametersFrame;
}

class ICHCParametersFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCParametersFrame(QWidget *parent = 0);
    ~ICHCParametersFrame();

protected:
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private:
    void ReplaceRow(int row);
    void UpdateMoldContent(const QStringList & commadnStrList);

    void UpdateSumSep();

    void UpdateActionMap();

    void InitSignals();

    void UpdateFileInfo_();


private:
    Ui::ICHCParametersFrame *ui;

    ICMoldItem moldItem_;
    bool isCopy_;
    bool isModified_;
    QDir recordsDir_;
    QDir subsDir_;
    QString oldRecordName_;
    int oldSubNum_;
    bool doesRecordEdited_;
    QString oldContentName_;
//    int selectRow_;
    QList<ICMoldItem> editContent_;
//    ICMold *moldFile_;


private slots:
    void on_subroutinesComboBox_activated(int index);
    void on_recordsComboBox_activated(const QString& fileName );
    void UpdateCurrentMoldItem(ICMoldItem);
    void ResetHostParam();
    void on_editToolButton_clicked();
    void on_deleteToolButton_clicked();
    void on_replacementToolButton_clicked();
    void on_insertToolButton_clicked();
    void on_copyToolButton_clicked();
};

#endif // ICHCPARAMETERSFRAME_H
