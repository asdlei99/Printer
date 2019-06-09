#include "dialog.h"
#include "ui_dialog.h"
#include "iconhelper.h"
#include "keyBoard.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->formInit();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::formInit()
{
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    /* 设置当前显示界面 */
    ui->stackedWidget_main->setCurrentIndex(0);

    /* 设置顶行风格 */
    ui->widget_menu->setProperty("nav", "top");
    ui->widget_top->setProperty("form", "title");

    ui->label_title->setFont(QFont("Microsoft Yahei", 20));
    ui->label_title->setText("打印机管理平台");

    /* 设置按钮 */
    QList<QToolButton *> tbtns = ui->widget_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        btn->setCheckable(true); // 选中效果
        connect(btn, &QToolButton::clicked, this, &Dialog::buttonClick);
    }
    ui->toolButton_mainPage->setChecked(true); // 设置为被选中

    ui->label_printer->setFont(QFont("Microsoft Yahei", 10));
    ui->label_printer->setText("选择打印机");

    IconHelper::Instance()->setIcon(ui->label_titilIcon, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->label_printerIcon, QChar(0xf108), 10);
    IconHelper::Instance()->setIcon(ui->label_transferMassageIcon, QChar(0xf256), 10);
    IconHelper::Instance()->setIcon(ui->label_wifiSelectIcon, QChar(0xf1eb), 10);
    IconHelper::Instance()->setIcon(ui->label_connectServerIcon, QChar(0xf233), 10);
    IconHelper::Instance()->setIcon(ui->label_sysSettingIcon, QChar(0xf185), 10);

    labelQssRed = QString("QLabel{color:rgb(%1);}").arg("255,94,94");
    labelQssGreen = QString("QLabel{color:rgb(%1);}").arg("128,255,0");
    labelQssYellow = QString("QLabel{color:rgb(%1);}").arg("255,255,80");

    ui->label_serverVal->setStyleSheet(labelQssRed);
    ui->label_wifiVal->setStyleSheet(labelQssRed);
    ui->label_printerVal->setStyleSheet(labelQssRed);
    ui->label_serverVal->setText("未连接");
    ui->label_wifiVal->setText("未连接");
    ui->label_printerVal->setText("未选择");

    QList<QLineEdit *> lineEdits = ui->stackedWidget_main->findChildren<QLineEdit *>();
    foreach (QLineEdit *lineEdit, lineEdits)
    {
        connect(lineEdit, &QLineEdit::selectionChanged, KeyBoard::getInstance(), &KeyBoard::slotLineEditSelect);
    }
}

void Dialog::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    QList<QToolButton *> tbtns = ui->widget_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        if (btn == b)
        {
            btn->setChecked(true);
        }
        else
        {
            btn->setChecked(false);
        }
    }

    if(name == "主界面")
    {
        ui->stackedWidget_main->setCurrentIndex(0);
    }
    else if(name == "设置")
    {
        ui->stackedWidget_main->setCurrentIndex(1);
    }
}
