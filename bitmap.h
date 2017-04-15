#ifndef BITMAP
#define BITMAP

#define HEADER_SIZE (0x800)
#define IMAGE_SIZE (0x800000)

#include "common.h"


void transpose(unsigned char *mtx, int m, int n);
void destory_charset_list(QList<Charset_Data> pl);
void destory_bitmap_list(QList<Stand_BitMap> bl);
void binfile_make_listheader(QList<Charset_Data> pl,unsigned char *lhdr);
void binfile_make_all(QList<Charset_Data> pl,unsigned char *data, qint64 size);
void bitmap_2_charset(QList<Stand_BitMap> bl, struct Charset_Data *p, int addr_start, struct Convert *convert_obj, class Character_Set *character_set_obj,qint64 maxsize);
void bitmap_extand(unsigned char *src, unsigned char *dst, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y);
void bitmap_format_to_stand(unsigned char *src, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, int mode, int order, int align,Stand_BitMap *map);
void bitmap_format_to_stand_for_preview(unsigned char *src,int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, Stand_BitMap *map);

class BitMap{
public:
    BitMap();
    ~BitMap();


public:
    void set_header_offset(qint64 size);
    void set_header_info(QString info);
    void make_binfile(unsigned char *data, qint64 size);

private:
    void transpose(unsigned char *mtx, int m, int n);
    void binfile_make_listheader(QList<Charset_Data> pl,unsigned char *lhdr);
    void bitmap_2_charset(QList<Stand_BitMap> bl, struct Charset_Data *p, int addr_start, struct Convert *convert_obj, class Character_Set *character_set_obj,qint64 maxsize);
    void bitmap_extand(unsigned char *src, unsigned char *dst, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y);
    void bitmap_format_to_stand(unsigned char *src, int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, int mode, int order, int align,Stand_BitMap *map);
    void bitmap_format_to_stand_for_preview(unsigned char *src,int ftbmp_width, int ftbmp_hight, int stand_width, int stand_hight, int pitch, int offset_x, int offset_y, Stand_BitMap *map);

private:
    QList<Charset_Data> pl;
    QList<Stand_BitMap> bl;

};



#endif // BITMAP

