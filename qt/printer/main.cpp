#include "dialog.h"
#include <QApplication>
#include <QFile>
#include <log/Log.h>
#include <lib/Parse.h>

int main(int argc, char *argv[])
{
    Parse::parseConfigFile();
    Log::getInstance().initLog(NULL, Log::LogDebug);

    QApplication a(argc, argv);

    /* 加载样式表 */
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        //QString paletteColor = qss.mid(20, 7);
        //qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setPalette(QPalette(QColor(75, 75, 75)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    a.setFont(QFont("Microsoft Yahei", 12));

    Dialog w;
    w.show();

    return a.exec();
}
