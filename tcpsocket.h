#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

class Tcpsocket : public QObject
{
    Q_OBJECT
public:
    explicit Tcpsocket(QObject *parent = nullptr);

    void doConnect();
    void doReply(QByteArray data);

signals:
    void parseData(QByteArray data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
};

#endif // TCPSOCKET_H
