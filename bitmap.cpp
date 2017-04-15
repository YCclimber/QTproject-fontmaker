
#include "bitmap.h"
#include "characterset.h"




int getNext(int i, int m,int n){
    return (i%n)*m + i/n;
}

int getPre(int i, int m,int n){
    return (i%m)*n + i/m;
}

void movedata(unsigned char *mtx, int i,int m, int n){
    int temp = mtx[i];
    int cur = i;
    int pre = getPre(cur,m,n);
    while(pre != i){
        mtx[cur] = mtx[pre];
        cur = pre;
        pre = getPre(cur, m, n);
    }
    mtx[cur] = temp;
}

void transpose(unsigned char *mtx, int m, int n){
    for(int i=0; i< (m*n); ++i){
        int next = getNext(i,m,n);
        while (next > i) {
            next = getNext(next, m, n);
        }
        if(next == i)
            movedata(mtx, i, m, n);
    }
}

#if 0 //maybe list.clear() is also free
void destory_charset_list(QList<Charset_Data> pl){
    QList<Charset_Data>::iterator iter = pl.begin();
    for(; iter != pl.end(); iter++){
        delete iter->buffer;
        iter->buffer = NULL;
    }
    pl.clear();
}

void destory_bitmap_list(QList<Stand_BitMap> bl){
    QList<Stand_BitMap>::iterator iter = bl.begin();
    for(; iter != bl.end(); iter++){
        delete iter->buffer;
        iter->buffer = NULL;
        qDebug("bitmap list point %x",&bl.at(0));
        qDebug("bitmap list buffer %x",iter->buffer);
    }
    bl.clear();
}
#endif

void binfile_make_listheader(QList<Charset_Data> pl,unsigned char *lhdr){
    QList<Charset_Data>::iterator iter = pl.begin();
    for(; iter != pl.end(); iter++){
        memcpy(lhdr, &(iter->charset_hdr), sizeof(struct Header_Charset));
        lhdr += sizeof(struct Header_Charset);
    }
}

void binfile_make_all(QList<Charset_Data> pl,unsigned char *data,qint64 size){
    binfile_make_listheader(pl,data + (unsigned int)size - HEADER_SIZE);
    QList<Charset_Data>::iterator iter = pl.begin();
    for(; iter != pl.end(); iter++){
        memcpy(&data[iter->charset_hdr.addr_start], iter->buffer, iter->charset_hdr.nums * iter->charset_hdr.unit);
    }
}

void bitmap_2_charset(QList<Stand_BitMap> bl, struct Charset_Data *p, int addr_start, struct Convert *convert_obj, class Character_Set* character_set_obj,qint64 maxsize){
    qDebug("bitmap count %d",bl.count());
    //struct Charset_Data *p = new Charset_Data;
    p->charset_hdr.encode = convert_obj->encode;
    qDebug("p->charset_hdr.encode %d",p->charset_hdr.encode);
    p->charset_hdr.reserve[0] = 0;
    p->charset_hdr.reserve[1] = 0;
    qDebug("p->charset_hdr.reserve %02x %02x",p->charset_hdr.reserve[0],p->charset_hdr.reserve[1]);
    p->charset_hdr.countmode = character_set_obj->rules.at(convert_obj->csa.rule_index).countmode;
    qDebug("p->charset_hdr.encode %d",p->charset_hdr.countmode);
    //p->charset_hdr.mode = mode;
    //qDebug("p->charset_hdr.mode %d",p->charset_hdr.mode);
    //p->charset_hdr.order = order;
    //qDebug("p->charset_hdr.order %d",p->charset_hdr.order);
    QList<Stand_BitMap>::iterator iter = bl.begin();
    p->charset_hdr.width = iter->width;
    qDebug("p->charset_hdr.width %d",p->charset_hdr.width);
    p->charset_hdr.hight = iter->rows;
    qDebug("p->charset_hdr.rows %d",p->charset_hdr.hight);
    p->charset_hdr.effect = convert_obj->fc.blod;
    qDebug("p->charset_hdr.blod %d",p->charset_hdr.effect);
    p->charset_hdr.pitch = iter->pitch;
    qDebug("p->charset_hdr.pitch %d",p->charset_hdr.pitch);
    p->charset_hdr.unit = iter->bytes;
    qDebug("p->charset_hdr.unit %d",p->charset_hdr.unit);
    p->charset_hdr.nums = bl.count();
    qDebug("p->charset_hdr.nums %d",p->charset_hdr.nums);

    p->charset_hdr.addr_start = addr_start;
    qDebug("p->charset_hdr.addr_start %x",p->charset_hdr.addr_start);
    p->charset_hdr.addr_end = ROUND(p->charset_hdr.addr_start + p->charset_hdr.nums * p->charset_hdr.unit, convert_obj->fc.alignarea_size) - 1;
    qDebug("p->charset_hdr.addr_end %x",p->charset_hdr.addr_end);
    p->buffer = new unsigned char[p->charset_hdr.nums * p->charset_hdr.unit]();
    memset(p->buffer,0,p->charset_hdr.nums * p->charset_hdr.unit);
    qDebug("p->buffer %p",p->buffer);
    p->charset_hdr.code_start = convert_obj->csa.lower;
    qDebug("p->charset_hdr.code_start %x",p->charset_hdr.code_start);
    p->charset_hdr.code_end = convert_obj->csa.upper;
    qDebug("p->charset_hdr.code_end %x",p->charset_hdr.code_end);
    p->charset_hdr.code_top = character_set_obj->rules.at(convert_obj->csa.rule_index).lower;
    qDebug("p->charset_hdr.code_top %x",p->charset_hdr.code_top);
    p->charset_hdr.code_bottom = character_set_obj->rules.at(convert_obj->csa.rule_index).upper;
    qDebug("p->charset_hdr.code_bottom %x",p->charset_hdr.code_bottom);

    unsigned char *temp = p->buffer;
    //QString bd;int j =0;
    for(; iter != bl.end(); iter++){
        //bd = QString("index:") + QString::number(j++,10) + QString("Data:");
        //for(int i=0;i<p->charset_hdr.unit;i++)
        //    bd += ( QString("0x") + QString::number(iter->buffer[i],16) + QString(" "));
        //qDebug()<<bd<<endl;
        memcpy(temp, iter->buffer, p->charset_hdr.unit);
        temp += p->charset_hdr.unit;
        if((maxsize - HEADER_SIZE)<(temp - p->buffer)){
            qDebug()<<"large than image size"<<endl;
        }
    }
}

void bitmap_extand(unsigned char *src, unsigned char *dst, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y){
    int m, n;
    qDebug("FT bitmap extand\n");
    //QString bmpdata("extand data:");
    for(int i = 0; i < ftbmp_hight; i++)
        for(int j = 0; j < ftbmp_width; j++){
            m = i + offset_y;
            n = j + offset_x;
            if(m >= stand_hight || m < 0 || n < 0 || n >= stand_width){
                continue;
            }
            dst[ m * stand_width + n] = (src[ i * (pitch) + (j>>3)] & (0x80 >> (j%8))) ? 1 : 0;
            //bmpdata += ( QString("0x") + QString::number(dst[m * stand_width + n],16) + QString(" ")) ;
        }
    //qDebug()<<bmpdata;
}

void bitmap_format_to_stand_for_preview(unsigned char *src,int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, Stand_BitMap *map){
    unsigned char *dst = new unsigned char[stand_width * stand_hight]();
    qDebug("bitmap_format_to_stand_for_preview dst %p",dst);
    //extand
    bitmap_extand(src, dst, ftbmp_width, ftbmp_hight, stand_width, stand_hight, pitch, offset_x, offset_y);
    //other
    map->buffer = dst;
    map->rows = stand_hight;
    map->width = stand_width;
}

void bitmap_format_to_stand(unsigned char *src, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, int mode, int order, int align,Stand_BitMap *map){
    unsigned char *mid = new unsigned char[stand_width * stand_hight]();
    memset(mid,0,stand_width * stand_hight);
    qDebug("bitmap_format_to_stand mid %p",mid);
    qDebug("stand_hight %d,stand_width %d mode %d order %d", stand_hight,stand_width,mode,order);
    //extand
    bitmap_extand(src, mid, ftbmp_width, ftbmp_hight, stand_width, stand_hight, pitch, offset_x, offset_y);
    //convert
    qDebug("convert bitmap\n");
    unsigned char *dst = NULL;
    switch(mode){
    case 0:
    default:
        map->pitch = (stand_width + 8*align -1)/(8 * align);
        map->bytes = (map->pitch) * stand_hight;
        dst = new unsigned char[map->bytes]();
        memset(dst,0,map->bytes);
        qDebug("bitmap_format_to_stand dst %p",dst);
        for(int i = 0; i < stand_hight; i++)
            for(int j = 0; j < stand_width; j++){
                dst[ i * (map->pitch) + (j>>3)] |= ( mid[i * stand_width + j] ? (order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                //qDebug(" %d,mid %2x ,dst %2x",i * (map->pitch) + (j>>3), mid[i * stand_width + j], dst[ i * (map->pitch) + (j>>3)]);
            }
        break;
    case 1:
        transpose(mid,stand_hight,stand_width);
        map->pitch = (stand_hight + 8*align -1)/(8 * align);
        map->bytes = (map->pitch) * stand_width;
        dst = new unsigned char[map->bytes]();
        memset(dst,0,map->bytes);
        qDebug("bitmap_format_to_stand dst %p",dst);
        for(int i = 0; i < stand_width; i++)
            for(int j = 0; j < stand_hight; j++){
                dst[ i * (map->pitch) + (j>>3)] |= ( mid[i * stand_hight + j] ? (order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                //qDebug(" %d,mid %2x ,dst %2x",i * (map->pitch) + (j>>3), mid[i * stand_hight + j], dst[ i * (map->pitch) + (j>>3)]);
            }
        break;
    }
    qDebug("bitmap pitch %d",map->pitch);
    qDebug("bitmap bytes %d",map->bytes);
    delete mid;
    //other;
    map->buffer = dst;
    qDebug("map->buffer %p",map->buffer);
    map->rows = stand_hight;
    map->width = stand_width;
}





#if 0

void bitmap_format_bit_to_byte(unsigned char *buffer, int m, int n, int pitch){
    unsigned char *temp = new unsigned char[m * pitch]();
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            temp[i*m+j] = ((buffer[ i * (pitch) + (j>>3)] << (j%8)) & 0x80) ? 1 : 0;
    delete buffer;
    buffer = temp;
}

void bitmap_format_byte_to_bit(unsigned char *buffer, int m, int n, int *pitch, int mode){
    unsigned char *temp = NULL;
    switch(mode){
    case 0:
    default:
        *pitch = (m+3)/4;
        temp = new unsigned char[m * (*pitch)]();
        for(int i = 0; i < m; i++)
            for(int j = 0; j < n; j++)
                temp[ i * (*pitch) + (j>>3)] |= (buffer[i*m+j]?(0x80 >> (j%8)):0);
        break;
    case 1:
        *pitch = (n+3)/4;
        temp = new unsigned char[n * (*pitch)]();
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                temp[ i * (*pitch) + (j>>3)] |= (buffer[j*m+i]?(0x80 >> (j%8)):0);


    }
    delete buffer;
    buffer = temp;
}
#endif

