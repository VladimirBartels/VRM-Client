#include <QCoreApplication>
#include "tcpsocket.h"

// test git hub on raspberry pi

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tcpsocket *mysocket = new tcpsocket();
    mysocket->doConnect();


    return a.exec();
}
