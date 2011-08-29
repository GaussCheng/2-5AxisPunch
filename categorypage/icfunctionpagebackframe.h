#ifndef ICFUNCTIONPAGEBACKFRAME_H
#define ICFUNCTIONPAGEBACKFRAME_H

#include <QFrame>
#include <QStackedLayout>

class ICFunctionPageFrame;

namespace Ui {
    class ICFunctionPageBackFrame;
}

class ICFunctionPageBackFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICFunctionPageBackFrame(QWidget *parent = 0);
    ~ICFunctionPageBackFrame();

Q_SIGNALS:
    void SettingPageShown(const QString&);

public slots:
    void ShowFunctionSelectPage();

private:
    Ui::ICFunctionPageBackFrame *ui;
    ICFunctionPageFrame * functionPage_;
    QStackedLayout * pageLayout_;
};

#endif // ICFUNCTIONPAGEBACKFRAME_H
