#include "characterset.h"
#include "gb18030_unicode_table.h"

/***Character_Set***/
static struct Character_Set_Rules rule_default[]={
    /*rule index   bytes    name                            countmode     lower             upper           */
    {       0,      2,      QString::fromUtf8("default") ,       0,         0x0000        ,     0xFFFF          },
};
#define Default_Rule_Num ( sizeof(rule_default) / sizeof(struct Character_Set_Rules) )

static struct Character_Set_Area area_default[]={
    /*rule index    name                                  lower         upper       curcode  */
    {       0,      QString::fromUtf8("ALL")   ,       0x00       , 0xFFFF    ,      0      },
};
#define Default_Area_Num ( sizeof(area_default) / sizeof(struct Character_Set_Area) )

static struct Character_Set_Area_Member member_default[]={
    /*rule index   name                            member           num           */
    {       0,     QString::fromUtf8("default") ,  area_default,     Default_Area_Num         },
};
#define Default_Member_Num ( sizeof(rule_default) / sizeof(struct Character_Set_Area_Member) )

void Character_Set::default_rules_init(){
    for (int i = 0; i < Default_Rule_Num; i++ ){
        rules.append(rule_default[i]);
    }
}

void Character_Set::default_members_init(){
    for (int i = 0; i < Default_Member_Num; i++ ){
        members.append(member_default[i]);
    }
}

void Character_Set::default_areas_init(){
    for (int j = 0; j < members.count(); j++ ){
        member_index_areas_init(j);
    }
}

void Character_Set::member_index_areas_init(int index){
    for (int i = 0; i < members.at(index).num_member; i++ ){

        qDebug("rule_index %d,countmode %d,low %x,upper %x,top %x,bottom %x\n",members.at(index).member[i].rule_index,rules.at(members.at(index).member[i].rule_index).countmode,
                                                members.at(index).member[i].lower,members.at(index).member[i].upper,
                                                    rules.at(members.at(index).member[i].rule_index).lower,rules.at(members.at(index).member[i].rule_index).upper);
        members.at(index).member[i].char_nums = area_chars_count(rules.at(members.at(index).member[i].rule_index).countmode,
                                                     members.at(index).member[i].lower,
                                                     members.at(index).member[i].upper,
                                                     rules.at(members.at(index).member[i].rule_index).lower,
                                                     rules.at(members.at(index).member[i].rule_index).upper);
        qDebug()<<members.at(index).member[i].name_id<<QString::fromUtf8(" char_nums:")<<members.at(index).member[i].char_nums;
        areas.append(members.at(index).member[i]);
    }
}


int Character_Set::get_countmode(int rule_index)
{
    if ( rules.isEmpty() || (rule_index > rules.count()-1) )
        return -1;
    return rules.at(rule_index).countmode;
}

int Character_Set::get_topper(int rule_index)
{
    if ( rules.isEmpty() || (rule_index > rules.count()-1) )
        return -1;
    return rules.at(rule_index).lower;
}
int Character_Set::get_bottomer(int rule_index)
{
    if ( rules.isEmpty() || (rule_index > rules.count()-1) )
        return -1;
    return rules.at(rule_index).upper;
}

int Character_Set::count_of_S_shape(unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit){
    int i,bflag,count = 1;
    int countlimit = 1;
    union hex t,b,tl,bl;
    t.ihex = top;
    b.ihex = bottom;
    tl.ihex = top_limit;
    bl.ihex = bottom_limit;
    qDebug("count_of_S_shape:");
    qDebug(" top %x, top limit %x, bottom %x, bottom limit %x\n",top, top_limit, bottom, bottom_limit);
    for(i=0;i<4;i++){
        if (b.chex[i] < t.chex[i]){
            if (i==3){
                qDebug("top large than bottom");
                return -1;
            }
            bflag = 1;
            b.chex[i+1] -= 1;
        }
        else{
            bflag = 0;
        }
        count += ( (b.chex[i] - t.chex[i] + ( bflag ? (bl.chex[i] - tl.chex[i] + 1): 0) ) * countlimit );
        qDebug("b.chex[%d](%x) - t.chex[%d](%x), countlimit %d count %d\n",i,b.chex[i],i,t.chex[i],countlimit,count);
        countlimit *= (bl.chex[i] - tl.chex[i] + 1);
    }
    qDebug("count_of_S_shape result:%d\n",count);
    return count;
}

int Character_Set::count_of_area_filling(unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit){
    return count_of_S_shape(top,bottom,top,bottom);
#if 0
    int i,count = 1;
    union hex t,b,tl,bl;
    t.ihex = top;
    b.ihex = bottom;
    tl.ihex = top_limit;
    bl.ihex = bottom_limit;
    //qDebug("count_of_area_filling:");
    //qDebug(" top %x, top limit %x, bottom %x, bottom limit %x",top, top_limit, bottom, bottom_limit);
    for(i=0;i<4;i++){
        if ((t.chex[i] < tl.chex[i]) || (b.chex[i] > bl.chex[i])){// out of area
            qDebug("out of area");
            return -1;
        }
        count += (b.chex[i] - t.chex[i]) * count;
    }

    return count;
#endif
}

int Character_Set::offset_of_S_shape(unsigned long code,unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit){
    if(code > bottom || code < top){
        qDebug("out of range");
        return -1;
    }
    union hex c,tl,bl;
    c.ihex = code;
    tl.ihex = top_limit;
    bl.ihex = bottom_limit;
    for(int i = 0; i < 4; i++)
    {
        if (c.chex[i] > bl.chex[i] || c.chex[i] < tl.chex[i]){
            qDebug("out of range");
            return -1;
        }
    }
    return count_of_S_shape(top,code,top_limit,bottom_limit) - 1;
}

int Character_Set::offset_of_area_filling(unsigned long code,unsigned long top,unsigned long bottom,unsigned long top_limit,unsigned long bottom_limit){
    if(code > bottom || code < top){
        qDebug("out of range");
        return -1;
    }
    union hex c,tl,bl;
    c.ihex = code;
    tl.ihex = top;
    bl.ihex = bottom;
    for(int i = 0; i < 4; i++)
    {
        if (c.chex[i] > bl.chex[i] || c.chex[i] < tl.chex[i]){
            qDebug("out of range");
            return -1;
        }
    }
    return count_of_S_shape(top,code,top,bottom) - 1;

}



int Character_Set::trans_table(struct Code_Table p[], unsigned long top, unsigned long bottom, unsigned long key){
    int mid = (top + bottom)/2;
    if(top > bottom)
        return 0;
    if(key == p[mid].src_code)
        return p[mid].dst_code;
    else if(key > p[mid].src_code)
        return trans_table(p,mid+1,bottom,key);
    else
        return trans_table(p,top,mid-1,key);
}

int Character_Set::trans_table_ex(struct Code_Table p[], unsigned long top, unsigned long bottom, unsigned long key){
    int mid = (top + bottom)/2;
    if(top > bottom)
        return 0;
    if(key == p[mid].dst_code)
        return p[mid].src_code;
    else if(key > p[mid].dst_code)
        return trans_table_ex(p,mid+1,bottom,key);
    else
        return trans_table_ex(p,top,mid-1,key);
}

int Character_Set::area_chars_count( int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit){
    //qDebug("top_limit %x,bottom_limit %x",top_limit,bottom_limit);
    qDebug("countmode %d",countmode);
    switch(countmode){
    case 0:
        return count_of_S_shape(top,bottom,top_limit,bottom_limit);
        break;
    case 1:
        return count_of_area_filling(top,bottom,top_limit,bottom_limit);
        break;

    default:
        return -1;
        break;
    }
}

int Character_Set::area_chars_offset( unsigned long code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit){
    //qDebug("top_limit %x,bottom_limit %x",top_limit,bottom_limit);
    qDebug("countmode %d",countmode);
    switch(countmode){
    case 0:
        return offset_of_S_shape(code,top,bottom,top_limit,bottom_limit);
        break;
    case 1:
        return offset_of_area_filling(code,top,bottom,top_limit,bottom_limit);
        break;

    default:
        return -1;
        break;
    }
}

int Character_Set::get_next_code(unsigned long* code, int rule_index, unsigned long top, unsigned long bottom){

    if(*code == bottom)
        return 1;
   //qDebug("count mode %d ", rules.at(rule_index).countmode);
    switch(rules.at(rule_index).countmode){
    case 0:
        return next_code(code, rules.at(rule_index).lower, rules.at(rule_index).upper);
    case 1:
        return next_code(code, top, bottom);
    default:
        qDebug("unknow count mode");
        return -1;
    }

}

int Character_Set::get_next_code_ex(unsigned long* code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit){

    if(*code == bottom)
        return 1;
    //qDebug("count mode %d ", countmode);
    switch(countmode){
    case 0:
        return next_code(code, top_limit, bottom_limit);
    case 1:
        return next_code(code, top, bottom);
    default:
        qDebug("unknow count mode");
        return -1;
    }
}

int Character_Set::get_pre_code_ex(unsigned long* code, int countmode, unsigned long top, unsigned long bottom, unsigned long top_limit, unsigned long bottom_limit){

    if(*code == bottom)
        return 1;
    //qDebug("count mode %d ", countmode);
    switch(countmode){
    case 0:
        return pre_code(code, top_limit, bottom_limit);
    case 1:
        return pre_code(code, top, bottom);
    default:
        qDebug("unknow count mode");
        return -1;
    }
}

int Character_Set::next_code(unsigned long* code, unsigned long top, unsigned long bottom){
    union hex t,b,temp;
    t.ihex = top;
    b.ihex = bottom;
    //qDebug("char area top %x,bottom %x cur code %x",t.ihex,b.ihex,*code);
    temp.ihex = *code;
    if(temp.chex[0] + 1 > b.chex[0]){
        temp.chex[0] = t.chex[0];
        if(temp.chex[1] + 1 > b.chex[1]){
            temp.chex[1] = t.chex[1];
            if(temp.chex[2] + 1 > b.chex[2]){
                temp.chex[2] = t.chex[2];
                if(temp.chex[3] + 1 > b.chex[3]){
                    return -1;
                }
                else
                   temp.chex[3] += 1;
            }
            else
                temp.chex[2] += 1;
        }
        else
            temp.chex[1] += 1;
    }
    else
        temp.chex[0] += 1;
    qDebug("next code %x",temp.ihex);
    *code = temp.ihex;
    return 0;
}

int Character_Set::pre_code(unsigned long* code, unsigned long top, unsigned long bottom){
    union hex t,b,temp;
    t.ihex = top;
    b.ihex = bottom;
    qDebug("char area top %x,bottom %x cur code %x",t.ihex,b.ihex,*code);
    temp.ihex = *code;
    if(temp.chex[0] - 1 < t.chex[0]){
        temp.chex[0] = b.chex[0];
        if(temp.chex[1] - 1 < t.chex[1]){
            temp.chex[1] = b.chex[1];
            if(temp.chex[2] - 1 < t.chex[2]){
                temp.chex[2] = b.chex[2];
                if(temp.chex[3] - 1 < t.chex[3]){
                    return -1;
                }
                else
                   temp.chex[3] -= 1;
            }
            else
                temp.chex[2] -= 1;
        }
        else
            temp.chex[1] -= 1;
    }
    else
        temp.chex[0] -= 1;
    qDebug("pre code %x",temp.ihex);
    *code = temp.ihex;
    return 0;
}

/***GB18030_Character_Set***/

void GB18030_Character_Set::default_rules_init(){
    for (int i = 0; i < Gb18030_Rule_Num; i++ ){
        rules.append(gb18030_rule_default[i]);
    }
}

void GB18030_Character_Set::default_members_init(){
    for (int i = 0; i < Gb18030_Member_Num; i++ ){
        members.append(gb18030_member_default[i]);
    }
}

int GB18030_Character_Set::to_unicode(unsigned long *in, unsigned long* out){
    if(*in <= 0x7F)
        *out = *in;
    //table 1
    else if(((*in >= 0x8140) && (*in <= 0xA0FE)) || ((*in >= 0xAA40) && (*in <= 0xFEA0))){
        *out = trans_table( gb_unicode_db_table_1, 0 , (GB_Unicode_DB_Table_1_Num - 1), *in);
    }
    //table 2
    else if(((*in >= 0xA1A1) && (*in <= 0xA9FE)) || ((*in >= 0xA840) && (*in <= 0xA9A0))){
        *out = trans_table( gb_unicode_db_table_2, 0 , (GB_Unicode_DB_Table_2_Num - 1), *in);
    }
    //table cjk-a
    else if((*in >= 0x8139EE39) && (*in <= 0x82358738)){
        *out = trans_table( gb_unicode_cjk_table_a, 0 , (Unicode_GB_CJK_Table_A_Num - 1), *in);
    }
    //table cjk-b
    //else if((*in >= 0x95328236) && (*in <= 0x9835F336)){
    else if((*in >= 0x90308130) && (*in <= 0xE339FE39)){
        *out = 0x10000 + ((*in >> 24)&0xff - 0x90)*12600 + ((*in >> 16)&0xff - 0x30)*1260 + ((*in >> 8)&0xff - 0x81)*10 + (*in)&0xff -0x30;
    }
    else
        return -1;
    return 0;
}

int GB18030_Character_Set::unicode_to(unsigned long *in, unsigned long* out){
    union{
    unsigned long a;
    unsigned char b[4];
    }data;
    unsigned long temp;
    qDebug("Input Unicode %x",*in);
    if(*in <= 0x7F)
        *out = *in;
    //table 1
    else if((*in >= 0x4E00) && (*in <= 0xFA29)){
        *out = trans_table_ex( unicode_gb_db_table_1, 0 , (GB_Unicode_DB_Table_1_Num - 1), *in);
    }
    //table cjk-a
    else if((*in >= 0x3400) && (*in <= 0x4DB5)){
        *out = trans_table_ex( gb_unicode_cjk_table_a, 0 , (Unicode_GB_CJK_Table_A_Num - 1), *in);
    }
    else if(*in <= 0xFFFF){
        *out = trans_table_ex( unicode_gb_db_table_2, 0 , (GB_Unicode_DB_Table_2_Num - 1), *in);
    }
    //BMP
    else if(*in > 0xFFFF){
        temp = *in - 0x10000;
        data.b[3] = temp / 12600 + 0x90;
        temp %= 12600;
        data.b[2] = temp /1260 + 0x30;
        temp %= 1260;
        data.b[1] = temp / 10 + 0x81;
        data.b[0] = temp % 10 + 0x30;
        *out = data.a;
    }
    else
        return -1;
    return 0;
}


/***Unicode_Character_Set***/

void Unicode_Character_Set::default_rules_init(){
    for (int i = 0; i < Unicode_Rule_Num; i++ ){
        rules.append(unicode_rule_default[i]);
    }
}

void Unicode_Character_Set::default_members_init(){
    for (int i = 0; i < Unicode_Member_Num; i++ ){
        members.append(unicode_member_default[i]);
    }
}
