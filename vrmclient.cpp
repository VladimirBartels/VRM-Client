#include "vrmclient.h"
#include <wiringPi.h>
#include <softPwm.h>

#include <QDebug>

#define PWM_START_VALUE 15  // [0..RANGE]
#define PWM_RANGE       100

// left side
#define D1_M1_F_PIN        11   // GPIO 17  Left Back
#define D1_M1_B_PIN        13   // GPIO 27
#define D1_M1_ENAPIN       15   // GPIO 2

#define D1_M2_F_PIN        29   // GPIO 5   Left Front
#define D1_M2_B_PIN        31   // GPIO 6
#define D1_M2_ENAPIN       33   // GPIO 13

// right side
#define D2_M1_F_PIN        16   // GPIO 23  Right Front
#define D2_M1_B_PIN        18   // GPIO 24
#define D2_M1_ENAPIN       22   // GPIO 25

#define D2_M2_F_PIN        36   // GPIO 16  Right Back
#define D2_M2_B_PIN        38   // GPIO 20
#define D2_M2_ENAPIN       40   // GPIO 21


Vrmclient::Vrmclient()
{
    _mySocket = new Tcpsocket();
    _mySocket->doConnect();

    _speed = eNoSpeed;

    connect(_mySocket, SIGNAL(parseData(QByteArray)), this, SLOT(parseData(QByteArray)), Qt::UniqueConnection);

    initGPIO();
}

// a command format 10 bytes:
// example: S1:0101E
// [0]   S   - start
// [1]   1   - id of a client
// [2]   :   - spacer between client id and a command id
// [3-6] 101 - a command id
// [7]   E   - end of the command
void Vrmclient::parseData(QByteArray data)
{
    uint commandId = 0;
    uint clientId = 0;
    bool isOk = false;

    QList<QByteArray> commands = data.split('E');

    foreach (QByteArray command, commands)
    {
        if (command.length() == 0) continue;

        qDebug() << "command is " + command;

        if (command.length() == 7 && command.at(0) == 'S' && command.at(2) == ':')
        {
            clientId = command.mid(1, 1).toUInt(&isOk);
            if (!isOk)
            {
                qDebug() << "Wrong format for Client Id";
                continue;
            }
            qDebug() << "Client Id = " + QString::number(clientId);

            commandId = command.mid(3, 4).toUInt(&isOk);    // take 4 bytes from 3rd byte

            if (isOk)
            {
                qDebug() << "Command Id = " + QString::number(commandId);

                if ((commandId >= eChangeSpeed) && (commandId < eChangeSpeed + eSpeedLast))
                {
                    _speed = commandId % eChangeSpeed;
                    commandId = eChangeSpeed;
                }

                switch (commandId)
                {
                case eMoveForward:
                    moveForward();
                    break;
                case eMoveBackward:
                    moveBackward();
                    break;
                case eMoveLeft:
                    moveLeft();
                    break;
                case eMoveRight:
                    moveRight();
                    break;
                case eTurnLeft:
                    turnLeft();
                    break;
                case eTurnRight:
                    turnRight();
                    break;
                case eStop:
                    stop();
                    break;
                case eChangeSpeed:
                    changeSpeed(_speed);
                    break;
                default:
                    qDebug() << "Unknown command";
                    break;
                }
            }
            else
            {
                qDebug() << "Wrong command ID";
            }
        }
        else
        {
            qDebug() << "Wrong command format";
        }
    }
}

void Vrmclient::initGPIO()
{
    //wiringPiSetup();
    //wiringPiSetupGpio();
    wiringPiSetupPhys();

    // init motor 1
    pinMode(D1_M1_F_PIN, OUTPUT);
    pinMode(D1_M1_B_PIN, OUTPUT);
    pinMode(D1_M1_ENAPIN, OUTPUT);

    digitalWrite(D1_M1_F_PIN, LOW);
    digitalWrite(D1_M1_B_PIN, LOW);
    digitalWrite(D1_M1_ENAPIN, HIGH);

    softPwmCreate(D1_M1_ENAPIN, PWM_START_VALUE, PWM_RANGE);

    // init motor 2
    pinMode(D1_M2_F_PIN, OUTPUT);
    pinMode(D1_M2_B_PIN, OUTPUT);
    pinMode(D1_M2_ENAPIN, OUTPUT);

    digitalWrite(D1_M2_F_PIN, LOW);
    digitalWrite(D1_M2_B_PIN, LOW);
    digitalWrite(D1_M2_ENAPIN, HIGH);

    softPwmCreate(D1_M2_ENAPIN, PWM_START_VALUE, PWM_RANGE);

    // init motor 3
    pinMode(D2_M1_F_PIN, OUTPUT);
    pinMode(D2_M1_B_PIN, OUTPUT);
    pinMode(D2_M1_ENAPIN, OUTPUT);

    digitalWrite(D2_M1_F_PIN, LOW);
    digitalWrite(D2_M1_B_PIN, LOW);
    digitalWrite(D2_M1_ENAPIN, HIGH);

    softPwmCreate(D2_M1_ENAPIN, PWM_START_VALUE, PWM_RANGE);

    // init motor 4
    pinMode(D2_M2_F_PIN, OUTPUT);
    pinMode(D2_M2_B_PIN, OUTPUT);
    pinMode(D2_M2_ENAPIN, OUTPUT);

    digitalWrite(D2_M2_F_PIN, LOW);
    digitalWrite(D2_M2_B_PIN, LOW);
    digitalWrite(D2_M2_ENAPIN, HIGH);

    softPwmCreate(D2_M2_ENAPIN, PWM_START_VALUE, PWM_RANGE);
}

void Vrmclient::moveForward()
{
    qDebug() << "moveForward";

    digitalWrite(D1_M1_F_PIN, HIGH);
    digitalWrite(D1_M1_B_PIN, LOW);

    digitalWrite(D1_M2_F_PIN, HIGH);
    digitalWrite(D1_M2_B_PIN, LOW);

    digitalWrite(D2_M1_F_PIN, HIGH);
    digitalWrite(D2_M1_B_PIN, LOW);

    digitalWrite(D2_M2_F_PIN, HIGH);
    digitalWrite(D2_M2_B_PIN, LOW);
}

void Vrmclient::moveBackward()
{
    qDebug() << "moveBackward";

    digitalWrite(D1_M1_F_PIN, LOW);
    digitalWrite(D1_M1_B_PIN, HIGH);

    digitalWrite(D1_M2_F_PIN, LOW);
    digitalWrite(D1_M2_B_PIN, HIGH);

    digitalWrite(D2_M1_F_PIN, LOW);
    digitalWrite(D2_M1_B_PIN, HIGH);

    digitalWrite(D2_M2_F_PIN, LOW);
    digitalWrite(D2_M2_B_PIN, HIGH);
}

void Vrmclient::moveLeft()
{
    qDebug() << "moveLeft";
}

void Vrmclient::moveRight()
{
    qDebug() << "moveRight";
}

void Vrmclient::turnLeft()
{
    qDebug() << "turnLeft";

    digitalWrite(D1_M1_F_PIN, LOW);
    digitalWrite(D1_M1_B_PIN, HIGH);

    digitalWrite(D1_M2_F_PIN, LOW);
    digitalWrite(D1_M2_B_PIN, HIGH);

    digitalWrite(D2_M1_F_PIN, HIGH);
    digitalWrite(D2_M1_B_PIN, LOW);

    digitalWrite(D2_M2_F_PIN, HIGH);
    digitalWrite(D2_M2_B_PIN, LOW);
}

void Vrmclient::turnRight()
{
    qDebug() << "turnRight";
    digitalWrite(D1_M1_F_PIN, HIGH);
    digitalWrite(D1_M1_B_PIN, LOW);

    digitalWrite(D1_M2_F_PIN, HIGH);
    digitalWrite(D1_M2_B_PIN, LOW);

    digitalWrite(D2_M1_F_PIN, LOW);
    digitalWrite(D2_M1_B_PIN, HIGH);

    digitalWrite(D2_M2_F_PIN, LOW);
    digitalWrite(D2_M2_B_PIN, HIGH);
}

void Vrmclient::stop()
{
    qDebug() << "stop";

    digitalWrite(D1_M1_F_PIN, LOW);
    digitalWrite(D1_M1_B_PIN, LOW);

    digitalWrite(D1_M2_F_PIN, LOW);
    digitalWrite(D1_M2_B_PIN, LOW);

    digitalWrite(D2_M1_F_PIN, LOW);
    digitalWrite(D2_M1_B_PIN, LOW);

    digitalWrite(D2_M2_F_PIN, LOW);
    digitalWrite(D2_M2_B_PIN, LOW);
}

void Vrmclient::changeSpeed(uint speed)
{
    qDebug() << "changeSpeed, speed = " + QString::number(speed);

    int pwmValue = 0;

    if (speed < eSpeedLast)
    {
        switch (speed)
        {
        case eNoSpeed:
            pwmValue = 0;
            break;
        case eSlow:
            pwmValue = 15;
            break;
        case eNormal:
            pwmValue = 25;
            break;
        case eFast:
            pwmValue = 50;
            break;
        case eTurbo:
            pwmValue = 75;
            break;
        case eUltraSound:
            pwmValue = 100;
            break;
        default:
            qDebug() << "Unknown Speed";
            break;
        }

        qDebug() << "pwmValue = " + QString::number(pwmValue);
        softPwmWrite(D1_M1_ENAPIN, pwmValue);
        softPwmWrite(D1_M2_ENAPIN, pwmValue);
        softPwmWrite(D2_M1_ENAPIN, pwmValue);
        softPwmWrite(D2_M2_ENAPIN, pwmValue);
    }
    else
    {
        qDebug() << "Speed beyond the limits";
    }
}
