#ifndef VRMCLIENT_H
#define VRMCLIENT_H

#include "tcpsocket.h"

#include <QObject>

enum eCarSpeed
{
    eNoSpeed    = 0,
    eSlow       = 1,
    eNormal     = 2,
    eFast       = 3,
    eTurbo      = 4,
    eUltraSound = 5,
    eSpeedLast
};

enum eCommands
{
    eMoveForward    = 100,
    eMoveBackward   = 101,
    eMoveLeft       = 102,
    eMoveRight      = 103,
    eTurnLeft       = 104,
    eTurnRight      = 105,
    eStop           = 106,
    eChangeSpeed    = 1050
};

class Vrmclient : public QObject
{
    Q_OBJECT
public:
    Vrmclient();

public slots:
    void parseData(QByteArray data);    // get data from Server via TCP/IP and parse them
                                        // signal is emitted by _mySocket

private:
    void initGPIO();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void turnLeft();
    void turnRight();
    void stop();
    void changeSpeed(uint speed);

    // main interface between client and server based on TCP/IP Protocol
    Tcpsocket *_mySocket;
    uint _speed;
};

#endif // VRMCLIENT_H
