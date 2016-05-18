#ifndef SIMULATEKNOB_H
#define SIMULATEKNOB_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

class QListWidgetItem;

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

    void on_onlineMachineList_itemDoubleClicked(QListWidgetItem *item);
    void OnConnectedClient();
    void OnDisconnectedClient();
    void OnReadyReadClient();

private:
    Ui::SimulateKnob *ui;
    QTcpServer server_;
    QTcpSocket connectedSocket_;
//    QMap<QString, QHostAddress> onlineClients_;
    QMap<QListWidgetItem*, QHostAddress> onlineClients_;
    QMap<QString, QListWidgetItem*> addrToItems_;
};

#endif // SIMULATEKNOB_H
