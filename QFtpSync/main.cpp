#include <QtWidgets/QApplication>

#include "QFtpSync.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("VicHouse");
    QCoreApplication::setApplicationName("QFtpSync");

    QFtpSync w;
    w.show();
    return a.exec();
}
