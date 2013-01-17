#ifndef SIMULATEKNOB_H
#define SIMULATEKNOB_H

#include <QDialog>

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
    
private:
    Ui::SimulateKnob *ui;
};

#endif // SIMULATEKNOB_H
