#ifndef HCMANUALADJUSTFRAME_H
#define HCMANUALADJUSTFRAME_H

#include <QWidget>
#include <QSignalMapper>
#include <QBitArray>

namespace Ui {
    class HCManualAdjustFrame;
}

class HCManualAdjustFrame : public QWidget
{
    Q_OBJECT

public:
    explicit HCManualAdjustFrame(QWidget *parent = 0);
    ~HCManualAdjustFrame();

    void ClearStatus();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private slots:
    void OnJogToolButtonClicked(int key);

    void on_jogSWButton_toggled(bool checked);

    void StatusRefreshed();

private:
    Ui::HCManualAdjustFrame *ui;
    QSignalMapper signalMapper_;
    QPixmap plusPic_;
    QPixmap minsPic_;
    QPixmap statusOnPix_;
    QPixmap statusOffPix_;
    QBitArray currentStatus_;
};

#endif // HCMANUALADJUSTFRAME_H
