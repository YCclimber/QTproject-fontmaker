#ifndef CHARSET_CONVERT_H
#define CHARSET_CONVERT_H
#include <QList>
#include <QObject>
#include <QThread>
#include <QFile>
#include <QCryptographicHash>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "characterset.h"
#include "common.h"

struct CharSet_Unit{
    Header_Charset charset_hdr;
    QByteArray bitmap;
};


class Charset_Convert:public QThread{

    Q_OBJECT

signals:
    void print_index_num(int cur,int total);
    void print_char_num(int cur,int total);
    void print_data_info(QString str);

public:
    Charset_Convert();
    ~Charset_Convert();
    int init(QList<Convert> &src_convert,QString output,QString ver,QString desc);
    int do_convert_bitmap(int type, QString str,unsigned long encode,Font_Config &font_cfg, QString ttf, QList<Draw_BitMap> &bitmaps);

protected:
    void run();

private:
    void do_convert_bat();
    int do_convert(Convert &converter);
    int save();
    void map_font_bit2byte_hori(unsigned char* dst,unsigned char* src,int src_width,int src_height,int src_pitch,int width,int height,int offset_x,int offset_y);
    void map_font_bit2byte_vert(unsigned char* dst,unsigned char* src,int src_width,int src_height,int src_pitch,int width,int height,int offset_x,int offset_y);
    void map_byte2bit(unsigned char* dst,unsigned char* src,int width,int height,int direction,int order);
    void array_transpose(unsigned char *mtx, int m, int n);
    int get_next(int i, int m,int n);
    int get_pre(int i, int m,int n);
    void move_data(unsigned char *mtx, int i,int m, int n);
    void moveIsland(unsigned char* mtx,int width,int height);

private:
    QList<CharSet_Unit> charset_units;
    QList<Convert> converts;
    QString out_file;
    size_t bin_version;
    QString describe;
    bool init_convert;
};

#endif // CHARSET_CONVERT_H
