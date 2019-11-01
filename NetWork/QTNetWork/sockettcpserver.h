#ifndef SOCKETTCPSERVER_H
#define SOCKETTCPSERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <cstring>
namespace Ui {
class SocketTCPServer;
}

class SocketTCPServer : public QDialog
{
    Q_OBJECT
    
public:
    explicit SocketTCPServer(QWidget *parent = 0);
    ~SocketTCPServer();
    
private:
    Ui::SocketTCPServer *ui;

    QTcpServer *mp_TCPServer;
    QTcpSocket *mp_TCPSocket;


private slots:

    void OnBtnInitSocket();
    void OnBtnSendData();
    void ServerReadData();
    void ServerNewConnection();
    void sServerDisConnection();
};

#endif // SOCKETTCPSERVER_H
