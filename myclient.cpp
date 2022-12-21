#include "myclient.h"
#include "ui_myclient.h"

MyClient::MyClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyClient)
{
    ui->setupUi(this);
    client_socket=new QTcpSocket(this);

}

MyClient::~MyClient()
{
    delete this->client_socket;//回收内存
    delete ui;
}


void MyClient::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text()==tr("连接服务器"))
    {
        QString ip=ui->lineEdit_IP->text();//获取ip
        int port=ui->lineEdit_Port->text().toInt();//获取端口号
        //取消已有连接
        client_socket->abort();
        //连接服务器
        client_socket->connectToHost(ip,port);
        bool isconnect=client_socket->waitForConnected();//等待直到连接成功
        //如果连接成功
        if(isconnect)
        {
            ui->textEdit->append("连接成功！！");
            ui->pushButton_Send->setEnabled(true);//发送按钮激活
            ui->pushButton_SendFile->setEnabled(true);//发送文件按钮激活
            //修改按键文字
            ui->pushButton_connect->setText("断开服务器连接");
            //接收缓冲区（服务器）信息
            connect(client_socket,&QTcpSocket::readyRead,this,&MyClient::ReadData);
        }
        else
        {
            ui->textEdit->append("连接失败！！");
        }
    }
    else
    {
        //断开连接
        client_socket->disconnectFromHost();
        ui->pushButton_connect->setText("连接服务器");
        ui->pushButton_Send->setEnabled(false);//关闭发送按钮
        ui->pushButton_SendFile->setEnabled(false);//关闭发送文件按钮
    }
}
//接收缓冲区信息函数
void MyClient::ReadData()
{
    QByteArray buf=client_socket->readAll();
    if(buf=="准备接收数据")
    {
        QFile file(ui->lineEdit_FilePath->text());
        if(!file.open(QIODevice::ReadWrite))
        {
            //读取文件失败
            return;
        }
        qint64 currentlen = 0;//当前已经发送的大小
        qint64 allLength = file.size();//总文件大小
        do
        {
            char data[1024];
            qint64 msize=file.read(data,1024);//读入文件放入数组中，返回读取到的大小
            //ui->textEdit->append(data);
            client_socket->write(data,msize);
            currentlen+=msize;//实时获取当前发送的数据大小
            ui->progressBar->setValue(currentlen*100/allLength);//更新界面进度条

        }
        while(currentlen<allLength);
    }
    ui->textEdit->append(buf);
}

void MyClient::on_pushButton_Send_clicked()
{
    QByteArray data=ui->lineEdit_Message->text().toUtf8();//获取信息
    client_socket->write(data);
    //判断是否写入成功
    bool iswrite=client_socket->waitForBytesWritten();
    if(iswrite)
    {
        //写入成功
    }
    else
    {
        //没有写入成功
    }
}

void MyClient::on_pushButton_SendFile_clicked()
{
        QFileInfo info(ui->lineEdit_FilePath->text());
        QString head=QString("%1&%2").arg(info.fileName()).arg(info.size());
        client_socket->write(head.toUtf8());
}

void MyClient::on_pushButton_SelectFile_clicked()
{
    path=QFileDialog::getOpenFileName(this,"选择文件","/","files(*.*)");
    ui->lineEdit_FilePath->setText(path);
}

void MyClient::on_progressBar_valueChanged(int value)
{
    value=ui->progressBar->value();
    if(value==100)
    {
        QMessageBox::information(this,"提示","传输完毕！");
    }
}
