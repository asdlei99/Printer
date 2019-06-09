#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = 0);
    ~InputDialog();

    bool connectInit();
    bool getOption();
    QString &getText();

    void setTitle(QString title);
    void setLabel(QString label);

signals:
    void finish();

public slots:
    void slotToolBtnYes();
    void slotToolBtnCancel();

private:
    Ui::InputDialog *ui;

    bool option;
    QString text;
};

#endif // INPUTDIALOG_H
