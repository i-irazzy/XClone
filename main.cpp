#include <QCoreApplication>
#include "clienapi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() <<  ClientAPI::getInstance()->query_to_server("fvfnyyv");
    return a.exec();
}
