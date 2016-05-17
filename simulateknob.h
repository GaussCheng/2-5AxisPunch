#ifndef SIMULATEKNOB_H
#define SIMULATEKNOB_H

#include <QDialog>
#include <QTcpServer>

namespace Ui {
class SimulateKnob;
}

class SimulateKnob : public QDialog
{
    Q_OBJECT
    
public:
    explicit SimulateKnob(QWidget *parent = 0);
    ~SimulateKnob();

signals:
    void manualButtonClicked();
    void stopButtonClicked();
    void autoButtonClicked();
    
private slots:
    void on_scanButton_clicked();
    void onClientServerUp();
    void onClientServerDown();

private:
    Ui::SimulateKnob *ui;
    QTcpServer server_;
    QList<QHostAddress> onlineClients_;
};

#endif // SIMULATEKNOB_H
