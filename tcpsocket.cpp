#include "tcpsocket.h"

#include <QDebug>

Tcpsocket::Tcpsocket(QObject *parent) : QObject(parent)
{
    _socket = new QTcpSocket(this);

    connect(_socket, SIGNAL(connected()),          this, SLOT(connected()));
    connect(_socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
    connect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(_socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));
}

Tcpsocket::~Tcpsocket()
{
    if (_socket)
    {
        delete _socket;
        _socket = nullptr;
    }
}

void Tcpsocket::doConnect()
{
    qDebug() << "connecting...";

    // this is not blocking call
    _socket->connectToHost("192.168.0.100", 80);

    // we need to wait...
    if(!_socket->waitForConnected(10000))
    {
        qDebug() << "Error: " << _socket->errorString();
    }
}

void Tcpsocket::doReply(QByteArray data)
{
    qDebug() << "sending...";
    QString str = "Got ";
    _socket->write(str.toUtf8() + data);
}

void Tcpsocket::connected()
{
    qDebug() << "connected...";
    _socket->write("Hello from Client!");
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
    QByteArray data = _socket->readAll();
    qDebug() << data;
    doReply(data);

    emit parseData(data);
}
