#include "layout.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

QString readTextFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Layout w;

   QString css = readTextFile(":/Style/Style/qss.qss");
    a.setStyleSheet(css);

    w.show();
    return a.exec();
}
