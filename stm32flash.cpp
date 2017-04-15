#include <QByteArray>
#include "stm32flash.h"
#include <QDebug>
#include <QDateTime>

//#define va_list void*
//#define va_arg(arg, type)    *(type*)arg; arg = (char*)arg + sizeof(type);
//#define va_start(arg, start) arg = (va_list)(((char*)&(start)) + sizeof(start))

int Bootloader_Commands::Send_Bytes_With_Complement(char data){
    return Send_Bytes_With_Complement(1,&data);
}

int Bootloader_Commands::Send_Bytes_With_Complement(short data){
    send_buf.clear();
    short complement;
    char *temp = (char*)&data;
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);
    complement = 0xffff - data;
    temp = (char*)&complement;
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);
    return Send();
}

int Bootloader_Commands::Send_Bytes_With_Complement(int data){
    send_buf.clear();
    int complement;
    char *temp = (char*)&data;
    send_buf.append(temp[3]);
    send_buf.append(temp[2]);
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);

    complement = 0xffffffff - data;
    temp = (char*)&complement;
    send_buf.append(temp[3]);
    send_buf.append(temp[2]);
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);
    return Send();
}


int Bootloader_Commands::Send_Bytes_With_Complement(char num,char data[]){
    send_buf.clear();
    char complement;
    for(int i =0; i < num; i++)
        send_buf.append(data[i]);
    for(int i =0; i < num; i++){
        complement = 0xff - data[i];
        send_buf.append(complement);
    }
    return Send();
}

int Bootloader_Commands::Send_Bytes_With_CheckSum(char data){
    return Send_Bytes_With_CheckSum(1,(char*)&data);
}

int Bootloader_Commands::Send_Bytes_With_CheckSum(short data){
    send_buf.clear();
    char checksum = 0;
    char *temp = (char*)&data;
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);
    checksum^=temp[1];
    checksum^=temp[0];
    send_buf.append(checksum);
    return Send();
}

int Bootloader_Commands::Send_Bytes_With_CheckSum(int data){
    send_buf.clear();
    char checksum = 0;
    char *temp = (char*)&data;
    send_buf.append(temp[3]);
    send_buf.append(temp[2]);
    send_buf.append(temp[1]);
    send_buf.append(temp[0]);
    checksum^=temp[3];
    checksum^=temp[2];
    checksum^=temp[1];
    checksum^=temp[0];
    send_buf.append(checksum);
    return Send();
}

int Bootloader_Commands::Send_Bytes_With_CheckSum(char num,char data[]){
    send_buf.clear();
    char checksum = 0;
    for(int i =0; i < num; i++){
        send_buf.append(data[i]);
        checksum ^= data[i];
    }
    send_buf.append(checksum);
    return Send();
}

int Bootloader_Commands::Send_NBytes_With_CheckSum(char num,char data[]){
    send_buf.clear();
    char checksum = num;
    send_buf.append(num);
    int count = (unsigned char)num;
    for(int i =0; i < count+1; i++){//N+1 DATA
        send_buf.append(data[i]);
        checksum ^= data[i];
    }
    send_buf.append(checksum);
    return Send();
}

int Bootloader_Commands::Send_NBytes_With_CheckSum(short num,short data[]){
    send_buf.clear();
    short checksum = num;
    int count = (unsigned char)num;
    send_buf.append((char)((num)&0xff));
    send_buf.append((char)(((num)>>8)&0xff));
    for(int i =0; i < count+1; i++){//N+1 data
        send_buf.append((char)(data[i]&0xff));
        send_buf.append((char)((data[i]>>8)&0xff));
        checksum ^= data[i];
    }
    char checksum_c;
    checksum_c = checksum&0xff;
    send_buf.append(checksum_c);
    checksum_c = (checksum>>8)&0xff;
    send_buf.append(checksum_c);
    return Send();
}


int Bootloader_Commands::ComOpen(QString port, struct PortSettings cur_portsetting){
    mycom = new QextSerialPort( port , cur_portsetting , QextSerialPort::Polling );
    if(!mycom->open(QIODevice::ReadWrite)){
        qDebug("open com failed");
        return -1;
    }
    mycom->flush();
    return 0;
}

void Bootloader_Commands::ComClose(){
    mycom->close();
    delete mycom;
    mycom = NULL;
}

int Bootloader_Commands::get_com_status(){
    if(!mycom)
        return 0;
    return mycom->isOpen()?1:0;
}

void Bootloader_Commands::log_transdata_setEnable(bool log_transdata_enable){
    log_dump = log_transdata_enable;
}

bool Bootloader_Commands::get_log_transdata_isEnable(){
    return log_dump;
}

int Bootloader_Commands::Send(){
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

int Bootloader_Commands::Receviebytes(int n){
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

int Bootloader_Commands::Recevie(){
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

int Bootloader_Commands::Wait_For_Ack(){
    char temp = receive_buf.at(0);
    receive_buf.remove(0,1);
    if(temp == ACK)
        return 0;
    if(temp == NACK)
        return -1;
    return -3;
}

int Bootloader_Commands::Command_Bootloader(){
    char step = 0;
    int count;
    send_buf.clear();
    receive_buf.clear();
loop:
    switch(step){
    case 0:
        send_buf.append((char)0x7f);
        Send();
        step++;
        break;
    case 1:
        if(0 > Wait_For_Ack())
            return -1;
        emit myprint_message(QString("Entry bootloader mode\n"));
        return 0;
    default:
        return -1;
    }
    //recevice
    count = 0;
    while(count++ < 500){
        msleep(1);
        if(!Recevie())
            goto loop;
    }
    return -1;
}



/* argc = 1; argv[0]:CMD_GET
*
*/
int Bootloader_Commands::Command_Get(char* out){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
        QByteArray bootloader_cmd;
        QByteArray version;
        qint64 now = QDateTime::currentMSecsSinceEpoch();
loop:   qDebug("entry step %d, %lld ms",step,QDateTime::currentMSecsSinceEpoch() - now);
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_GET);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            step++;
            if(receive_buf.isEmpty())break;
        case 2:
            if(receive_buf.count() < (receive_buf.at(0) + 2)){
                break;
            }
            //regesist cmd

            for(int i=0; i < receive_buf.at(0) ;i++){
                bootloader_cmd.append(receive_buf.at(i+2));
            }
            version.append(receive_buf.at(1));
            emit this->myprint_dump(QString("Bootloader version: "),version);
            emit this->myprint_dump(QString("Support CMD: "),bootloader_cmd);

            //deal with
            return 0;
        default:
            return -1;
        }
        //recevice
        count = 0;
        while(count++ < 50){
            msleep(10);
            if(!Recevie())
                goto loop;
        }
        return -1;
}


/* argc = 1; argv[0]:CMD_GV
*
*/
int Bootloader_Commands::Command_Get_Version_Protection(char* out){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
        QByteArray operation1;
        QByteArray operation2;
        QByteArray version;
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_GV);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            step++;
            if(receive_buf.isEmpty())break;
        case 2:
            if(receive_buf.count() < 3){
                break;
            }
            //deal with
            qDebug("Version %x",receive_buf.at(0));
            qDebug("Operation 1 %x",receive_buf.at(1));
            qDebug("Operation 1 %x",receive_buf.at(2));
            version.append(receive_buf.at(0));
            operation1.append(receive_buf.at(1));
            operation2.append(receive_buf.at(2));
            emit this->myprint_dump(QString("Bootloader version: "),version);
            emit this->myprint_dump(QString("Operation 1: "),operation1);
            emit this->myprint_dump(QString("Operation 2: "),operation2);
            return 0;
        default:
            return -1;
        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

/* argc = 1; argv[0]:CMD_GID
*
*/
int Bootloader_Commands::Command_Get_ID(char* out){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
        QByteArray id;
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_GID);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            step++;
            if(receive_buf.isEmpty())break;
        case 2:
            if(receive_buf.count() < (receive_buf.at(0) + 2)){
                break;
            }
            //deal with
            qDebug("ID %x%x",receive_buf.at(1),receive_buf.at(2));
            id.append(receive_buf.at(1));
            id.append(receive_buf.at(2));
            emit this->myprint_dump(QString("Chip ID: "),id);
            return 0;
        default:
            return -1;

        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

/* argc = 1; argv[0]:CMD_RW; argv[1~4]:Read Address; argv[5]: Read Num;
*
*/
int Bootloader_Commands::Command_Read_Memory(int address,char N,char* out){
        char step = 0;
        int count;
        int except_N = 0;
        send_buf.clear();
        receive_buf.clear();
        unsigned int num = (unsigned char)N;
        qint64 now = QDateTime::currentMSecsSinceEpoch();
loop:   qDebug("entry step %d, %lld ms",step,QDateTime::currentMSecsSinceEpoch() - now);
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_RM);
            step++;
            except_N = 1;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;

            Send_Bytes_With_CheckSum(address);
            step++;
            except_N = 1;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            Send_Bytes_With_Complement(N);
            step++;
            except_N = 1;
            break;
        case 3:
            if(0 > Wait_For_Ack())
                return -1;
            step++;
            except_N = num + 1;
            if(receive_buf.isEmpty())break;
        case 4:
            if(receive_buf.count() < (num + 1)){
                break;
            }
            memcpy(out,receive_buf.data(),receive_buf.count());
            emit this->myprint_message(QString("Read address: ") + QString::number(address,16) + QString(" Bytes: ") + QString::number(num+1,16) + QString(" success\n"));
            if(!this->isRunning())
                emit this->myprint_dump(QString("Read data: "),receive_buf);
            return 0;
        default:
            return -1;

        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Receviebytes(except_N))
                goto loop;
        }
        return -1;
}

/* argc = 1; argv[0]:CMD_RW; argv[1~4]:Go Address;
*
*/
int Bootloader_Commands::Command_Go(int address){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_GO);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            Send_Bytes_With_CheckSum(address);
            step++;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            qDebug("Go success\n");
            emit this->myprint_message(QString("Jump to address: ") + QString::number(address,16) + QString("\nDisconnect later\n"));
            //deal
            return 0;
        default:
            return -1;
        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

int Bootloader_Commands::Command_Write_Memory(int address,char N,char* in){
        char step = 0;
        int count;
        int except_N = 0;
        send_buf.clear();
        receive_buf.clear();
        qint64 now = QDateTime::currentMSecsSinceEpoch();
loop:   qDebug("entry step %d, %lld ms",step,QDateTime::currentMSecsSinceEpoch() - now);
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_WM);
            step++;
            except_N = 1;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            Send_Bytes_With_CheckSum(address);//address
            step++;
            except_N = 1;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            Send_NBytes_With_CheckSum(N, in);//num + data
            step++;
            except_N = 1;
            break;
        case 3:
            if(0 > Wait_For_Ack())
                return -1;
            qDebug("write address %x success\n",address);
            emit this->myprint_message(QString("Write address: ") + QString::number(address,16) + QString(" Bytes: ") + QString::number(int(N)+1,16) + QString(" success\n"));
            return 0;
        default:
            return -1;
        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Receviebytes(except_N))
                goto loop;
        }
        return -1;
}

int Bootloader_Commands::Command_Erase(char N,char *pages){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
        QByteArray pagearray;
        if(N != (char)0xff){
            unsigned int num = (unsigned char)N;
            pagearray = QByteArray(pages,num+1);
        }

loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_ER);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            if(N == (char)0xff){
                Send_Bytes_With_Complement(N);
                emit this->myprint_message(QString("Erase all pages\n"));
            }
            else{
                Send_NBytes_With_CheckSum(N,pages);
                emit this->myprint_dump(QString("Erase pages: "),pagearray);
            }
            step++;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            qDebug("erase success\n");
            emit this->myprint_message(QString("Erase success\n"));
            return 0;
        default:
            return -1;


        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

int Bootloader_Commands::Command_Erase_Extend(short N, short*pages){
        char step = 0;
        int count;
        send_buf.clear();
        receive_buf.clear();
        //QByteArray pagearray(pages,nums);
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_EE);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            if(N >= (short)0xfffd){
                Send_Bytes_With_Complement(N);
                emit this->myprint_message(QString("Erase all pages\n"));
            }
            else{
                Send_NBytes_With_CheckSum(N,pages);
                emit this->myprint_message(QString("Erase pages\n"));
            }
            step++;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            qDebug("erase success\n");
            emit this->myprint_message(QString("Erase success\n"));
            return 0;
        default:
            return -1;


        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

int Bootloader_Commands::Command_Write_Protect(char N,char * sectors){
		char step = 0;
        int count;
		send_buf.clear();
		receive_buf.clear();
        unsigned int num = (unsigned char)N;
        QByteArray sectorarray(sectors,num+1);
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_WP);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;

            Send_Bytes_With_CheckSum(N,sectors);
            step++;
            break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            emit this->myprint_dump(QString("Write protect sectors: "),sectorarray);
            return 0;

        default:
            return -1;

        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;

}


int Bootloader_Commands::Command_Write_Unprotect(){
		char step = 0;
        int count;
		send_buf.clear();
		receive_buf.clear();
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_WUP);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
                step++;
            if(receive_buf.isEmpty())break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            emit this->myprint_message(QString("Write unprotect\n"));

            return 0;

        default:
            return -1;

        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;

}

int Bootloader_Commands::Command_Readout_Protect(){
		char step = 0;
        int count;
		send_buf.clear();
		receive_buf.clear();

loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_RP);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            if(receive_buf.isEmpty())break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            emit this->myprint_message(QString("Readout protect\n"));
            return 0;

        default:
            return -1;

        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}


int Bootloader_Commands::Command_Readout_Unprotect(){
		char step = 0;
        int count;
		send_buf.clear();
		receive_buf.clear();
loop:
        switch(step){
        case 0:
            Send_Bytes_With_Complement((char)CMD_RUP);
            step++;
            break;
        case 1:
            if(0 > Wait_For_Ack())
                return -1;
            if(receive_buf.isEmpty())break;
        case 2:
            if(0 > Wait_For_Ack())
                return -1;
            emit this->myprint_message(QString("Readout unprotect\n"));
            return 0;

        default:
            return -1;
        }
        //recevice
        count = 0;
        while(count++ < 500){
            msleep(1);
            if(!Recevie())
                goto loop;
        }
        return -1;
}

void Bootloader_Commands::set_down_mode(){
    down_or_read = TRUE;
}

void Bootloader_Commands::set_read_mode(){
    down_or_read = FALSE;
}

void Bootloader_Commands::set_down_args(QString filename){
    fm_filename = filename;
}

void Bootloader_Commands::set_down_args(QString filename,int address){
    fm_filename = filename;
    flash_addr = address;
}

void Bootloader_Commands::set_down_args(QString filename,int address,int size){
    fm_filename = filename;
    flash_addr = address;
    flash_size = size;
}

int Bootloader_Commands::down_firmware(int address,int len,char *data){
    int write_len;
    int total =len;
    char unuse[256];
    //erase all
    if(0 > Command_Erase((char)0xff,unuse)){
        qDebug("erase memory failed");
        return -1;
    }
    //write
    while(len){
        write_len = len > 256 ? 256 : len;//N
        if(0>Command_Write_Memory(address,(char)(write_len-1),data)){
            qDebug("write memory 0x%x failed",address);
            return -1;
        }
        //adjust
        len -= write_len;
        address += write_len;
        data += write_len;
        emit this->mypercent((qint64)((total-len)*100/total));
    }
}

int Bootloader_Commands::read_firmware(int address,int len,char *data){
    int read_len;
    int total =len;
    //write
    while(len){
        read_len = len > 256 ? 256 : len;//N
        if(0>Command_Read_Memory(address,(char)(read_len-1),data)){
            qDebug("read memory 0x%x failed",address);
            return -1;
        }
        //adjust
        len -= read_len;
        address += read_len;
        data += read_len;
        emit this->mypercent((qint64)((total-len)*100/total));
    }
}

void Bootloader_Commands::run(){
    if(down_or_read){
#if 0
        QFile file(fm_filename);
        qDebug()<<fm_filename;
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<"Can't open the file";
            return;
        }
        flash_size = file.size();
        if(firmware)
            delete firmware;
        firmware = new char[flash_size]();
        file.read(firmware,flash_size);
        if(0>down_firmware(flash_addr,flash_size,firmware)){
            return;
        }
#else
        if(0>down_firmware_hex(fm_filename)){
            return;
        }
#endif
    }
    else{
        QFile file(fm_filename);
        qDebug()<<fm_filename;
        if(!file.open(QIODevice::WriteOnly)){
            qDebug()<<"Can't open the file";
            return;
        }
        QDataStream out(&file);
        if(firmware)
            delete firmware;
        firmware = new char[flash_size]();
        if(0>read_firmware(flash_addr,flash_size,firmware)){
            return;
        }
        out.writeRawData(firmware,flash_size);
    }

}

unsigned char Bootloader_Commands::ascii_to_hex(unsigned char c){
    if (c >= '0' && c <= '9')
        c -= 48;
    else if( c >= 'A' && c <= 'F')
        c -= (65-10);
    else if( c >= 'a' && c <= 'f')
        c -= (97-10);
    return c;
}

int Bootloader_Commands::hextobin_line(unsigned char *dst, unsigned char *src, int size){
    if(size & 0x01){
        return -1;
    }
    for(int i=0; i < size; i+=2){
        dst[i/2] = ((ascii_to_hex(src[i])&0x0f) << 4) + (ascii_to_hex(src[i+1])&0x0f);
    }
    return 0;
}

int Bootloader_Commands::down_firmware_hex(QString filename){
    unsigned char readline[256];
    int readlinesize;
    unsigned char dataline[128];
    unsigned char data_len;
    unsigned int address_high = 0,address_low;
    unsigned int address;
    unsigned char data_type;
    unsigned char cc;
    int len;
    QFile file(filename);
    qDebug()<<filename;
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Can't open the file";
        return -1;
    }
    flash_size = len = file.size();
    //erase all
    if(0 > Command_Erase((char)0xff,NULL)){
        qDebug("erase memory failed");
        return -1;
    }
    while((readlinesize = file.readLine((char*)readline,256)) > 0){
        len -= readlinesize;
        emit this->mypercent((qint64)((flash_size-len)*100/flash_size));
        if(readline[0] == ':'){
            if(!hextobin_line(dataline,&readline[1],readlinesize-1)){
                data_len = dataline[0];
                data_type = dataline[3];
                cc = 0;
                for(int i =0; i < (data_len + 4) ;i++){
                    cc += dataline[i];
                    //qDebug(" 0x%x",dataline[i]);
                }
                cc = 0xff - cc;
                cc += 1;
                if(cc != dataline[data_len + 4]){
                    qDebug("error data checksum\n");
                    return -1;
                }
                switch(data_type){
                case 0:
                    address_low = (((unsigned int)dataline[1] << 8) + dataline[2])&0xffff;
                    qDebug("address_low 0x%x",address_low);
                    address = (address_high<<16) + address_low;
                    qDebug("write address 0x%x",address);
                    if(0>Command_Write_Memory((int)address,(char)(data_len-1),(char*)(&dataline[4]))){
                        qDebug("write memory 0x%x failed",address);
                        return -1;
                    }
                    break;
                case 1:
                    qDebug("end of file\n");
                    return 0;
                case 2:

                    break;
                case 4:
                    address_high = (unsigned int)dataline[4] << 8 + dataline[5];
                    qDebug("address_high 0x%x",address_high);
                    break;
                case 5:// next ip address
                    address = ((unsigned int)dataline[4]<<24) + ((unsigned int)dataline[5]<<16) + ((unsigned int)dataline[6]<<8) + (unsigned int)dataline[7];
                    //Command_Go((int)address);
                    break;
                default:
                    qDebug("error data type\n");
                    return -1;
                }
            }
            else{
                qDebug("error data can't trans to char\n");
                return -1;
            }
        }
        else{
            qDebug("error head without ':'\n");
            return -1;
        }

    }
}

