#include <QCoreApplication>
#include "vrmclient.h"
#include "generics.h"

// test git hub on raspberry pi

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    writeLog("Client has started");

    Vrmclient *vrmClient = new Vrmclient();

    return a.exec();
}
