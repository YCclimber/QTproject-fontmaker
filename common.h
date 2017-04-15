#ifndef COMMON
#define COMMON

#include <QString>
#include <QList>
#include <QDebug>

union hex{
    unsigned long ihex;
    unsigned char chex[4];
};

#define ROUND(x,y) (((x) + (y) - 1)& ~((y)-1))

struct Code_Table{
    unsigned long src_code;
    unsigned long dst_code;
};



struct Character_Set_Rules{
    int index;
    int bytes;//single byte,double byte,four byte
    QString name;
    int countmode;//area filling 1 or S-shaped 0
    unsigned long lower;
    unsigned long upper;
};

struct Character_Set_Area{
    int rule_index;//single byte,double byte,four byte
    QString name_id;
    unsigned long lower;
    unsigned long upper;
    int char_nums;
};

struct Character_Set_Area_Member{
    int index;
    QString name;
    struct Character_Set_Area *member;
    int num_member;
};

class Draw_BitMap{
public:
    int rows;
    int width;
    int pitch;
    int unit;
    QByteArray bitmap;
};

struct Fontlib_Header{
    unsigned char magic[8];
    unsigned int version;
    unsigned int size;
    unsigned int offset;
    unsigned char md5[16];
    unsigned char time[6];//[0]:year [1]:month [2]:day [3]:hour [4]:min [5]:sec
    unsigned char reserve[20];
    unsigned char describe[64];
    unsigned char unuse;//='\0'
    unsigned char checksum;
};

//effect define
//#define FONT_BLOD (1<<0)
//#define FONT_ITALIC (1<<1)
//#define FONT_UNDERLINE (1<<2)
//#define FONT_STRIKEOUT (1<<3)

enum FONT_EFFECT{
    FONT_NORMAL,
    FONT_BOLD,
    FONT_BOLD2,
};


#define HORIZONTAL 0
#define VERCIAL 1


struct Header_Charset{
    unsigned short encode;
    unsigned char width;
    unsigned char height;
    unsigned short effect;//bold underline and other
    unsigned char reserve[2];
    unsigned short unit;
    unsigned char pitch;
    unsigned char countmode;
    unsigned int nums;
    unsigned int addr_start;
    unsigned int addr_end;
    unsigned int code_start;
    unsigned int code_end;
    unsigned int code_top;
    unsigned int code_bottom;
};

struct Charset_Data{
    struct Header_Charset charset_hdr;
    unsigned char* buffer;
};

struct Font_Config{
    int width;
    int height;
    int woWidth;
    int woHeight;
    int bold;
    int horiBold;
    int vertBold;
    QString strMode;
    QString strOrder;
    int mode;
    int order;
    int align;
};

struct Convert{
    QString charset;
    int encode;
    QString sourcepath;
    int character_set_encode;
    unsigned short nameid;
    struct Font_Config fc;
    struct Character_Set_Area csa;
};

struct Compare_Header{
    unsigned short encode;
    unsigned char width;
    unsigned char height;
    unsigned short effect;
    unsigned char reserve[2];
};

struct Trim_Char_Info{
    struct Compare_Header cur_hdr;
    QString charset_name;
    unsigned long code;
    unsigned char mode;//这个未保存至镜像中，应硬件不大会改动，该值固定，测试时自己指定
    unsigned char order;
    unsigned int offset;
    unsigned int addr_offset;
    unsigned int addr_start;
    QByteArray raw_bitmap;
    QByteArray modify_bitmap;
};

struct Diff_Bin{
    unsigned int offset;
    unsigned int length;
    QByteArray bytes;
};

struct Patch_Header{
    unsigned char magic[16];
    unsigned char srcMd5[16];
    unsigned char dstMd5[16];
};

#endif // COMMON

