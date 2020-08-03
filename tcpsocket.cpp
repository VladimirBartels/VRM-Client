#include "tcpsocket.h"

tcpsocket::tcpsocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    _counter = 0;

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void tcpsocket::doConnect()
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

void tcpsocket::doReply(QByteArray data)
{
    qDebug() << "sending...";
    QString str = "Got ";
    socket->write(str.toUtf8() + data);
}

void tcpsocket::connected()
{
    qDebug() << "connected...";
    socket->write("Hello from Client!");
}

void tcpsocket::disconnected()
{
    qDebug() << "disconnected...";
}

void tcpsocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void tcpsocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    QByteArray data = socket->readAll();
    qDebug() << data;
    doReply(data);
}
