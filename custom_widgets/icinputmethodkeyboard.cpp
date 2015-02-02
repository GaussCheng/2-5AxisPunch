#include "icinputmethodkeyboard.h"
#include "ui_icinputmethodkeyboard.h"
#include <QSqlQuery>
#include <QDebug>

QList<QPushButton*> cnButtons;

ICInputMethodKeyboard::ICInputMethodKeyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICInputMethodKeyboard)
{
    ui->setupUi(this);
//    this->setWindowFlags(this->windowFlags() | Qt::Tool);
    this->setFocusPolicy(Qt::NoFocus);
    QList<QToolButton*> inputButtons = findChildren<QToolButton*>();
    for(int i = 0; i != inputButtons.size(); ++i)
    {
        connect(inputButtons.at(i),
                SIGNAL(clicked()),
                &signalMapper_,
                SLOT(map()));
        signalMapper_.setMapping(inputButtons.at(i), inputButtons.at(i)->text());
    }
    connect(&signalMapper_,
            SIGNAL(mapped(QString)),
            SLOT(OnInputButtonClicked(QString)));
    ui->matchContainer->hide();
    cnButtons<<ui->cn_1<<ui->cn_2<<ui->cn_3<<ui->cn_4<<ui->cn_5;
    for(int i = 0; i != cnButtons.size(); ++i)
    {
        connect(cnButtons.at(i),
                SIGNAL(clicked()),
                SLOT(OnCnButtonClicked()));
    }
}

ICInputMethodKeyboard::~ICInputMethodKeyboard()
{
    delete ui;
}

void ICInputMethodKeyboard::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString currentPy;
QStringList matchingList;
int currentMachingGroup = 0;

QStringList Matching(const QString& py)
{
    QStringList ret;
    QSqlQuery query;
//    query.exec(QString("SELECT DISTINCT HanZi FROM tb_zh_CN_gb2312 WHERE PinYin Like '%0%' ORDER BY Freq DESC LIMIT %1, %2")
//               .arg(py).arg(currentMachingGroup * cnButtons.size()).arg(cnButtons.size()));
    query.exec(QString("SELECT HanZi FROM tb_zh_CN_gb2312 WHERE PinYin Like '%0%' ORDER BY Freq DESC LIMIT %1, %2")
               .arg(py).arg(currentMachingGroup * cnButtons.size()).arg(cnButtons.size()));
    while(query.next())
    {
        ret.append(query.value(0).toString());
    }
    return ret;
}

void ICInputMethodKeyboard::on_btn_ent_clicked()
{
    this->accept();
}


void ICInputMethodKeyboard::OnInputButtonClicked(const QString &text)
{
    if(ui->btn_shift->isChecked())
    {
        if(text.at(0).isDigit())
            editor_->insertPlainText(text);
        else if(text.size() > 1)
            editor_->insertPlainText(text.left(1));
        else
            editor_->insertPlainText(text.toUpper());
    }
    else
    {
        if(text.at(0).isDigit())
            editor_->insertPlainText(text);
        else if(text.size() > 1)
            editor_->insertPlainText(text.right(1));
        else if(IsChEn_())
        {
//            editor_->insertPlainText(text.toLower());
            currentPy += text;
            ui->pyLabel->setText(currentPy.toLower());
            matchingList =  Matching(currentPy);
            currentMachingGroup = 0;
            if(matchingList.size() > cnButtons.size())
            {
                ShowMaching_(matchingList.mid(0, cnButtons.size()));
            }
            else
            {
                ShowMaching_(matchingList);
            }

        }
        else
        {
            editor_->insertPlainText(text.toLower());
        }
    }
}

void ICInputMethodKeyboard::on_btn_sw_clicked()
{
    if(IsChEn_())
    {
        ui->btn_sw->setText("EN");
        ui->matchContainer->hide();
    }
    else
    {
        ui->btn_sw->setText("CH");
        ui->matchContainer->show();
    }
}

bool ICInputMethodKeyboard::IsChEn_() const
{
    return ui->btn_sw->text() == "CH";
}

void ICInputMethodKeyboard::on_btn_bs_clicked()
{
    if(IsChEn_() && !currentPy.isEmpty())
    {
        currentPy.chop(1);
        if(currentPy.isEmpty())
        {
            matchingList.clear();
        }
        else
            matchingList =  Matching(currentPy);
        ui->pyLabel->setText(currentPy.toLower());
        currentMachingGroup = 0;
        if(matchingList.size() > cnButtons.size())
        {
            ShowMaching_(matchingList.mid(0, cnButtons.size()));
        }
        else
        {
            ShowMaching_(matchingList);
        }
    }
    else
    {
        QKeyEvent *e = new QKeyEvent(QKeyEvent::KeyPress,
                                     Qt::Key_Backspace,
                                     Qt::NoModifier);
        qApp->postEvent(editor_, e);
    }
}

void ICInputMethodKeyboard::on_btn_space_clicked()
{
    editor_->insertPlainText(" ");
}

void ICInputMethodKeyboard::ShowMaching_(const QStringList &texts)
{
    int restSize = cnButtons.size() - texts.size();
    for(int i = 0 ; i != texts.size(); ++i)
    {
        cnButtons[i]->setText(texts.at(i));
    }
    for(int i = texts.size(); i < restSize; ++i)
    {
        cnButtons[i]->setText("");
    }
//    ui->label->setText(texts.join(" "));
}

void ICInputMethodKeyboard::on_nextGroup_clicked()
{
//    int groups = matchingList.size() / cnButtons.size();
//    groups = matchingList.size() % cnButtons.size() ? groups + 1 : groups;

//    currentMachingGroup = ++currentMachingGroup % groups;
    ++currentMachingGroup;
    matchingList = Matching(currentPy);
    ShowMaching_(matchingList.mid(0, cnButtons.size()));
}

void ICInputMethodKeyboard::on_upGroup_clicked()
{
//    int groups = matchingList.size() / cnButtons.size();
//    groups = matchingList.size() % cnButtons.size() ? groups + 1 : groups;

//    if(currentMachingGroup == 0) currentMachingGroup = groups - 1;
//    else
//        currentMachingGroup = --currentMachingGroup % groups;
    if(currentMachingGroup > 0)
    {
        --currentMachingGroup;
    }
    matchingList = Matching(currentPy);
    ShowMaching_(matchingList.mid(0, cnButtons.size()));
}

void ICInputMethodKeyboard::OnCnButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    editor_->insertPlainText(btn->text());
    QSqlQuery query;
    query.exec(QString("UPDATE tb_zh_CN_gb2312 SET Freq = Freq + 1 WHERE HanZi = '%1' AND PinYin = '%2'")
               .arg(btn->text()).arg(currentPy.toLower()));
    currentPy.clear();
    matchingList.clear();
    ShowMaching_(QStringList());
    currentMachingGroup = 0;
    ui->pyLabel->setText(currentPy);
}
