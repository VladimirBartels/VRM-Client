#include "generics.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

bool writeLog(QString text)
{
    QString filename = "/home/pi/Qt/VRM-Client/Log.txt";

    QFile file(filename);

    if (file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString() << " " << text << endl;

        file.flush();
        file.close();

        return true;
    }
    else
    {
        return false;
    }
}
