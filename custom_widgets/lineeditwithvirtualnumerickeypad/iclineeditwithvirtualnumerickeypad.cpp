#include "iclineeditwithvirtualnumerickeypad.h"

#include <QIntValidator>
#include <QMessageBox>
#include <QWheelEvent>
#include <math.h>

#include "virtualnumerickeypaddialog.h"
#include "icparameterconversion.h"

#include <QDebug>

//public:
ICLineEditWithVirtualNumericKeypad::ICLineEditWithVirtualNumericKeypad(QWidget * parent)
    : QLineEdit(parent),
    decimalPlaces_(0)
{
    this->setReadOnly(true);
    virtualNumericKeypadDialog_ = VirtualNumericKeypadDialog::Instance();
}

ICLineEditWithVirtualNumericKeypad::~ICLineEditWithVirtualNumericKeypad()
{
}

int ICLineEditWithVirtualNumericKeypad::TransThisTextToThisInt() const
{
    return ICParameterConversion::TransTextToThisInt(text(), DecimalPlaces());
}

void ICLineEditWithVirtualNumericKeypad::SetThisIntToThisText(int inputNum)
{
    this->setText(ICParameterConversion::TransThisIntToThisText(inputNum, this->DecimalPlaces()));
}

//protected:
void ICLineEditWithVirtualNumericKeypad::mouseReleaseEvent(QMouseEvent *e)
{
    connect(virtualNumericKeypadDialog_,
            SIGNAL(EnterComplete(QString)),
            this,
            SLOT(SetCurrentText(QString)));

    this->setStyleSheet("background:lightgreen;");
    virtualNumericKeypadDialog_->ResetDisplay();

    virtualNumericKeypadDialog_->exec();
    disconnect(virtualNumericKeypadDialog_,
               SIGNAL(EnterComplete(QString)),
               this,
               SLOT(SetCurrentText(QString)));
    QLineEdit::mouseReleaseEvent(e);
    this->setStyleSheet("");
}

void ICLineEditWithVirtualNumericKeypad::wheelEvent(QWheelEvent * wheelEvent)
{
    int num = ICParameterConversion::TransTextToThisInt(this->text(), this->DecimalPlaces());
    QIntValidator * intValidator = (QIntValidator *)this->validator();
    if(intValidator != NULL)
    {
        num += wheelEvent->delta();
        if(num < intValidator->bottom())
        {
            num = intValidator->bottom();
        }
        else if(num > intValidator->top())
        {
            num = intValidator->top();
        }
        this->setText(ICParameterConversion::TransThisIntToThisText(num, this->DecimalPlaces()));
    }
}

//private slots:
void ICLineEditWithVirtualNumericKeypad::SetCurrentText(const QString &currentText)
{
    QIntValidator * intValidator = (QIntValidator *)this->validator();
    if(intValidator != NULL)
    {
        if(this->DecimalPlaces() > 0
           && currentText.contains(QChar('.'))
            && currentText.split(".").at(1).count() > this->DecimalPlaces())
            {
            QMessageBox::warning(this, tr("Input Error"),
                                 tr("There are only ")
                                 + QString::number(this->DecimalPlaces())
                                 + tr(" decimal place\n")
                                 + tr("Please enter an digit between ")
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->bottom(), this->DecimalPlaces()) + '~'
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->top(), this->DecimalPlaces()),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return;
        }
        else if(this->DecimalPlaces() == 0 && currentText.contains(QChar('.')) )
        {
            QMessageBox::warning(this, tr("Input Error"),
                                 tr("The value must be an integer\n"
                                    "Please enter an integer between ")
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->bottom(), this->DecimalPlaces()) + '~'
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->top(), this->DecimalPlaces()),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return;
        }

        int currentValue = ICParameterConversion::TransTextToThisInt(currentText, this->DecimalPlaces());
        if(currentValue > intValidator->top() || currentValue < intValidator->bottom())
        {
            QMessageBox::warning(this, tr("Out of range"),
                                 tr("The value you entered out of range.\n"
                                    "Please enter an digit between ")
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->bottom(), this->DecimalPlaces()) + '~'
                                 + ICParameterConversion::TransThisIntToThisText(intValidator->top(), this->DecimalPlaces()),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return;
        }

        QString valueStr = currentText;
        if(this->DecimalPlaces() != 0)
        {
            int diff = 0;
            if(currentText.contains("."))
            {
                diff = this->DecimalPlaces() - currentText.split(".").at(1).count();
            }
            else
            {
                diff = this->DecimalPlaces();
                valueStr += ".";
            }

            if(diff > 0)
            {
                for(int i = 0; i < diff; ++i)
                {
                    valueStr += "0";
                }
            }
        }

        this->setText(ICParameterConversion::TransThisIntToThisText(currentValue, DecimalPlaces()));
        this->setText(valueStr);
    }
    else
    {
        this->setText(currentText);
    }
}

ICIncrementalLineEdit::ICIncrementalLineEdit(QWidget *parent)
    : ICLineEditWithVirtualNumericKeypad(parent)
{

}

void ICIncrementalLineEdit::SetCurrentText(const QString &currentText)
{
    QString tempText = currentText;
    if(tempText.isEmpty())
    {
        return;
    }
    if(tempText.at(0).isDigit() || tempText.at(0) == '.')
    {
        tempText.prepend('+');
    }
    ICLineEditWithVirtualNumericKeypad::SetCurrentText(tempText);
}
