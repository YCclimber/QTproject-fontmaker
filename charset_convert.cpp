#include "charset_convert.h"



Charset_Convert::Charset_Convert()
{
    init_convert = 0;
}

Charset_Convert::~Charset_Convert()
{
    converts.clear();
    charset_units.clear();
}

int Charset_Convert::get_next(int i, int m,int n)
{
    return (i%n)*m + i/n;
}

int Charset_Convert::get_pre(int i, int m,int n)
{
    return (i%m)*n + i/m;
}

void Charset_Convert::move_data(unsigned char *mtx, int i,int m, int n)
{
    int temp = mtx[i];
    int cur = i;
    int pre = get_pre(cur,m,n);
    while(pre != i){
        mtx[cur] = mtx[pre];
        cur = pre;
        pre = get_pre(cur, m, n);
    }
    mtx[cur] = temp;
}
/* Transpose array
 * Param in     @ m: y
 *              @ n: x
 *              @ mtx: src
 * Param out    @ mtx: dst
 * No return
 */
void Charset_Convert::array_transpose(unsigned char *mtx, int m, int n)
{
    for(int i=0; i< (m*n); ++i){
        int next = get_next(i,m,n);
        while (next > i) {
            next = get_next(next, m, n);
        }
        if(next == i)
            move_data(mtx, i, m, n);
    }
}

/* Trans font map to byte array,all default HORIZONTAL
 * Param in     @ src: source font map produce by freetype (one bit one pixel)
 *              @ width: bitmap's width
 *              @ height: bitmap's height
 *              @ offset_x: the font map X offset with bitmap
 *              @ offset_y: the font map Y offset with bitmap
 * Param out    @ dst: target byte map (one byte one pixel)
 * No retrun;
 */
void Charset_Convert::map_font_bit2byte_hori(unsigned char* dst,unsigned char* src,int src_width,int src_height,int src_pitch,int width,int height,int offset_x,int offset_y)
{
    int m, n;
    int pitch = src_pitch;


    QString info("extract data:");
    for(int i = 0; i < src_height; i++){
        for(int j = 0; j < src_width; j++){
            m = i + offset_y;
            n = j + offset_x;
            if(m >= height || m < 0 || n < 0 || n >= width){
                continue;
            }
            dst[ m * width + n] = (src[ i * (pitch) + (j>>3)] & (0x80 >> (j%8))) ? 1 : 0;
            info += ( QString("0x") + QString::number(dst[m * width + n],16) + QString(" ")) ;
        }
    }
    qDebug()<<info;
}

void Charset_Convert::map_font_bit2byte_vert(unsigned char* dst,unsigned char* src,int src_width,int src_height,int src_pitch,int width,int height,int offset_x,int offset_y)
{
    int m, n;
    int pitch = src_pitch;


    QString info("extract data:");
    for(int i = 0; i < src_width; i++){
        for(int j = 0; j < src_height; j++){
            m = i + offset_x;
            n = j + offset_y;
            if(m >= height || m < 0 || n < 0 || n >= width){
                continue;
            }
            dst[ m * height + n] = (src[ i * (pitch) + (j>>3)] & (0x80 >> (j%8))) ? 1 : 0;
            info += ( QString("0x") + QString::number(dst[m * height + n],16) + QString(" ")) ;
        }
    }
    qDebug()<<info;
}

void Charset_Convert::map_byte2bit(unsigned char* dst,unsigned char* src,int width,int height,int direction,int order)
{
    int pitch,bytes;

    switch(direction){
    case HORIZONTAL:
    default:
        pitch = (width + 8 -1)/8;
        bytes = pitch * height;
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++){
                dst[ i * pitch + (j>>3)] |= ( src[i * width + j] ? (order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                //qDebug(" %d,src %2x ,dst %2x",i * pitch + (j>>3), mid[i * width + j], dst[ i * pitch + (j>>3)]);
            }
        break;
    case VERCIAL:
        array_transpose(src,height,width);
        pitch = (height + 8 -1)/8;;
        bytes = pitch * height;
        for(int i = 0; i < width; i++)
            for(int j = 0; j < height; j++){
                dst[ i * pitch + (j>>3)] |= ( src[i * height + j] ? (order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                //qDebug(" %d,src %2x ,dst %2x",i * pitch + (j>>3), mid[i * height + j], dst[ i * pitch + (j>>3)]);
            }
        break;
    }
}

int Charset_Convert::init(QList<Convert> &src_converts,QString output,QString ver,QString desc)
{
    //init
    converts = src_converts;
    if (converts.isEmpty()){
        qDebug("empty convert\n");
        return -1;
    }
    out_file = output;
    if (desc.length() > 64)
        return -1;
    describe = desc;
    bool ok;
    bin_version = ver.toULong(&ok,16);
    init_convert = 1;
    return 0;
}

int Charset_Convert::save()
{
    if(charset_units.isEmpty()){
        qDebug("空数据!");
        return -1;
    }

    int i;
    Fontlib_Header ft_hdr;
    memset(&ft_hdr, 0 ,sizeof(Fontlib_Header));

    memcpy(ft_hdr.magic, "WiZaRpOs",8);
    ft_hdr.version = bin_version;
    memcpy(ft_hdr.describe, describe.toAscii().data(),64);

    size_t ss= sizeof(Fontlib_Header) + sizeof(Header_Charset) * (charset_units.count()+1);
    ft_hdr.offset = ROUND(ss,0x1000);
    ft_hdr.size = charset_units.last().charset_hdr.addr_end + ft_hdr.offset;

    unsigned char *bin = new unsigned char[ft_hdr.size];
    if (bin == NULL){
        qDebug("fail new bin");
        return -1;
    }
    memset(bin,0,ft_hdr.size);
    //memcpy(bin,c_hdr,sizeof(Fontlib_Header));
    unsigned char *bin_index = bin + sizeof(Fontlib_Header);
    unsigned char *bin_bitmap;
    unsigned long bytes;
    char checksum = 0;
    QList<CharSet_Unit>::iterator iter = charset_units.begin();
    for(; iter != charset_units.end(); iter++){

        memcpy(bin_index, &(iter->charset_hdr), sizeof(Header_Charset));
        for(i=0;i<sizeof(Header_Charset);i++)
            checksum ^= bin_index[i];
        bin_index += sizeof(Header_Charset);
        bin_bitmap = bin + ft_hdr.offset + iter->charset_hdr.addr_start;
        bytes = iter->charset_hdr.nums * iter->charset_hdr.unit;
        memcpy(bin_bitmap, iter->bitmap.data(), bytes);
    }
    //add charset hdr checksum
    bin_index[0] = 0;
    bin_index[1] = 0;
    bin_index[2] = checksum;
    qDebug("font list checksum 0x%x",checksum);
    //to be:md5
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba = QByteArray((const char*)(bin + sizeof(Fontlib_Header)), ft_hdr.size-sizeof(Fontlib_Header));
    md.addData(ba);
    bb = md.result();
    qDebug()<<"MD5:"<<QString(bb.toHex());
    for(i=0;i<bb.count();i++)
        ft_hdr.md5[i]=bb.at(i);
    unsigned char c_hdr[sizeof(Fontlib_Header)];
    memcpy(c_hdr,&ft_hdr,sizeof(Fontlib_Header));
    checksum = 0;
    for(i=0;i<sizeof(Fontlib_Header)-1;i++)
        checksum ^= c_hdr[i];
    ft_hdr.checksum = checksum;
    c_hdr[sizeof(Fontlib_Header)-1] = ft_hdr.checksum;
    qDebug("font header checksum 0x%x",checksum);

    memcpy(bin,c_hdr,sizeof(Fontlib_Header));

    out_file += (QString("_Md5_") + QString(bb.toHex()).left(4) + QString(".bin"));
    QFile file(out_file);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug("打开文件失败!");
        return -1;
    }
    QDataStream out(&file);
    out.writeRawData((const char*)bin,ft_hdr.size);
    file.close();
    delete bin;
}

void Charset_Convert::run()
{
    if(init_convert){
        do_convert_bat();
    }
    init_convert = 0;
}

void Charset_Convert::do_convert_bat()
{
    qDebug("convert begin\n");
    charset_units.clear();
    QList<Convert>::iterator iter = converts.begin();
    int i = 1;
    for(;iter!=converts.end();iter++,i++){
        emit print_index_num(i,converts.count());
        if(0 > do_convert((*iter))){
            qDebug()<<QString::fromUtf8("failed covert,id=")<<iter->csa.name_id;
            return;
        }
    }
    save();
}

int Charset_Convert::do_convert(Convert &converter){
    FT_Library pFTLib = NULL;
    FT_Face pFTFace = NULL;
    FT_Error pFTError = 0;
    int count = 0;
    int ret = -1;
    unsigned char* stand_bitmap = NULL;
    unsigned char* mid_bitmap = NULL;
    unsigned long cur_charcode;
    int offset_x,offset_y;
    //get configure
    int encode = converter.encode;
    int direction = converter.fc.mode;
    int width = converter.fc.width;
    int height = converter.fc.height;
    int blod = converter.fc.bold;
    int horiBold = converter.fc.horiBold;
    int vertBold = converter.fc.vertBold;
    int order = converter.fc.order;
    int align = converter.fc.align;
    int pitch = (direction==HORIZONTAL) ? (width+7)/8 : (height+7)/8;
    int unit = (direction==HORIZONTAL) ? pitch*height: pitch*width;
    int woWidth = converter.fc.woWidth;
    int woHeight = converter.fc.woHeight;
    int font_set_width;
    int font_set_height;
    QString ttf_path = converter.sourcepath;

    //prepare
    QFile file(converter.sourcepath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Can't open the file";
        return -1;
    }
    qint64 size = file.size();
    char* new_face = NULL;
    new_face = new char[size]();
    file.read(new_face,size);
    qDebug()<<ttf_path;
    //detect char set
    Character_Set *cur_character_set = NULL;
    switch(encode){
    case Character_Set::ENCODE_GB18030:
        cur_character_set = new GB18030_Character_Set;
        break;
    case Character_Set::ENCODE_UCS_4:
        cur_character_set = new Unicode_Character_Set;
        break;
    default:
        qDebug("unknow code encode");
        return -1;
    }
    int rule_index = converter.csa.rule_index;
    int count_mode = cur_character_set->get_countmode(rule_index);
    int nums = converter.csa.char_nums;
    int lower = converter.csa.lower;
    int upper = converter.csa.upper;
    int topper = cur_character_set->get_topper(rule_index);
    int bottom = cur_character_set->get_bottomer(rule_index);

    //set configure
    CharSet_Unit *m_charset_unit = new CharSet_Unit();
    m_charset_unit->charset_hdr.encode = encode;
    m_charset_unit->charset_hdr.width = width;
    m_charset_unit->charset_hdr.height = height;
    m_charset_unit->charset_hdr.effect = blod;
    m_charset_unit->charset_hdr.reserve[0] = 0;
    m_charset_unit->charset_hdr.reserve[1] = 0;
    m_charset_unit->charset_hdr.unit = unit;
    m_charset_unit->charset_hdr.pitch = pitch;
    m_charset_unit->charset_hdr.countmode = count_mode;
    m_charset_unit->charset_hdr.nums = nums;
    m_charset_unit->charset_hdr.code_start = lower;
    m_charset_unit->charset_hdr.code_end = upper;
    m_charset_unit->charset_hdr.code_top = topper;
    m_charset_unit->charset_hdr.code_bottom = bottom;

    //Init FreeType Lib to manage memory
    pFTError = FT_Init_FreeType( & pFTLib);
    if (pFTError)
    {
        qDebug("There is some error when Init Library");
        return -1;
    }
    qDebug("FreeType init done!");

    //create font face from font file
    pFTError = FT_New_Memory_Face(pFTLib, (FT_Byte*)new_face, size, 0,&pFTFace);
    if (!pFTError)
    {
        qDebug("FT New Face open done!");
        FT_Glyph glyph;
        FT_UInt glyph_index;
        FT_ULong unicode_charcode;
        cur_charcode = converter.csa.lower;
        int percent_size = nums/100 ?  : 4;
        int percent_count = 0;

        for(count=0;count<nums;count++)
        {
            if(count >= percent_count){
                qDebug("count %d percent_count %d",count,percent_count);
                percent_count += percent_size;
                emit print_char_num(count,nums);
            }
            cur_character_set->to_unicode( &cur_charcode, &unicode_charcode);
            qDebug("cur area code 0x%x, count = %d, unicode_charcode = 0x%x\n",cur_charcode,count,unicode_charcode);

            //adjust width (half full)
            if(unicode_charcode <128)
            {
                qDebug("half width and other size adjust");
                font_set_width = (width + woWidth) * 2;
            }
            else
            {
                font_set_width = width + woWidth;
            }
            font_set_height = height + woHeight;
            FT_Set_Pixel_Sizes(pFTFace,font_set_width,font_set_height);
            qDebug("FT Set Pixel Sizes width %d, height %d!",font_set_width,font_set_height);

            glyph_index = FT_Get_Char_Index(pFTFace,  unicode_charcode );
            qDebug("glyph index = %d\n",glyph_index);
            qDebug("FT_Load_Glyph\n");
            pFTError = FT_Load_Glyph(pFTFace, glyph_index, FT_LOAD_DEFAULT);
            if (pFTError)
            {
                qDebug("FT_Load_Glyph error\n");
                ret = -1;
                goto clean;
            }

            //set effect
            FT_Pos xStrength,yStrength;
            xStrength = horiBold<<3;// 0<<6; 1<<6
            yStrength = vertBold<<3;

            qDebug("FT Set blod index %d!",blod);
            FT_Outline_EmboldenXY(&pFTFace->glyph->outline , xStrength, yStrength);

            qDebug("FT_Get_Glyph\n");
            pFTError = FT_Get_Glyph(pFTFace -> glyph,  & glyph);
            if (!pFTError)
            {

                //FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0 , 1);
                qDebug("FT_Glyph_To_Bitmap\n");
                FT_Glyph_To_Bitmap(&glyph, ft_render_mode_mono, 0 , 1);
                FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
                FT_Bitmap *bitmap  =  &(bitmap_glyph -> bitmap);
                qDebug("bitmap rows %d, width %d, horiAdvan %d vertAdvan %d",bitmap->rows,bitmap->width,(pFTFace->glyph->metrics.horiAdvance>>6),(pFTFace->glyph->metrics.vertAdvance>>6));

#if 1
                int bitmap_ascender = (pFTFace->size->metrics.ascender >>6);
                int bitmap_width = bitmap->width;//((pFTFace->glyph->metrics.width ) >>6);
                int bitmap_height = bitmap->rows;//((pFTFace->glyph->metrics.height ) >>6);
                int bitmap_left = bitmap_glyph->left;
                int bitmap_top = bitmap_ascender - bitmap_glyph->top;

                if(bitmap_left < 0 || (bitmap_left + bitmap_width) > width)
                {
                    if (bitmap_width > width)
                        offset_x = (width - bitmap_width)/2;
                    else
                        offset_x = 0;
                }
                else
                {
                    if (bitmap_width > width)
                        offset_x = (width - bitmap_width)/2;
                    else if (bitmap_left + bitmap_width > width)
                        offset_x = (width - bitmap_width);
                    else
                        offset_x = bitmap_left;
                }

                if ( bitmap_top < 0)
                {
                    if (bitmap_height > height)
                        offset_y = (height - bitmap_height)/2;
                    else
                        offset_y = 0;
                }
                else
                {
                    if (bitmap_height > height)
                        offset_y = (height - bitmap_height)/2;
                    else if (bitmap_top + bitmap_height > height)
                        offset_y = (height - bitmap_height);
                    else
                        offset_y = bitmap_top;
                }
#endif

                qDebug()<<"offset_x = "<<offset_x;
                qDebug()<<"offset_y = "<<offset_y;

                stand_bitmap = new unsigned char[unit];
                memset(stand_bitmap,0,unit);
                if(!stand_bitmap){
                    qDebug("new stand_bitmap failed\n\n");
                    goto clean;
                }
                mid_bitmap = new unsigned char[width*height];
                memset(mid_bitmap,0,width*height);
                if(!mid_bitmap){
                    qDebug("new stand_bitmap failed\n\n");
                    delete stand_bitmap;
                    goto clean;
                }
                if (bitmap->width != 0){
                    //redo static model
                    map_font_bit2byte_hori(mid_bitmap, bitmap->buffer, bitmap->width,bitmap->rows,bitmap->pitch, width, height, offset_x, offset_y);
                    //moveIsland(mid_bitmap,width,height);
                    //compress to bit pixel
                    map_byte2bit(stand_bitmap, mid_bitmap,  width, height, direction, order);
                }
                //save
                m_charset_unit->bitmap.append((const char*)stand_bitmap,unit);
                //free
                delete mid_bitmap;
                mid_bitmap = NULL;
                delete stand_bitmap;
                stand_bitmap = NULL;
                //free glyph
                FT_Done_Glyph(glyph);
                glyph = NULL;
            }
            else{
                qDebug("FT_Get_Glyph error\n");
                ret = -1;
                goto clean;
            }

            ret = cur_character_set->get_next_code(&cur_charcode, rule_index, lower, upper);
            //qDebug("get next result %d",ret);
            if(ret == -1)
                break;
        }

        if(count!=nums)
        {
            qDebug("count %d nums %d",count,nums);
            ret = -1;
            goto clean;
        }
        else
            emit print_char_num(count,nums);

        //convert to block
        qDebug("convert to block");
        int addr_start,addr_end;
        if(charset_units.empty())
            addr_start = 0;
        else
            addr_start = ROUND(charset_units.last().charset_hdr.addr_end, align);
        m_charset_unit->charset_hdr.addr_start = addr_start;
        addr_end = addr_start + (unit*nums);
        m_charset_unit->charset_hdr.addr_end = addr_end;
        charset_units.append(*m_charset_unit);
        qDebug("charset count %d",charset_units.count());
        ret = 0;
    }
    else{
        qDebug("Free New Face open failed!");
        ret = -1;
    }

clean:
    //  free face
    FT_Done_Face(pFTFace);
    qDebug("Free pFTLib\n");
    FT_Done_FreeType(pFTLib);


    if(cur_character_set)
        delete cur_character_set;
    if(new_face)
        delete new_face;

    return ret;
}

struct CharCode{
    FT_ULong src_charcode;
    FT_ULong unicode_charcode;
};

int Charset_Convert::do_convert_bitmap(int type, QString str,unsigned long encode,Font_Config &font_cfg, QString ttf, QList<Draw_BitMap> &bitmaps){
    FT_Library pFTLib = NULL;
    FT_Face pFTFace = NULL;
    FT_Error pFTError = 0;
    unsigned char *stand_bitmap = NULL;
    unsigned char *mid_bitmap = NULL;
    int ret = -1;
    int offset_x,offset_y;
    //get configure
    int direction = font_cfg.mode;
    int width = font_cfg.width;
    int height = font_cfg.height;
    int woWidth = font_cfg.woWidth;
    int woHeight = font_cfg.woHeight;
    int font_set_width = width + woWidth;
    int font_set_height = height + woHeight;
    int blod = font_cfg.bold;
    int horiBold = font_cfg.horiBold;
    int vertBold = font_cfg.vertBold;
    int order = font_cfg.order;
    int pitch = (direction==HORIZONTAL) ? (width+7)/8 : (height+7)/8;
    int unit = (direction==HORIZONTAL) ? pitch*height: pitch*width;
    int i,j;
    int k;

    Character_Set *cur_character_set = NULL;
    qDebug("encode %d",encode);
    switch(encode){
    case Character_Set::ENCODE_GB18030:
        cur_character_set = new GB18030_Character_Set;
        break;
    case Character_Set::ENCODE_UCS_4:
        cur_character_set = new Unicode_Character_Set;
        break;
    default:
        qDebug("unknow code encode");
        return -1;
    }

    int length;
    CharCode *code;

    if(type == 0)
    {
        length = 1;
        code = new CharCode[length];
        bool ok;
        code[0].src_charcode = str.toULong(&ok,16);
        qDebug("source code = 0x%x",code[0].src_charcode);
        cur_character_set->to_unicode(&code[0].src_charcode, &code[0].unicode_charcode);
    }
    else
    {
        length = str.count();
        code = new CharCode[length];
        for(i = 0; i < length; i++)
        {
            code[i].unicode_charcode = str.at(i).unicode();
            cur_character_set->unicode_to(&code[i].unicode_charcode, &code[i].src_charcode);
        }
    }

    //Init FreeType Lib to manage memory
    pFTError = FT_Init_FreeType( & pFTLib);
    if (pFTError)
    {
        pFTLib = 0 ;
        qDebug("There is some error when Init Library");
        return -1;
    }
    //qDebug("FreeType init done!");
    //create font face from font file
    qDebug()<<ttf.toStdString().c_str();
    pFTError = FT_New_Face(pFTLib, ttf.toStdString().c_str(), 0, &pFTFace);
    if (!pFTError)
    {
        //qDebug("FT New Face open done!");
        //FT_Set_Pixel_Sizes(pFTFace,fcfg->width,fcfg->height);
        //FT_Set_Char_Size(pFTFace,fcfg->width*64,fcfg->height*64,64,64 );

        FT_Glyph glyph;
        FT_UInt glyph_index;
        FT_ULong unicode_charcode;

        QString printstr("{");

        for(int k = 0; k < length; k++){

            unicode_charcode = code[k].unicode_charcode;
            qDebug("unicode = 0x%x",unicode_charcode);
            //restore initial state
            if(width != font_cfg.width){
                width = font_cfg.width;
                pitch = (direction==HORIZONTAL) ? (width+7)/8 : (height+7)/8;
                unit = (direction==HORIZONTAL) ? pitch*height: pitch*width;
            }
            //set width(half full)
            if(unicode_charcode <128){
                if(type==0){
                    font_set_width *= 2;
                }
                else{
                    font_set_width = width + woWidth*2;
                    font_set_height = height + woHeight*2;
                    width /= 2; //ascii is half width
                    pitch = (direction==HORIZONTAL) ? (width+7)/8 : (height+7)/8;
                    unit = (direction==HORIZONTAL) ? pitch*height: pitch*width;
                }
            }

            FT_Set_Pixel_Sizes(pFTFace,font_set_width,font_set_height);
            qDebug("FT Set Pixel Sizes width %d, height %d!",font_set_width,font_set_height);

            glyph_index = FT_Get_Char_Index(pFTFace,  unicode_charcode );
            qDebug("glyph index = %d",glyph_index);

            pFTError = FT_Load_Glyph(pFTFace, glyph_index, FT_LOAD_DEFAULT);
            if(pFTError){
                goto clean;
            }

            //set effect
            FT_Pos xStrength,yStrength;
            xStrength = horiBold<<3;// 0<<6; 1<<6
            yStrength = vertBold<<3;

            qDebug("FT Set blod index %d!",blod);
            FT_Outline_EmboldenXY(&pFTFace->glyph->outline , xStrength, yStrength);

            pFTError = FT_Get_Glyph(pFTFace -> glyph,  & glyph);
            if (!pFTError)
            {
                //  convert glyph to bitmap with 256 gray
                //FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0 , 1);
                FT_Glyph_To_Bitmap(&glyph, ft_render_mode_mono, 0 , 1);
                FT_BitmapGlyph    bitmap_glyph = (FT_BitmapGlyph)glyph;
                FT_Bitmap *bitmap  =  &(bitmap_glyph -> bitmap);
#if 1
                qDebug()<<"size->metrics.x_scale = "<<((pFTFace->size->metrics.x_scale ) >>6);
                qDebug()<<"size->metrics.y_scale = "<<((pFTFace->size->metrics.y_scale ) >>6);
                qDebug()<<"size->metrics.ascender = "<<((pFTFace->size->metrics.ascender ) >>6);
                qDebug()<<"size->metrics.descender = "<<((pFTFace->size->metrics.descender ) >>6);
                qDebug()<<"size->metrics.height = "<<((pFTFace->size->metrics.height )>>6);
                qDebug()<<"size->metrics.max_advance = "<<((pFTFace->size->metrics.max_advance ) >>6);
                qDebug()<<"glyph->metrics.width = "<<((pFTFace->glyph->metrics.width ) >>6);
                qDebug()<<"glyph->metrics.height = "<<((pFTFace->glyph->metrics.height ) >>6);
                qDebug()<<"glyph->metrics.horiBearingX = "<<((pFTFace->glyph->metrics.horiBearingX ) >>6);
                qDebug()<<"glyph->metrics.horiBearingY = "<<((pFTFace->glyph->metrics.horiBearingY ) >>6);
                qDebug()<<"glyph->metrics.horiAdvance = "<<((pFTFace->glyph->metrics.horiAdvance ) >>6);
                qDebug()<<"glyph->metrics.vertBearingX = "<<((pFTFace->glyph->metrics.vertBearingX ) >>6);
                qDebug()<<"glyph->metrics.vertBearingY = "<<((pFTFace->glyph->metrics.vertBearingY ) >>6);
                qDebug()<<"glyph->metrics.vertAdvance = "<<((pFTFace->glyph->metrics.vertAdvance ) >>6);
                qDebug()<<"glyph->bitmap.left = "<< (bitmap_glyph->left);
                qDebug()<<"glyph->bitmap.top = "<<(bitmap_glyph->top);
#endif

                qDebug("source bitmap rows %d,width %d pitch %d",bitmap->rows,bitmap->width,bitmap->pitch);
#if 0
                QString sbd = QString("origin data:\n");
                for(i = 0; i < bitmap->rows; i++){
                    for(j = 0; j < bitmap->pitch; j++){
                        sbd += QString(" 0x") + QString::number(bitmap->buffer[j],16);
                    }
                    sbd += QString("\n");
                }
                qDebug()<<sbd;

#endif

#if 1
                int bitmap_ascender = (pFTFace->size->metrics.ascender >>6);
                int bitmap_width = bitmap->width;//((pFTFace->glyph->metrics.width ) >>6);
                int bitmap_height = bitmap->rows;//((pFTFace->glyph->metrics.height ) >>6);
                int bitmap_left = bitmap_glyph->left;
                int bitmap_top = bitmap_ascender - bitmap_glyph->top;

                if(bitmap_left < 0 || (bitmap_left + bitmap_width) > width)
                {
                    if (bitmap_width > width)
                        offset_x = (width - bitmap_width)/2;
                    else
                        offset_x = 0;
                }
                else
                {
                    if (bitmap_width > width)
                        offset_x = (width - bitmap_width)/2;
                    else if (bitmap_left + bitmap_width > width)
                        offset_x = (width - bitmap_width);
                    else
                        offset_x = bitmap_left;
                }

                if ( bitmap_top < 0)
                {
                    if (bitmap_height > height)
                        offset_y = (height - bitmap_height)/2;
                    else
                        offset_y = 0;
                }
                else
                {
                    if (bitmap_height > height)
                        offset_y = (height - bitmap_height)/2;
                    else if (bitmap_top + bitmap_height > height)
                        offset_y = (height - bitmap_height);
                    else
                        offset_y = bitmap_top;
                }
#endif

                qDebug()<<"offset_x = "<<offset_x;
                qDebug()<<"offset_y = "<<offset_y;

                stand_bitmap = new unsigned char[unit];
                memset(stand_bitmap,0,unit);
                if(!stand_bitmap){
                    qDebug("new stand_bitmap failed\n\n");
                    goto clean;
                }
                if (bitmap->width != 0){
                    mid_bitmap = new unsigned char[width*height];
                    memset(mid_bitmap,0,width*height);
                    if(!mid_bitmap){
                        qDebug("new stand_bitmap failed\n\n");
                        delete stand_bitmap;
                        goto clean;
                    }
                    //redo static model
                    map_font_bit2byte_hori(mid_bitmap, bitmap->buffer, bitmap->width, bitmap->rows, bitmap->pitch, width, height, offset_x, offset_y);
#if 0
                    qDebug("mid bitmap height %d,width %d offset_x %d offset_y %d",height,width,offset_x,offset_y);
                    QString sbd = QString("mid trans data:\n");
                    for(i = 0; i < height; i++){
                        for(j = 0; j < width; j++){
                            sbd = sbd + QString(" 0x") + QString::number(mid_bitmap[i*width+j],16);
                        }
                        sbd += QString("\n");
                    }
                    qDebug()<<sbd;

#endif
                    //moveIsland(mid_bitmap,width,height);
                    //compress to bit pixel
                    map_byte2bit(stand_bitmap, mid_bitmap,  width, height, direction, order);
                }

#if 1
                //print info
                printstr = QString(" /*") + QString::fromUtf16((const ushort *)&unicode_charcode) + QString("(0x") + QString::number(code[k].src_charcode,16).toUpper() +  QString(")*/\n");
                printstr += QString("{");
                for(i = 0; i < unit; i++){
                    printstr += (QString("0x") + QString::number(stand_bitmap[i],16) + QString(",") );
                }
                printstr += QString("}\n");
                print_data_info(printstr);
#endif

                //save
                Draw_BitMap *tmp = new Draw_BitMap();
                tmp->rows = height;
                tmp->width = width;
                tmp->pitch = pitch;
                tmp->unit = unit;
                tmp->bitmap.append((const char*)stand_bitmap,unit);
                bitmaps.append(*tmp);
                delete tmp;
                //clean
                delete mid_bitmap;
                delete stand_bitmap;

                //free glyph
                FT_Done_Glyph(glyph);
                glyph  =  NULL;
            }
        }
        ret = 0;
    }

clean:
    //free face
    FT_Done_Face(pFTFace);
    pFTFace  =  NULL;

    qDebug("Free pFTLib\n");
    FT_Done_FreeType(pFTLib);

    if(cur_character_set)
        delete cur_character_set;
    return ret;
}

void Charset_Convert::moveIsland(unsigned char* mtx,int width,int height)
{
    int i,j;
    int top,bottom,left,right;
    int topLeft,topRight,bottomLeft,bottomRight;
    unsigned char(*matrix)[width] = (unsigned char(*)[width])mtx;

    for(j=0;j<height;j++)
    {
        for(i=0;i<width;i++)
        {
            //qDebug("x:%d, y:%d = %d",i,j,matrix[j][i]);
            if(matrix[j][i]){
                top = (j > 0) ? matrix[j-1][i] : 0;
                //qDebug("top %d",top);
                bottom = (j < (height-1)) ? matrix[j+1][i] : 0;
                //qDebug("bottom %d",bottom);
                left = (i > 0) ? matrix[j][i-1] : 0;
                //qDebug("left %d",left);
                right =  (i < (width-1)) ? matrix[j][i+1] : 0;
                //qDebug("right %d",right);
                topLeft = (j > 0 && i > 0) ? matrix[j-1][i-1] : 0;
                topRight = (j > 0 && i < (width-1)) ? matrix[j-1][i+1] : 0;
                bottomLeft = (j < (height-1) && i > 0) ? matrix[j+1][i-1] : 0;
                bottomRight = (j < (height-1) && i < (width-1)) ? matrix[j+1][i+1] : 0;

                if(!(top| bottom|left|right) && (topLeft+topRight+bottomLeft+bottomRight) < 2){
                    matrix[j][i] = 0;
                    qDebug("set x:%d y:%d 0", i, j);
                }
            }

        }
    }
}
