#include "tcpsocket.h"

#include <QDebug>

Tcpsocket::Tcpsocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),          this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));
}

void Tcpsocket::doConnect()
{
    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost("192.168.0.100", 80);

    // we need to wait...
    if(!socket->waitForConnected(10000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void Tcpsocket::doReply(QByteArray data)
{
    qDebug() << "sending...";
    QString str = "Got ";
    socket->write(str.toUtf8() + data);
}

void Tcpsocket::connected()
{
    qDebug() << "connected...";
    socket->write("Hello from Client!");
}

void Tcpsocket::disconnected()
{
    qDebug() << "disconnected...";
}

void Tcpsocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void Tcpsocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    QByteArray data = socket->readAll();
    qDebug() << data;
    doReply(data);

    emit parseData(data);
}
