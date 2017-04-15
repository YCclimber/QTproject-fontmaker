#ifndef STM32FLASH
#define STM32FLASH

#include "qextserialport.h"
#include <QThread>
#include <QDebug>
#include <QObject>


#define ACK 0x79
#define NACK 0x1F

//command define
#define CMD_GET (0x00)
#define CMD_GV  (0x01)
#define CMD_GID (0x02)
#define CMD_RM  (0x11)
#define CMD_GO  (0x21)
#define CMD_WM  (0x31)
#define CMD_ER  (0x43)
#define CMD_EE  (0x44)
#define CMD_WP  (0x63)
#define CMD_WUP (0x73)
#define CMD_RP  (0x82)
#define CMD_RUP (0x92)

class Bootloader_Commands : public QThread{
Q_OBJECT

signals:
    void mypercent(qint64);
    void myprint_dump(QString,QByteArray);
    void myprint_message(QString);

protected:
    void run();

public:
    int ComOpen(QString port, struct PortSettings);
    void ComClose();
    int get_com_status();
    void set_down_args(QString filename);
    void set_down_args(QString filename,int address);
    void set_down_args(QString filename,int address,int size);
    void set_down_mode();
    void set_read_mode();
    void log_transdata_setEnable(bool);
    bool get_log_transdata_isEnable();

    int Command_Bootloader();
    int Command_Get(char* out);
    int Command_Get_Version_Protection(char* out);
    int Command_Get_ID(char* out);
    int Command_Read_Memory(int address,char N,char *out);
    int Command_Go(int address);
    int Command_Write_Memory(int address,char N,char *in);
    int Command_Erase(char N,char *pages);
    int Command_Erase_Extend(short N,short *pages);
    int Command_Write_Protect(char nums,char *sectors);
    int Command_Write_Unprotect();
    int Command_Readout_Protect();
    int Command_Readout_Unprotect();

private:
    int Send_Bytes_With_Complement(char data);
    int Send_Bytes_With_Complement(short data);
    int Send_Bytes_With_Complement(int data);
    int Send_Bytes_With_Complement(char num,char data[]);
    int Send_Bytes_With_CheckSum(char data);
    int Send_Bytes_With_CheckSum(short data);
    int Send_Bytes_With_CheckSum(int data);
    int Send_Bytes_With_CheckSum(char num,char data[]);
    int Send_NBytes_With_CheckSum(short num,short data[]);
    int Send_NBytes_With_CheckSum(char num,char data[]);
    int Send();
    int Recevie();
    int Receviebytes(int n);
    int Wait_For_Ack();
    int down_firmware(int address,int len,char *data);
    int read_firmware(int address,int len,char *data);
    int down_firmware_hex(QString filename);
    unsigned char ascii_to_hex(unsigned char c);
    int hextobin_line(unsigned char *dst, unsigned char *src, int size);

private:
    QByteArray send_buf;
    QByteArray receive_buf;
    class QextSerialPort *mycom=NULL;
    QString fm_filename;
    int flash_addr;
    char* firmware=NULL;
    bool log_dump=FALSE;
    int flash_size;
    bool down_or_read=TRUE;//down = TRUE,read = FALSE

};
#endif // STM32FLASH

