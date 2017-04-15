#ifndef CHARACTERSET
#define CHARACTERSET

#include "common.h"
#include <QList>



class Character_Set{
public:
enum{
    ENCODE_GB18030 = (54936UL),
    ENCODE_UCS_4 = (65005UL),
};

public:
    //info
    virtual int get_encode(){ return 0; }
    virtual QString get_name(){ return QString("NULL"); }
    virtual int get_countmode(int rule_index);
    virtual int get_topper(int rule_index);
    virtual int get_bottomer(int rule_index);

    //about rules
    virtual void default_rules_init();
    //about members
    virtual void default_members_init();
    //about area
    virtual void default_areas_init();
    virtual void member_index_areas_init(int index);
    virtual int area_chars_count( int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);
    virtual int area_chars_offset( unsigned long code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);

    virtual int get_next_code(unsigned long* code, int rule_index, unsigned long top, unsigned long bottom);
    virtual int get_next_code_ex(unsigned long* code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);
    virtual int get_pre_code_ex(unsigned long* code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);

    virtual int to_unicode(unsigned long *in, unsigned long*out){ *out = *in; return 0; }
    virtual int unicode_to(unsigned long *in, unsigned long*out){ *out = *in; return 0; }

    //other
    int count_of_area_filling(unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit);
    int count_of_S_shape(unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit);
    int offset_of_area_filling(unsigned long code,unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit);
    int offset_of_S_shape(unsigned long code,unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit);
    int next_code(unsigned long* code, unsigned long top, unsigned long bottom);
    int pre_code(unsigned long* code, unsigned long top, unsigned long bottom);

    int trans_table(struct Code_Table p[], unsigned long top, unsigned long bottom, unsigned long key);
    int trans_table_ex(struct Code_Table p[], unsigned long top, unsigned long bottom, unsigned long key);

 public:
    QList<struct Character_Set_Rules>rules;
    QList<struct Character_Set_Area>areas;
    QList<struct Character_Set_Area_Member>members;
};

/***GB18030_Character_Set***/
class GB18030_Character_Set : public Character_Set{

public:
    GB18030_Character_Set(){ default_rules_init(); default_members_init(); /*default_areas_init()*/; }
    ~GB18030_Character_Set(){ rules.clear();members.clear();}
    //info
    int get_encode(){return Character_Set::ENCODE_GB18030;}
    QString get_name(){ return QString::fromUtf8("GB18030");}

    //about rules
    void default_rules_init();
    //members
    void default_members_init();
    //about area
    //void default_areas_init();
    //int area_chars_count( int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);
    //int area_chars_offset( unsigned long code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit);
    //int get_next_code(unsigned long* code, int rule_index, unsigned long top, unsigned long bottom);

    int to_unicode(unsigned long *in, unsigned long*out);
    int unicode_to(unsigned long *in, unsigned long*out);

};

static struct Character_Set_Rules gb18030_rule_default[]={
    /*rule index   bytes    name                            countmode     lower             upper           */
    {       0,      1,      QString::fromUtf8("单字节") ,       0,         0x00        ,     0x7F          },
    {       1,      2,      QString::fromUtf8("双字节1") ,      1,         0x8140      ,     0xFE7E        },
    {       2,      2,      QString::fromUtf8("双字节2") ,      1,         0x8180      ,     0xFEFE        },
    {       3,      4,      QString::fromUtf8("四字节") ,       0,         0x81308130  ,     0xFE39FE39    },
    {       4,      4,      QString::fromUtf8("无限制") ,       0,         0x00000000  ,     0xFFFFFFFF    },
};
#define Gb18030_Rule_Num ( sizeof(gb18030_rule_default) / sizeof(struct Character_Set_Rules) )

static struct Character_Set_Area gb18030_area_Ascii[]={
    /*rule index    name                                  lower         upper       curcode  */
    {       0,      QString::fromUtf8("单字节区")   ,       0x00       , 0x7F    ,      0      },
};
#define Gb18030_Area_Ascii_Num ( sizeof(gb18030_area_Ascii) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area gb18030_area_Symbols[]={
    /*rule index    name                                  lower         upper       curcode  */
    {       2,      QString::fromUtf8("双字节1区" ) ,       0xA1A1     , 0xA9FE  ,      0      },
    {       1,      QString::fromUtf8("双字节5区1") ,       0xA840     , 0xA97E  ,      0      },
    {       2,      QString::fromUtf8("双字节5区2") ,       0xA880     , 0xA9A0  ,      0      },
};
#define Gb18030_Area_Symbols_Num ( sizeof(gb18030_area_Symbols) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area gb18030_area_Hanzi[]={
    /*rule index    name                                  lower         upper       curcode  */
    {       2,      QString::fromUtf8("双字节2区" ) ,       0xB0A1     , 0xF7FE  ,      0      },
    {       1,      QString::fromUtf8("双字节3区1") ,       0x8140     , 0xA07E  ,      0      },
    {       2,      QString::fromUtf8("双字节3区2") ,       0x8180     , 0xA0FE  ,      0      },
    {       1,      QString::fromUtf8("双字节4区1") ,       0xAA40     , 0xFE7E  ,      0      },
    {       2,      QString::fromUtf8("双字节4区2") ,       0xAA80     , 0xFEA0  ,      0      },
    {       3,      QString::fromUtf8("四字节CJK扩充A区") ,  0x8139EE39 , 0x82358738  ,  0       },
    {       3,      QString::fromUtf8("四字节CJK扩充B区") ,  0x95328236 , 0x9835F336  ,  0       },
};
#define Gb18030_Area_Hanzi_Num ( sizeof(gb18030_area_Hanzi) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area_Member gb18030_member_default[]={
 {   0,              QString::fromUtf8("ASCII"),                  gb18030_area_Ascii,                  Gb18030_Area_Ascii_Num },
 {   1,              QString::fromUtf8("符号区"),                  gb18030_area_Symbols,                  Gb18030_Area_Symbols_Num },
 {   2,              QString::fromUtf8("汉字区"),                  gb18030_area_Hanzi,                  Gb18030_Area_Hanzi_Num },
// {   3,              QString::fromUtf8("用户自定义区"),                  unicode_area_Modifier_Letters,                  Unicode_Area_Modifier_Letters_Num },
};
#define Gb18030_Member_Num ( sizeof(gb18030_member_default) / sizeof(struct Character_Set_Area_Member) )

/***Unicode_Character_Set***/

class Unicode_Character_Set : public Character_Set{
public:
    Unicode_Character_Set(){ default_rules_init(); default_members_init(); /*default_areas_init()*/; }
    ~Unicode_Character_Set(){ };

public:
    //info
    int get_encode(){return ENCODE_UCS_4;}
    QString get_name(){ return QString::fromUtf8("Unicode");}

    //about rules
    void default_rules_init();
    //members
    void default_members_init();
    //about area
    //void default_areas_init();
};

static struct Character_Set_Rules unicode_rule_default[]={
    /*rule index   bytes    name                            countmode     lower             upper           */
    {       0,      4,      QString::fromUtf8("基本平面") ,       0,         0x0000     ,     0xFFFFFFFF      },
};
#define Unicode_Rule_Num ( sizeof(unicode_rule_default) / sizeof(struct Character_Set_Rules) )

enum UnicodeClass{
    European_Scripts,
    Modifier_Letters,
    Combining_Marks,
    African_Scripts,
    Middle_Eastern_Scripts,
    Central_Asian_Scripts,
    South_Asian_Scripts,
    Southeast_Asian_Scripts,
    Indonesia_Oceania_Scripts,
    East_Asian_Scripts,
    American_Scripts,
    Other_Scripts,
    Notational_Systems,
    Punctuation,
    Alphanumeric_Symbols,
    Technical_Symbols,
    ASCII_Digits,
    Mathematical_Symbols,
    Emoji_Pictographs,
    Other_Symbols,
    Specials,
    Private_Use,
    Surrogates,
    Noncharacters,
};

static struct Character_Set_Area unicode_area_European_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //European Scripts
    {       0,      QString::fromUtf8("Armenian")   ,               0x0530       ,  0x058F    ,      0      },
    {       0,      QString::fromUtf8("Armenian Ligatures" ) ,      0xFB13     ,    0xFB17  ,      0      },
    {       0,      QString::fromUtf8("Caucasian Albanian") ,       0x10530     ,   0x1056F  ,      0      },
    {       0,      QString::fromUtf8("Cypriot Syllabary") ,        0x10800     ,   0x1083F  ,      0      },
    {       0,      QString::fromUtf8("Cyrillic" ) ,                0x0400     ,    0x04FF  ,      0      },
    {       0,      QString::fromUtf8("Cyrillic Supplement") ,      0x0500    ,     0x052F  ,      0      },
    {       0,      QString::fromUtf8("Cyrillic Extended-A") ,      0x2DE0     ,    0x2DFF  ,      0      },
    {       0,      QString::fromUtf8("Cyrillic Extended-B") ,      0xA640     ,    0xA69F  ,      0      },
    {       0,      QString::fromUtf8("Cyrillic Extended-C") ,      0x1C80     ,    0x1C8F  ,      0      },
    {       0,      QString::fromUtf8("Elbasan")   ,                0x10500       , 0x1052F    ,      0      },
    {       0,      QString::fromUtf8("Georgian" ) ,                0x10A0     ,    0x10FF  ,      0      },
    {       0,      QString::fromUtf8("Georgian Supplement") ,      0x2D00     ,    0x2D2F  ,      0      },
    {       0,      QString::fromUtf8("Glagolitic") ,               0x2C00     ,    0x2C5F  ,      0      },
    {       0,      QString::fromUtf8("Glagolitic Supplement" ) ,   0x1E000     ,   0x1E02F ,      0      },
    {       0,      QString::fromUtf8("Gothic") ,                   0x10330     ,   0x1034F  ,      0      },
    {       0,      QString::fromUtf8("Greek") ,                    0x0370     ,    0x03FF  ,      0      },
    {       0,      QString::fromUtf8("Greek Extended") ,           0x1F00     ,    0x1FFF  ,      0      },
    {       0,      QString::fromUtf8("Ancient Greek Numbers") ,    0x10140     ,   0x1018F  ,      0      },
    {       0,      QString::fromUtf8("Basic Latin (ASCII)")   ,    0x0000       ,  0x007F    ,      0      },
    {       0,      QString::fromUtf8("Latin-1 Supplement")   ,     0x0080       ,  0x00FF    ,      0      },
    {       0,      QString::fromUtf8("Latin Extended-A")   ,       0x0100       ,  0x017F    ,      0      },
    {       0,      QString::fromUtf8("Latin Extended-B")   ,       0x0180       ,  0x024F    ,      0      },
    {       0,      QString::fromUtf8("Latin Extended-C")   ,       0x2C60       ,  0x2C7F    ,      0      },
    {       0,      QString::fromUtf8("Latin Extended-D")   ,       0xA720       ,  0xA7FF    ,      0      },
    {       0,      QString::fromUtf8("Latin Extended-E")   ,       0xAB30       ,  0xAB6F   ,      0      },
    {       0,      QString::fromUtf8("Latin Extended Additional"), 0x1E00       , 0x1EFF    ,      0      },
    {       0,      QString::fromUtf8("Latin Ligatures")   ,        0xFB00       ,  0xFB06    ,      0      },
    {       0,      QString::fromUtf8("Fullwidth Latin Letters"),   0xFF00       , 0xFF5E    ,      0      },
    {       0,      QString::fromUtf8("IPA Extensions")   ,         0x0250       ,  0x02AF    ,      0      },
    {       0,      QString::fromUtf8("Phonetic Extensions")   ,    0x1D00       ,  0x1D7F    ,      0      },
    {       0,      QString::fromUtf8("Phonetic Extensions Supplement")   , 0x1D80       , 0x1DBF    ,      0      },
    {       0,      QString::fromUtf8("Linear A")   ,               0x10600       , 0x1077F    ,      0      },
    {       0,      QString::fromUtf8("Linear B Syllabary")   ,     0x10000       , 0x1007F    ,      0      },
    {       0,      QString::fromUtf8("Linear B Ideograms")   ,     0x10080      ,  0x100FF    ,      0      },
    {       0,      QString::fromUtf8("Aegean Numbers")   ,         0x10100       , 0x1013F    ,      0      },
    {       0,      QString::fromUtf8("Ogham")   ,                  0x1680       ,  0x169F    ,      0      },
    {       0,      QString::fromUtf8("Old Hungarian")   ,          0x10C80       , 0x10CFF    ,      0      },
    {       0,      QString::fromUtf8("Old Italic")   ,             0x10300       , 0x1032F    ,      0      },
    {       0,      QString::fromUtf8("Old PermicEast Asian Scripts")   ,             0x10350       , 0x1037F    ,      0      },
    {       0,      QString::fromUtf8("Phaistos Disc")   ,          0x101D0       , 0x101FF    ,      0      },
    {       0,      QString::fromUtf8("Runic")   ,                  0x16A0       ,  0x16FF    ,      0      },
    {       0,      QString::fromUtf8("Shavian")   ,                0x10450       , 0x1047F    ,      0      },
};
#define Unicode_Area_European_Scripts_Num ( sizeof(unicode_area_European_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Modifier_Letters[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Modifier Letters
    {       0,      QString::fromUtf8("Modifier Tone Letters")   ,  0xA700       , 0xA71F    ,      0      },
    {       0,      QString::fromUtf8("Spacing Modifier Letters"),  0x02B0       , 0x02FF    ,      0      },
    {       0,      QString::fromUtf8("Superscripts and Subscripts")   ,    0x2070      , 0x209F    ,      0      },
};
#define Unicode_Area_Modifier_Letters_Num ( sizeof(unicode_area_Modifier_Letters) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Combining_Marks[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Combining Marks
    {       0,      QString::fromUtf8("Combining Diacritical Marks")   ,    0x0300       , 0x036F    ,      0      },
    {       0,      QString::fromUtf8("Combining Diacritical Marks Extended")   ,       0x1AB0       , 0x1AFF    ,      0      },
    {       0,      QString::fromUtf8("Combining Diacritical Marks Supplement")   ,     0x1DC0       , 0x1DFF    ,      0      },
    {       0,      QString::fromUtf8("Combining Diacritical Marks for Symbols")   ,    0x20D0       , 0x20FF    ,      0      },
    {       0,      QString::fromUtf8("Combining Half Marks")   ,   0xFE20       , 0xFE2F    ,      0      },
};
#define Unicode_Area_Combining_Marks_Num ( sizeof(unicode_area_Combining_Marks) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_African_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //African Scripts
    {       0,      QString::fromUtf8("Adlam")   ,                  0x1E900       , 0x1E95F    ,      0      },
    {       0,      QString::fromUtf8("Bamum")   ,                  0xA6A0       , 0xA6FF    ,      0      },
    {       0,      QString::fromUtf8("Bamum Supplement")   ,       0x16800       , 0x16A3F    ,      0      },
    {       0,      QString::fromUtf8("Bassa Vah")   ,              0x16AD0       , 0x16AFF    ,      0      },
    {       0,      QString::fromUtf8("Coptic ")   ,                0x2C80       , 0x2CFF    ,      0      },
    {       0,      QString::fromUtf8("Coptic in Greek block")   ,  0x03E2       , 0x3EF    ,      0      },
    {       0,      QString::fromUtf8("Coptic Epact Numbers")   ,   0x102E0       , 0x102FF    ,      0      },
    {       0,      QString::fromUtf8("Egyptian Hieroglyphs ")   ,  0x13000       , 0x1342F    ,      0      },
    {       0,      QString::fromUtf8("Ethiopic")   ,               0x1200       , 0x137F    ,      0      },
    {       0,      QString::fromUtf8("Ethiopic Supplement")   ,    0x1380       , 0x139F    ,      0      },
    {       0,      QString::fromUtf8("Ethiopic Extended")   ,      0x2D80       , 0x2DDF    ,      0      },
    {       0,      QString::fromUtf8("Ethiopic Extended-A")   ,    0xAB00       , 0xAB2F    ,      0      },
    {       0,      QString::fromUtf8("Mende Kikakui")   ,          0x1E800       , 0x1E8DF    ,      0      },
    {       0,      QString::fromUtf8("Meroitic Cursive")   ,       0x109A0       , 0x109FF    ,      0      },
    {       0,      QString::fromUtf8("Meroitic Hieroglyphs")   ,   0x10980       , 0x1099F    ,      0      },
    {       0,      QString::fromUtf8("N'Ko")   ,                   0x07C0       , 0x07FF    ,      0      },
    {       0,      QString::fromUtf8("Osmanya")   ,                0x10480       , 0x104AF    ,      0      },
    {       0,      QString::fromUtf8("Tifinagh")   ,               0x2D30       , 0x2D7F    ,      0      },
    {       0,      QString::fromUtf8("Vai")   ,                    0xA500       , 0xA63F    ,      0      },
};
#define Unicode_Area_African_Scripts_Num ( sizeof(unicode_area_African_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Middle_Eastern_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Middle Eastern Scripts
    {       0,      QString::fromUtf8("Anatolian Hieroglyphs")   ,  0x14400       , 0x1467F    ,      0      },
    {       0,      QString::fromUtf8("Arabic")   ,                 0x0600       , 0x06FF    ,      0      },
    {       0,      QString::fromUtf8("Arabic Supplement")   ,      0x0750       , 0x077F    ,      0      },
    {       0,      QString::fromUtf8("Arabic Extended-A")   ,      0x08A0       , 0x08FF    ,      0      },
    {       0,      QString::fromUtf8("Arabic Presentation Forms-A")   ,       0xFB50       , 0xFDFF    ,      0      },
    {       0,      QString::fromUtf8("Arabic Presentation Forms-B")   ,       0xFE70       , 0xFEFF    ,      0      },
    {       0,      QString::fromUtf8("Aramaic, Imperial")   ,      0x10840       , 0x1085F    ,      0      },
    {       0,      QString::fromUtf8("Avestan")   ,                0x10B00       , 0x10B3F    ,      0      },
    {       0,      QString::fromUtf8("Carian")   ,                 0x102A0       , 0x102DF    ,      0      },
    {       0,      QString::fromUtf8("Cuneiform")   ,              0x12000       , 0x123FF    ,      0      },
    {       0,      QString::fromUtf8("Cuneiform Numbers and Punctuation")   , 0x12400       , 0x1247F    ,      0      },
    {       0,      QString::fromUtf8("Early Dynastic Cuneiform"),  0x12480       , 0x1254F    ,      0      },
    {       0,      QString::fromUtf8("Old Persian")   ,            0x103A0       , 0x103DF    ,      0      },
    {       0,      QString::fromUtf8("Ugaritic")   ,               0x10380       , 0x1039F    ,      0      },
    {       0,      QString::fromUtf8("Hatran")   ,                 0x108E0       , 0x108FF    ,      0      },
    {       0,      QString::fromUtf8("Hebrew")   ,                 0x0590       , 0x05FF    ,      0      },
    {       0,      QString::fromUtf8("Hebrew Presentation Forms"), 0xFB1D       , 0xFB4F    ,      0      },
    {       0,      QString::fromUtf8("Lycian")   ,                 0x10280       , 0x1029F    ,      0      },
    {       0,      QString::fromUtf8("Lydian")   ,                 0x10920       , 0x1093F    ,      0      },
    {       0,      QString::fromUtf8("Mandaic")   ,                0x0840       , 0x085F    ,      0      },
    {       0,      QString::fromUtf8("Nabataean")   ,              0x10880       , 0x108AF    ,      0      },
    {       0,      QString::fromUtf8("Old North Arabian")   ,      0x10A80       , 0x10A9F    ,      0      },
    {       0,      QString::fromUtf8("Old South Arabian")   ,      0x10A60       , 0x10A7F    ,      0      },
    {       0,      QString::fromUtf8("Pahlavi, Inscriptional")  ,  0x10B60       , 0x10B7F    ,      0      },
    {       0,      QString::fromUtf8("Pahlavi, Psalter")   ,       0x10B80       , 0x10BAF    ,      0      },
    {       0,      QString::fromUtf8("Palmyrene")   ,              0x10860       , 0x1087F    ,      0      },
    {       0,      QString::fromUtf8("Parthian, Inscriptional") ,  0x10B40       , 0x10B5F    ,      0      },
    {       0,      QString::fromUtf8("Phoenician")   ,             0x10900       , 0x1091F    ,      0      },
    {       0,      QString::fromUtf8("Samaritan")   ,              0x0800       , 0x083F    ,      0      },
    {       0,      QString::fromUtf8("Syriac")   ,                 0x0700       , 0x074F    ,      0      },
};
#define Unicode_Area_Middle_Eastern_Scripts_Num ( sizeof(unicode_area_Middle_Eastern_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Central_Asian_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Central Asian Scripts
    {       0,      QString::fromUtf8("Manichaean")   ,             0x10AC0       , 0x10AFF    ,      0      },
    {       0,      QString::fromUtf8("Marchen")   ,                0x11C70       , 0x11CBF    ,      0      },
    {       0,      QString::fromUtf8("Mongolian")   ,              0x1800       , 0x18AF    ,      0      },
    {       0,      QString::fromUtf8("Mongolian Supplement")   ,   0x11660       , 0x1167F    ,      0      },
    {       0,      QString::fromUtf8("Old Turkic")   ,             0x10C00       , 0x10C4F    ,      0      },
    {       0,      QString::fromUtf8("Phags-Pa")   ,               0xA840       , 0xA87F    ,      0      },
    {       0,      QString::fromUtf8("Tibetan")   ,                0x0F00       , 0x0FFF    ,      0      },
};
#define Unicode_Area_Central_Asian_Scripts_Num ( sizeof(unicode_area_Central_Asian_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_South_Asian_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //South Asian Scripts
    {       0,      QString::fromUtf8("Ahom")   ,                   0x11700       , 0x1173F    ,      0      },
    {       0,      QString::fromUtf8("Bengali and Assamese")   ,   0x0980       , 0x09FF    ,      0      },
    {       0,      QString::fromUtf8("Bhaiksuki")   ,              0x11C00       , 0x11C6F    ,      0      },
    {       0,      QString::fromUtf8("Brahmi")   ,                 0x11000       , 0x1107F    ,      0      },
    {       0,      QString::fromUtf8("Chakma")   ,                 0x11100       , 0x1114F    ,      0      },
    {       0,      QString::fromUtf8("Devanagari")   ,             0x0900       , 0x097F    ,      0      },
    {       0,      QString::fromUtf8("Devanagari Extended")   ,    0xA8E0       , 0xA8FF    ,      0      },
    {       0,      QString::fromUtf8("Grantha")   ,                0x11300       , 0x1137F    ,      0      },
    {       0,      QString::fromUtf8("Gujarati")   ,               0x0A80       , 0x0AFF    ,      0      },
    {       0,      QString::fromUtf8("Gurmukhi")   ,               0x0A00       , 0x0A7F    ,      0      },
    {       0,      QString::fromUtf8("Kaithi")   ,                 0x11080       , 0x110CF    ,      0      },
    {       0,      QString::fromUtf8("Kannada")   ,                0x0C80       , 0x0CFF    ,      0      },
    {       0,      QString::fromUtf8("Kharoshthi")   ,             0x10A00       , 0x10A5F    ,      0      },
    {       0,      QString::fromUtf8("Khojki")   ,                 0x11200       , 0x1124F    ,      0      },
    {       0,      QString::fromUtf8("Khudawadi")   ,              0x112B0       , 0x112FF    ,      0      },
    {       0,      QString::fromUtf8("Lepcha")   ,                 0x1C00       , 0x1C4F    ,      0      },
    {       0,      QString::fromUtf8("Limbu")   ,                  0x1900       , 0x194F    ,      0      },
    {       0,      QString::fromUtf8("Mahajani")   ,               0x11150       , 0x1117F    ,      0      },
    {       0,      QString::fromUtf8("Malayalam")   ,              0x0D00       , 0x0D7F    ,      0      },
    {       0,      QString::fromUtf8("Meetei Mayek")   ,           0xABC0       , 0xABFF    ,      0      },
    {       0,      QString::fromUtf8("Meetei Mayek Extensions") ,  0xAAE0       , 0xAAFF    ,      0      },
    {       0,      QString::fromUtf8("Modi")   ,                   0x11600       , 0x1165F    ,      0      },
    {       0,      QString::fromUtf8("Mro")   ,                    0x16A40       , 0x16A6F    ,      0      },
    {       0,      QString::fromUtf8("Multani")   ,                0x11280       , 0x112AF    ,      0      },
    {       0,      QString::fromUtf8("Newa")   ,                   0x11400       , 0x1147F    ,      0      },
    {       0,      QString::fromUtf8("Ol Chiki")   ,               0x1C50       , 0x1C7F    ,      0      },
    {       0,      QString::fromUtf8("Oriya (Odia)")   ,           0x0B00       , 0x0B7F    ,      0      },
    {       0,      QString::fromUtf8("Saurashtra")   ,             0xA880       , 0xA8DF    ,      0      },
    {       0,      QString::fromUtf8("Sharada")   ,                0x11180       , 0x111DF    ,      0      },
    {       0,      QString::fromUtf8("Siddham")   ,                0x11580       , 0x115FF    ,      0      },
    {       0,      QString::fromUtf8("Sinhala")   ,                0x0D80       , 0x0DFF    ,      0      },
    {       0,      QString::fromUtf8("Sinhala Archaic Numbers") ,  0x111E0       , 0x111FF    ,      0      },
    {       0,      QString::fromUtf8("Sora Sompeng")   ,           0x110D0       , 0x110FF    ,      0      },
    {       0,      QString::fromUtf8("Syloti Nagri")   ,           0xA800       , 0xA82F    ,      0      },
    {       0,      QString::fromUtf8("Takri")   ,                  0x11680       , 0x116CF    ,      0      },
    {       0,      QString::fromUtf8("Tamil")   ,                  0x0B80       , 0x0BFF    ,      0      },
    {       0,      QString::fromUtf8("Telugu")   ,                 0x0C00       , 0x0C7F    ,      0      },
    {       0,      QString::fromUtf8("Thaana")   ,                 0x0780       , 0x07BF    ,      0      },
    {       0,      QString::fromUtf8("Tirhuta")   ,                0x11480       , 0x114DF    ,      0      },
    {       0,      QString::fromUtf8("Vedic Extensions")   ,       0x1CD0       , 0x1CFF    ,      0      },
    {       0,      QString::fromUtf8("Warang Citi")   ,            0x118A0       , 0x118FF    ,      0      },
};
#define Unicode_Area_South_Asian_Scripts_Num ( sizeof(unicode_area_South_Asian_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Southeast_Asian_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Southeast Asian Scripts
    {       0,      QString::fromUtf8("Cham")   ,                   0xAA00       , 0xAA5F    ,      0      },
    {       0,      QString::fromUtf8("Kayah Li")   ,               0xA900       , 0xA92F    ,      0      },
    {       0,      QString::fromUtf8("Khmer")   ,                  0x1780       , 0x17FF    ,      0      },
    {       0,      QString::fromUtf8("Khmer Symbols")   ,          0x19E0       , 0x19FF    ,      0      },
    {       0,      QString::fromUtf8("Lao")   ,                    0x0E80       , 0x0EFF    ,      0      },
    {       0,      QString::fromUtf8("Myanmar")   ,                0x1000       , 0x109F    ,      0      },
    {       0,      QString::fromUtf8("Myanmar Extended-A")   ,     0xAA60       , 0xAA7F    ,      0      },
    {       0,      QString::fromUtf8("Myanmar Extended-B")   ,     0xA9E0       , 0xA9FF    ,      0      },
    {       0,      QString::fromUtf8("New Tai Lue")   ,            0x1980       , 0x19DF    ,      0      },
    {       0,      QString::fromUtf8("Pahawh Hmong")   ,           0x16B00       , 0x16B8F    ,      0      },
    {       0,      QString::fromUtf8("Pau Cin Hau")   ,            0x11AC0       , 0x11AFF    ,      0      },
    {       0,      QString::fromUtf8("Tai Le")   ,                 0x1950       , 0x197F    ,      0      },
    {       0,      QString::fromUtf8("Tai Tham")   ,               0x1A20       , 0x1AAF    ,      0      },
    {       0,      QString::fromUtf8("Tai Viet")   ,               0xAA80       , 0xAADF    ,      0      },
    {       0,      QString::fromUtf8("Thai")   ,                   0x0E00       , 0x0E7F    ,      0      },
};
#define Unicode_Area_Southeast_Asian_Scripts_Num ( sizeof(unicode_area_Southeast_Asian_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Indonesia_Oceania_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Indonesia & Oceania Scripts
    {       0,      QString::fromUtf8("Balinese")   ,               0x1B00       , 0x1B7F    ,      0      },
    {       0,      QString::fromUtf8("Batak")   ,                  0x1BC0       , 0x1BFF    ,      0      },
    {       0,      QString::fromUtf8("Buginese")   ,               0x1A00       , 0x1A1F    ,      0      },
    {       0,      QString::fromUtf8("Buhid")   ,                  0x1740       , 0x175F    ,      0      },
    {       0,      QString::fromUtf8("Hanunoo")   ,                0x1720       , 0x173F    ,      0      },
    {       0,      QString::fromUtf8("Javanese")   ,               0xA980       , 0xA9DF    ,      0      },
    {       0,      QString::fromUtf8("Rejang")   ,                 0xA930       , 0xA95F    ,      0      },
    {       0,      QString::fromUtf8("Sundanese")   ,              0x1B80       , 0x1BBF    ,      0      },
    {       0,      QString::fromUtf8("Sundanese Supplement")   ,   0x1CC0       , 0x1CCF    ,      0      },
    {       0,      QString::fromUtf8("Tagalog")   ,                0x1700       , 0x171F    ,      0      },
    {       0,      QString::fromUtf8("Tagbanwa")   ,               0x1760       , 0x177F    ,      0      },
};
#define Unicode_Area_Indonesia_Oceania_Scripts_Num ( sizeof(unicode_area_Indonesia_Oceania_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_East_Asian_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //East Asian Scripts
    {       0,      QString::fromUtf8("Bopomofo")   ,               0x3100       , 0x312F    ,      0      },
    {       0,      QString::fromUtf8("Bopomofo Extended")   ,      0x31A0       , 0x31BF    ,      0      },
    {       0,      QString::fromUtf8("CJK Unified Ideographs")  ,  0x4E00       , 0x9FFF    ,      0      },
    {       0,      QString::fromUtf8("CJK Extension-A")   ,        0x3400       , 0x4DBF    ,      0      },
    {       0,      QString::fromUtf8("CJK Extension B")   ,        0x20000       , 0x2A6DF    ,      0      },
    {       0,      QString::fromUtf8("CJK Extension C")   ,        0x2A700       , 0x2B73F    ,      0      },
    {       0,      QString::fromUtf8("CJK Extension D")   ,        0x2B740       , 0x2B81F    ,      0      },
    {       0,      QString::fromUtf8("CJK Extension E")   ,        0x2B820       , 0x2CEAF    ,      0      },
    {       0,      QString::fromUtf8("CJK Compatibility Ideographs")   ,           0xF900       , 0xFAFF    ,      0      },
    {       0,      QString::fromUtf8("CJK Compatibility Ideographs Supplement"),   0x2F800       , 0x2FA1F    ,      0      },
    {       0,      QString::fromUtf8("CJK Radicals / KangXi Radicals")   ,         0x2F00       , 0x2FDF    ,      0      },
    {       0,      QString::fromUtf8("CJK Radicals Supplement"),   0x2E80       , 0x2EFF    ,      0      },
    {       0,      QString::fromUtf8("CJK Strokes")   ,            0x31C0       , 0x31EF    ,      0      },
    {       0,      QString::fromUtf8("Ideographic Description Characters")   ,     0x2FF0       , 0x2FFF    ,      0      },
    {       0,      QString::fromUtf8("Hangul Jamo")   ,            0x1100       , 0x11FF    ,      0      },
    {       0,      QString::fromUtf8("Hangul Jamo Extended-A")   , 0xA960       , 0xA97F    ,      0      },
    {       0,      QString::fromUtf8("Hangul Jamo Extended-B")   , 0xD7B0       , 0xD7FF    ,      0      },
    {       0,      QString::fromUtf8("Hangul Compatibility Jamo") ,0x3130       , 0x318F    ,      0      },
    {       0,      QString::fromUtf8("Halfwidth Jamo")   ,         0xFFA0       , 0xFFDC    ,      0      },
    {       0,      QString::fromUtf8("Hangul Syllables")   ,       0xAC00       , 0xD7AF    ,      0      },
    {       0,      QString::fromUtf8("Hiragana")   ,               0x3040       , 0x309F    ,      0      },
    {       0,      QString::fromUtf8("Katakana")   ,               0x30A0       , 0x30FF    ,      0      },
    {       0,      QString::fromUtf8("Katakana Phonetic Extensions")   ,           0x31F0       , 0x31FF    ,      0      },
    {       0,      QString::fromUtf8("Kana Supplement")   ,        0x1B000       , 0x1B0FF    ,      0      },
    {       0,      QString::fromUtf8("Halfwidth Katakana")   ,     0xFF65       , 0xFF9F    ,      0      },
    {       0,      QString::fromUtf8("Kanbun")   ,                 0x3190       , 0x319F    ,      0      },
    {       0,      QString::fromUtf8("Lisu")   ,                   0xA4D0       , 0xA4FF    ,      0      },
    {       0,      QString::fromUtf8("Miao")   ,                   0x16F00       , 0x16F9F    ,      0      },
    {       0,      QString::fromUtf8("Tangut")   ,                 0x17000       , 0x187FF    ,      0      },
    {       0,      QString::fromUtf8("Tangut Components")   ,      0x18800       , 0x18AFF    ,      0      },
    {       0,      QString::fromUtf8("Yi Syllables")   ,           0xA000       , 0xA48F    ,      0      },
    {       0,      QString::fromUtf8("Yi Radicals")   ,            0xA490       , 0xA4CF    ,      0      },
};
#define Unicode_Area_East_Asian_Scripts_Num ( sizeof(unicode_area_East_Asian_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_American_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //American Scripts
    {       0,      QString::fromUtf8("Cherokee")   ,               0x13A0       , 0x13FF    ,      0      },
    {       0,      QString::fromUtf8("Cherokee Supplement")   ,    0xAB70       , 0xABBF    ,      0      },
    {       0,      QString::fromUtf8("Deseret")   ,                0x10400       , 0x1044F    ,      0      },
    {       0,      QString::fromUtf8("Osage")   ,                  0x104B0       , 0x104FF    ,      0      },
    {       0,      QString::fromUtf8("Unified Canadian Aboriginal Syllabics")   ,       0x1400       , 0x167F    ,      0      },
    {       0,      QString::fromUtf8("UCAS Extended")   ,          0x18B0       , 0x18FF    ,      0      },
};
#define Unicode_Area_American_Scripts_Num ( sizeof(unicode_area_American_Scripts) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Other_Scripts[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Other
    {       0,      QString::fromUtf8("Alphabetic Presentation Forms")   ,          0xFB00       , 0xFB4F    ,      0      },
    {       0,      QString::fromUtf8("ASCII Characters")   ,       0x0000       , 0x007F    ,      0      },
    {       0,      QString::fromUtf8("Halfwidth and Fullwidth Forms")   ,          0xFF00       , 0xFFEF    ,      0      },
};
#define Unicode_Area_Other_Scripts_Num ( sizeof(unicode_area_Other_Scripts) / sizeof(struct Character_Set_Area) )

    /*Symbols and Punctuation*/
static struct Character_Set_Area unicode_area_Notational_Systems[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Notational Systems
    {       0,      QString::fromUtf8("Braille Patterns")   ,       0x2800       , 0x28FF    ,      0      },
    {       0,      QString::fromUtf8("Musical Symbols")   ,        0x1D100       , 0x1D1FF    ,      0      },
    {       0,      QString::fromUtf8("Ancient Greek Musical Notation")   ,         0x1D200       , 0x1D24F    ,      0      },
    {       0,      QString::fromUtf8("Byzantine Musical Symbols"), 0x1D000       , 0x1D0FF    ,      0      },
    {       0,      QString::fromUtf8("Duployan")   ,               0x1BC00       , 0x1BC9F    ,      0      },
    {       0,      QString::fromUtf8("Shorthand Format Controls"), 0x1BCA0       , 0x1BCAF    ,      0      },
    {       0,      QString::fromUtf8("Sutton SignWriting")   ,     0x1D800       , 0x1DAAF    ,      0      },
};
#define Unicode_Area_Notational_Systems_Num ( sizeof(unicode_area_Notational_Systems) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Punctuation[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Punctuation
    {       0,      QString::fromUtf8("General Punctuation")   ,    0x2000       , 0x206F    ,      0      },
    {       0,      QString::fromUtf8("ASCII Punctuation")   ,      0x0021       , 0x007F    ,      0      },
    {       0,      QString::fromUtf8("Latin-1 Punctuation")   ,    0x00A1       , 0x00BF    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Punctuation") , 0x2E00       , 0x2E7F    ,      0      },
    {       0,      QString::fromUtf8("CJK Symbols and Punctuation")  ,             0x3000       , 0x303F    ,      0      },
    {       0,      QString::fromUtf8("Ideographic Symbols and Punctuation")   ,    0x16FE0       , 0x16FFF    ,      0      },
    {       0,      QString::fromUtf8("CJK Compatibility Forms"),   0xFE30       ,  0xFE4F    ,      0      },
    {       0,      QString::fromUtf8("Halfwidth and Fullwidth Forms")   ,          0xFF00       , 0xFFEF    ,      0      },
    {       0,      QString::fromUtf8("Small Form Variants")   ,    0xFE50       , 0xFE6F    ,      0      },
    {       0,      QString::fromUtf8("Vertical Forms")   ,         0xFE10       , 0xFE1F    ,      0      },
};
#define Unicode_Area_Punctuation_Num ( sizeof(unicode_area_Punctuation) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Alphanumeric_Symbols[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Alphanumeric Symbols
    {       0,      QString::fromUtf8("Letterlike Symbols")   ,     0x2100       ,  0x214F    ,      0      },
    {       0,      QString::fromUtf8("Roman Symbols")   ,          0x10190       , 0x101CF    ,      0      },
    {       0,      QString::fromUtf8("Mathematical Alphanumeric Symbols")   ,      0x1D400       , 0x1D7FF   ,      0      },
    {       0,      QString::fromUtf8("Arabic Mathematical Alphabetic Symbols")   ,       0x1EE00       , 0x1EEFF    ,      0      },
    {       0,      QString::fromUtf8("Enclosed Alphanumerics")  ,  0x2460       ,  0x24FF    ,      0      },
    {       0,      QString::fromUtf8("Enclosed Alphanumeric Supplement")   ,       0x1F100       , 0x1F1FF    ,      0      },
    {       0,      QString::fromUtf8("Enclosed CJK Letters and Months")   ,        0x3200       , 0x32FF    ,      0      },
    {       0,      QString::fromUtf8("Enclosed Ideographic Supplement")   ,        0x1F200       , 0x1F2FF    ,      0      },
    {       0,      QString::fromUtf8("CJK Compatibility")   ,      0x3300       ,  0x33FF    ,      0      },
    {       0,      QString::fromUtf8("Additional Squared Symbols")   ,             0x2100       , 0x214F    ,      0      },
};
#define Unicode_Area_Alphanumeric_Symbols_Num ( sizeof(unicode_area_Alphanumeric_Symbols) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Technical_Symbols[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Technical Symbols
    {       0,      QString::fromUtf8("APL symbols")   ,            0x2336       ,  0x237A    ,      0      },
    {       0,      QString::fromUtf8("Control Pictures")   ,       0x2400       ,  0x243F    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Technical") ,  0x2300       ,  0x23FF    ,      0      },
    {       0,      QString::fromUtf8("Optical Character Recognition (OCR)")   ,    0x2440       , 0x245F    ,      0      },
};
#define Unicode_Area_Technical_Symbols_Num ( sizeof(unicode_area_Technical_Symbols) / sizeof(struct Character_Set_Area) )

    //Numbers & Digits
static struct Character_Set_Area unicode_area_ASCII_Digits[]={
    /*rule index    name                                            lower         upper       curcode  */
    //ASCII Digits
    {       0,      QString::fromUtf8("ASCII Digits")   ,           0x0030       , 0x0039    ,      0      },
    {       0,      QString::fromUtf8("Fullwidth ASCII Digits")   , 0xFF10       , 0xFF19    ,      0      },
    {       0,      QString::fromUtf8("Common Indic Number Forms"), 0xA830       , 0xA83F    ,      0      },
    {       0,      QString::fromUtf8("Coptic Epact Numbers")   ,   0x102E0       , 0x102FF    ,      0      },
    {       0,      QString::fromUtf8("Counting Rod Numerals")   ,  0x1D360       , 0x1D37F    ,      0      },
    {       0,      QString::fromUtf8("Cuneiform Numbers and Punctuation"),         0x12400       , 0x1247F    ,      0      },
    {       0,      QString::fromUtf8("Number Forms")   ,           0x2150       , 0x218F    ,      0      },
    {       0,      QString::fromUtf8("Rumi Numeral Symbols")   ,   0x10E60       , 0x10E7F    ,      0      },
    {       0,      QString::fromUtf8("Sinhala Archaic Numbers") ,  0x111E0       , 0x111FF    ,      0      },
    {       0,      QString::fromUtf8("Super and Subscripts")   ,   0x2070       , 0x209F    ,      0      },
};
#define Unicode_Area_ASCII_Digits_Num ( sizeof(unicode_area_ASCII_Digits) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Mathematical_Symbols[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Mathematical Symbols
    {       0,      QString::fromUtf8("Arrows")   ,                 0x2190       , 0x21FF    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Arrows-A")   ,  0x27F0       , 0x27FF    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Arrows-B")   ,  0x2900       , 0x297F    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Arrows-C")   ,  0x1F800       , 0x1F8FF    ,      0      },
    {       0,      QString::fromUtf8("Additional Arrows")   ,      0x2B00       , 0x2BFF    ,      0      },//???
    {       0,      QString::fromUtf8("Miscellaneous Symbols and Arrows")   ,       0x2B00       , 0x2BFF    ,      0      },
    {       0,      QString::fromUtf8("Mathematical Alphanumeric Symbols")   ,      0x1D400       , 0x1D7FF    ,      0      },
    {       0,      QString::fromUtf8("Arabic Mathematical Alphabetic Symbols")   , 0x1EE00       , 0x1EEFF    ,      0      },
    {       0,      QString::fromUtf8("Letterlike Symbols")   ,     0x2100       , 0x214F    ,      0      },
    {       0,      QString::fromUtf8("Mathematical Operators")   , 0x2200       , 0x22FF    ,      0      },
    {       0,      QString::fromUtf8("Basic operators: Plus, Factorial,")   ,      0x0000       , 0x007F    ,      0      },
    {       0,      QString::fromUtf8("Division, Multiplication") , 0x0080       , 0x00FF    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Mathematical Operators")   ,    0x2A00       , 0x2AFF    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Mathematical Symbols-A")   ,   0x27C0       , 0x27EF    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Mathematical Symbols-B")   ,   0x2980       , 0x29FF    ,      0      },
    {       0,      QString::fromUtf8("Floors and Ceilings")   ,    0x2308       , 0x230B    ,      0      },
    {       0,      QString::fromUtf8("Invisible Operators")   ,    0x2061       , 0x2064    ,      0      },
    {       0,      QString::fromUtf8("Geometric Shapes")   ,       0x25A0       , 0x25FF    ,      0      },
    {       0,      QString::fromUtf8("Additional Shapes")   ,      0x2B00       , 0x2BFF    ,      0      },
    {       0,      QString::fromUtf8("Box Drawing")   ,            0x2500       , 0x257F    ,      0      },
    {       0,      QString::fromUtf8("Block Elements")   ,         0x2580       , 0x259F    ,      0      },
    {       0,      QString::fromUtf8("Geometric Shapes Extended")   ,              0x1F780       , 0x1F7FF   ,      0      },
};
#define Unicode_Area_Mathematical_Symbols_Num ( sizeof(unicode_area_Mathematical_Symbols) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Emoji_Pictographs[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Emoji & Pictographs
    {       0,      QString::fromUtf8("Dingbats")   ,               0x2700       , 0x27BF    ,      0      },
    {       0,      QString::fromUtf8("Ornamental Dingbats")   ,    0x1F650      , 0x1F67F    ,      0      },
    {       0,      QString::fromUtf8("Emoticons")   ,              0x1F600       , 0x1F64F    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Symbols")   ,  0x2600       ,  0x26FF    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Symbols And Pictographs")   ,  0x1F300       , 0x1F5FF    ,      0      },
    {       0,      QString::fromUtf8("Supplemental Symbols and Pictographs")   ,   0x1F900       , 0x1F9FF    ,      0      },
    {       0,      QString::fromUtf8("Transport and Map Symbols"), 0x1F680       , 0x1F6FF   ,      0      },
};
#define Unicode_Area_Emoji_Pictographs_Num ( sizeof(unicode_area_Emoji_Pictographs) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Other_Symbols[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Other Symbols
    {       0,      QString::fromUtf8("Alchemical Symbols")   ,     0x1F700       , 0x1F77F    ,      0      },
    {       0,      QString::fromUtf8("Ancient Symbols")   ,        0x10190       , 0x101CF    ,      0      },
    {       0,      QString::fromUtf8("Currency Symbols")   ,       0x20A0       , 0x20CF    ,      0      },
    {       0,      QString::fromUtf8("Dollar Sign")   ,            0x0024       , 0x0024    ,      0      },
    {       0,      QString::fromUtf8(" Euro Sign")   ,             0x20AC       , 0x20AC    ,      0      },
    {       0,      QString::fromUtf8("Yen, Pound and Cent")   ,    0x00A2       , 0x00A5   ,      0      },
    {       0,      QString::fromUtf8("Fullwidth Currency Symbols"),0xFF04       , 0xFF04    ,      0      },
    {       0,      QString::fromUtf8("Rial Sign")   ,              0xFB50      , 0xFB50    ,      0      },
    {       0,      QString::fromUtf8("Chess,")   ,                 0x2654       , 0x265F    ,      0      },
    {       0,      QString::fromUtf8("Checkers/Draughts")   ,      0x26C0      , 0x26C3    ,      0      },
    {       0,      QString::fromUtf8("Domino Tiles")   ,           0x1F030       , 0x1F09F    ,      0      },
    {       0,      QString::fromUtf8("Japanese Chess")   ,         0x2616       , 0x2617    ,      0      },
    {       0,      QString::fromUtf8("Mahjong Tiles")   ,          0x1F000       , 0x1F02F    ,      0      },
    {       0,      QString::fromUtf8("Playing Cards")   ,          0x1F0A0       , 0x1F0FF    ,      0      },
    {       0,      QString::fromUtf8("Card suits")   ,             0x2660       , 0x2667    ,      0      },
    {       0,      QString::fromUtf8("Miscellaneous Symbols and Arrows")   ,       0x2B00       , 0x2BFF    ,      0      },
    {       0,      QString::fromUtf8("Yijing Mono-,")   ,          0x268A       , 0x268F    ,      0      },
    {       0,      QString::fromUtf8("Di- and Trigrams")   ,       0x2630       , 0x2637    ,      0      },
    {       0,      QString::fromUtf8("Yijing Hexagram Symbols") ,  0x4DC0       , 0x4DFF    ,      0      },
    {       0,      QString::fromUtf8("Tai Xuan Jing Symbols")   ,  0x1D300       , 0x1D35F    ,      0      },
};
#define Unicode_Area_Other_Symbols_Num ( sizeof(unicode_area_Other_Symbols) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Specials[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Specials
    {       0,      QString::fromUtf8("Controls C0")   ,            0x0000       , 0x001F    ,      0      },
    {       0,      QString::fromUtf8("Controls C1")   ,            0x0080       , 0x009F    ,      0      },
    {       0,      QString::fromUtf8("Layout Controls")   ,        0x2000       , 0x206F    ,      0      },
    {       0,      QString::fromUtf8("Invisible Operators")   ,    0x2061       , 0x2064    ,      0      },
    {       0,      QString::fromUtf8("Specials")   ,               0xFFF0       , 0xFFFF    ,      0      },
    {       0,      QString::fromUtf8("Tags")   ,                   0xE0000       , 0xE007F    ,      0      },
    {       0,      QString::fromUtf8("Variation Selectors")   ,    0xFE00       ,  0xFE0F    ,      0      },
    {       0,      QString::fromUtf8("Variation Selectors Supplement"),            0xE0100       , 0xE01EF    ,      0      },
};
#define Unicode_Area_Specials_Num ( sizeof(unicode_area_Specials) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Private_Use[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Private Use
    {       0,      QString::fromUtf8("Private Use Area")   ,       0xE000       , 0xF8FF    ,      0      },
    {       0,      QString::fromUtf8("Supplementary Private Use Area-A")   ,      0xF0000       , 0xFFFFD    ,      0      },
    {       0,      QString::fromUtf8("Supplementary Private Use Area-B")   ,      0x100000       , 0x10FFFD    ,      0      },
};
#define Unicode_Area_Private_Use_Num ( sizeof(unicode_area_Private_Use) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Surrogates[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Surrogates
    {       0,      QString::fromUtf8("High Surrogates")   ,        0xD800       , 0xDBFF    ,      0      },
    {       0,      QString::fromUtf8("Low Surrogates")   ,         0xDC00       , 0xDFFF    ,      0      },
};
#define Unicode_Area_Surrogates_Num ( sizeof(unicode_area_Surrogates) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area unicode_area_Noncharacters[]={
    /*rule index    name                                            lower         upper       curcode  */
    //Noncharacters in Charts
    {       0,      QString::fromUtf8("Range in Arabic Presentation Forms-A")   ,       0xFDD0       , 0xFDEF    ,      0      },
    {       0,      QString::fromUtf8("Range in Specials")   ,      0xFFFE       , 0xFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of BMP")   ,          0xFFF0       , 0xFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane1")   ,       0x1FF80       , 0x1FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane2")   ,       0x2FF80       , 0x2FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane3")   ,       0x3FF80       , 0x3FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane4")   ,       0x4FF80       , 0x4FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane5")   ,       0x5FF80       , 0x5FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane6")   ,       0x6FF80       , 0x6FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane7")   ,       0x7FF80       , 0x7FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane8")   ,       0x8FF80       , 0x8FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane9")   ,       0x9FF80       , 0x9FFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane10")   ,      0xAFF80       , 0xAFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane11")   ,      0xBFF80       , 0xBFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane12")   ,      0xCFF80       , 0xCFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane13")   ,      0xDFF80       , 0xDFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane14")   ,      0xEFF80       , 0xEFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane15")   ,      0xFFF80       , 0xFFFFF    ,      0      },
    {       0,      QString::fromUtf8("end of Plane16")   ,      0x10FF80       , 0x10FFFF    ,      0      },
};
#define Unicode_Area_Noncharacters_Num ( sizeof(unicode_area_Noncharacters) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area_Member unicode_member_default[]={
 {   European_Scripts,              QString::fromUtf8("European Scripts"),                  unicode_area_European_Scripts,                  Unicode_Area_European_Scripts_Num },
 {   Modifier_Letters,              QString::fromUtf8("Modifier Letters"),                  unicode_area_Modifier_Letters,                  Unicode_Area_Modifier_Letters_Num },
 {   Combining_Marks,               QString::fromUtf8("Combining Marks"),                   unicode_area_Combining_Marks,                   Unicode_Area_Combining_Marks_Num },
 {   African_Scripts,               QString::fromUtf8("African Scripts"),                   unicode_area_African_Scripts,                   Unicode_Area_African_Scripts_Num },
 {   Middle_Eastern_Scripts,        QString::fromUtf8("Middle Eastern_Scripts"),            unicode_area_Middle_Eastern_Scripts,            Unicode_Area_Middle_Eastern_Scripts_Num },
 {   Central_Asian_Scripts,         QString::fromUtf8("Central Asian_Scripts"),             unicode_area_Central_Asian_Scripts,             Unicode_Area_Central_Asian_Scripts_Num },
 {   South_Asian_Scripts,           QString::fromUtf8("South Asian_Scripts"),               unicode_area_South_Asian_Scripts,               Unicode_Area_South_Asian_Scripts_Num },
 {   Southeast_Asian_Scripts,       QString::fromUtf8("Southeast Asian_Scripts"),           unicode_area_Southeast_Asian_Scripts,           Unicode_Area_Southeast_Asian_Scripts_Num },
 {   Indonesia_Oceania_Scripts,     QString::fromUtf8("Indonesia Oceania_Scripts"),         unicode_area_Indonesia_Oceania_Scripts,         Unicode_Area_Indonesia_Oceania_Scripts_Num },
 {   East_Asian_Scripts,            QString::fromUtf8("East_Asian Scripts"),                unicode_area_East_Asian_Scripts,                Unicode_Area_East_Asian_Scripts_Num },
 {   American_Scripts,              QString::fromUtf8("American Scripts"),                  unicode_area_American_Scripts,                  Unicode_Area_American_Scripts_Num },
 {   Other_Scripts,                 QString::fromUtf8("Other Scripts"),                     unicode_area_Other_Scripts,                     Unicode_Area_Other_Scripts_Num },
 {   Notational_Systems,             QString::fromUtf8("Notational Systems"),                 unicode_area_Notational_Systems,                 Unicode_Area_Notational_Systems_Num },
 {   Punctuation,                   QString::fromUtf8("Punctuation"),                       unicode_area_Punctuation,                       Unicode_Area_Punctuation_Num },
 {   Alphanumeric_Symbols,          QString::fromUtf8("Alphanumeric Symbols"),              unicode_area_Alphanumeric_Symbols,              Unicode_Area_Alphanumeric_Symbols_Num },
 {   Technical_Symbols,             QString::fromUtf8("Technical Symbols"),                 unicode_area_Technical_Symbols,                 Unicode_Area_Technical_Symbols_Num },
 {   ASCII_Digits,                  QString::fromUtf8("ASCII Digits"),                      unicode_area_ASCII_Digits,                      Unicode_Area_ASCII_Digits_Num },
 {   Mathematical_Symbols,          QString::fromUtf8("Mathematical Symbols"),              unicode_area_Mathematical_Symbols,              Unicode_Area_Mathematical_Symbols_Num },
 {   Emoji_Pictographs,             QString::fromUtf8("Emoji Pictographs"),                 unicode_area_Emoji_Pictographs,                 Unicode_Area_Emoji_Pictographs_Num },
 {   Other_Symbols,                 QString::fromUtf8("Other Symbols"),                     unicode_area_Other_Symbols,                     Unicode_Area_Other_Symbols_Num },
 {   Specials,                      QString::fromUtf8("Specials"),                          unicode_area_Specials,                          Unicode_Area_Specials_Num },
 {   Private_Use,                   QString::fromUtf8("Private Use"),                       unicode_area_Private_Use,                       Unicode_Area_Private_Use_Num },
 {   Surrogates,                    QString::fromUtf8("Surrogates"),                        unicode_area_Surrogates,                        Unicode_Area_Surrogates_Num },
 {   Noncharacters,                 QString::fromUtf8("Noncharacters"),                     unicode_area_Noncharacters,                     Unicode_Area_Noncharacters_Num },
};
#define Unicode_Member_Num ( sizeof(unicode_member_default) / sizeof(struct Character_Set_Area_Member) )

#endif // CHARACTERSET

