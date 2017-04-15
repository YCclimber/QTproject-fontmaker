#include "convertthread.h"

convertthread::convertthread(QList<Convert> &convertlist,qint64 maxsize)
{
    converts = convertlist;
    MAX_SIZE = maxsize;
    qDebug("convert count %d\n",converts.count());
}

convertthread::~convertthread()
{
    if(convert_character_set){
        delete convert_character_set;
        convert_character_set = NULL;
    }
}

void convertthread::get_charsets(QList<struct Charset_Data> &out){
    out = charsets;
}

void convertthread::run(){
    QList<Convert>::iterator iter = converts.begin();
    //qDebug("convert begin\n");
    //struct Convert *cur_convert;
    int i = 1;
    for(;iter!=converts.end();iter++,i++){
        //clear
        //qDebug("convert loop %d\n",i);
        bitmaps.clear();
        //print
        //qDebug("convert num print");
        emit this->convertnum(i,converts.count());
        //do
        //qDebug("get convert obj");
        //cur_convert = &(*iter);
        //qDebug("do convert obj");
        if(0 > do_convert_bat(&(*iter))){
            qDebug()<<QString::fromUtf8("failed covert,id=")<<iter->csa.name_id;
            return;
        }
    }
    //emit convertdone();
    //this->terminate();
    //this->wait(1);
}

int convertthread::do_convert_bat(struct Convert *convert_obj){
    pFTLib = NULL;
    pFTFace = NULL;
    pFTError = 0 ;
    int count = 1;
    int ret = -1;
    //Init FreeType Lib to manage memory
    pFTError = FT_Init_FreeType( & pFTLib);
    if (pFTError)
    {
        qDebug("There is some error when Init Library");
        return -1;
    }
    qDebug("FreeType init done!");

    switch(convert_obj->encode){
    case ENCODE_GB18030:
        convert_character_set = new class GB18030_Character_Set;
        break;
    case ENCODE_UCS_4:
        convert_character_set = new class Unicode_Character_Set;
        break;
    default:
        qDebug("unknow code encode");
        return -1;
    }
    //
    QFile file(convert_obj->sourcepath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Can't open the file";
        return -1;
    }
    qint64 size = file.size();
    if(new_memory_face)
        delete new_memory_face;
    new_memory_face = new char[size]();
    file.read(new_memory_face,size);

    //create font face from font file
    qDebug()<<convert_obj->sourcepath;
    pFTError = FT_New_Memory_Face(pFTLib, (FT_Byte*)new_memory_face, size, 0,&pFTFace);
    //pFTError = FT_New_Face(pFTLib, convert_obj->sourcepath.toStdString().c_str(), 0, &pFTFace);
    if (!pFTError)
    {
        qDebug("FT New Face open done!");
        FT_Set_Pixel_Sizes(pFTFace,convert_obj->fc.width,convert_obj->fc.hight);
        qDebug("FT Set Pixel Sizes width %d, hight %d!",convert_obj->fc.width,convert_obj->fc.hight);
        FT_Glyph glyph;
        FT_UInt glyph_index;
        FT_ULong unicode_charcode;
        //QString str_charcnt;
        unsigned long cur_charcode = convert_obj->csa.lower;

loop:
        emit this->convertcharnum(count,convert_obj->csa.char_nums);//!!!!!此处 会引起崩溃
        qDebug("cur area code 0x%x",cur_charcode);
        qDebug("count = %d",count);
        convert_character_set->to_unicode( &cur_charcode, &unicode_charcode);
        qDebug("unicode = 0x%x",unicode_charcode);
        glyph_index = FT_Get_Char_Index(pFTFace,  unicode_charcode );
        qDebug("glyph index = %d",glyph_index);
        qDebug("FT_Load_Glyph\n");
        pFTError = FT_Load_Glyph(pFTFace, glyph_index, FT_LOAD_DEFAULT);
        if (pFTError)
        {
            qDebug("FT_Load_Glyph error\n");
            return -1;
        }
        qDebug("FT_Get_Glyph\n");
        pFTError = FT_Get_Glyph(pFTFace -> glyph,  & glyph);
        if (!pFTError)
        {

            //FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0 , 1);
            qDebug("FT_Glyph_To_Bitmap\n");
            FT_Glyph_To_Bitmap(&glyph, ft_render_mode_mono, 0 , 1);
            FT_BitmapGlyph    bitmap_glyph = (FT_BitmapGlyph)glyph;
            FT_Bitmap *bitmap  =  &(bitmap_glyph -> bitmap);
            qDebug("bitmap rows %d,width %d",bitmap->rows,bitmap->width);
            //store
            Stand_BitMap *new_bmp = new Stand_BitMap;
            if(!new_bmp){
                qDebug("new Stand_BitMap failed\n\n");
                return -1;
            }
            int stand_width;
            if(bitmap->width == 0 && unicode_charcode < 128 ){
                stand_width = (pFTFace->glyph->metrics.horiAdvance>>6)/2;//just for ASCII
            }
            else
                stand_width = (pFTFace->glyph->metrics.horiAdvance>>6);
#if 0
            bitmap_format_to_stand(bitmap->buffer,
                                   bitmap->width,
                                   bitmap->rows,
                                   stand_width,//(pFTFace->glyph->metrics.horiAdvance>>6),
                                   (pFTFace->glyph->metrics.vertAdvance>>6),
                                   bitmap->pitch,
                                   bitmap_glyph->left,
                                   ((pFTFace->size->metrics.ascender >>6) - bitmap_glyph->top),
                                   convert_obj->fc.mode,
                                   convert_obj->fc.order,
                                   convert_obj->fc.alignchar_size,
                                   new_bmp);
#else
            int stand_hight = (pFTFace->glyph->metrics.vertAdvance>>6);
            unsigned char *mid = new unsigned char[stand_width * stand_hight]();
            qDebug("bitmap_format_to_stand mid %p",mid);
            qDebug("stand_hight %d,stand_width %d mode %d order %d", stand_hight,stand_width,convert_obj->fc.mode,convert_obj->fc.order);
            //extand
            bitmap_extand(bitmap->buffer, mid,
                          bitmap->width, bitmap->rows,
                          stand_width, stand_hight, bitmap->pitch,
                          bitmap_glyph->left, ((pFTFace->size->metrics.ascender >>6) - bitmap_glyph->top));
            //convert
            qDebug("convert bitmap\n");
            switch(convert_obj->fc.mode){
            case 0:
            default:
                new_bmp->pitch = (stand_width + 8*convert_obj->fc.alignchar_size -1)/(8 * convert_obj->fc.alignchar_size);
                new_bmp->bytes = (new_bmp->pitch) * stand_hight;
                new_bmp->buffer = new unsigned char[new_bmp->bytes]();
                for(int i = 0; i < stand_hight; i++)
                    for(int j = 0; j < stand_width; j++){
                        new_bmp->buffer[ i * (new_bmp->pitch) + (j>>3)] |= ( mid[i * stand_width + j] ? (convert_obj->fc.order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                        //qDebug(" %d,mid %2x ,map->buffer  %2x",i * (map->pitch) + (j>>3), mid[i * stand_width + j], map->buffer [ i * (map->pitch) + (j>>3)]);
                    }
                break;
            case 1:
                transpose(mid,stand_hight,stand_width);
                new_bmp->pitch = (stand_hight + 8*convert_obj->fc.alignchar_size -1)/(8 * convert_obj->fc.alignchar_size);
                new_bmp->bytes = (new_bmp->pitch) * stand_width;
                new_bmp->buffer = new unsigned char[new_bmp->bytes]();
                for(int i = 0; i < stand_width; i++)
                    for(int j = 0; j < stand_hight; j++){
                        new_bmp->buffer[ i * (new_bmp->pitch) + (j>>3)] |= ( mid[i * stand_hight + j] ? (convert_obj->fc.order ? (1 << (j%8)): (0x80 >> (j%8))) : 0 );
                        //qDebug(" %d,mid %2x ,map->buffer  %2x",i * (map->pitch) + (j>>3), mid[i * stand_hight + j], map->buffer [ i * (map->pitch) + (j>>3)]);
                    }
                break;
            }
            qDebug("bitmap pitch %d",new_bmp->pitch);
            qDebug("bitmap bytes %d",new_bmp->bytes);
            delete mid;
            new_bmp->rows = stand_hight;
            new_bmp->width = stand_width;
#endif
            bitmaps.append(*new_bmp);
            delete new_bmp;
            //  free glyph
            FT_Done_Glyph(glyph);
            glyph  =  NULL;
        }
        else{
            qDebug("FT_Get_Glyph error\n");
            return -1;
        }

next:
        count++;
        ret = convert_character_set->get_next_code(&cur_charcode,
                                              convert_obj->csa.rule_index,
                                              convert_obj->csa.lower,
                                              convert_obj->csa.upper
                                              );
        //qDebug("get next result %d",ret);
        if(ret == 0)
            goto loop;
        else if(ret == -1)
          qDebug()<<"out of range";

        //  free face
        FT_Done_Face(pFTFace);
        pFTFace = NULL;
        //convert to block
        qDebug("convert to block");
        struct Charset_Data *p = new Charset_Data;
        int addr_start;
        if(charsets.empty())
            addr_start = 0;
        else
            addr_start = ROUND(charsets.last().charset_hdr.addr_end, convert_obj->fc.alignarea_size);
        bitmap_2_charset(bitmaps,
                         p,
                         addr_start,
                         convert_obj,
                         convert_character_set,
                         MAX_SIZE);
        for(int i = 0; i < bitmaps.count(); i++){
            delete bitmaps.at(i).buffer;
        }
        bitmaps.clear();
        charsets.append(*p);
        qDebug("charset count %d",charsets.count());
    }
    else{
        qDebug("Free New Face open failed!");
        return -1;
    }
    qDebug("Free pFTLib\n");
    FT_Done_FreeType(pFTLib);
    qDebug("delete pFTLib\n");
    if(convert_character_set)
        delete convert_character_set;
    return 0;
}
