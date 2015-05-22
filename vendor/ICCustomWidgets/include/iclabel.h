#ifndef ICLABEL_H
#define ICLABEL_H

#include <QLabel>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ICLabel(QWidget *parent = 0);
    virtual ~ICLabel(){}

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    void ChangeText(const QString& text);
    void SetSpecialText(const QString& text) { specialText_ = text;}
    void SetEnSpecialText(bool isEnable)
    {
        if(enSpecialText_ != isEnable)
        {
            enSpecialText_ = isEnable;
            if(isEnable)setText(SpecialText());
        }

    }

    bool EnSpecialText() const { return enSpecialText_;}
    QString SpecialText() const { return specialText_;}


signals:

public slots:

private:
    QString specialText_;
    bool enSpecialText_;
};

inline void ICLabel::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

inline void ICLabel::ChangeText(const QString &text)
{
    if(EnSpecialText())
    {
        return;
    }
    if(text != this->text())
    {
        setText(text);
    }
}


#endif // ICLABEL_H
