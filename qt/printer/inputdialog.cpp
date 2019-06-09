#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "keyBoard.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog),
    option(false)
{
    ui->setupUi(this);

    this->setProperty("form", "title");
    connectInit();
}

InputDialog::~InputDialog()
{
    delete ui;
}

bool InputDialog::connectInit()
{
    connect(ui->lineEdit, &QLineEdit::selectionChanged, KeyBoard::getInstance(), &KeyBoard::slotLineEditSelect);
    connect(ui->toolButton_Yes, &QToolButton::clicked, this, &InputDialog::slotToolBtnYes);
    connect(ui->toolButton_Yes, &QToolButton::clicked, KeyBoard::getInstance(), &KeyBoard::hide);
    connect(ui->toolButton_Cancel, &QToolButton::clicked, this, &InputDialog::slotToolBtnCancel);
    connect(ui->toolButton_Cancel, &QToolButton::clicked, KeyBoard::getInstance(), &KeyBoard::hide);

    return true;
}

void InputDialog::slotToolBtnYes()
{
    option = true;
    text = ui->lineEdit->text();
    this->hide();
    ui->lineEdit->clear();
    finish();
}

void InputDialog::slotToolBtnCancel()
{
    option = false;
    text = ui->lineEdit->text();
    this->hide();
    ui->lineEdit->clear();
    finish();
}

bool InputDialog::getOption()
{
    return option;
}

QString &InputDialog::getText()
{
    return text;
}

void InputDialog::setTitle(QString title)
{
    ui->label_Title->setText(title);
}

void InputDialog::setLabel(QString label)
{
    ui->label_Label->setText(label);
}
