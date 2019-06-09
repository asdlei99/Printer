#include "dialog.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 加载样式表 */
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    a.setFont(QFont("Microsoft Yahei", 9));

    Dialog w;
    w.show();

    return a.exec();
}
