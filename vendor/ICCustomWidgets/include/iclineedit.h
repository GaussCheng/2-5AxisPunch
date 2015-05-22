#ifndef ICLINEEDIT_H
#define ICLINEEDIT_H

#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>
#include <qmath.h>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICLineEdit : public QLineEdit
{
    Q_OBJECT
    /*
     *属性
    */
    Q_PROPERTY(int decimals READ Decimals WRITE setDecimals)
    Q_PROPERTY(QString suffix READ Suffix WRITE setSuffix)
    Q_PROPERTY(QString prefix READ Prefix WRITE setPrefix)
    Q_PROPERTY(QString specialValue READ SpecialValueText WRITE setSpecialValueText)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)


public:
    explicit ICLineEdit(QWidget *parent = 0);
    virtual ~ICLineEdit();
    QSize sizeHint() const { return QSize(76, 32);}
    QSize minimumSizeHint() const { return sizeHint();}

    bool IsOnlyNumeric() const { return isOnlyNumeric_;}
    void SetOnlyNumeric(bool numeric)
    {
        isOnlyNumeric_ = numeric;
        isOnlyNumeric_ ? setInputMethodHints(Qt::ImhFormattedNumbersOnly) : setInputMethodHints(Qt::ImhNone);
    }
    double maximum() { return doubleValidator_->top();}
    double minimum() { return doubleValidator_->bottom();}
    int Decimals() const { return decimals_;}
    void SetDecimals(int decimals);
    void setDecimals(int decimals);
    void SetSuffix(const QString & suffix );
    void setSuffix(const QString & suffix );
    QString Suffix() const { return suffix_;}
    QString Prefix() const { return prefix_;}
    void SetPrefix(const QString & prefix);
    void setPrefix(const QString & prefix);
    QString SpecialValueText() const{ return specialValueText_;}
    void SetSpecialValueText(const QString& text) {specialValueText_ = text;}
    void setSpecialValueText(const QString& text) {specialValueText_ = text;}
    int SpecialValue() const { return specialValue_;}
    void SetSpecialValue(int val) { specialValue_ = val;}

    double LastValue() const { return lastValue_;}
    QString LastValueStr() const { return QString("%1").arg(lastValue_,0, 'f', Decimals());}

    double FormatInt(int value);//
    void SetValidator( const QValidator * v );//

    void BindSpecialValueText(int val, const QString& text) {specialValue_ = val; specialValueText_ = text;}

    QString TipsFormat() const { return tipsFormat_;}
    void SetTipsFormat(const QString& format) { tipsFormat_ = format;UpdateTips_();}

//    void SetAddValue();
//    void SetSubValue();

    double Value();
    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);
    void BlockValidator(bool isBlock) {isBlockValidator_ = isBlock;}

//    void* operator new(size_t size);

//    void operator delete(void* p);

//    static void ExpandObjectPool(size_t size);
//    static void FreeObjectPool();

signals:
    void ValueChanged(double value); //obsoleted
    void ValueTextChanged(const QString& text);
public slots:
    void SetMaximum(double max);
    void SetMinimum(double min);
    void setMaximum(double max);
    void setMinimum(double min);
    void SetRange(double min, double max);
    void SetValue(double value);
    void SetValue(int value);
    void setText(const QString &text) {QLineEdit::setText(text); oldValue_ = text.toDouble();}
//    void UpdateTip(){if(validator() != NULL) UpdateTips_();}
private slots:
    void OnEditFinished();
protected:
    static QList<int> numericKeys_;
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void changeEvent(QEvent *e);
#ifdef Q_WS_QWS
    void inputMethodEvent ( QInputMethodEvent * event ) ;
#endif
private:
    void WrapSuffix_(const QString& text);
    void WrapPrefix_(const QString& text);
    void WrapText_(const QString& text);
    void UpdateTips_();

protected:
//    static QList<int> numericKeys_;

private:
    QString wrapText_;
    double oldValue_;
    double lastValue_;
    bool isOnlyNumeric_;
    int decimals_;
    QDoubleValidator* doubleValidator_;
    QString suffix_;
    QString specialValueText_;
    QString prefix_;
    int specialValue_;
    QString tipsFormat_;
    bool isBlockValidator_;
};

inline void ICLineEdit::SetMaximum(double max)
{
    if(doubleValidator_ == NULL)
    {
        doubleValidator_ = new QDoubleValidator();
        doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    }
    doubleValidator_->setTop(max);

    UpdateTips_();
}

inline void ICLineEdit::setMaximum(double max)
{
    if(doubleValidator_ == NULL)
    {
        doubleValidator_ = new QDoubleValidator();
        doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    }
    doubleValidator_->setTop(max);

    UpdateTips_();
}

inline void ICLineEdit::SetMinimum(double min)
{
    if(doubleValidator_ == NULL)
    {
        doubleValidator_ = new QDoubleValidator();
        doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    }
    doubleValidator_->setBottom(min);
    UpdateTips_();
}


inline void ICLineEdit::setMinimum(double min)
{
    if(doubleValidator_ == NULL)
    {
        doubleValidator_ = new QDoubleValidator();
        doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    }
    doubleValidator_->setBottom(min);
    UpdateTips_();
}

inline void ICLineEdit::SetRange(double min, double max)
{
    if(doubleValidator_ == NULL)
    {
        doubleValidator_ = new QDoubleValidator();
        doubleValidator_->setNotation(QDoubleValidator::StandardNotation);
    }
    doubleValidator_->setBottom(min);
    doubleValidator_->setTop(max);
//    SetValidator(doubleValidator_);
    UpdateTips_();
}

inline void ICLineEdit::SetDecimals(int decimals)
{
    if(decimals < 0)
    {
        decimals = 0;
    }
    decimals_ = decimals;
    WrapText_(QString("%1").arg(text().left(text().size() - Suffix().size()).toDouble(), 0, 'f', decimals));
//    wrapText_.clear();
}

inline void ICLineEdit::setDecimals(int decimals)
{
    if(decimals < 0)
    {
        decimals = 0;
    }
    decimals_ = decimals;
    WrapText_(QString("%1").arg(text().left(text().size() - Suffix().size()).toDouble(), 0, 'f', decimals));
//    wrapText_.clear();
}

inline void ICLineEdit::SetSuffix(const QString &suffix)
{
    int chopSize = suffix_.size();
    suffix_ = suffix;
    WrapSuffix_(text().left(text().size() - chopSize));
//    setText(text() + Suffix); //这句能不能代替上面这行???
}

inline void ICLineEdit::setSuffix(const QString &suffix)
{
    int chopSize = suffix_.size();
    suffix_ = suffix;
    WrapSuffix_(text().left(text().size() - chopSize));
//    setText(text() + Suffix); //
}

inline void ICLineEdit::SetPrefix(const QString &prefix)
{
    int chopSize = prefix_.size();
    prefix_ = prefix;
    WrapPrefix_(text().right(text().size() - chopSize));
}


inline void ICLineEdit::setPrefix(const QString &prefix)
{
    int chopSize = prefix_.size();
    prefix_ = prefix;
    WrapPrefix_(text().right(text().size() - chopSize));
}

inline double ICLineEdit::FormatInt(int value)
{
    return value / qPow(10, Decimals());
}

inline void ICLineEdit::UpdateTips_()
{
    if(doubleValidator_ != NULL)
    {
#ifndef QT_NO_STATUSTIP
        setStatusTip(TipsFormat().arg(doubleValidator_->top(), 0, 'f', Decimals()).arg(doubleValidator_->bottom(), 0, 'f', Decimals()));
#endif
        return;
    }
    const QIntValidator* val = qobject_cast<const QIntValidator*>(validator());
    const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(validator());
    if(val != NULL)
    {
#ifndef QT_NO_STATUSTIP
        setStatusTip(TipsFormat().arg(val->top()).arg(val->bottom()));
#endif
    }
    else if(dv != NULL)
    {
#ifndef QT_NO_STATUSTIP
        setStatusTip(TipsFormat().arg(dv->top(), 0, 'f', Decimals()).arg(dv->bottom(), 0, 'f', Decimals()));
#endif
    }
}

inline void ICLineEdit::SetValidator(const QValidator *v)
{
    const QIntValidator* val = qobject_cast<const QIntValidator*>(v);
    const QDoubleValidator* dv = qobject_cast<const QDoubleValidator*>(v);
    if(val != NULL)
    {
#ifndef QT_NO_STATUSTIP
        setStatusTip(TipsFormat().arg(val->top()).arg(val->bottom()));
#endif
    }
    else if(dv != NULL)
    {
#ifndef QT_NO_STATUSTIP
        setStatusTip(TipsFormat().arg(dv->top(), 0, 'f', Decimals()).arg(dv->bottom(), 0, 'f', Decimals()));
#endif
    }
    setValidator(v);
}

inline void ICLineEdit::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

//inline void ICLineEdit::ExpandObjectPool(size_t size)
//{
//    if(static_cast<int>(size) < objectPool_.size())
//    {
//        return;
//    }
//    size = size - objectPool_.size();
////    ICLineEdit *objects[size];
//    ICLineEdit *objects = (ICLineEdit*)malloc(size * sizeof(ICLineEdit));
//    for(size_t i = 0; i != size; ++i)
//    {
//        objectPool_.append(objects + i);
//    }
//}

//inline void ICLineEdit::FreeObjectPool()
//{
//    for(int i = 0; i != objectPool_.size(); ++i)
//    {
//        free(objectPool_[i]);
//    }
//}

//inline void* ICLineEdit::operator new(size_t size)
//{
//    Q_UNUSED(size)
//    if(objectPool_.isEmpty())
//    {
//        ExpandObjectPool(64);
//    }
//    void* ret = objectPool_.back();
//    objectPool_.pop_back();
//    return ret;
//}

//inline void ICLineEdit::operator delete(void *p)
//{
//    objectPool_.append(p);
//}

#endif // ICLINEEDIT_H

