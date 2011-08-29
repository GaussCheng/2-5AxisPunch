#ifndef ICINSTRUCTIONPAGEFRAME_H
#define ICINSTRUCTIONPAGEFRAME_H

#include <QFrame>
#include <QStackedLayout>
#include <QMap>

namespace Ui {
    class ICInstructionPageFrame;
}

class ICInstructionPageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICInstructionPageFrame(QWidget *parent = 0, QStackedLayout * functionBackPageLayout = NULL);
    ~ICInstructionPageFrame();

    void ShowProgramMonitor();
protected:
    void changeEvent(QEvent *);

private slots:
    void InstructionPageButtonClicked();
    void ShowProgramEditorPage();
    void LoadFileInRecords(const QString& name);

private:
    void InitInstructionPage();
    void InitSignal();

    void UpdateTranslate();

private:
    Ui::ICInstructionPageFrame *ui;
    QWidget * macroEditor_;
    QWidget * programMonitor_;
//    QWidget * programParam_;
    QWidget * programEditor_;
    QWidget * parameterEditor_;
    QMap <QWidget *, QWidget *> programButtonToPage_;
    QStackedLayout * functionBackPageLayout_;
};

extern ICInstructionPageFrame* icInstructionPage;

#endif // ICINSTRUCTIONPAGEFRAME_H
