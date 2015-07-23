#ifndef ICHCTIMEFRAME_H
#define ICHCTIMEFRAME_H

#include <QFrame>
#include <QList>
#include <QMap>


class QLineEdit;
class ICLineEditWrapper;

namespace Ui {
    class ICHCTimeFrame;
}

class ICHCTimeFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICHCTimeFrame(QWidget *parent = 0);
    ~ICHCTimeFrame();

protected:
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);
private:
    void InitInterFace();
    void BindingParam_(QLineEdit* edit, int addr);

private:
    Ui::ICHCTimeFrame *ui;
    QList<ICLineEditWrapper*> wrappers_;

private slots:
    void OnConfigChanged(QObject* w, const QString& newV, const QString& oldV);
    void OnConfigChanged(const QString& text);
    void OnConfigChanged(int v);
    void OnConfigChanged(int v, int ov);
    void OnConfigChanged(bool b);
private:
    QMap<QObject*, int> editorToConfigIDs_;
};

#endif // ICHCTIMEFRAME_H
