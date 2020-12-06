#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>

class Tcpsocket : public QObject
{
    Q_OBJECT
public:
    explicit Tcpsocket(QObject *parent = nullptr);
    ~Tcpsocket();

signals:
    void parseData(QByteArray data);    // a signal to client to process data received from server and decide what to do with them

public slots:
    void connected();                   // successfully connected to the server
    void disconnected();                // server is diconnected
    void bytesWritten(qint64 bytes);    // number of bytes written to the server
    void readyRead();                   // got some message from the server

    void doConnect();                   // do connect to server

private:

    void doReply(QByteArray data);      // do reply to server like sending ack

    QTcpSocket *_socket;                // this will establish a connection with the server
    QTimer     *_tryToReconnect;        // try to connect to server every RECONNECTION_TIME
    bool        _isConnected;           // flag is Spcket connected to the server
};

#endif // TCPSOCKET_H
