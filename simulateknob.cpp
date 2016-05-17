#include "simulateknob.h"
#include "ui_simulateknob.h"
#include <QProcess>
#include <QTcpSocket>

SimulateKnob::SimulateKnob(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulateKnob)
{
    ui->setupUi(this);
    connect(ui->manualButton,
            SIGNAL(clicked()),
            SIGNAL(manualButtonClicked()));
    connect(ui->stopButton,
            SIGNAL(clicked()),
            SIGNAL(stopButtonClicked()));
    connect(ui->autoButton,
            SIGNAL(clicked()),
            SIGNAL(autoButtonClicked()));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    server_.listen(QHostAddress::Any, 8888);
    connect(&server_, SIGNAL(newConnection()), SLOT(onClientServerUp()));
}

SimulateKnob::~SimulateKnob()
{
    delete ui;
}

void SimulateKnob::on_scanButton_clicked()
{

}

void SimulateKnob::onClientServerUp()
{
    QTcpSocket* socket = server_.nextPendingConnection();
//    connect(socket, SIGNAL(disconnected()),SLOT(onClientServerDown()));
    QHostAddress clientAddr = socket->peerAddress();
    for(int i = 0;  i< onlineClients_.size(); ++i)
    {
        if(clientAddr == onlineClients_.at(i))
            return;
    }
    onlineClients_.append(clientAddr);
    socket->close();
}

void SimulateKnob::onClientServerDown()
{

}
