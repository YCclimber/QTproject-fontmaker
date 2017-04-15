#ifndef BINFILE
#define BINFILE


struct Header_Charset{
    int encode;
    char name[8];
    int id;
    unsigned short width;
    unsigned short hight;
    unsigned short pitch;
    unsigned short unit;
    int nums;
    unsigned int code_start;
    unsigned int code_end;
    unsigned int addr_start;
    unsigned int addr_end;
};

struct Header_Font{
    char magic[8];
    int list_num;
    int mode;
};

struct Charset_Data{
    struct Header_Charset charset_hdr;
    unsigned char* buffer;
};























#endif // BINFILE

