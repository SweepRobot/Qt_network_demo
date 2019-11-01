#include "sockettcpserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SocketTCPServer w;
    w.show();
    
    return a.exec();
}
