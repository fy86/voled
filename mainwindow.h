#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QPainter>
#include <QImage>
#include <QByteArray>
#include <QQueue>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QImage>

#include <arpa/inet.h>
#include <linux/input.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int m_sn;

    QImage m_img;
    QImage *m_pImg;
    int m_snImg;

    void initUDP();
    void draw1();

    QUdpSocket *m_pUDP;
    QByteArray m_baFB;
    QQueue<QByteArray> m_qba;

    void sendKeyUDP(int key);

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;

public slots:
    void slotReadData();

private slots:
    void on_pushButton_7_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_8_clicked();
};

#endif // MAINWINDOW_H
