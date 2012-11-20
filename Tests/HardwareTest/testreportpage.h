#ifndef TESTREPORTPAGE_H
#define TESTREPORTPAGE_H

#include <QWidget>

namespace Ui {
class TestReportPage;
}

class TestReportPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestReportPage(QWidget *parent = 0);
    ~TestReportPage();

    void SetKeyTestResult(const QString& desc);
    void SetColorTestResult(const QString& desc);
    void SetBrightnessTestResult(const QString& desc);
    void SetSoundTestResult(const QString& desc);
    void SetTouchScreenTestResult(const QString& desc);
    void SetLEDTestResult(const QString& desc);
    void SetCommunicateTestResult(const QString& desc);
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_restartButton_clicked();

private:
    Ui::TestReportPage *ui;
};

#endif // TESTREPORTPAGE_H
