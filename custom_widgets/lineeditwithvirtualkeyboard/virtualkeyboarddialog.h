#ifndef VIRTUALKEYBOARDDIALOG_H
#define VIRTUALKEYBOARDDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class VirtualKeyboardDialog;
}

class VirtualKeyboardDialog : public QDialog {
    Q_OBJECT
public:
    static VirtualKeyboardDialog* Instance()
    {
        if(instance_.isNull())
        {
            instance_.reset(new VirtualKeyboardDialog());
        }
        return instance_.data();
    }

    ~VirtualKeyboardDialog();

    void ResetDisplay();

    QString GetCurrentText() const;

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::VirtualKeyboardDialog *ui;

signals:
    void EnterComplete(const QString &enterString);

private slots:
    void on_backspace_clicked();
    void on_cancel_clicked();
    void on_ok_clicked();

    void KeyboardClicked();
    void on_del_clicked();

private:
    static QScopedPointer<VirtualKeyboardDialog> instance_;
    VirtualKeyboardDialog(QWidget *parent = 0);
};

#endif // VIRTUALKEYBOARDDIALOG_H
