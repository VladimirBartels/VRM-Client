#include "tcpsocket.h"
#include "generics.h"

#include <QDebug>

#define RECONNECTION_TIME 10000
#define MAX_RETRY         5


Tcpsocket::Tcpsocket(QObject *parent) : QObject(parent)
{
    _socket = new QTcpSocket(this);
    _tryToReconnect = new QTimer();
    _tryToReconnect->setSingleShot(true);
    _isConnected = false;

    connect(_socket, SIGNAL(connected()),          this, SLOT(connected()));
    connect(_socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
    connect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(_socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));

    connect(_tryToReconnect, SIGNAL(timeout()),    this, SLOT(doConnect()));
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
    static int count = 0;

    if (!_isConnected)
    {
        qDebug() << "connecting...";

        // this is not blocking call
        _socket->connectToHost("192.168.0.100", 80);

        writeLog("Client is trying to connect to the server...");
        // we need to wait...
        if(!_socket->waitForConnected(10000))
        {
            qDebug() << "Error: " << _socket->errorString();
            writeLog("Client could not connect to the server:");
            writeLog(_socket->errorString());

            count++;

            // try to reconnect after some time
            qDebug() << "Socket will try to reconnect after 10s";
            writeLog("Socket will try to reconnect after 10s");
            _tryToReconnect->start(RECONNECTION_TIME);

            if (count == MAX_RETRY)
            {
                writeLog("Client is closing");
                exit(0);
            }
        }
    }
    else
    {
        qDebug() << "Socket is already connected to the server";
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
    writeLog("Client is connected to the server");
    _socket->write("Hello from Client!");

    _isConnected = true;
}

void Tcpsocket::disconnected()
{
    qDebug() << "Server has disconnected...";

    _isConnected = false;
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
