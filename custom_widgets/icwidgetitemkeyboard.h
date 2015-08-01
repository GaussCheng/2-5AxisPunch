#ifndef ICWIDGETITEMKEYBOARD_H
#define ICWIDGETITEMKEYBOARD_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QSignalMapper>

namespace Ui {
class ICWidgetItemKeyboard;
}

class ICWidgetItemKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit ICWidgetItemKeyboard(QWidget *parent = 0);
    ~ICWidgetItemKeyboard();

    void SetTextEditor(QTableWidgetItem* editor) { editor_ = editor;}
    void Reset();
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_btn_ent_clicked();
    void OnInputButtonClicked(const QString& text);
    void ShowMaching_(const QStringList& texts);
    void InsertContent(const QString& content);

    void on_btn_sw_clicked();

    void on_btn_bs_clicked();

    void on_btn_space_clicked();

    void on_nextGroup_clicked();

    void on_upGroup_clicked();

    void OnCnButtonClicked();

private:
    bool IsChEn_() const;
    QStringList Matching(const QString& py);

    Ui::ICWidgetItemKeyboard *ui;
    QTableWidgetItem * editor_;
    QSignalMapper signalMapper_;
    QList<QPushButton*> cnButtons;
    QString content_;
    QString currentPy;
    QStringList matchingList;
    int currentMachingGroup;
};

#endif // ICWidgetItemKeyboard_H
