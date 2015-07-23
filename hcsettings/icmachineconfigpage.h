#ifndef ICMACHINECONFIGPAGE_H
#define ICMACHINECONFIGPAGE_H

#include <QWidget>
#include <QMap>

class ICLineEditWrapper;

namespace Ui {
    class ICMachineConfigPage;
}

class ICMachineConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ICMachineConfigPage(QWidget *parent = 0);
    ~ICMachineConfigPage();

protected:
    void changeEvent(QEvent *e);
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private:
    void UpdateAxisDefine_();
    void ShowWidgets_(QList<QWidget*>& widgets);
    void HideWidgets_(QList<QWidget*>& widgets);
    Ui::ICMachineConfigPage *ui;
    QList<ICLineEditWrapper*> wrappers_;

    int axisDefine_;
    QList<QList<QWidget*> > axisWidgets_;

private slots:
    void OnConfigChanged(QObject* w, const QString& newV, const QString& oldV);
    void OnConfigChanged(const QString& text);
    void OnConfigChanged(int v);
    void OnConfigChanged(int v, int ov);
    void OnConfigChanged(bool b);
private:
    QMap<QObject*, int> editorToConfigIDs_;
};

#endif // ICMACHINECONFIGPAGE_H
