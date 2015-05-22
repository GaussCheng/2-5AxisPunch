#ifndef ICTABLEVIEW_H
#define ICTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICTableView : public QTableView
{
    Q_OBJECT
public:
    enum UpDownWay {UpDownWay_SingleRow, UpDownWay_Page};
    explicit ICTableView(QWidget *parent = 0);
    ~ICTableView();

    void SetUpKey(int key);
    void SetDownKey(int key);

    UpDownWay CurrentUpDownWay() const { return upDownWay_;}
    void SetUpDownWay(UpDownWay way) { upDownWay_ = way;}

    QList<int> SelectedRowsIndex() const;

    void InvalidCurrentIndex() { setCurrentIndex(QModelIndex());}

    void SetEnableUpAndDown(bool isEn) { isEn_ = isEn; }


    QFont TextFont() const { return font_;}
    void SetTextFont(const QFont& font);

signals:
    void CurrentIndexChanged(int);
public slots:
    void UpKeyClicked() { UpActionTrigger();}
    void DownKeyClicked() {DownActionTrigger();}

protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
private slots:
    void UpActionTrigger();
    void DownActionTrigger();

protected:
    void showEvent(QShowEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *e);
private:
    QAction *upAction_;
    QAction *downAction_;
    QFont font_;
    QWidget* mask_;
    UpDownWay upDownWay_;
    bool isEn_;

};

inline void ICTableView::SetTextFont(const QFont &font)
{
    font_ = font;
    font_.setPixelSize((font_.pointSize() << 2) / 3);
    this->setFont(font_);
    this->horizontalHeader()->setFont(font_);
    this->verticalHeader()->setFont(font_);
}

#endif // ICTABLEVIEW_H
