#include "myserver.h"
#include "ui_myserver.h"

MyServer::MyServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyServer)
{
    ui->setupUi(this);
    Server =new QTcpServer(this);
    //客户端连接信号槽
    connect(Server,&QTcpServer::newConnection,this,&MyServer::ClientConnect);
    ishead=true;//判断文件头和数据的变量
    isfile=false;//判断消息和文件的变量
}

MyServer::~MyServer()
{
    Server->close();
    Server->deleteLater();
    delete this->Socket;
    delete ui;
}


void MyServer::on_pushButton_Listen_clicked()
{
    if(ui->pushButton_Listen->text()=="开始监听")
    {
        //从输入框获取端口号
        int port=ui->lineEdit_ListenPort->text().toInt();
        //监听指定端口的所有ip
        if(!Server->listen(QHostAddress::Any,port))
        {
               //若出错，则输出出错信息
               qDebug()<<Server->errorString();
               return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("取消监听");
    }
    else
    {
        Socket->abort();
        //取消监听
        Server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("开始监听");
    }
}

void MyServer::on_pushButton_SendMessage_clicked()
{
    if(ui->comboBox->count()==0)return;//如果没有连接，按钮无效
    Socket->write(ui->lineEdit_SendMessage->text().toUtf8());
}

void MyServer::ClientConnect()
{
    //解析所有客户连接
    while(Server->hasPendingConnections())//当服务器有一个挂起的连接时返回true
    {
        //连接上后通过socket获取连接信息
        Socket=Server->nextPendingConnection();
        str =QString("[ip:%1,port:%2]").arg(Socket->peerAddress().toString()).arg(Socket->peerPort());
        //提示连接成功
        ui->textEdit->append(str+"连接成功");
        //复选框选项为连接服务器的ip
        ui->comboBox->addItem(str);
        //将socket地址放入combobox属性内
        //ui.comboBox->setItemData(ui.comboBox->count()-1, QVariant((int)socket));
        //监听客户端是否有消息发送
        connect(Socket,&QTcpSocket::readyRead,this,&MyServer::ReadData1);
    }
}

void MyServer::ReadData1()
{
    QByteArray buf=Socket->readAll();
    if(isfile)
    {
        if(ishead)
        {
            //如果是头，解析头（文件名，文件大小）
            QString str=QString(buf);
            ui->textEdit->append(str);
            QStringList strlist=str.split("&");
            fileName=strlist.at(0);//解析头文件名
            fileSize=strlist.at(1).toInt();//解析头文件大小
            ishead=false;//下次接收到的文件就是我们的数据
            recvSize=0;
            filebuf.clear();
            Socket->write("准备接收数据");
        }
        else
        {
            //根据文件名和文件大小接收和保存文件
            filebuf.append(buf);
            //ui->textEdit->append(buf);
            recvSize+=buf.size();//每接收一次文件,当前文件大小+1
            //当接收文件大小等于总文件大小，文件数据接收完毕
            if(recvSize>=fileSize)
            {
                //保存文件
                QFile file(file_path+"/"+fileName);
                file.open(QIODevice::WriteOnly);
                file.write(filebuf);
                file.close();
                ishead=true;
                isfile=false;


                QString s= QString::number(recvSize);
                ui->textEdit->append(s);
                ui->pushButton_recvfile->setEnabled(true);
            }
        }
    }
    else
        ui->textEdit->append(str+QString(buf));
}

void MyServer::on_pushButton_recvfile_clicked()
{
    isfile=true;
    ui->pushButton_recvfile->setEnabled(false);
    file_path=QFileDialog::getExistingDirectory(this,"选择路径","./");
    ui->lineEdit_SaveFilePath->setText(file_path);
}
