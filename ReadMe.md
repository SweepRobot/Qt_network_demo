https://blog.csdn.net/bailang_zhizun/article/details/78327974
 

QT创建TCP Socket通信

       最近在学习QT，了解到QT可以进行SOCKET网络通信，进行学习，并建立一个简单的聊天DEMO。为了测试是否能与VS2012下的程序进行通信，在VS2012下建立一个客户端程序，进行通信测试，发现可以进行通信。由此也可以证明，对于采用同一种通信协议(TCP)的两个程序而言，不管是采用什么编译器，尽管采用的语法不同，仍是能够进行通信的。下面先对QT的TCP通信机制进行简单的介绍，然后再介绍基于QT的聊天DEMO具体的实现过程；最后介绍与VS2012下的程序通信。

1、QT的TCP Socket通信机制
        QT的TCP Socket通信仍然有服务端、客户端之分。服务端通过监听某个端口来监听是否有客户端连接到来，如果有连接到来，则建立新的SOCKET连接；客户端通过IP和PORT连接服务端，当成功建立连接之后，就可进行数据的收发了。需要注意的是，在QT中，QT把SOCKET当成输入输出流来对待的，数据的收发是通过read()和write()来进行的，需要与我们常见的send()与recv()进行区分。

要在QT进行SOCKET通信，需要在 工程名.pro 文件中输入 QT       += network，如下所示：



   a）：服务端通信机制
        在服务端，建立SOCKET通信需要用到两个类QTcpServer和QTcpSocket。其中QTcpServer是用来建立QT的Server端对象，QTcpSocket是用来建立SOCKET通信的Socket套接字对象。通信建立流程如下所示：

1）：建立QTcpServer类的对象
QTcpServer* mp_TCPServer ;
mp_TCPServer = new QTcpServer();
2）：监听
      QT中，通过listen()建立对端口的监听。使用方式如下：mp_TCPServer->listen(地址类型， 端口号);

int port = ui->m_portLineEdit->text().toInt();	//获得端口号
if(!mp_TCPServer->listen(QHostAddress::Any, port))
{
     QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
     return;
}
其中，QHostAddress定义了集中特殊的IP地址，如

QHostAddress::Null表示一个空地址；

QHostAddress::LocalHost表示IPv4的本机地址127.0.0.1；

QHostAddress::LocalHostIPv6表示IPv6的本机地址；

QHostAddress::Broadcast表示广播地址255.255.255.255；

QHostAddress::Any表示IPv4的任意地址；

QHostAddress::AnyIPv6表示IPv6的任意地址。

3）：关联接收连接信号与槽函数
        服务端通过信号 SIGNAL:newConnection() 来判断是否接收到了新的连接，当服务端接收到一个客户端的连接时，就会触发信号newConnection()，此时调用相应的槽函数（如自定义函数：ServerNewConnection()）保存新接收到的连接；所以需要在服务端监听端口之后建立信号与槽函数的连接。通过connect函数建立联系：

connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
        在ServerNewConnection()函数中，通过nextPendingConnection()函数获得连接客户端的SOCKET套接字：

mp_TCPSocket = mp_TCPServer->nextPendingConnection();
4）：接收数据
        在QT中QT通过信号SIGNAL:readyRead()来判断是否有数据传入，当客户端向服务端成功发送数据之后，就会在服务端触发readyRead()信号，此时通过调用相应的自定义的槽函数（如：ServerReadData()）保存接收到的数据；通过connect函数建立信号readyRead()与槽函数ServerReadData()的连接：

connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
在接收函数ServerReadData()函数中通过read()函数获取数据：

mp_TCPSocket->read(buffer, 1024);
需要注意的是read()函数有多个重载函数，保存接收数据的数据类型可以是QByteArray也可以是char*类型，根据个人习惯或者任务需求选择合适的read()函数。不过，为了保持一致性，建议选择char*类型，一是因为数据类型容易识别；二是因为熟悉C\C++语言开发的对char*应该比较熟悉，防止使用上的错误。

5）：发送数据
       在QT中，通过write函数向外部发送数据：

int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
if( -1 == sendRe)
{
   QMessageBox::information(this, "QT网络通信", "服务端发送数据失败！");
}
   b）：客户端通信机制
        客户端的通信机制与服务端相比要相对简单，只用到了QTcpSocket一个类。

1）：建立QTcpSocket类的对象
建立Socket的套接字：

QTcpSocket* mp_clientSocket;
mp_clientSocket = new QTcpSocket();
2）：连接服务端
客户端通过connectToHost(IP, Port)函数连接服务端

mp_clientSocket->connectToHost(ip, port);
3）：接收数据
       客户端接收数据与服务端接收数据的机制是相同的。通过readyRead()信号是否被触发来判断是否有数据传入，如果该信号被触发，则调用自定义函数（如：ClientRecvData()）来保存接收到的数据。通过connect()函数，将信号readyRead()与槽函数ClientRecvData()建立映射关系。

在槽函数ClientRecvData()中通过read()函数接收数据，具体使用方法请参考服务端接收数据。

4）：发送数据
客户端发送数据也是通过write()函数来实现，具体使用方法请参考服务端发送数据

2、QT基于TCP Socket的通信实例
该部分主要是DEMO的具体实现。

   a）：服务端示例
1）：在sockettcpserver.h中添加具体如下代码：
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
2）：在构造函数中添加如下代码：
ui->m_portLineEdit->setText("5550");
    connect(ui->m_initSocketBtn, SIGNAL(clicked()), this, SLOT(OnBtnInitSocket()));
    connect(ui->m_sendData, SIGNAL(clicked()), this, SLOT(OnBtnSendData()));
3）：ServerNewConnection()具体实现：
//获取客户端连接
    mp_TCPSocket = mp_TCPServer->nextPendingConnection();
    if(!mp_TCPSocket)
    {
        QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(sServerDisConnection()));
    }
4）：ServerReadData()具体实现：
char buffer[1024] = {0};
    mp_TCPSocket->read(buffer, 1024);
    if( strlen(buffer) > 0)
    {
        QString showNsg = buffer;
        ui->m_recvDataTextEdit->append(showNsg);
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "未正确接收数据");
        return;
    }
5）：OnBtnInitSocket()具体实现：
mp_TCPServer = new QTcpServer();
    int port = ui->m_portLineEdit->text().toInt();
    if(!mp_TCPServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return;
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "服务器监听成功！");
    }
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
6）：OnBtnSendData()具体实现：
char sendMsgChar[1024] = {0};
    QString sendMsg = ui->m_inputTextEdit->toPlainText();
    if(sendMsg.isEmpty())
    {
        QMessageBox::information(this, "QT网络通信", "发送数据为空，请输入数据");
        return;
    }
    strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
    if(mp_TCPSocket->isValid())
    {
        int sendRe = mp_TCPSocket->write(sendMsgChar, strlen(sendMsgChar));
        if( -1 == sendRe)
        {
            QMessageBox::information(this, "QT网络通信", "服务端发送数据失败！");
        }
    }
    else
    {
        QMessageBox::information(this, "QT网络通信", "套接字无效！");
    }
7）：sServerDisConnection()具体实现：
QMessageBox::information(this, "QT网络通信", "与客户端的连接断开");
    return;
8）：ui界面设计具体如下：


   b）：客户端示例
1）：在sockettcpclient.h中添加如下代码：
private slots:
    void on_m_connectServerBtn_clicked();
 
    void on_pushButton_2_clicked();
 
    void ClientRecvData();
 
private:
    Ui::SocketTCPClient *ui;
 
    QTcpSocket *mp_clientSocket;
2）：在构造函数中添加如下代码：
ui->m_serverIPLineEdit->setText("127.0.0.1");
    ui->m_serverPortLineEdit_2->setText("5550");
3）：on_m_connectServerBtn_clicked()函数具体实现如下：
mp_clientSocket = new QTcpSocket();
    QString ip = ui->m_serverIPLineEdit->text();\
    int port = ui->m_serverPortLineEdit_2->text().toInt();
    mp_clientSocket->connectToHost(ip, port);
    if(!mp_clientSocket->waitForConnected(30000))
    {
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }
     connect(mp_clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
4）：on_pushButton_2_clicked()函数具体实现如下：
 //获取TextEdit控件中的内容
    QString sendMsg = ui->m_sendTextEdit->toPlainText();
    char sendMsgChar[1024] = {0};
    strcpy_s(sendMsgChar, sendMsg.toStdString().c_str());
    int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
    if(sendRe == -1)
    {
         QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
         return;
    }
5）：ClientRecvData()函数具体实现如下：
//将接收内容存储到字符串中
    char recvMsg[1024] = {0};
    int recvRe = mp_clientSocket->read(recvMsg, 1024);
    if(recvRe == -1)
    {
        QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
        return;
    }
    QString showQstr = recvMsg;
    ui->m_recvTextEdit_2->setText(showQstr);
6）：客户端ui具体设计如下：


7）：最终实现如下图所示：


需要具体工程文件的可以访问：http://download.csdn.net/download/bailang_zhizun/10037740

3、基于QT的SOCKET程序与基于VS2012的SOCKET程序的通信
为了验证QT中SOCKET程序能否与VS2012中的SOCKET程序正常通信，编写了一个VS2012版的客户端程序，与QT版的服务端程序进行通信。双方都采用正常方式编写。经测试，双方能够正常通信。

结果如下所示：



PS:

1): 通过对两各程序的对比，总的来说，QT版的实现方式要比VS2012版的实现方式简单很多，因为QT把SOCKET相关的类进行了很好的封装，只暴露了几个简单的接口函数就能够实现SOCET的通信；而VS2012版的类的封装性不如QT，使用起来比较麻烦，需要记比较多的接口函数。

而且在QT中，基本上不需要翻看其他的内容，如果要查看某个函数的用法只需要按F1就可以，很方便；而VS2012版的，你懂得；

2): 其中我觉得区别最大的就是SOCKET通信的接收连接、接收数据的机制。在QT中，它采用的是信号-槽的形式，关于SOCKET通信的相关操作，可以通过信号的方式来触发对应的函数；而在VS2012中，它的实现方式则就要传统很多。比如拿服务端接收连接来说，QT只需要连接信号newConnection()与接收函数即可，不管会接收多少个连接，都会以非阻塞的方式在对应的槽函数中建立对应的套接字；而在VS2012中，接收一个连接很简单，但是当要接收多个连接时，while循环显然是不可用的，只能建立线程函数专门接收连接。

对于接收数据也是一样的，QT只需要建立信号readyRead()与具体的槽函数的映射关系，然后在槽函数读取数据即可。而在VS2012中，则需要通过while循环去接收数据，对于需要并行处理数据的程序来说，则需要引入多线程。

3): 本人只是对最常见的实现方式进行了对比，属于比较小白的。如有不足之处，还请见谅并指出，大家共同进步。
————————————————
版权声明：本文为CSDN博主「bailang_zhizun」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/bailang_zhizun/article/details/78327974
