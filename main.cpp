#include <QCoreApplication>
#include "tcpsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tcpsocket *mysocket = new tcpsocket();
    mysocket->doConnect();


    return a.exec();
}
