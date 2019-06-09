#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void formInit();

private slots:
    void buttonClick();

private:
    Ui::Dialog *ui;

    QString labelQssRed;
    QString labelQssGreen;
    QString labelQssYellow;

};

#endif // DIALOG_H
