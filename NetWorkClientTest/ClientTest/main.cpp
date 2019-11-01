#include "sockettcpclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SocketTCPClient w;
    w.show();
    
    return a.exec();
}
