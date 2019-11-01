#include "sockettcpserver.h"
#include "ui_sockettcpserver.h"

#include <QMessageBox>

SocketTCPServer::SocketTCPServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SocketTCPServer)
{
    ui->setupUi(this);

    ui->m_portLineEdit->setText("5550");

    connect(ui->m_initSocketBtn, SIGNAL(clicked()), this, SLOT(OnBtnInitSocket()));
    connect(ui->m_sendData, SIGNAL(clicked()), this, SLOT(OnBtnSendData()));
}

SocketTCPServer::~SocketTCPServer()
{
    delete ui;
}

void SocketTCPServer::ServerNewConnection()
{
    //获取客户端连接
    mp_TCPSocket = mp_TCPServer->nextPendingConnection();

    if(!mp_TCPSocket)
    {
        QMessageBox::information(this, "QT connect", "not get connect");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT connect", "accept client");

        //当有数据接收时，会触发信号 SIGNAL:readyRead()，此时执行槽函数ServerReadData()
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));

        //当断开连接时
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
    }
}

void SocketTCPServer::ServerReadData()
{
    QByteArray buffer;

    buffer = mp_TCPSocket->read(1024);

    if(buffer.isEmpty())
    {
        QMessageBox::information(this, "QT connect", "未正确接收数据");
        return;
    }
    else
    {
        QString showMsg = buffer;
        ui->m_recvDataTextEdit->append(showMsg);
    }
    /*
    char buffer[1024] = {0};

    mp_TCPSocket->read(buffer, 1024);

    if( strlen(buffer) > 0)
    {
        QString showNsg = buffer;
        ui->m_recvDataTextEdit->append(showNsg);
    }
    else
    {
        QMessageBox::information(this, "QT connect", "未正确接收数据");
        return;
    }
    */

}

void SocketTCPServer::OnBtnInitSocket()
{
    mp_TCPServer = new QTcpServer();

    int port = ui->m_portLineEdit->text().toInt();
    if(!mp_TCPServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::information(this, "QT connect", "fail");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT connect", "success");
    }

    //当检测到有新连接时，会触发信号 SIGNAL:newConnection()，此时执行槽函数ServerNewConnection()
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
}

void SocketTCPServer::OnBtnSendData()
{
    char sendMsgChar[1024] = {0};

    QString sendMsg = ui->m_inputTextEdit->toPlainText();

    if(sendMsg.isEmpty())
    {
        QMessageBox::information(this, "QT connect", "empty");
        return;
    }

    std::strcpy(sendMsgChar, sendMsg.toStdString().c_str());

    if(mp_TCPSocket->isValid())
    {
        int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));

        if( -1 == sendRe)
        {
            QMessageBox::information(this, "QT connect", "send fail");
        }
    }
    else
    {
        QMessageBox::information(this, "QT connect", "invalid");
    }
}

void SocketTCPServer::sServerDisConnection()
{
    QMessageBox::information(this, "QT connect", "disconnect");

    return;
}


