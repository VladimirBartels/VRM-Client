#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

class tcpsocket : public QObject
{
    Q_OBJECT
public:
    explicit tcpsocket(QObject *parent = nullptr);

    void doConnect();
    void doReply(QByteArray data);

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    int _counter;

};

#endif // TCPSOCKET_H
