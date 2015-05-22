#ifndef ICSLIDINGWIDGET_H
#define ICSLIDINGWIDGET_H

#include <QWidget>
#include <QWidgetList>
#include "ICCustomWidgets_global.h"
#include <QStackedLayout>

#ifndef QT_NO_ANIMATION
class QPropertyAnimation;
class QAnimationGroup;
#endif

class ICCUSTOMWIDGETSSHARED_EXPORT ICSlidingWidget : public QWidget
{
    Q_OBJECT
public:
    enum ICSlidingWay{
        SlidingWay_None,
#ifndef QT_NO_ANIMATION
        SlidingWay_SlidingUp,
        SlidingWay_SlidingDown,
        SlidingWay_SlidingLeft,
        SlidingWay_SlidingRight
#endif
    };

    explicit ICSlidingWidget(QWidget *parent = 0);
    ~ICSlidingWidget();

    int AddWidget(QWidget* widget);
    QWidget* Widget(int index) { return stackedLayout_->widget(index);/*widgets_[index];*/}
    void SetCurrentWidget(QWidget* widget, ICSlidingWay way = SlidingWay_None);
    QWidget* CurrentWidget() { return stackedLayout_->currentWidget();/*widgets_[currentIndex_];*/}
    int CurrentIndex() const { return stackedLayout_->currentIndex();/*currentIndex_;*/}
    void SetCurrentIndex(int index, ICSlidingWay way = SlidingWay_None);
    bool IsSliding() const { return isSliding_;}
    void SetSliding(bool isSliding) {isSliding_ = isSliding;}

    int Count() const { return stackedLayout_->count();/*widgets_.size();*/}

signals:
    void currentChanged ( int index );
    void widgetRemoved ( int index );
public slots:
private slots:
    void OnFinished();

protected:
    //    void showEvent(QShowEvent *e);
//    bool eventFilter(QObject *obj, QEvent *e);

private:
#ifndef QT_NO_ANIMATION
    QPropertyAnimation* animation1_;
    QPropertyAnimation* animation2_;
    QAnimationGroup* animationGroup_;
#endif

    QStackedLayout* stackedLayout_;

    QWidget* lastWidget_;
//    QWidgetList widgets_;
    int currentIndex_;
    bool isSliding_;
};

inline void ICSlidingWidget::SetCurrentIndex(int index, ICSlidingWay way)
{
    //    SetCurrentWidget(widgets_[index], way);
    SetCurrentWidget(stackedLayout_->widget(index), way);
}

#endif // ICSLIDINGWIDGET_H
