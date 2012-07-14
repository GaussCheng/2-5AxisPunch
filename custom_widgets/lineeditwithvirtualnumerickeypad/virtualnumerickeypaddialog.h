#ifndef VIRTUALNUMERICKEYPADDIALOG_H
#define VIRTUALNUMERICKEYPADDIALOG_H

#include <QDialog>
#include <QScopedPointer>

class QSignalMapper;

namespace Ui {
    class VirtualNumericKeypadDialog;
}

class VirtualNumericKeypadDialog : public QDialog {
    Q_OBJECT
public:
    static VirtualNumericKeypadDialog* Instance()
    {
        if(instance_.isNull())
        {
            instance_.reset(new VirtualNumericKeypadDialog());
        }
        return instance_.data();
    }
    ~VirtualNumericKeypadDialog();

    void ResetDisplay();

protected:
    void changeEvent(QEvent *e);

signals:
    void EnterComplete(const QString &enterString);

public slots:
    void KeyboardClicked(QString buttonNum);

private:
    static QScopedPointer<VirtualNumericKeypadDialog> instance_;
    QSignalMapper * numerickSignalMapper_;
    VirtualNumericKeypadDialog(QWidget * parent = 0);

private:
    Ui::VirtualNumericKeypadDialog *ui;

private slots:
    void on_minusToolButton_clicked();
    void on_cancelToolButton_clicked();
    void on_backspaceToolButton_clicked();
    void on_okToolButton_clicked();
    void on_clearToolButton_clicked();
    void on_addButton_clicked();
};

#endif // VIRTUALNUMERICKEYPADDIALOG_H
