#ifndef ICTUNEMOLDDIALOG_H
#define ICTUNEMOLDDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class ICTuneMoldDialog;
}

class ICTuneMoldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICTuneMoldDialog(QWidget *parent = 0);
    ~ICTuneMoldDialog();

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void on_cancelButton_clicked();

    void on_startButton_toggled(bool checked);
    void OnTimeOut();
    void RefreshStatus();

private:
    Ui::ICTuneMoldDialog *ui;
    QTimer timer_;
};

#endif // ICTUNEMOLDDIALOG_H
