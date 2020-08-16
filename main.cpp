#include <QCoreApplication>
#include "vrmclient.h"

// test git hub on raspberry pi

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Vrmclient *vrmClient = new Vrmclient();

    return a.exec();
}
