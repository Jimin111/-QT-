#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QtWidgets>
#include<QTcpServer>
#include<QTcpSocket>
#include<string>

QT_BEGIN_NAMESPACE
namespace Ui { class MyServer; }
QT_END_NAMESPACE

class MyServer : public QMainWindow
{
    Q_OBJECT

public:
    MyServer(QWidget *parent = nullptr);
    ~MyServer();

private slots:
    void on_pushButton_Listen_clicked();

    void on_pushButton_SendMessage_clicked();

    void on_pushButton_recvfile_clicked();

private:
    Ui::MyServer *ui;
    QTcpServer* Server;
    QTcpSocket* Socket;//一个客户端对应一个socket
    QString str;//ip+端口号
    void ClientConnect();
    void ReadData1();
    bool ishead;
    bool isfile;
    QString fileName;
    int fileSize;//接收文件的总大小
    int recvSize;//当前接收文件的大小
    QByteArray filebuf;//当前接收的文件数据
    QString file_path;//保存文件的路径
};
#endif // MYSERVER_H
