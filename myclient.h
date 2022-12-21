#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MyClient; }
QT_END_NAMESPACE

class MyClient : public QMainWindow
{
    Q_OBJECT

public:
    MyClient(QWidget *parent = nullptr);
    ~MyClient();

private slots:
    void on_pushButton_connect_clicked();
    void ReadData();
    void on_pushButton_Send_clicked();

    void on_pushButton_SendFile_clicked();

    void on_pushButton_SelectFile_clicked();

    void on_progressBar_valueChanged(int value);

private:
    Ui::MyClient *ui;
    QTcpSocket* client_socket;//创建socket指针
    QString path;//文件路径
};
#endif // MYCLIENT_H
