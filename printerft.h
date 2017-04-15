#ifndef PRINTERFT_H
#define PRINTERFT_H

#include "qextserialport.h"
#include <QThread>
#include <QDebug>
#include <QObject>

#define ACK 0x79
#define NACK 0x1F

class PrinterFont : public QThread {
Q_OBJECT

public:
    PrinterFont(QString port, struct PortSettings cur_portsetting);
    ~PrinterFont();
signals:
    void mypercent(qint64);
    void myprint_dump(QString,QByteArray);
    void myprint_message(QString);

protected:
    void run();

public:
    enum MODE{
        Down_Mode,
        Verift_Mode,
        Patch_Mode,
    };

public:
    int ComOpen(QString port, struct PortSettings);
    void ComClose();
    void LogSetEnable(bool log_enable);
    bool LogGetStatus();
    void SetMode(enum MODE);
    void SetFile(QString);
    int getMode();

private:
    int Send();
    int RecevieBytes(int);
    int Recevie();
    int Wait_For_Ack();
    int fontPatchSrcMd5Check(char*);
    void down();
    void check();
    void patch();

private:
    class QextSerialPort *mycom=NULL;
    QByteArray send_buf;
    QByteArray receive_buf;
    QString fontfile;
    bool log_dump;
    enum MODE run_mode;
};


#endif // PRINTERFT_H
