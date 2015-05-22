/********************************************************************************
** Form generated from reading UI file 'icpassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICPASSWORDDIALOG_H
#define UI_ICPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <icbutton.h>
#include "icinputmethodedit.h"
#include "iclabel.h"

QT_BEGIN_NAMESPACE

class Ui_ICPasswordDialog
{
public:
    QVBoxLayout *verticalLayout;
    ICLabel *tips;
    QFormLayout *formLayout;
    ICLabel *label;
    ICInputMethodEdit *firstPassword;
    ICLabel *label_2;
    ICInputMethodEdit *secPassword;
    QHBoxLayout *horizontalLayout;
    ICButton *okButton;
    ICButton *cancelButton;

    void setupUi(QDialog *ICPasswordDialog)
    {
        if (ICPasswordDialog->objectName().isEmpty())
            ICPasswordDialog->setObjectName(QString::fromUtf8("ICPasswordDialog"));
        ICPasswordDialog->setWindowModality(Qt::NonModal);
        ICPasswordDialog->resize(278, 153);
        verticalLayout = new QVBoxLayout(ICPasswordDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tips = new ICLabel(ICPasswordDialog);
        tips->setObjectName(QString::fromUtf8("tips"));

        verticalLayout->addWidget(tips);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new ICLabel(ICPasswordDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        firstPassword = new ICInputMethodEdit(ICPasswordDialog);
        firstPassword->setObjectName(QString::fromUtf8("firstPassword"));
        firstPassword->setStyleSheet(QString::fromUtf8("ICInputMethodEdit{\n"
"    padding:0px 5px;\n"
"    border: 2px solid gray;\n"
"    border-radius: 10px;\n"
"    padding: 0 8px;\n"
"    background: yellow;\n"
"    selection-background-color: darkgray\n"
"}\n"
"\n"
"ICInputMethodEdit:focus{\n"
"    background: rgb(246, 153, 63);\n"
"    selection-background-color: darkgray\n"
"}"));

        formLayout->setWidget(0, QFormLayout::FieldRole, firstPassword);

        label_2 = new ICLabel(ICPasswordDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        secPassword = new ICInputMethodEdit(ICPasswordDialog);
        secPassword->setObjectName(QString::fromUtf8("secPassword"));
        secPassword->setStyleSheet(QString::fromUtf8("ICInputMethodEdit{\n"
"    padding:0px 5px;\n"
"    border: 2px solid gray;\n"
"    border-radius: 10px;\n"
"    padding: 0 8px;\n"
"    background: yellow;\n"
"    selection-background-color: darkgray\n"
"}\n"
"\n"
"ICInputMethodEdit:focus{\n"
"    background: rgb(246, 153, 63);\n"
"    selection-background-color: darkgray\n"
"}"));

        formLayout->setWidget(1, QFormLayout::FieldRole, secPassword);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        okButton = new ICButton(ICPasswordDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(0, 30));
        okButton->setMaximumSize(QSize(16777215, 30));
        okButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(okButton);

        cancelButton = new ICButton(ICPasswordDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(0, 30));
        cancelButton->setMaximumSize(QSize(16777215, 30));
        cancelButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ICPasswordDialog);

        QMetaObject::connectSlotsByName(ICPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ICPasswordDialog)
    {
        ICPasswordDialog->setWindowTitle(QApplication::translate("ICPasswordDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        tips->setText(QString());
        label->setText(QApplication::translate("ICPasswordDialog", "Input Password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ICPasswordDialog", "Input Again:", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("ICPasswordDialog", "Ok", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ICPasswordDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ICPasswordDialog: public Ui_ICPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICPASSWORDDIALOG_H
