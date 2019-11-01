#include "sockettcpclient.h"
#include "ui_sockettcpclient.h"

#include <QMessageBox>

SocketTCPClient::SocketTCPClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SocketTCPClient)
{
    ui->setupUi(this);

    ui->m_serverIPLineEdit->setText("127.0.0.1");
    ui->m_serverPortLineEdit_2->setText("5550");
}

SocketTCPClient::~SocketTCPClient()
{
    delete ui;
}

void SocketTCPClient::on_m_connectServerBtn_clicked()
{
    mp_clientSocket = new QTcpSocket();

    QString ip = ui->m_serverIPLineEdit->text();\
    int port = ui->m_serverPortLineEdit_2->text().toInt();

    mp_clientSocket->connectToHost(ip, port);

    if(!mp_clientSocket->waitForConnected(30000))
    {
        QMessageBox::information(this, "QT connect", "fail");
        return;
    }

    //当有消息到达时，会触发信号 SIGNAL:readyRead(), 此时就会调用槽函数ClientRecvData()
     connect(mp_clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
}

void SocketTCPClient::on_pushButton_2_clicked()
{
    //获取TextEdit控件中的内容
    QString sendMsg = ui->m_sendTextEdit->toPlainText();

    //转换成字符串发送
    char sendMsgChar[1024] = {0};
    std::strcpy(sendMsgChar, sendMsg.toStdString().c_str());

    int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));

    if(sendRe == -1)
    {
         QMessageBox::information(this, "QT connect", "send fail");
         return;
    }
}

void SocketTCPClient::ClientRecvData()
{
    //将接收内容存储到字符串中
    char recvMsg[1024] = {0};
    int recvRe = mp_clientSocket->read(recvMsg, 1024);

    if(recvRe == -1)
    {
        QMessageBox::information(this, "QT connect", "accept fail");
        return;
    }

    QString showQstr = recvMsg;
    ui->m_recvTextEdit_2->setText(showQstr);
}
