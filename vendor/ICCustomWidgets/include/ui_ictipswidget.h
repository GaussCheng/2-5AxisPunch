/********************************************************************************
** Form generated from reading UI file 'ictipswidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICTIPSWIDGET_H
#define UI_ICTIPSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include "iclabel.h"

QT_BEGIN_NAMESPACE

class Ui_ICTipsWidget
{
public:
    QVBoxLayout *verticalLayout;
    ICLabel *msgLabel;

    void setupUi(QDialog *ICTipsWidget)
    {
        if (ICTipsWidget->objectName().isEmpty())
            ICTipsWidget->setObjectName(QString::fromUtf8("ICTipsWidget"));
        ICTipsWidget->resize(305, 60);
        verticalLayout = new QVBoxLayout(ICTipsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        msgLabel = new ICLabel(ICTipsWidget);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));
        msgLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(msgLabel);


        retranslateUi(ICTipsWidget);

        QMetaObject::connectSlotsByName(ICTipsWidget);
    } // setupUi

    void retranslateUi(QDialog *ICTipsWidget)
    {
        ICTipsWidget->setWindowTitle(QApplication::translate("ICTipsWidget", "Tips", 0, QApplication::UnicodeUTF8));
        msgLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ICTipsWidget: public Ui_ICTipsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICTIPSWIDGET_H
