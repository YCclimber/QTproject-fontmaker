#include <QByteArray>
#include "printerft.h"
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>
#include <common.h>

PrinterFont::PrinterFont(QString port, struct PortSettings cur_portsetting){
    log_dump = 0;
    ComOpen(port,cur_portsetting);
}

PrinterFont::~PrinterFont(){
    ComClose();
}

int PrinterFont::ComOpen(QString port, struct PortSettings cur_portsetting){
    if (mycom){
        if (mycom->isOpen()){
            mycom->flush();
            mycom->close();
        }
        delete mycom;
        mycom = NULL;
    }
    mycom = new QextSerialPort( port , cur_portsetting , QextSerialPort::Polling );
    if(!mycom->open(QIODevice::ReadWrite)){
        qDebug("open com failed");
        return -1;
    }
    mycom->flush();
    return 0;
}

void PrinterFont::ComClose(){
    if(mycom){
        mycom->close();
        delete mycom;
        mycom = NULL;
    }
}

void PrinterFont::SetMode(enum MODE mode){
    run_mode = mode;
}

int PrinterFont::getMode(){
    return run_mode;
}

void PrinterFont::SetFile(QString filename)
{
    fontfile = filename;
}

void PrinterFont::LogSetEnable(bool log_enable){
    log_dump = log_enable;
}

bool PrinterFont::LogGetStatus(){
    return log_dump;
}

int PrinterFont::Send(){
    /*
    if(send_buf.count())
        qDebug("send data:");
    for(int i=0;i<send_buf.count();i++)
        qDebug(" %x",send_buf.at(i));
    */
    if(log_dump){
        if(send_buf.count())
            emit this->myprint_dump(QString("<Send> : "),send_buf);
    }
    //qint64 ss = QDateTime::currentMSecsSinceEpoch();
    mycom->write(send_buf);
    //qDebug("write %d bytes,take %lld ms",send_buf.count(),QDateTime::currentMSecsSinceEpoch() - ss);
    return 0;
}

int PrinterFont::RecevieBytes(int n){
    //qint64 rr = QDateTime::currentMSecsSinceEpoch();
    receive_buf = mycom->read(n);
    //if(receive_buf.count())
    //    qDebug("read %d bytes,take %lld ms",n,QDateTime::currentMSecsSinceEpoch() - rr);
    /*
    if(receive_buf.count())
        qDebug("recv data:");
    for(int i=0;i<receive_buf.count();i++)
        qDebug(" %x",receive_buf.at(i));
    */
    if(log_dump){
        if(receive_buf.count())
            emit this->myprint_dump(QString("<Recv> : "),receive_buf);
    }
    return receive_buf.isEmpty();
}

int PrinterFont::Recevie(){
    receive_buf = mycom->readAll();
    /*
    if(receive_buf.count())
        qDebug("recv data:");
    for(int i=0;i<receive_buf.count();i++)
        qDebug(" %x",receive_buf.at(i));
    */
    if(log_dump){
        if(receive_buf.count())
            emit this->myprint_dump(QString("<Recv> : "),receive_buf);
    }
    return receive_buf.isEmpty();
}

int PrinterFont::Wait_For_Ack(){
    char temp = receive_buf.at(0);
    receive_buf.remove(0,1);
    if(temp == ACK)
        return 0;
    if(temp == NACK)
        return -1;
    return -3;
}



void PrinterFont::down(){


    QFile file(fontfile);
    if(!file.open(QIODevice::ReadOnly)){
        emit myprint_message(QString::fromUtf8("打开文件失败!"));
        return;
    }
    int size = file.size();
    //qDebug("open file\n");
    char buf[4096+10+1];
    buf[0] = 0x1b;
    buf[1] = 0x46;
    int address = 0;
    int ctrlen;
    int count;
    int i;
    char checksum;
    emit myprint_message(QString("Begin Send")+fontfile +QString("\n"));
    qDebug("begine down file\n");
    while(1){
        checksum = 0;
        ctrlen = file.read(buf+10, 4096);
        qDebug("file read %d\n",ctrlen);
        if (ctrlen < 0){
            qDebug("file read error\n");
            break;
        }
        if (ctrlen == 0){
            qDebug("file send end\n");
            emit mypercent(100);
            emit myprint_message(QString("\nFinished Send")+fontfile +QString("\n"));
            break;
        }
        memcpy(buf+2, &address, 4);
        memcpy(buf+6, &ctrlen, 4);
        for(i=2;i<ctrlen+10;i++)
            checksum ^= buf[i];
        buf[10+ctrlen] = checksum;
        send_buf = QByteArray(buf,10+ctrlen+1);
        qDebug("send data %d\n",ctrlen+10+1);
        Send();
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie()){
                //qDebug("recevie data!\n");
                break;
            }
        }
        if(count>=500)
        {
            qDebug("Out of time,No ack!\n");
            emit myprint_message(QString::fromUtf8("Out of time,No ack!failed download!"));
            break;
        }
        if(Wait_For_Ack()){
           qDebug("Nack!\n");
           emit myprint_message(QString::fromUtf8("Nack!failed download!"));
           break;
        }

        address += ctrlen;
        emit mypercent((qint64)((address)*100/size));
    }

}

int PrinterFont::fontPatchSrcMd5Check(char *image)
{
    Fontlib_Header srcHeader;
    Patch_Header* patchHeader = (Patch_Header*)image;
    char cmd[2]={0x1b,0x3e};
    send_buf = QByteArray(cmd,2);
    Send();
    //recevice
    int count = 0;
    while(count++ < 500){
        msleep(1);
        if(!Recevie()){
            //qDebug("recevie data!\n");
            break;
        }
    }
    if(count>=500)
    {
        qDebug("Out of time,No ack!\n");
        emit myprint_message(QString::fromUtf8("Out of time,No ack!failed download!"));
        return -1;
    }
    if(receive_buf.count() >= sizeof(Fontlib_Header)){
        memcpy(&srcHeader, receive_buf.data(), sizeof(Fontlib_Header));
        receive_buf.remove(0,sizeof(Fontlib_Header));
    }
    else
        return -1;
    if( !memcpy( srcHeader.md5, patchHeader->srcMd5, 16)){
        return 0;
    }
    else{
        return -1;
    }
}

void PrinterFont::patch(){


    QFile file(fontfile);
    if(!file.open(QIODevice::ReadOnly)){
        emit myprint_message(QString::fromUtf8("打开文件失败!"));
        return;
    }
    int size = file.size();
    char *image = new char[size]();
    file.read( image, size);
    file.close();
    char *temp = image;
    if( 0 > fontPatchSrcMd5Check(image))
    {
        emit myprint_message(QString::fromUtf8("打开文件失败!"));
        delete image;
        return;
    }
    temp += sizeof(Patch_Header);
    //qDebug("open file\n");
    char cmd[2] = {0x1b,0x46};
    int address;
    int length;
    int left = (size-sizeof(Patch_Header));
    int count;
    int i;
    char checksum;
    emit myprint_message(QString("Begin Send")+fontfile +QString("\n"));
    qDebug("begine down file\n");
    while(1){
        checksum = 0;
        address = *((int*)(temp));
        length = *((int*)(temp + 4));
        if(left < length+9){
            qDebug("file error length\n");
            emit myprint_message(QString("file error length\n"));
            break;
        }
        for(i=0;i<length+9;i++)
            checksum ^= temp[i];
        if(checksum){
            qDebug("file data error with checksum\n");
            emit myprint_message(QString::fromUtf8("file data error with checksum\n"));
            break;
        }
        send_buf = QByteArray(cmd,2);
        send_buf.append(temp, 9+length);
        //qDebug("send data %d\n",ctrlen+10+1);
        Send();
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie()){
                //qDebug("recevie data!\n");
                break;
            }
        }
        if(count>=500)
        {
            qDebug("Out of time,No ack!\n");
            emit myprint_message(QString::fromUtf8("Out of time,No ack!failed download!"));
            break;
        }
        if(Wait_For_Ack()){
           qDebug("Nack!\n");
           emit myprint_message(QString::fromUtf8("Nack!failed download!"));
           break;
        }

        temp += (9+length);
        left -= (9+length);
        emit mypercent((qint64)((size-left)*100/size));
    }
    delete image;
}

void PrinterFont::check(){
    int count;
    QFile file(fontfile);
    if(!file.open(QIODevice::ReadOnly)){
        emit myprint_message(QString::fromUtf8("打开文件失败!"));
        return;
    }
    qint64 size = file.size();
    char* buf = (char*)qMalloc(size);
    if (!buf){
        qDebug("qmalloc error");
        return;
    }
    file.read(buf,size);
    QString md5_l;
    QByteArray b_md5_l;
    b_md5_l = QCryptographicHash::hash(buf,QCryptographicHash::Md5);
    md5_l.append(b_md5_l.toHex());
    emit myprint_message(QString::fromUtf8("Origin MD5:") + md5_l);
    char cmd[2]={0x1b,0x43};
    send_buf = QByteArray(cmd,2);
    Send();
    //recevice
    count = 0;
    while(count++ < 300){
        sleep(1);
        if(!Recevie()){
            //qDebug("recevie data!\n");
            break;
        }
    }
    if(count>=500)
    {
        qDebug("Out of time!\n");
        emit myprint_message(QString::fromUtf8("Out of time!"));
        return;
    }
    QString md5_r;
    QByteArray b_md5_r;
    b_md5_r = receive_buf;
    md5_r.append(b_md5_r.toHex());
    emit myprint_message(QString::fromUtf8("Remote MD5:") + md5_r);
    if (b_md5_r != b_md5_l){
        emit myprint_message(QString::fromUtf8("Different MD5!"));
    }
    else{
        emit myprint_message(QString::fromUtf8("Same MD5!"));
    }

}

void PrinterFont::run(){

    if (run_mode == Down_Mode){
        down();
    }
    else if (run_mode == Verift_Mode){
        check();
    }
    else if(run_mode == Patch_Mode){
        patch();
    }

}
