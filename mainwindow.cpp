#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pImg = new QImage(256<<1,64<<1,QImage::Format_RGB32);
    m_sn=0;
    m_snImg = 100;

    //m_pUDP = new QUdpSocket(this);
    initUDP();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUDP()
{
    m_pUDP = new QUdpSocket(this);
    //QHostAddress gAddr=QHostAddress("239.255.43.21");

    m_pUDP->bind(QHostAddress::AnyIPv4,57700,QUdpSocket::ShareAddress);
    //m_pus->joinMulticastGroup(QHostAddress("239.255.43.21"));

    connect(m_pUDP, SIGNAL(readyRead()),this, SLOT(slotReadData()));

}

void MainWindow::slotReadData()
{
    //qDebug(" udp read data");
    while (m_pUDP->hasPendingDatagrams()) {
        //qDebug(" udp read data while");
        QByteArray datagram;
        datagram.resize(m_pUDP->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_pUDP->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        if(datagram.size()>0){
            m_qba.enqueue(datagram);
            //qDebug(" udp byte 0 : %x", 0x0ff & datagram.at(0));

            update();
        }
        //qDebug("udp rcv %d",datagram.size());

        //for(int i=0;i<datagram.size();i++){
            //qDebug(" udp server : %02x",0x0ff & datagram.at(i));
            //emit newChar(datagram.at(i));
        //}
        //processTheDatagram(datagram);
    }

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QByteArray ba;
    int max=0,min=255;
    int x,y;
    int h,s,v;
    char ch,ch1=0;
    char *p;
    int offset,shift;
    QPainter painter(this);
    QPainter painterimg(m_pImg);

    QColor color(0x0ff,0,0);

    if(m_qba.isEmpty())return;
    ba=m_qba.dequeue();


    for(y=0;y<64;y++){
        for(x=0;x<256;x++){
            offset = y*128 + (x>>1);
            shift= (1 & (1+x))<<2;
            p = ba.data() + offset;
            ch = ((*p)>>shift)& 0x0f;
            //qDebug(" x:%d y:%d   ,,  offset:%d shift:%d ch:%x",x,y,offset,shift,0x0ff & ch);
            //QColor(m_img.pixel(x,y)).getHsv(&h,&s,&v);
            //if(v>max)max=v;
            //if(v<min)min=v;
            //v=(v>>4)<<4;
            switch(ch&0x0ff){
            case 0x0f:
                v=0xff;break;
            case 0x1:
                v=0x90;break;
            default:
                v=0;break;

            }

            //v=0x0ff & (ch<<4);
            //if(v>0x10)
                painter.setPen(QColor(0,v,v));
            //else
                //painter.setPen(QColor(0,v,v));

            painter.drawPoint((x<<1)+20,(y<<1)+20);
            painter.drawPoint((x<<1)+20+1,(y<<1)+20);
            painter.drawPoint((x<<1)+20,(y<<1)+20+1);
            painter.drawPoint((x<<1)+20+1,(y<<1)+20+1);


            //if(v>0x10)
                painterimg.setPen(QColor(0,v,v));
            //else
                //painterimg.setPen(QColor(0,0,0));


            painterimg.drawPoint(x<<1,y<<1);
            painterimg.drawPoint(1+(x<<1),y<<1);
            painterimg.drawPoint(x<<1,1+(y<<1));
            painterimg.drawPoint(1+(x<<1),1+(y<<1));
#if 0
            if(ch!=ch1){
                qDebug("0x%x",ch);
                ch1=ch;
            }
#endif
        }
    }
    //qDebug("max:%d(%x)  min:%d(%x",max,max,min,min);
    //qDebug(" repaint %d",m_sn++);

}

void MainWindow::on_pushButton_7_clicked()
{
#if 0
    QUdpSocket udp;
    char buf[9000];
    int i;
    qDebug("udp send");
    for(i=0;i<8192;i++)buf[i]=0x0ff;
    udp.writeDatagram(buf,8192,QHostAddress(QString("192.168.0.104")),57700);
#endif
    QUdpSocket udp;
    int nbuf[10];
    nbuf[0]=0x55aaaa55;
    udp.writeDatagram((char*)nbuf,4,QHostAddress(ui->lineEdit->text()),57700);
#if 0
    const QHostAddress &localaddress = QHostAddress::LocalHost;
    foreach(const QHostAddress &addr, QNetworkInterface::allAddresses()){
        if(addr.protocol()==QAbstractSocket::IPv4Protocol && addr!=localaddress){
            qDebug(" ======================= ipaddress : %s",addr.toString().toLatin1().data());
            break;
        }
    }
#endif
}
void MainWindow::sendKeyUDP(int key)
{
    QUdpSocket udp;
    int nbuf[10];
    nbuf[0]=0x55abab55;
    nbuf[1]=key;
    udp.writeDatagram((char*)nbuf,8,QHostAddress(ui->lineEdit->text()),57700);

}

// key.up
void MainWindow::on_pushButton_3_clicked()
{
    sendKeyUDP(htonl(KEY_UP));

}
// key.left
void MainWindow::on_pushButton_4_clicked()
{
    sendKeyUDP(htonl(KEY_LEFT));

}
// key.down
void MainWindow::on_pushButton_6_clicked()
{
    sendKeyUDP(htonl(KEY_DOWN));


}
// key.right
void MainWindow::on_pushButton_5_clicked()
{
    sendKeyUDP(htonl(KEY_RIGHT));

}
// key.ok
void MainWindow::on_pushButton_clicked()
{
    sendKeyUDP(htonl(KEY_ENTER));

}
// key.back
void MainWindow::on_pushButton_2_clicked()
{
    sendKeyUDP(htonl(KEY_BACKSPACE));

}

void MainWindow::on_pushButton_8_clicked()
{
    char buf[100];
    sprintf(buf,"img%d.png",m_snImg++);
    m_pImg->save(buf);
}
