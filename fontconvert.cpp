#include <QtGui>
#include <QDebug>
#include <QMessageBox>

#include "fontconvert.h"

FontConvert::FontConvert(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
    //UI init
    preview_Init();
    make_Init();


    flash_blCmdSlt(0);
    down_comInit();

    flash_comInit();
    //no use hide
    lineEdit_Flash_binDownAddr->hide();
    label_40->hide();

    /*Preview*/
    //select ttf source path
    connect(pushButton_Pre_ttfBrs,SIGNAL(clicked()),this,SLOT(pre_ttfSrcBrs()));
    //select charset
    connect(comboBox_Pre_charset,SIGNAL(currentIndexChanged(int)),this,SLOT(pre_setEnable(int)));
    //select input mode
    connect(comboBox_Pre_inputMode,SIGNAL(currentIndexChanged(int)),this,SLOT(pre_inPutSlt(int)));
    //input weight of width set range
    connect(lineEdit_Pre_width,SIGNAL(textChanged(QString)),this,SLOT(pre_setWeightWidthRange(QString)));
    //input weight of height set range
    connect(lineEdit_Pre_height,SIGNAL(textChanged(QString)),this,SLOT(pre_setWeightHeightRange(QString)));
    //click preview
    connect(pushButton_do_preview,SIGNAL(clicked()),this,SLOT(pre_doPreview()));

    /*Make*/
    //select ttf source path
    connect(pushButton_Make_ttfBrs,SIGNAL(clicked()),this,SLOT(make_ttfSrcBrs()));
    //select comobox charset
    connect(comboBox_Make_charset,SIGNAL(currentIndexChanged(int)),this,SLOT(make_charsetSlt(int)));
    //input weight of width set range
    connect(lineEdit_Make_width,SIGNAL(textChanged(QString)),this,SLOT(make_setWeightWidthRange(QString)));
    //input weight of height set range
    connect(lineEdit_Make_height,SIGNAL(textChanged(QString)),this,SLOT(make_setWeightHeightRange(QString)));
    //click charcode table
    connect(tableWidget_Make_charsetArea,SIGNAL(cellClicked(int,int)),this,SLOT(make_ruleSltShow(int,int)));
    //click reset rules button
    connect(pushButton_Make_resetRule,SIGNAL(clicked()),this,SLOT(make_ruleValueReset()));
    //click userdefine rules checktable
    connect(pushButton_Make_userDef,SIGNAL(clicked()),this,SLOT(make_usrDefShow()));
    //click add charcode rules button
    connect(pushButton_Make_addRule,SIGNAL(clicked()),this,SLOT(make_addRule()));
    //click del charcode rules button
    connect(pushButton_Make_delRule,SIGNAL(clicked()),this,SLOT(make_ruleSltDel()));
    //click convert table
    connect(tableWidget_Make_converts,SIGNAL(cellClicked(int,int)),this,SLOT(make_convDescSltShow(int,int)));
    //click add convert button
    connect(pushButton_Make_addConv,SIGNAL(clicked()),this,SLOT(make_addConv()));
    //click del convert button
    connect(pushButton_Make_delConv,SIGNAL(clicked()),this,SLOT(make_delConv()));
    //click move up convert button
    connect(pushButton_Make_convMoveUp,SIGNAL(clicked()),this,SLOT(make_convMoveUp()));
    //click move down convert button
    connect(pushButton_Make_convMoveDown,SIGNAL(clicked()),this,SLOT(make_convMoveDown()));
    //click describe botton
    connect(pushButton_Make_convDesc,SIGNAL(clicked()),this,SLOT(make_convDescShow()));
    //click convert run button
    connect(pushButton_Make_start,SIGNAL(clicked()),this,SLOT(make_convStart()));
    //click convert stop button
    connect(pushButton_Make_stop,SIGNAL(clicked()),this,SLOT(make_convStop()));

    /*Trim*/
    //select font bin source path
    connect(pushButton_Trim_fontBinBrs,SIGNAL(clicked()),this,SLOT(trim_fontBinSrcBrs()));
    //click load button
    connect(pushButton_Trim_fontBinLoad,SIGNAL(clicked()),this,SLOT(trim_fontBinLoad()));
    //select comobox fontset
    connect(comboBox_Trim_charset,SIGNAL(currentIndexChanged(int)),this,SLOT(trim_charsetSlt(int)));
    //textChange char code
    connect(lineEdit_Trim_charCode,SIGNAL(textChanged(QString)),this,SLOT(trim_charClear(QString)));
    //click test pre
    connect(pushButton_Trim_getPreChar,SIGNAL(clicked()),this,SLOT(trim_getPreChar()));
    //click test next
    connect(pushButton_Trim_getNextChar,SIGNAL(clicked()),this,SLOT(trim_getNextChar()));
    //click test char
    connect(pushButton_Trim_getChar,SIGNAL(clicked()),this,SLOT(trim_getChar()));
    //modify test char
    connect(tableWidget_Trim_charView,SIGNAL(cellClicked(int,int)),this,SLOT(trim_charViewPixelClick(int,int)));
    //zoomin test char
    connect(pushButton_Trim_charViewZoomIn,SIGNAL(clicked()),this,SLOT(trim_CharViewZoomIn()));
    //zoomout test char
    connect(pushButton_Trim_charViewZoomOut,SIGNAL(clicked()),this,SLOT(trim_CharViewZoomOut()));
    //recover test char
    connect(pushButton_Trim_charRecover,SIGNAL(clicked()),this,SLOT(trim_charViewRecover()));
    //save modify test char
    connect(pushButton_Trim_charSave,SIGNAL(clicked()),this,SLOT(trim_charSave()));
    //sync modify test char
    connect(pushButton_Trim_newFontBinSave,SIGNAL(clicked()),this,SLOT(trim_newFontBinSave()));

    /*Down*/
    connect(pushButton_Down_fontBinBrs,SIGNAL(clicked()),this,SLOT(down_fontBinSrcBrs()));
    connect(pushButton_Down_fontBinDl,SIGNAL(clicked()),this,SLOT(down_fontBinDown()));
    connect(pushButton_Down_fontBinCheck,SIGNAL(clicked()),this,SLOT(down_fontBinCheck()));
    connect(textBrowser_Down_log,SIGNAL(textChanged()),this,SLOT(down_logMoveEnd()));
    connect(pushButton_Down_logClear,SIGNAL(clicked()),this,SLOT(down_logClear()));
    connect(pushButton_Down_fontBinPatchSrcBrs,SIGNAL(clicked()),this,SLOT(down_patchFontBinSrcBrs()));
    connect(pushButton_Down_fontBinPatchDstBrs,SIGNAL(clicked()),this,SLOT(down_patchFontBinDstBrs()));
    connect(pushButton_Down_fontBinPatchBuild,SIGNAL(clicked()),this,SLOT(down_patchBuild()));
    connect(pushButton_Down_fontPatchSrcBrs,SIGNAL(clicked()),this,SLOT(down_patchSrcBrs()));
    connect(pushButton_Down_fontPatchSync,SIGNAL(clicked()),this,SLOT(down_doPatch()));

    /*Flash*/
    connect(pushButton_Flash_dataLog,SIGNAL(clicked()),this,SLOT(flash_logDataShow()));
    connect(pushButton_Flash_logClear,SIGNAL(clicked()),this,SLOT(flash_logClear()));
    connect(pushButton_Flash_blMode,SIGNAL(clicked()),this,SLOT(flash_blMode()));
    connect(comboBox_Flash_blCmd,SIGNAL(currentIndexChanged(int)),this,SLOT(flash_blCmdSlt(int)));
    connect(pushButton_Flash_blCmdSend,SIGNAL(clicked()),this,SLOT(flash_blCmdSend()));
    connect(comboBox_Flash_comOpen,SIGNAL(clicked()),this,SLOT(flash_comOpen()));
    connect(pushButton_Flash_firmwareBrs,SIGNAL(clicked()),this,SLOT(flash_firmwareSrcBrs()));
    connect(pushButton_Flash_firmwareDown,SIGNAL(clicked()),this,SLOT(flash_firmwareDown()));
    connect(pushButton_Flash_firmwareExport,SIGNAL(clicked()),this,SLOT(flash_firmwareExport()));
    connect(textBrowser_Flash_log,SIGNAL(textChanged()),this,SLOT(flash_logMoveEnd()));

    /*Merge*/
    connect(pushButton_Merge_masterFontBinSrcBrs,SIGNAL(clicked()),this,SLOT(merge_masterFontBinSrcBrs()));
    connect(pushButton_Merge_masterFontBinSrcLoad,SIGNAL(clicked()),this,SLOT(merge_masterFontBinLoad()));
    connect(pushButton_Merge_slaveFontBinSrcBrs,SIGNAL(clicked()),this,SLOT(merge_slaveFontBinSrcBrs()));
    connect(pushButton_Merge_slaveFontBinSrcLoad,SIGNAL(clicked()),this,SLOT(merge_slaveFontBinLoad()));
    //ops
    connect(pushButton_Merge_masterFontBinStructMoveUp,SIGNAL(clicked()),this,SLOT(merge_masterFontBinStructMoveUp()));
    connect(pushButton_Merge_masterFontBinStructMoveDown,SIGNAL(clicked()),this,SLOT(merge_masterFontBinStructMoveDown()));
    connect(pushButton_Merge_masterFontBinStructAdd,SIGNAL(clicked()),this,SLOT(merge_masterFontBinStructAddFromSlave()));
    connect(pushButton_Merge_masterFontBinStructDel,SIGNAL(clicked()),this,SLOT(merge_masterFontBinStructDel()));
    connect(pushButton_Merge_masterFontBinStructSave,SIGNAL(clicked()),this,SLOT(merge_masterFontBinSave()));
}

void FontConvert::preview_Init()
{
    pre_inPutSlt(0);
    pre_setEnable(0);
}

void FontConvert::messageBox(QString msg){
    QMessageBox *new_msg = new QMessageBox;
    new_msg->setText(msg);
    new_msg->exec();
}

void FontConvert::pre_ttfSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字体"),
        "./ttf",
        tr("TTF (*.ttf)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Pre_ttfSrc->setText(filename);
    }
}

void FontConvert::pre_setEnable(int index)
{
    pushButton_do_preview->setEnabled(index?1:0);
}

void FontConvert::pre_inPutSlt(int index){
    if(index){
        label_Pre_charCode->hide();
        label_Pre_charString->show();
        lineEdit_Pre_charCode->hide();
        textEdit_Pre_charString->show();
        textEdit_Pre_charString->clear();
    }
    else{
        label_Pre_charCode->show();
        label_Pre_charString->hide();
        lineEdit_Pre_charCode->show();
        lineEdit_Pre_charCode->clear();
        textEdit_Pre_charString->hide();

    }
}

void FontConvert::pre_setWeightWidthRange(QString textValue)
{
    int value = textValue.toInt()/2;
    spinBox_Pre_weightofWidth->setValue(0);
    spinBox_Pre_weightofWidth->setRange(-value,value);
}

void FontConvert::pre_setWeightHeightRange(QString textValue)
{
    int value = textValue.toInt()/2;
    spinBox_Pre_weightofHeight->setValue(0);
    spinBox_Pre_weightofHeight->setRange(-value,value);
}

void FontConvert::pre_outPut(QString str)
{
    textEdit_output->insertPlainText(str);
}


void FontConvert::pre_doPreview(){

    if(lineEdit_Pre_ttfSrc->text().isEmpty()){
        messageBox(QString::fromUtf8("没有TTF字体文件!"));
        return;
    }

    Font_Config cur_fontCfg;
    cur_fontCfg.strMode = comboBox_Pre_mode->currentText();
    cur_fontCfg.strOrder = comboBox_Pre_order->currentText();
    cur_fontCfg.mode = comboBox_Pre_mode->currentIndex();
    cur_fontCfg.order = comboBox_Pre_order->currentIndex();
    cur_fontCfg.width = lineEdit_Pre_width->text().toInt();
    cur_fontCfg.height = lineEdit_Pre_height->text().toInt();
    cur_fontCfg.bold = comboBox_Pre_blod->currentIndex();
    cur_fontCfg.woWidth = spinBox_Pre_weightofWidth->value();
    cur_fontCfg.woHeight = spinBox_Pre_weightofHeight->value();
    cur_fontCfg.horiBold = spinBox_Pre_horiBold->value();
    cur_fontCfg.vertBold = spinBox_Pre_vertBold->value();

    int encode;
    QString ttf = lineEdit_Pre_ttfSrc->text();
    switch (comboBox_Pre_charset->currentIndex())
    {
       case 1:
            encode = Character_Set::ENCODE_GB18030;
            break;
       case 2:
            encode = Character_Set::ENCODE_UCS_4;
            break;
       default:
            return;
    }
    int type = comboBox_Pre_inputMode->currentIndex();
    QString str;
    switch (type)
    {
       case 0:
            str = lineEdit_Pre_charCode->text();
            break;
       case 1:
            str = textEdit_Pre_charString->toPlainText();
            break;
       default:
            return;
    }

    QList<Draw_BitMap> bitmaps;
    Charset_Convert *tmp = new Charset_Convert;
    connect(tmp,SIGNAL(print_data_info(QString)),this,SLOT(pre_outPut(QString)));
    tmp->do_convert_bitmap( type, str, encode, cur_fontCfg, ttf, bitmaps);
    my_drawbitmap_spare(tableWidget_Pre_bitmap,cur_fontCfg.mode,cur_fontCfg.order,bitmaps);
    disconnect(tmp,SIGNAL(print_data_info(QString)),this,SLOT(pre_outPut(QString)));
    delete tmp;
    bitmaps.clear();
}


/*********************************************************/
void FontConvert::make_Init()
{
    pushButton_Make_userDef->setEnabled(0);
    groupBox_Make_userDef->hide();
    tableWidget_Make_convInfo->hide();
    make_convOptSetEnable(0);
}

void FontConvert::make_ttfSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字体"),
        ".",
        tr("TTF (*.ttf)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Make_ttfSrc->setText(filename);
    }
}

void FontConvert::make_charsetSlt(int index)
{
    switch(index){
    case 1:
        make_charset = new GB18030_Character_Set;
        break;
    case 2:
        make_charset = new Unicode_Character_Set;
        break;
    default:
        pushButton_Make_userDef->setEnabled(0);
        comboBox_Make_charsetMbr->setEnabled(0);
        return;
    }
    comboBox_Make_charsetMbr->setEnabled(1);
    pushButton_Make_userDef->setEnabled(1);
    make_charsetMbrInit();
    make_usrDefInit();
    make_charsetMbrSlt(0);
}

void FontConvert::make_setWeightWidthRange(QString textValue)
{
    int value = textValue.toInt()/2;
    spinBox_Make_weightofWidth->setValue(0);
    spinBox_Make_weightofWidth->setRange(-value,value);
}

void FontConvert::make_setWeightHeightRange(QString textValue)
{
    int value = textValue.toInt()/2;
    spinBox_Make_weightofHeight->setValue(0);
    spinBox_Make_weightofHeight->setRange(-value,value);
}



void FontConvert::make_charsetMbrInit(){
    //select comobox charset member
    disconnect(comboBox_Make_charsetMbr,SIGNAL(currentIndexChanged(int)),this,SLOT(make_charsetMbrSlt(int)));
    comboBox_Make_charsetMbr->clear();
    comboBox_Make_charsetMbr->addItem(QString("ALL"));
    QList<Character_Set_Area_Member>::iterator iter = make_charset->members.begin();
    for(; iter != make_charset->members.end(); iter++){
        comboBox_Make_charsetMbr->addItem(iter->name);
    }
    //select comobox charset member
    connect(comboBox_Make_charsetMbr,SIGNAL(currentIndexChanged(int)),this,SLOT(make_charsetMbrSlt(int)));
    comboBox_Make_charsetMbr->setCurrentIndex(0);
}
void FontConvert::make_usrDefInit()
{
    //click charare mode button
    disconnect(comboBox_Make_Rule,SIGNAL(currentIndexChanged(int)),this,SLOT(make_usrDefShowBytes(int)));
    comboBox_Make_Rule->clear();
    QList<Character_Set_Rules>::iterator iter = make_charset->rules.begin();
    for(; iter != make_charset->rules.end(); iter++){
        comboBox_Make_Rule->addItem(iter->name);
    }
    //click charare mode button
    connect(comboBox_Make_Rule,SIGNAL(currentIndexChanged(int)),this,SLOT(make_usrDefShowBytes(int)));
    comboBox_Make_Rule->setCurrentIndex(0);
}

void FontConvert::make_charsetMbrSlt(int index){
    make_charset->areas.clear();
    if(index)
        make_charset->member_index_areas_init(index-1);
    else
        make_charset->default_areas_init();
    make_charsetMbrShow();
}

void FontConvert::make_charsetMbrShow(){
    if(make_charset->areas.isEmpty())
        return;
    tableWidget_Make_charsetArea->setColumnCount(4);//
    tableWidget_Make_charsetArea->setEditTriggers(QAbstractItemView::NoEditTriggers);//disable edit
    //tableWidget_Make_charsetArea->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度

    tableWidget_Make_charsetArea->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    //tableWidget_Make_charsetArea->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList headers;
    headers<<QString::fromUtf8("区块")<<QString::fromUtf8("开始")<<QString::fromUtf8("结束")<<QString::fromUtf8("字符统计");
    tableWidget_Make_charsetArea->setHorizontalHeaderLabels(headers);
    tableWidget_Make_charsetArea->setRowCount(make_charset->areas.count());
    QTableWidgetItem *item0,*item1,*item2,*item3;


    for (int i = 0; i < make_charset->areas.count() ; i++ ){
        item0 = new QTableWidgetItem( make_charset->areas.at(i).name_id );
        tableWidget_Make_charsetArea->setItem(i,0,item0);

        qDebug()<<QString::number( make_charset->areas.at(i).lower, 16 ).toUpper()<<"~"<<QString::number( make_charset->areas.at(i).upper, 16 ).toUpper();
        item1 = new QTableWidgetItem( QString::number( make_charset->areas.at(i).lower, 16 ).toUpper() );
        tableWidget_Make_charsetArea->setItem(i,1,item1);
        item2 = new QTableWidgetItem( QString::number( make_charset->areas.at(i).upper, 16 ).toUpper());
        tableWidget_Make_charsetArea->setItem(i,2,item2);

        item3 = new QTableWidgetItem( QString::number( make_charset->areas.at(i).char_nums, 10 ) );
        tableWidget_Make_charsetArea->setItem(i,3,item3);
    }
    tableWidget_Make_charsetArea->selectRow(0);
    tableWidget_Make_charsetArea->resizeColumnsToContents();
}

void FontConvert::make_ruleSltDel(){
    if(!make_charset->areas.isEmpty()){
        make_charset->areas.removeAt(tableWidget_Make_charsetArea->currentRow());
        make_convShow();
        //qDebug()<<tableWidget_Make_charsetArea->currentRow();
        if(!make_charset->areas.isEmpty()){
            make_convDescSltShow(tableWidget_Make_charsetArea->currentRow(),0);
        }
    }

}

void FontConvert::make_addRule(){
    Character_Set_Area *new_area = new Character_Set_Area;
    bool ok;
    qDebug()<<"add code";
    new_area->name_id = lineEdit_areaid->text();
    QList<Character_Set_Area>::iterator iter = make_charset->areas.begin();
    for(;iter!=make_charset->areas.end();iter++){
        if (!QString::compare(new_area->name_id,iter->name_id))
        {
            qDebug()<<"exist id,try another";
            messageBox(QString::fromUtf8("规则名已存在,请重新输入!"));
            return;
        }
    }
    new_area->rule_index = comboBox_Make_Rule->currentIndex();
    qDebug()<<new_area->name_id <<"rule index:"<<new_area->rule_index;
    int bytes = make_charset->rules.at(new_area->rule_index).bytes;
    union hex lower,upper;
    if (1 == bytes){
        lower.chex[3] = 0;
        lower.chex[2] = 0;
        lower.chex[1] = 0;
        lower.chex[0] = lineEdit_1stB_lower->text().toInt(&ok,16);
        upper.chex[3] = 0;
        upper.chex[2] = 0;
        upper.chex[1] = 0;
        upper.chex[0] = lineEdit_1stB_upper->text().toInt(&ok,16);
    }
    if (2 == bytes){
        lower.chex[3] = 0;
        lower.chex[2] = 0;
        lower.chex[1] = lineEdit_1stB_lower->text().toInt(&ok,16);
        lower.chex[0] = lineEdit_2ndB_lower->text().toInt(&ok,16);
        upper.chex[3] = 0;
        upper.chex[2] = 0;
        upper.chex[1] = lineEdit_1stB_upper->text().toInt(&ok,16);
        upper.chex[0] = lineEdit_2ndB_upper->text().toInt(&ok,16);
    }
    if (4 == bytes){
        lower.chex[3] = lineEdit_1stB_lower->text().toInt(&ok,16);
        lower.chex[2] = lineEdit_2ndB_lower->text().toInt(&ok,16);
        lower.chex[1] = lineEdit_3rdB_lower->text().toInt(&ok,16);
        lower.chex[0] = lineEdit_4thB_lower->text().toInt(&ok,16);
        upper.chex[3] = lineEdit_1stB_upper->text().toInt(&ok,16);
        upper.chex[2] = lineEdit_2ndB_upper->text().toInt(&ok,16);
        upper.chex[1] = lineEdit_3rdB_upper->text().toInt(&ok,16);
        upper.chex[0] = lineEdit_4thB_upper->text().toInt(&ok,16);
    }
    new_area->lower = lower.ihex;
    new_area->upper = upper.ihex;
    int count = -1;

    count = make_charset->area_chars_count(make_charset->rules.at(new_area->rule_index).countmode,
                                               new_area->lower,
                                               new_area->upper,
                                               make_charset->rules.at(new_area->rule_index).lower,
                                               make_charset->rules.at(new_area->rule_index).upper);
    if (count < 0){
        qDebug()<<QString::fromUtf8("count:")<<count;
        return;
    }
    else
        new_area->char_nums = count;

    //
    make_charset->areas.append(*new_area);
    make_charsetMbrShow();
}



void FontConvert::make_usrDefShow(){
   if (groupBox_Make_userDef->isHidden())
      groupBox_Make_userDef->show();
   else
      groupBox_Make_userDef->hide();
   lineEdit_areaid->clear();
   comboBox_Make_Rule->setCurrentIndex(0);
   make_ruleValueReset();
   make_usrDefShowBytes(0);
}


void FontConvert::make_usrDefShowBytes(int rule_index){
    switch(make_charset->rules.at(rule_index).bytes){
    case 1: make_ruleShow1Bytes();
        break;
    case 2: make_ruleShow2Bytes();
        break;
    case 4: make_ruleShow4Bytes();
        break;
    default:
        break;
    }
    union hex lower,upper;
    lower.ihex = make_charset->rules.at(rule_index).lower;
    upper.ihex = make_charset->rules.at(rule_index).upper;
    label_1stB_lower->setText(QString::number(lower.chex[0],16).toUpper());
    label_1stB_upper->setText(QString::number(upper.chex[0],16).toUpper());
    label_2ndB_lower->setText(QString::number(lower.chex[1],16).toUpper());
    label_2ndB_upper->setText(QString::number(upper.chex[1],16).toUpper());
    label_3rdB_lower->setText(QString::number(lower.chex[2],16).toUpper());
    label_3rdB_upper->setText(QString::number(upper.chex[2],16).toUpper());
    label_4thB_lower->setText(QString::number(lower.chex[3],16).toUpper());
    label_4thB_upper->setText(QString::number(upper.chex[3],16).toUpper());
}

void FontConvert::make_ruleShow1Bytes(){
    lineEdit_1stB_lower->show();
    lineEdit_2ndB_lower->hide();
    lineEdit_3rdB_lower->hide();
    lineEdit_4thB_lower->hide();
    lineEdit_1stB_upper->show();
    lineEdit_2ndB_upper->hide();
    lineEdit_3rdB_upper->hide();
    lineEdit_4thB_upper->hide();
    label_firstByte->show();
    label_1stB_lower->show();
    label_1stB_upper->show();
    label_1st_to->show();
    label_secondByte->hide();
    label_2ndB_lower->hide();
    label_2ndB_upper->hide();
    label_2nd_to->hide();
    label_thirdByte->hide();
    label_3rdB_lower->hide();
    label_3rdB_upper->hide();
    label_3rd_to->hide();
    label_fourthByte->hide();
    label_4thB_lower->hide();
    label_4thB_upper->hide();
    label_4th_to->hide();
}

void FontConvert::make_ruleShow2Bytes(){
    lineEdit_1stB_lower->show();
    lineEdit_2ndB_lower->show();
    lineEdit_3rdB_lower->hide();
    lineEdit_4thB_lower->hide();
    lineEdit_1stB_upper->show();
    lineEdit_2ndB_upper->show();
    lineEdit_3rdB_upper->hide();
    lineEdit_4thB_upper->hide();
    label_firstByte->show();
    label_1stB_lower->show();
    label_1stB_upper->show();
    label_1st_to->show();
    label_secondByte->show();
    label_2ndB_lower->show();
    label_2ndB_upper->show();
    label_2nd_to->show();
    label_thirdByte->hide();
    label_3rdB_lower->hide();
    label_3rdB_upper->hide();
    label_3rd_to->hide();
    label_fourthByte->hide();
    label_4thB_lower->hide();
    label_4thB_upper->hide();
    label_4th_to->hide();
}

void FontConvert::make_ruleShow4Bytes(){
    lineEdit_1stB_lower->show();
    lineEdit_2ndB_lower->show();
    lineEdit_3rdB_lower->show();
    lineEdit_4thB_lower->show();
    lineEdit_1stB_upper->show();
    lineEdit_2ndB_upper->show();
    lineEdit_3rdB_upper->show();
    lineEdit_4thB_upper->show();
    label_firstByte->show();
    label_1stB_lower->show();
    label_1stB_upper->show();
    label_1st_to->show();
    label_secondByte->show();
    label_2ndB_lower->show();
    label_2ndB_upper->show();
    label_2nd_to->show();
    label_thirdByte->show();
    label_3rdB_lower->show();
    label_3rdB_upper->show();
    label_3rd_to->show();
    label_fourthByte->show();
    label_4thB_lower->show();
    label_4thB_upper->show();
    label_4th_to->show();
}

void FontConvert::make_ruleValueReset(){
    lineEdit_1stB_lower->setText(QString::fromUtf8("0"));
    lineEdit_2ndB_lower->setText(QString::fromUtf8("0"));
    lineEdit_3rdB_lower->setText(QString::fromUtf8("0"));
    lineEdit_4thB_lower->setText(QString::fromUtf8("0"));
    lineEdit_1stB_upper->setText(QString::fromUtf8("0"));
    lineEdit_2ndB_upper->setText(QString::fromUtf8("0"));
    lineEdit_3rdB_upper->setText(QString::fromUtf8("0"));
    lineEdit_4thB_upper->setText(QString::fromUtf8("0"));
}

void FontConvert::make_ruleSltShow(int rows,int cloumn){
    union hex lower,upper;
    int bytes;
    QList<Character_Set_Area>::iterator iter = make_charset->areas.begin();
    for(int i = 0;iter!=make_charset->areas.end();iter++,i++){
        if (!QString::compare(tableWidget_Make_charsetArea->item(rows,0)->text(),iter->name_id)){
            lower.ihex = iter->lower;
            upper.ihex = iter->upper;
            lineEdit_areaid->setText(iter->name_id);
            comboBox_Make_Rule->setCurrentIndex(iter->rule_index);
            bytes = make_charset->rules.at(iter->rule_index).bytes;
            if (1 == bytes){
                lineEdit_1stB_lower->setText((QString::number(lower.chex[0],16).toUpper()));
                lineEdit_1stB_upper->setText((QString::number(upper.chex[0],16).toUpper()));
            }
            if (2 == bytes){
                lineEdit_1stB_lower->setText((QString::number(lower.chex[1],16).toUpper()));
                lineEdit_2ndB_lower->setText((QString::number(lower.chex[0],16).toUpper()));
                lineEdit_1stB_upper->setText((QString::number(upper.chex[1],16).toUpper()));
                lineEdit_2ndB_upper->setText((QString::number(upper.chex[0],16).toUpper()));
            }
            if (4 == bytes){
                lineEdit_1stB_lower->setText((QString::number(lower.chex[3],16).toUpper()));
                lineEdit_2ndB_lower->setText((QString::number(lower.chex[2],16).toUpper()));
                lineEdit_3rdB_lower->setText((QString::number(lower.chex[1],16).toUpper()));
                lineEdit_4thB_lower->setText((QString::number(lower.chex[0],16).toUpper()));
                lineEdit_1stB_upper->setText((QString::number(upper.chex[3],16).toUpper()));
                lineEdit_2ndB_upper->setText((QString::number(upper.chex[2],16).toUpper()));
                lineEdit_3rdB_upper->setText((QString::number(upper.chex[1],16).toUpper()));
                lineEdit_4thB_upper->setText((QString::number(upper.chex[0],16).toUpper()));
            }
        }
    }
}


void FontConvert::make_convDescShow(){
    if (tableWidget_Make_convInfo->isHidden()){
        tableWidget_Make_convInfo->show();
    }
    else
        tableWidget_Make_convInfo->hide();
}

void FontConvert::make_convShow(){
    if(converts.isEmpty())
        return;
    tableWidget_Make_converts->setColumnCount(3);
    tableWidget_Make_converts->setRowCount(converts.count()?:1);
    tableWidget_Make_converts->setEditTriggers(QAbstractItemView::NoEditTriggers);//disable edit
    //tableWidget_Make_converts->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度

    tableWidget_Make_converts->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    //tableWidget_Make_converts->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList headers;
    headers<<QString::fromUtf8("字符集")<<QString::fromUtf8("区块")<<QString::fromUtf8("特征");
    tableWidget_Make_converts->setHorizontalHeaderLabels(headers);
    QString str;
    QTableWidgetItem *item;
    QList<Convert>::iterator iter = converts.begin();
    for(int i=0;iter!=converts.end();iter++,i++){
        item = new QTableWidgetItem(iter->charset);
        tableWidget_Make_converts->setItem(i,0,item);
        item = new QTableWidgetItem(iter->csa.name_id);
        tableWidget_Make_converts->setItem(i,1,item);
        str = QString::number(iter->fc.width,10) + QString::fromUtf8("*") + QString::number(iter->fc.height);
        switch(iter->fc.bold){
            case FONT_NORMAL:
                str+=QString(",正常");
                break;
            case FONT_BOLD:
                str+=QString(",加粗+1");
                break;
            case FONT_BOLD2:
                str+=QString(",加粗+2");
                break;
            default:
                str+=QString(",未知");
                break;
        }
        item = new QTableWidgetItem(str);
        tableWidget_Make_converts->setItem(i,2,item);
    }
    tableWidget_Make_converts->resizeColumnsToContents();
    make_convOptSetEnable(1);
}

void FontConvert::make_convOptSetEnable(bool setValue){
    pushButton_Make_convMoveUp->setEnabled(setValue);
    pushButton_Make_convMoveDown->setEnabled(setValue);
    pushButton_Make_delConv->setEnabled(setValue);
    pushButton_Make_start->setEnabled(setValue);
}

void FontConvert::make_convMoveUp(){
    int cur_convert_index = tableWidget_Make_converts->currentRow();
    //qDebug()<<cur_cilist_index;
    if(cur_convert_index){
        converts.swap(cur_convert_index,cur_convert_index - 1);
        make_convShow();
        tableWidget_Make_converts->selectRow(cur_convert_index - 1);
    }
}

void FontConvert::make_convMoveDown(){
    int cur_convert_index = tableWidget_Make_converts->currentRow();
    //qDebug()<<cur_cilist_index;
    //qDebug()<<(convertlist.count()-1);
    if(cur_convert_index != (converts.count()-1)){
        converts.swap(cur_convert_index,cur_convert_index + 1);
        make_convShow();
        tableWidget_Make_converts->selectRow(cur_convert_index + 1);
    }
}

void FontConvert::make_delConv(){
    if(!converts.empty()){
        QList<QTableWidgetSelectionRange>ranges = tableWidget_Make_converts->selectedRanges();
        int count = ranges.count();
        for(int i = 0; i < count; i++){
            int toprow = ranges.at(i).topRow();
            int bottomrow = ranges.at(i).bottomRow();
            for(int j = bottomrow; j >= toprow; j--) {
                qDebug("currentRow %d, converts count %d",j,converts.count());
                if(j>(converts.count()-1))
                    return;
                converts.removeAt(j);
                tableWidget_Make_converts->selectRow(j-1);
            }
        }
        if(!converts.empty()){
            make_convShow();
            make_convDescSltShow(tableWidget_Make_converts->currentRow(),0);
        }
        else{
            tableWidget_Make_converts->clear();
            make_convOptSetEnable(0);
            return;
        }
    }
}

void FontConvert::make_addConv(){
    Convert *new_convert = new Convert;

    if(lineEdit_Make_ttfSrc->text().isEmpty()){
        messageBox(QString::fromUtf8("没有TTF字体文件!"));
        return;
    }
    //common part
    Font_Config *new_fontCfg = new Font_Config;

    new_fontCfg->width = lineEdit_Make_width->text().toInt();
    new_fontCfg->height = lineEdit_Make_height->text().toInt();
    new_fontCfg->bold = comboBox_Make_blod->currentIndex();
    new_fontCfg->strMode = comboBox_Make_mode->currentText();
    new_fontCfg->strOrder = comboBox_Make_order->currentText();
    new_fontCfg->mode = comboBox_Make_mode->currentIndex();
    new_fontCfg->order = comboBox_Make_order->currentIndex();
    new_fontCfg->align = comboBox_Make_align->currentText().toInt();
    new_fontCfg->woWidth = spinBox_Make_weightofWidth->text().toInt();
    new_fontCfg->woHeight = spinBox_Make_weightofHeight->text().toInt();

    new_convert->sourcepath = lineEdit_Make_ttfSrc->text();
    //qDebug()<<new_mcilist->sourcepath;
    new_convert->charset = comboBox_Make_charset->currentText();
    //qDebug()<<new_mcilist->charset;
    new_convert->encode = make_charset->get_encode();

    QList<QTableWidgetSelectionRange>ranges = tableWidget_Make_charsetArea->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++){
        int toprow = ranges.at(i).topRow();
        int bottomrow = ranges.at(i).bottomRow();
        for(int j = toprow; j <= bottomrow; j++) {
            memcpy(&(new_convert->csa),&make_charset->areas.at(j),sizeof(Character_Set_Area));
            qDebug()<<new_convert->csa.name_id;
            memcpy(&(new_convert->fc),new_fontCfg,sizeof(Font_Config));
            converts.append(*new_convert);
        }
    }
    delete new_fontCfg;
    delete new_convert;
    make_convShow();
}

void FontConvert::make_convDescSltShow(int rows,int cloumn){
    if(converts.isEmpty())
        return;
    make_convOptSetEnable(1);
    tableWidget_Make_convInfo->setColumnCount(1);
    tableWidget_Make_convInfo->setRowCount(9);
    tableWidget_Make_convInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//disable edit
    tableWidget_Make_convInfo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    tableWidget_Make_convInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    tableWidget_Make_convInfo->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList hheaders;
    hheaders<<QString::fromUtf8("值");
    tableWidget_Make_convInfo->setHorizontalHeaderLabels(hheaders);
    QStringList vheaders;
    vheaders<<QString::fromUtf8("字符集")<<QString::fromUtf8("源")<<QString::fromUtf8("区块") \
           <<QString::fromUtf8("范围")<<QString::fromUtf8("字符计数")<<QString::fromUtf8("点阵大小") \
           <<QString::fromUtf8("渲染")<<QString::fromUtf8("取模方式")<<QString::fromUtf8("对齐")  ;
    tableWidget_Make_convInfo->setVerticalHeaderLabels(vheaders);

    Convert cur_select_convert = converts.at(rows);
    QTableWidgetItem *item;
    QString str;
    qDebug()<<tableWidget_Make_converts->item(rows,0)->text()<<";"<<tableWidget_Make_converts->item(rows,1)->text();

                item = new QTableWidgetItem(cur_select_convert.charset);
                tableWidget_Make_convInfo->setItem(0,0,item);

                item = new QTableWidgetItem(cur_select_convert.sourcepath);
                tableWidget_Make_convInfo->setItem(1,0,item);

                item = new QTableWidgetItem(cur_select_convert.csa.name_id);
                tableWidget_Make_convInfo->setItem(2,0,item);

                str = QString::number(cur_select_convert.csa.lower,16).toUpper()+ QString::fromUtf8("~")+ QString::number(cur_select_convert.csa.upper,16).toUpper();
                item = new QTableWidgetItem(str);
                tableWidget_Make_convInfo->setItem(3,0,item);

                item = new QTableWidgetItem(QString::number(cur_select_convert.csa.char_nums,10));
                tableWidget_Make_convInfo->setItem(4,0,item);

                str = QString::number(cur_select_convert.fc.width,10) + QString::fromUtf8("*") + QString::number(cur_select_convert.fc.height);
                item = new QTableWidgetItem(str);
                tableWidget_Make_convInfo->setItem(5,0,item);


                switch(cur_select_convert.fc.bold){
                    case FONT_NORMAL:
                        str=QString(",正常");
                        break;
                    case FONT_BOLD:
                        str=QString(",加粗+1");
                        break;
                    case FONT_BOLD2:
                        str=QString(",加粗+2");
                        break;
                    default:
                        str=QString(",未知");
                        break;
                }
                item = new QTableWidgetItem(str);
                tableWidget_Make_convInfo->setItem(6,0,item);

                str = cur_select_convert.fc.strMode + QString::fromUtf8(",") + cur_select_convert.fc.strOrder;
                item = new QTableWidgetItem(str);
                tableWidget_Make_convInfo->setItem(7,0,item);

                str = QString::fromUtf8("区块") + QString::number(cur_select_convert.fc.align,10) + QString::fromUtf8("字节对齐");
                item = new QTableWidgetItem(str);
                tableWidget_Make_convInfo->setItem(8,0,item);

}




void FontConvert::make_convStart(){
    if(lineEdit_Make_outPutFontBin->text().isEmpty()){
        messageBox(QString("please set out put filename!"));
        return;
    }
    if(lineEdit_Make_fontVer->text().isEmpty()){
        messageBox(QString("please set version(4 numbers)!"));
        return;
    }
    if (converts.isEmpty()){
        messageBox(QString("no convert objects!"));
        return;
    }

    QString out_path;
    out_path = QCoreApplication::applicationDirPath();
    out_path += QString("/") + lineEdit_Make_outPutFontBin->text();
    qDebug()<<out_path;


    QString str_version = lineEdit_Make_fontVer->text();
    QString str_describe = lineEdit_Make_describe->text();


    my_convThd = new Charset_Convert();
    if(!my_convThd){
        qDebug("create convert thread fail\n");
        return;
    }
    qDebug("create convert thread\n");
    if (0 > my_convThd->init(converts,out_path,str_version,str_describe))
    {
        qDebug("convert thread init failed\n");
        delete my_convThd;
        my_convThd = NULL;
        return;
    }
    connect(my_convThd,SIGNAL(print_index_num(int,int)),this,SLOT(make_convDispIndex(int,int)),Qt::AutoConnection);
    connect(my_convThd,SIGNAL(print_char_num(int,int)),this,SLOT(make_convDispNum(int,int)),Qt::AutoConnection);
    my_convThd->start();
    sleep(1);
}

void FontConvert::make_convStop(){
    if(my_convThd){
        disconnect(my_convThd,SIGNAL(print_index_num(int,int)),this,SLOT(make_convDispIndex(int,int)));
        disconnect(my_convThd,SIGNAL(print_char_num(int,int)),this,SLOT(make_convDispNum(int,int)));
        my_convThd->terminate();
        my_convThd->wait(1);
        delete my_convThd;
        my_convThd = NULL;
    }
}

void FontConvert::make_convDispIndex(int cur,int total){
    QString str_taskcount;
    str_taskcount = QString::number(cur,10) + "/" + QString::number(total,10);
    label_Make_procIndexCount->setText(str_taskcount);
}

void FontConvert::make_convDispNum(int cur,int total){
    QString str_charcnt;
    str_charcnt = QString::number(cur,10) + "/" + QString::number(total,10);
    label_Make_procNumCount->setText(str_charcnt);
    int percent = cur*100/total;
    progressBar_Make_conv->setValue(percent);
}


/********************************************************************/
void FontConvert::trim_fontBinSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字库"),
        ".",
        tr("BIN(*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Trim_fontBinSrc->setText(filename);
    }

};

void FontConvert::trim_Init()
{
    tableWidget_Trim_charView->clear();
    pushButton_Trim_charRecover->setEnabled(0);
    pushButton_Trim_charSave->setEnabled(0);
    pushButton_Trim_newFontBinSave->setEnabled(0);
    pushButton_Trim_getPreChar->setEnabled(0);
    pushButton_Trim_getNextChar->setEnabled(0);
    trimChars.clear();
    label_Trim_newFontBin->clear();
    label_Trim_fonBinSuffix->setText(QString("_Md5_XXXX.bin"));
}

char* FontConvert::newFileImage(QString filename,int& size)
{
    char* p = NULL;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        messageBox(QString::fromUtf8("打开文件失败!"));
        return NULL;
    }
    size = file.size();
    p = new char[size]();
    file.read( p, size);
    file.close();
    return p;
}

void FontConvert::freeFileImage(char* p)
{
    if(p){
        delete p;
        p = NULL;
    }
}

void FontConvert::trim_fontBinLoad(){
    QString filename;
    int size;
    filename = lineEdit_Trim_fontBinSrc->text();
    if(filename.isEmpty())
        return;

    char* image = newFileImage(filename,size);
    if(!image)
        return;
    fontBinStructLoad(image,trim_charsets,
                      lineEdit_Trim_fontBinVer,lineEdit_Trim_fontBinDesc,
                      lineEdit_Trim_fontBinOffset,lineEdit_Trim_fontBinMd5);
    freeFileImage(image);
    fontBinStructShow(tableWidget_Trim_fontBinStruct,trim_charsets);
    //clear
    trim_Init();
}

void FontConvert::fontBinStructLoad(char* image, QList<CharSet_Unit> &charsets,
                                    QLineEdit* lineEdit_ver=0, QLineEdit* lineEdit_desc=0, QLineEdit* lineEdit_offset=0, QLineEdit* lineEdit_md5=0)
{
    charsets.clear();
    Fontlib_Header *lib_hdr;
    Header_Charset *cur_charset_hdr;
    CharSet_Unit *new_charset;
    lib_hdr = (Fontlib_Header*)image;
    int i;
    char checksum = 0;
    for(i=0;i<int(sizeof(Fontlib_Header));i++)
        checksum ^= image[i];
    if(checksum){
        messageBox(QString("font lib header check error!"));
        return;
    }
    qDebug("sizeof Fontlib_Header %d",int(sizeof(Fontlib_Header)));
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba = QByteArray((const char*)(image + sizeof(Fontlib_Header)), lib_hdr->size-sizeof(Fontlib_Header));
    md.addData(ba);
    bb = md.result();
    qDebug()<<"Crypto MD5:"<<QString(bb.toHex());
    if (memcmp(lib_hdr->md5, bb.data(), 16)){
        messageBox(QString("font lib md5 check error!"));
        return;
    }

    //to do: show lib meessage
    unsigned char cc[65]={0};
    memcpy(cc,lib_hdr->describe,64);
    QByteArray md5 = QByteArray((const char*)lib_hdr->md5,16);

    if(lineEdit_ver)
        lineEdit_ver->setText(QString::number(lib_hdr->version,16));
    if(lineEdit_desc)
        lineEdit_desc->setText(QString((const char *)cc));
    if(lineEdit_offset)
        lineEdit_offset->setText(QString("0x")+QString::number(lib_hdr->offset,16));
    if(lineEdit_md5)
        lineEdit_md5->setText(md5.toHex());

    //creat charset
    cur_charset_hdr = (Header_Charset *)(image + sizeof(Fontlib_Header));
    for( ; cur_charset_hdr->encode != 0; cur_charset_hdr++ ){
        new_charset = new CharSet_Unit;
        memcpy(&(new_charset->charset_hdr),cur_charset_hdr,sizeof(Header_Charset));
        new_charset->bitmap = QByteArray(image + lib_hdr->offset + cur_charset_hdr->addr_start, cur_charset_hdr->addr_end - cur_charset_hdr->addr_start);
        charsets.append(*new_charset);
    }

}

void FontConvert::fontBinStructShow(QTableWidget* tableWidget_fontBinStruct,QList<CharSet_Unit>&localCharsets){
    if(localCharsets.empty())
        return;

    tableWidget_fontBinStruct->setColumnCount(11);//
    tableWidget_fontBinStruct->setEditTriggers(QAbstractItemView::NoEditTriggers);//disable edit
    //tableWidget_fontBinStruct->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    tableWidget_fontBinStruct->resizeColumnsToContents();
    tableWidget_fontBinStruct->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    tableWidget_fontBinStruct->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList headers;
    headers<<QString::fromUtf8("字符集")<<QString::fromUtf8("宽")<<QString::fromUtf8("高")<<QString::fromUtf8("渲染")<<QString::fromUtf8("位图大小")<<QString::fromUtf8("模向大小")
          <<QString::fromUtf8("开始")<<QString::fromUtf8("结束")<<QString::fromUtf8("字符统计")<<QString::fromUtf8("起始地址")<<QString::fromUtf8("结束地址");
    tableWidget_fontBinStruct->setHorizontalHeaderLabels(headers);
    tableWidget_fontBinStruct->setRowCount(localCharsets.count());
    QTableWidgetItem *item;
    QString name;
    QString str;

    QList<CharSet_Unit>::Iterator iter = localCharsets.begin();
    for (int i = 0; iter != localCharsets.end() ; iter++,i++ ){
        switch(iter->charset_hdr.encode){
        case Character_Set::ENCODE_GB18030:
            name = QString("GB18030(") + QString::number(Character_Set::ENCODE_GB18030) + QString(")") ;
            break;
        case Character_Set::ENCODE_UCS_4:
            name = QString("Unicode(")+ QString::number(Character_Set::ENCODE_UCS_4) + QString(")");
            break;
        default:
            name = QString("unknow");
            break;
        }

        item = new QTableWidgetItem( name );
        tableWidget_fontBinStruct->setItem(i,0,item);

        item = new QTableWidgetItem( QString::number( iter->charset_hdr.width, 10 ) );
        tableWidget_fontBinStruct->setItem(i,1,item);
        item = new QTableWidgetItem( QString::number( iter->charset_hdr.height, 10 ) );
        tableWidget_fontBinStruct->setItem(i,2,item);
        switch(iter->charset_hdr.effect){
            case FONT_NORMAL:
                str=QString("正常");
                break;
            case FONT_BOLD:
                str=QString("加粗+1");
                break;
            case FONT_BOLD2:
                str=QString("加粗+2");
                break;
            default:
                str=QString("未知");
                break;
        }
        item = new QTableWidgetItem( str );
        tableWidget_fontBinStruct->setItem(i,3,item);

        item = new QTableWidgetItem( QString::number( iter->charset_hdr.unit, 10 ) );
        tableWidget_fontBinStruct->setItem(i,4,item);
        item = new QTableWidgetItem( QString::number( iter->charset_hdr.pitch, 10 ) );
        tableWidget_fontBinStruct->setItem(i,5,item);

        item = new QTableWidgetItem( QString::number( iter->charset_hdr.code_start, 16 ).toUpper() );
        tableWidget_fontBinStruct->setItem(i,6,item);
        item = new QTableWidgetItem( QString::number( iter->charset_hdr.code_end, 16 ).toUpper() );
        tableWidget_fontBinStruct->setItem(i,7,item);

        item = new QTableWidgetItem( QString::number( iter->charset_hdr.nums, 10 ) );
        tableWidget_fontBinStruct->setItem(i,8,item);
        item = new QTableWidgetItem( QString::number( iter->charset_hdr.addr_start, 16 ).toUpper() );
        tableWidget_fontBinStruct->setItem(i,9,item);
        item = new QTableWidgetItem( QString::number( iter->charset_hdr.addr_end, 16 ).toUpper() );
        tableWidget_fontBinStruct->setItem(i,10,item);
    }
    tableWidget_fontBinStruct->resizeColumnsToContents();
    tableWidget_fontBinStruct->selectRow(0);

}

void FontConvert::trim_charsetSlt(int index){
    if(trim_charset){
        delete trim_charset;
        trim_charset = NULL;
    }
    switch(index){
    case 1:
        trim_charset = new GB18030_Character_Set;
        break;
    case 2:
        trim_charset = new Unicode_Character_Set;
        break;
    default:
        break;
    }
    pushButton_Trim_getChar->setEnabled(index?1:0);
}

void FontConvert::trim_getPreChar()
{
    bool ok;
    unsigned long code= lineEdit_Trim_charCode->text().toUInt(&ok ,16);
    if(trim_curCharsetHdr.encode == 0)
        return;
    if(!trim_charset)
        return;

    int ret = trim_charset->get_pre_code_ex(&code,trim_curCharsetHdr.countmode,
                                   trim_curCharsetHdr.code_start,trim_curCharsetHdr.code_end,
                                   trim_curCharsetHdr.code_top,trim_curCharsetHdr.code_bottom);
    if(ret < 0)
        return;
    lineEdit_Trim_charCode->setText(QString::number(code,16).toUpper());
    trim_getChar();
}

void FontConvert::trim_getNextChar()
{
    bool ok;
    unsigned long code= lineEdit_Trim_charCode->text().toUInt(&ok ,16);
    if(trim_curCharsetHdr.encode == 0)
        return;
    if(!trim_charset)
        return;

    int ret = trim_charset->get_next_code_ex(&code,trim_curCharsetHdr.countmode,
                                   trim_curCharsetHdr.code_start,trim_curCharsetHdr.code_end,
                                   trim_curCharsetHdr.code_top,trim_curCharsetHdr.code_bottom);
    if(ret < 0)
        return;
    lineEdit_Trim_charCode->setText(QString::number(code,16).toUpper());
    trim_getChar();
}

void FontConvert::trim_getChar(){
    unsigned long unicode_charcode;
    tableWidget_Trim_charView->clear();
    if(lineEdit_Trim_charCode->text().isEmpty())
        return;
    if(lineEdit_Trim_height->text().isEmpty())
        return;
    if(lineEdit_Trim_width->text().isEmpty())
        return;

    new_trimChar = new Trim_Char_Info();
    bool ok;
    switch(comboBox_Trim_inPutMode->currentIndex()){
    case 0:
        new_trimChar->code = lineEdit_Trim_charCode->text().toUInt(&ok ,16);
        break;
    case 1:
        unicode_charcode = lineEdit_Trim_charCode->text().at(0).unicode();
        trim_charset->unicode_to( &(unicode_charcode), &(new_trimChar->code) );
        break;
    default:
        break;
    }

    qDebug("test code: %x",(unsigned int)(new_trimChar->code));
    new_trimChar->cur_hdr.encode = trim_charset->get_encode();
    new_trimChar->charset_name = trim_charset->get_name();
    new_trimChar->cur_hdr.width = lineEdit_Trim_width->text().toUInt();
    new_trimChar->cur_hdr.height = lineEdit_Trim_height->text().toUInt();
    new_trimChar->mode = comboBox_Trim_mode->currentIndex();
    new_trimChar->order = comboBox_Trim_order->currentIndex();
    new_trimChar->cur_hdr.effect = comboBox_Trim_blod->currentIndex();
    if( 0 > trim_getCharBitmap(new_trimChar)){
        qDebug("get target error");
        pushButton_Trim_charRecover->setEnabled(0);
        pushButton_Trim_charSave->setEnabled(0);
        pushButton_Trim_getPreChar->setEnabled(0);
        pushButton_Trim_getNextChar->setEnabled(0);
        delete new_trimChar;
        new_trimChar = NULL;
        return;
    }
    trim_charDataShow(new_trimChar->code,new_trimChar->raw_bitmap);
    trim_charShow();
    pushButton_Trim_charRecover->setEnabled(1);
    pushButton_Trim_charSave->setEnabled(1);
    pushButton_Trim_getPreChar->setEnabled(1);
    pushButton_Trim_getNextChar->setEnabled(1);

}

int FontConvert::trim_getCharBitmap(Trim_Char_Info *test_char){
    //get the area which code belong to

    if(!test_char){
        qDebug("null point");
        return -1;
    }
    int offset = 0;
    //lookup at modify data first
    QList<Trim_Char_Info>::Iterator iter_m = trimChars.begin();
    for (; iter_m != trimChars.end() ; iter_m++)
    {
        if( !memcmp( &(test_char->cur_hdr), &(iter_m->cur_hdr) , sizeof(Compare_Header) ) )
            if(test_char->code == iter_m->code){
                test_char->offset = iter_m->offset;
                test_char->addr_start = iter_m->addr_start;
                test_char->addr_offset = iter_m->addr_offset;
                test_char->raw_bitmap = iter_m->raw_bitmap;
                test_char->modify_bitmap = iter_m->modify_bitmap;
                return 0;
            }
    }
    //lookup at raw data
    QList<CharSet_Unit>::Iterator iter = trim_charsets.begin();
    for (; iter != trim_charsets.end() ; iter++)
    {
        if( !memcmp( &(test_char->cur_hdr), &(iter->charset_hdr) , sizeof(Compare_Header) ) )
        {
            if( test_char->code >= iter->charset_hdr.code_start && test_char->code <= iter->charset_hdr.code_end )
            {
                //qDebug("countmode %d\n",iter->charset_hdr.countmode);
                //qDebug("code_start 0x%x\n",iter->charset_hdr.code_start);
                //qDebug("code_end 0x%x\n",iter->charset_hdr.code_end);
                //qDebug("code_top 0x%x\n",iter->charset_hdr.code_top);
                //qDebug("code_bottom 0x%x\n",iter->charset_hdr.code_bottom);

                offset = trim_charset->area_chars_offset(test_char->code,
                                                                iter->charset_hdr.countmode,
                                                                iter->charset_hdr.code_start,
                                                                iter->charset_hdr.code_end,
                                                                iter->charset_hdr.code_top,
                                                                iter->charset_hdr.code_bottom);
                qDebug("offset %d",offset);

                if(offset < 0){
                    continue;
                }
                memcpy(&trim_curCharsetHdr, &(iter->charset_hdr), sizeof(Header_Charset));
                test_char->offset = offset;
                test_char->addr_start = iter->charset_hdr.addr_start;
                test_char->addr_offset =offset * iter->charset_hdr.unit;
                test_char->raw_bitmap.clear();
                test_char->raw_bitmap.append(iter->bitmap.mid(test_char->addr_offset, iter->charset_hdr.unit));
                test_char->modify_bitmap.clear();
                test_char->modify_bitmap = test_char->raw_bitmap;
                return 0;
            }
        }
    }
    memset(&trim_curCharsetHdr,0,sizeof(Header_Charset));
    return -1;
}

void FontConvert::trim_charDataShow(unsigned long code,QByteArray b)
{
    int i;
    QString bitmapdata;
    bitmapdata = QString("/*0x") + QString::number(code,16).toUpper() + QString("H*/\n{");
    for(i=0; i < b.count(); i++){
        bitmapdata += ((i%8) ? QString("0x") : QString("\n0x")) + QString("%1").arg(b[i],2,16,QChar('0')).right(2).toUpper() + QString(",") ;
    }
    bitmapdata += QString("\n},");
    textBrowser_Trim_charData->setText(bitmapdata);
}

void FontConvert::trim_charShow()
{
    if(!new_trimChar){
        qDebug("null point");
        return ;
    }
    //bitmap
    my_drawbitmap_extract(tableWidget_Trim_charView,new_trimChar);

    //info
    tableWidget_Trim_charInfo->setColumnCount(1);
    tableWidget_Trim_charInfo->setRowCount(6);
    tableWidget_Trim_charInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//disable edit
    tableWidget_Trim_charInfo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    tableWidget_Trim_charInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    tableWidget_Trim_charInfo->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList hheaders;
    hheaders<<QString::fromUtf8("值");
    tableWidget_Trim_charInfo->setHorizontalHeaderLabels(hheaders);
    QStringList vheaders;
    vheaders<<QString::fromUtf8("字符集")<<QString::fromUtf8("encode")<<QString::fromUtf8("点阵") \
           <<QString::fromUtf8("编码")<<QString::fromUtf8("位移")<<QString::fromUtf8("地址");
    tableWidget_Trim_charInfo->setVerticalHeaderLabels(vheaders);

    QTableWidgetItem *item;
    QString text;

    item = new QTableWidgetItem( new_trimChar->charset_name );
    tableWidget_Trim_charInfo->setItem(0,0,item);

    item = new QTableWidgetItem( QString::number( new_trimChar->cur_hdr.encode, 10 ) );
    tableWidget_Trim_charInfo->setItem(1,0,item);

    text = QString::number( new_trimChar->cur_hdr.width, 10 ) + QString("x") + QString::number( new_trimChar->cur_hdr.height, 10 );
    item = new QTableWidgetItem( text );
    tableWidget_Trim_charInfo->setItem(2,0,item);

    item = new QTableWidgetItem( QString::number( new_trimChar->code, 16 ) );
    tableWidget_Trim_charInfo->setItem(3,0,item);

    item = new QTableWidgetItem( QString::number( new_trimChar->offset, 10 ) );
    tableWidget_Trim_charInfo->setItem(4,0,item);

    item = new QTableWidgetItem( QString::number( new_trimChar->addr_offset, 16 ) );
    tableWidget_Trim_charInfo->setItem(5,0,item);

}

void FontConvert::trim_charClear(QString text)
{
    tableWidget_Trim_charView->clear();
    tableWidget_Trim_charInfo->clear();
    pushButton_Trim_getChar->setEnabled(!text.isEmpty()&&comboBox_Trim_charset->currentIndex());
}

void FontConvert::trim_CharViewZoomIn()
{
    int i;
    int column = tableWidget_Trim_charView->columnCount();
    int row = tableWidget_Trim_charView->rowCount();
    int size = tableWidget_Trim_charView->columnWidth(0);
    if(size < 50)
        size += 2;
    for( i=0; i<column; i++)
        tableWidget_Trim_charView->setColumnWidth(i,size);
    for( i=0; i<row; i++)
        tableWidget_Trim_charView->setRowHeight(i,size);
}

void FontConvert::trim_CharViewZoomOut()
{
    int i;
    int column = tableWidget_Trim_charView->columnCount();
    int row = tableWidget_Trim_charView->rowCount();
    int size = tableWidget_Trim_charView->columnWidth(0);
    if(size > 5)
        size -= 2;
    for( i=0; i<column; i++)
        tableWidget_Trim_charView->setColumnWidth(i,size);
    for( i=0; i<row; i++)
        tableWidget_Trim_charView->setRowHeight(i,size);
}

void FontConvert::trim_charViewPixelClick(int x,int y)
{
    QColor col;
    col = tableWidget_Trim_charView->item(x,y)->backgroundColor();
    col = (col == QColor(0,0,0)) ? QColor(255,255,255) : QColor(0,0,0);
    tableWidget_Trim_charView->item(x,y)->setBackgroundColor(col);
}

void FontConvert::trim_charViewRecover()
{
    if(!new_trimChar){
        qDebug("null point");
        return ;
    }
    my_drawbitmap_extract(tableWidget_Trim_charView,new_trimChar);
    //lookup at modify data first
    QList<Trim_Char_Info>::Iterator iter_m = trimChars.begin();
    for (int i = 0; iter_m != trimChars.end() ; iter_m++,i++)
    {
        if( !memcmp( &(new_trimChar->cur_hdr), &(iter_m->cur_hdr) , sizeof(Compare_Header) ) )
            if(new_trimChar->code == iter_m->code){
                new_trimChar->modify_bitmap = new_trimChar->raw_bitmap;
                trimChars.removeAt(i);
            }
    }
    if(trimChars.isEmpty())
       pushButton_Trim_newFontBinSave->setEnabled(0);
}

void FontConvert::trim_charSave()
{
    if(!new_trimChar){
        qDebug("null point");
        return ;
    }
    my_drawbitmap_modifySave(tableWidget_Trim_charView,new_trimChar);
}

void FontConvert::trim_newFontBinSave()
{
    if(lineEdit_Trim_fontBinSrc->text().isEmpty())
        return;
    QString filename;
    filename = lineEdit_Trim_fontBinSrc->text();
    QFile filein(filename);
    if(!filein.open(QIODevice::ReadOnly)){
        messageBox(QString::fromUtf8("打开文件失败!"));
        return;
    }
    qint64 size = filein.size();

    char* image = new char[size]();
    filein.read(image,size);
    filein.close();

    Fontlib_Header *lib_hdr;
    lib_hdr = (Fontlib_Header*)image;
    unsigned int modify_addr;

    textBrowser_Trim_charData->insertPlainText(QString("/******Modify char******/\n"));
    QList<Trim_Char_Info>::iterator iter = trimChars.begin();
    for(;iter!=trimChars.end();iter++){
        modify_addr = lib_hdr->offset + iter->addr_start + iter->addr_offset;
        memcpy( image + modify_addr, iter->modify_bitmap.data(), iter->modify_bitmap.count());
        textBrowser_Trim_charData->insertPlainText(QString("//address=0x%1\n").arg(modify_addr,8,16,QChar('0')));
        trim_charDataShow(iter->code,iter->modify_bitmap);
    }

    //to be:md5
    int i;
    unsigned char checksum;
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba = QByteArray((const char*)(image + sizeof(Fontlib_Header)), lib_hdr->size-sizeof(Fontlib_Header));
    md.addData(ba);
    bb = md.result();
    qDebug()<<"MD5:"<<QString(bb.toHex());
    qDebug()<<"md5 size:"<<bb.count();
    for(i=0;i<bb.count();i++)
        lib_hdr->md5[i]=bb.at(i);
    checksum = 0;
    for(i=0;i<int(sizeof(Fontlib_Header)-1);i++)
        checksum ^= image[i];
    lib_hdr->checksum = checksum;
    qDebug("font header checksum 0x%x",checksum);

    //save
    filename = lineEdit_Trim_fontBinSrc->text();
    int pos = filename.lastIndexOf("_Md5_");
    pos = (pos==-1) ? filename.count() : pos;
    QString newfilename = filename.left(pos);
    QString fileSuffix = (QString("_Md5_") + QString(bb.toHex()).left(4) + QString(".bin"));
    label_Trim_fonBinSuffix->setText(fileSuffix);
    newfilename += fileSuffix;
    label_Trim_newFontBin->setText(newfilename);
    QFile fileout(newfilename);
    if(!fileout.open(QIODevice::WriteOnly)){
        messageBox(QString::fromUtf8("打开文件失败!"));
        return;
    }
    QDataStream out(&fileout);
    out.writeRawData((const char*)image,lib_hdr->size);
    fileout.close();
    delete image;

}


//mode 0 horizontal 1 vertical  order 0  reverse order 1
void FontConvert::my_drawbitmap_spare(QTableWidget* table_bitmap,int mode,int order,QList<Draw_BitMap> &bitmaps){
    if(bitmaps.empty())
        return;
    table_bitmap->clear();
    int rows = 0,columns = 0;
    int b_rows = 0, b_columns = 0;
    table_bitmap->horizontalHeader()->setVisible(false);//header hide
    table_bitmap->verticalHeader()->setVisible(false);
    QString value;
    QColor col;
    int i,j;
    unsigned char bit;
    int table_x,table_y;

    QList<Draw_BitMap>::iterator iter = bitmaps.begin();
    for(int count =0; iter != bitmaps.end(); iter++,count ++){

        if(iter->rows > rows){
            rows = iter->rows;
            table_bitmap->setRowCount(rows);
            //qDebug()<<"rows = "<<iter->rows;
            for(i = b_rows;i < rows; i++)
                table_bitmap->setRowHeight(i,10);
        }
        b_columns = columns;
        columns += iter->width;
        table_bitmap->setColumnCount(columns);
        //qDebug()<<"columns = "<<iter->width;
        for( i = b_columns; i < columns; i++)
            table_bitmap->setColumnWidth(i,10);

        for(i = 0; i < ( mode? iter->width : iter->rows ); i++){
            for(j = 0; j < ( mode? iter->rows : iter->width ); j++){
                value = QString(" ");//
                bit = (iter->bitmap.at( i * (iter->pitch) + (j>>3) ) &  (order ? (1 << (j%8)): (0x80 >> (j%8))) )? 1 : 0;
                //qDebug("i %d,j %d,bit %d",i,j,bit);
                col = bit ? QColor(0,0,0) : QColor(255,255,255);
                table_x = ( mode ? j : i ) + b_rows;
                table_y = ( mode ? i : j ) + b_columns;
                if(table_x >= rows || table_x < 0 || table_y < 0 || table_y >= columns){
                    continue;
                }
                //qDebug()<<"i = "<<i<<" j = "<<j<<" value = "<<value;
                //qDebug()<<"draw_rows = "<<(i + b_rows)<<" draw_columns = "<<(j + b_columns)<<" bit = "<<value;
                table_bitmap->setItem( table_x, table_y, new QTableWidgetItem(value));
                table_bitmap->item( table_x, table_y)->setTextColor(col);
                table_bitmap->item( table_x, table_y)->setTextAlignment(Qt::AlignCenter);
                table_bitmap->item( table_x, table_y)->setBackgroundColor(col);
            }
        }
    }
}

#if 1
void FontConvert::my_drawbitmap_extract(QTableWidget* table_bitmap,Trim_Char_Info *test_char)
{
    if(!test_char)
        return;

    table_bitmap->clear();

    QString value;
    QColor col;
    unsigned char bit;
    int i,j,table_x,table_y;

    table_bitmap->horizontalHeader()->setVisible(false);//header hide
    table_bitmap->verticalHeader()->setVisible(false);
    table_bitmap->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_bitmap->setSelectionMode(QAbstractItemView::NoSelection);

    int width = test_char->cur_hdr.width;
    int height = test_char->cur_hdr.height;
    int mode = test_char->mode;
    int pitch = mode == HORIZONTAL ? (width+7)/8 : (height+7)/8;
    int order = test_char->order;

    table_bitmap->setRowCount(height);
    table_bitmap->setColumnCount(width);


    for( i = 0; i < width; i++)
        table_bitmap->setColumnWidth(i,10);
    for( i = 0; i < height; i++)
        table_bitmap->setRowHeight(i,10);

    for(i = 0; i < ( mode? width : height ); i++){
        for(j = 0; j < ( mode? height : width ); j++){
            value = QString(" ");//
            bit = (test_char->modify_bitmap.at( i * pitch + (j>>3) ) &  (order ? (1 << (j%8)): (0x80 >> (j%8))) )? 1 : 0;
            //qDebug("i %d,j %d,bit %d",i,j,bit);
            col = bit ? QColor(0,0,0) : QColor(255,255,255);
            table_x = ( mode ? j : i );
            table_y = ( mode ? i : j );

            if(table_x >= height || table_x < 0 || table_y < 0 || table_y >= width){
                continue;
            }
            table_bitmap->setItem( table_x, table_y, new QTableWidgetItem(value));
            table_bitmap->item( table_x, table_y)->setTextColor(col);
            table_bitmap->item( table_x, table_y)->setTextAlignment(Qt::AlignCenter);
            table_bitmap->item( table_x, table_y)->setBackgroundColor(col);
        }
    }

}

void FontConvert::my_drawbitmap_modifySave(QTableWidget* table_bitmap,Trim_Char_Info *test_char)
{
    if(!test_char)
        return;

    QColor col;
    unsigned char bit;
    int i,j;

    int width = test_char->cur_hdr.width;
    int height = test_char->cur_hdr.height;
    int mode = test_char->mode;
    int pitch = mode == HORIZONTAL ? (width+7)/8 : (height+7)/8;
    int order = test_char->order;
    int units = test_char->mode == HORIZONTAL ? pitch*height : pitch*width;

    int rows = table_bitmap->rowCount();
    int columns = table_bitmap->columnCount();

    if(rows!=height && columns != width){
        messageBox(QString("bitmap is bigger"));
        return;
    }
    test_char->modify_bitmap.clear();
    test_char->modify_bitmap.fill(0,units);
    int table_x,table_y;
    unsigned char tmp;
    for(i = 0; i < ( mode? width : height ); i++){
        for(j = 0; j < ( mode? height : width ); j++){
            table_x = ( mode ? j : i );
            table_y = ( mode ? i : j );
            col = table_bitmap->item(table_x,table_y)->backgroundColor();
            bit = (col == QColor(0,0,0)) ? 1 : 0;
            tmp = test_char->modify_bitmap.at(i*pitch + (j>>3));
            //qDebug("table_x %d,table_y %d,bit %d,%x",table_x,table_y,bit,tmp);
            if(bit)
                tmp |= (order ? (1 << (j%8)): (0x80 >> (j%8)));
            test_char->modify_bitmap[(i*pitch + (j>>3))] = tmp;

        }
    }
    QList<Trim_Char_Info>::Iterator iter_m;
    if(test_char->raw_bitmap.count() == test_char->modify_bitmap.count()){
        trim_charDataShow(test_char->code,test_char->modify_bitmap);
        if(test_char->raw_bitmap != test_char->modify_bitmap){
            pushButton_Trim_newFontBinSave->setEnabled(1);
            iter_m = trimChars.begin();
            for (; iter_m != trimChars.end() ; iter_m++)
            {
                if( !memcmp( &(test_char->cur_hdr), &(iter_m->cur_hdr) , sizeof(Compare_Header) ) )
                    if(test_char->code == iter_m->code){
                        iter_m->offset = test_char->offset;
                        iter_m->addr_start = test_char->addr_start;
                        iter_m->addr_offset = test_char->addr_offset;
                        iter_m->raw_bitmap = test_char->raw_bitmap;
                        iter_m->modify_bitmap = test_char->modify_bitmap;
                        return;
                    }
            }
            trimChars.append(*test_char);
        }
        else
        {
            iter_m = trimChars.begin();
            for (int i = 0; iter_m != trimChars.end() ; iter_m++, i++)
            {
                if( !memcmp( &(test_char->cur_hdr), &(iter_m->cur_hdr) , sizeof(Compare_Header) ) )
                    if(test_char->code == iter_m->code){
                        trimChars.removeAt(i);
                        if(trimChars.isEmpty())
                           pushButton_Trim_newFontBinSave->setEnabled(0);
                        return;
                    }
            }
        }
    }
}
#endif


void FontConvert::down_comInit(){
    //refer to qextserialbase enum
    QStringList protlist;
    QStringList baudratelist;
    QStringList databitslist;
    QStringList stopbitslist;
    QStringList paritylist;
    QStringList flowcontrollist;

    protlist<<"/dev/ttyUSB0"<<"/dev/ttyUSB1"<<"/dev/ttyUSB2"<<"/dev/ttyUSB3"<<"/dev/ttyUSB4"<<"/dev/ttyUSB5"\
              <<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyS2"<<"/dev/ttyS3"<<"/dev/ttyS4"<<"/dev/ttyS5"<<"/dev/ttyS6";

    baudratelist<<"50"<<"75"<<"110"<<"134"<<"150"<<"200"<<"300"<<"600"<<"1200"<<"1800"\
               <<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"<<"56000"<<"57600"<<"76800"<<"115200"\
                <<"128000"<<"256000";

    databitslist<<"5"<<"6"<<"7"<<"8";

    stopbitslist<<"1"<<"1.5"<<"2";

    paritylist<<"None"<<"Odd"<<"Even";

    flowcontrollist<<"OFF"<<"HardWare"<<"Xon/Xoff";

    comboBox_Down_comPort->addItems(protlist);
    comboBox_Down_comPort->setCurrentIndex(0);

    comboBox_Down_comBaudrate->addItems(baudratelist);
    comboBox_Down_comBaudrate->setCurrentIndex(19);

    comboBox_Down_comDatabits->addItems(databitslist);
    comboBox_Down_comDatabits->setCurrentIndex(3);

    comboBox_Down_comStopbits->addItems(stopbitslist);
    comboBox_Down_comStopbits->setCurrentIndex(0);

    comboBox_Down_comParity->addItems(paritylist);
    comboBox_Down_comParity->setCurrentIndex(0);

    comboBox_Down_comFlowcontrol->addItems(flowcontrollist);
    comboBox_Down_comFlowcontrol->setCurrentIndex(1);


}

void FontConvert::formatHex(QByteArray &bytes)
{
    char c;
    for (int i=0; i < bytes.count() ;i++)
    {
        c = bytes.at(i);
        if ( !((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || ( c >= 'A' && c <= 'F')) )
        {
            bytes.remove(i,1);
            i--;
        }
        //qDebug("%c bytes.count %d",c,bytes.count());
    }
}

void FontConvert::blankPadding(QByteArray &bytes){
    for (int i=2; i < bytes.count() ;i+=3)
    {
        bytes.insert(i,' ');
    }

}

QString FontConvert::toDump(QByteArray data){
    QByteArray barray;
    for(int i = 0; i < data.count() ; i++ ){
        barray.append(hex2Ascii((data.at(i) >> 4) & 0xf));
        barray.append(hex2Ascii((data.at(i) >> 0) & 0xf));
        barray.append(' ');
    }
    if(data.count())
        barray.append('\n');
    QString s = barray;
    return s;
}

void FontConvert::down_logDump(QString tile,QByteArray data){
    textBrowser_Down_log->insertPlainText(tile);
    QByteArray temp;
    for(int i=0;i<data.count();i++){
        temp.append(hex2Ascii((data.at(i)>>4) & 0x0f));
        temp.append(hex2Ascii(data.at(i) & 0x0f));
        temp.append(' ');
    }
    temp.append('\n');
    textBrowser_Down_log->insertPlainText(temp);
}

void FontConvert::down_log(QString msg){
    textBrowser_Down_log->insertPlainText(msg);
}

void FontConvert::down_logClear(){
    textBrowser_Down_log->clear();
}

void FontConvert::down_logMoveEnd(){
    textBrowser_Down_log->moveCursor(QTextCursor::End);
}

char FontConvert::hex2Ascii(char c){
    if (c >= 0 && c <= 9)
        c += 48;
    else if( c >= 10 && c <= 15)
        c += (65-10);
    return c;
}

char FontConvert::ascii2Hex(char c){
    if (c >= '0' && c <= '9')
        c -= 48;
    else if( c >= 'A' && c <= 'F')
        c -= (65-10);
    else if( c >= 'a' && c <= 'f')
        c -= (97-10);
    return c;
}

void FontConvert::down_fontBinSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字库"),
        ".",
        tr("BIN(*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Down_fontBinSrc->setText(filename);
    }

}

void FontConvert::down_fontBinDownProcShow(qint64 percent){
    progressBar_Down_fontBinDl->setValue(percent);
    textBrowser_Down_log->insertPlainText(QString('.'));
}

void FontConvert::down_patchDownProcShow(qint64 percent){
    progressBar_Down_fontPatchSync->setValue(percent);
    textBrowser_Down_log->insertPlainText(QString('.'));
}

void FontConvert::down_fontBinDownCheckPatch(int slt){

    if (my_fontThd) {
        if (my_fontThd->isRunning()){
            my_fontThd->terminate();
            my_fontThd->wait(1);
        }
        switch (my_fontThd->getMode()) {
        case 0:
            disconnect(my_fontThd,SIGNAL(mypercent(qint64)),this,SLOT(down_fontBinDownProcShow(qint64)));
            break;
        case 2:
            disconnect(my_fontThd,SIGNAL(mypercent(qint64)),this,SLOT(down_fontBinPatchDownProcShow(qint64)));
            break;
        default:
            break;
        }

        disconnect(my_fontThd,SIGNAL(myprint_dump(QString,QByteArray)),this,SLOT(down_logDump(QString,QByteArray)));
        disconnect(my_fontThd,SIGNAL(myprint_message(QString)),this,SLOT(down_log(QString)));
        delete my_fontThd;
        my_fontThd = NULL;
    }


    //get com setting
    down_portSetting.BaudRate = (BaudRateType)(comboBox_Down_comBaudrate->currentIndex());
    down_portSetting.DataBits = (DataBitsType)(comboBox_Down_comDatabits->currentIndex());
    down_portSetting.StopBits = (StopBitsType)(comboBox_Down_comStopbits->currentIndex());
    down_portSetting.Parity = (ParityType)(comboBox_Down_comParity->currentIndex());
    down_portSetting.FlowControl = (FlowType)(comboBox_Down_comFlowcontrol->currentIndex());
    down_portSetting.Timeout_Millisec = 200;
    //get port
    QString port = comboBox_Down_comPort->currentText();

    my_fontThd = new PrinterFont(port, down_portSetting);
    switch (slt) {
    case 0:
        my_fontThd->SetMode(PrinterFont::Down_Mode);
        my_fontThd->SetFile(lineEdit_Down_fontBinSrc->text());
        connect(my_fontThd,SIGNAL(mypercent(qint64)),this,SLOT(down_fontBinDownProcShow(qint64)));
        break;
    case 1:
        my_fontThd->SetMode(PrinterFont::Verift_Mode);
        my_fontThd->SetFile(lineEdit_Down_fontBinSrc->text());
        break;
    case 2:
        my_fontThd->SetMode(PrinterFont::Patch_Mode);
        my_fontThd->SetFile(lineEdit_Down_fontBinPatchSrc->text());
        connect(my_fontThd,SIGNAL(mypercent(qint64)),this,SLOT(down_fontBinPatchDownProcShow(qint64)));
        break;
    default:
        break;
    }

    connect(my_fontThd,SIGNAL(mypercent(qint64)),this,SLOT(down_fontBinDownProcShow(qint64)));
    connect(my_fontThd,SIGNAL(myprint_dump(QString,QByteArray)),this,SLOT(down_logDump(QString,QByteArray)));
    connect(my_fontThd,SIGNAL(myprint_message(QString)),this,SLOT(down_log(QString)));
    my_fontThd->start();

}

void FontConvert::down_fontBinDown(){
    if(lineEdit_Down_fontBinSrc->text().isEmpty())
        return;
    down_fontBinDownCheckPatch(0);
}

void FontConvert::down_fontBinCheck(){
    if(lineEdit_Down_fontBinSrc->text().isEmpty())
        return;
    down_fontBinDownCheckPatch(1);
}

void FontConvert::down_patchFontBinSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字库"),
        ".",
        tr("BIN(*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Down_patchFontBinSrc->setText(filename);
        progressBar_Down_fontBinPatchBuild->setValue(0);
    }
}

void FontConvert::down_patchFontBinDstBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字库"),
        ".",
        tr("BIN(*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Down_patchFontBinDst->setText(filename);
        progressBar_Down_fontBinPatchBuild->setValue(0);
    }
}

void FontConvert::down_patchBuild()
{
    QString filename;
    int sizeSrc,sizeDst;
    filename = lineEdit_Down_patchFontBinSrc->text();
    if(filename.isEmpty())
        return;
    char* imageSrc = newFileImage(filename, sizeSrc);
    if(!imageSrc)
        return;
    filename = lineEdit_Down_patchFontBinDst->text();
    if(filename.isEmpty())
        return;
    char* imageDst = newFileImage(filename, sizeDst);
    if(!imageDst)
        return;


    QByteArray srcMd5 = QByteArray((const char*)((Fontlib_Header*)imageSrc)->md5,16);
    QByteArray DstMd5 = QByteArray((const char*)((Fontlib_Header*)imageDst)->md5,16);
    QString patchname = QString("font_Md5_") + srcMd5.toHex().left(4) + QString("_") + DstMd5.toHex().left(4) + QString(".patch");
    Patch_Header patchHdr;
    memcpy(patchHdr.magic, "WiZaRpOsBinPatch", 16);
    memcpy(patchHdr.srcMd5, srcMd5.data(), 16);
    memcpy(patchHdr.dstMd5, DstMd5.data(), 16);

    int CMPBLOCK=0x1000;//because flash block is 4k
    int pos = 0;
    int ctlen;
    int minlen;
    QList<Diff_Bin> diffDatas;
    Diff_Bin diffData;
    int j;
    char checksum;
    char c[4];
    while( pos<sizeSrc && pos<sizeDst ){
        minlen = (sizeSrc-pos)>(sizeDst-pos) ? (sizeDst-pos) : (sizeSrc-pos);
        ctlen = minlen>CMPBLOCK ? CMPBLOCK : minlen;
        if( memcmp(imageSrc+pos, imageDst+pos, ctlen))
        {
            checksum = 0;
            diffData.offset = pos;
            diffData.length = ctlen;
            diffData.bytes = QByteArray(imageDst+pos, ctlen);
            //checksum
            memcpy(c,&diffData.offset,4);
            for(j = 0; j<4; j++)
                checksum ^= c[j];
            memcpy(c,&diffData.length,4);
            for(j = 0; j<4; j++)
                checksum ^= c[j];
            for(j = 0; j<ctlen; j++)
                checksum ^= imageDst[pos+j];
            //add
            diffData.bytes.append(checksum);
            diffDatas.append(diffData);

        }
        progressBar_Down_fontBinPatchBuild->setValue(pos*100/sizeDst);
        pos += ctlen;
    }
    if(pos<sizeDst)
    {
        diffData.offset = pos;
        diffData.length = sizeDst-pos;
        diffData.bytes = QByteArray(imageDst, sizeDst-pos);
        diffDatas.append(diffData);
    }
    freeFileImage(imageDst);
    freeFileImage(imageSrc);

    int size = 0;
    size += (sizeof(Patch_Header));
    QList<Diff_Bin>::iterator iter;
    for(iter = diffDatas.begin(); iter!=diffDatas.end(); iter++){
        size += (8 + iter->bytes.count());
    }


    char* patchData = new char[size]();
    memcpy(patchData, &patchHdr, sizeof(Patch_Header));
    char* tmp = patchData + sizeof(Patch_Header);
    for(iter = diffDatas.begin(); iter!=diffDatas.end(); iter++){
        memcpy(tmp, &iter->offset, 4);
        memcpy(tmp+4, &iter->length, 4);
        memcpy(tmp+8, iter->bytes.data(), iter->bytes.count());
        tmp += (8 + iter->bytes.count());
    }

    QFile fileout(patchname);
    if(!fileout.open(QIODevice::WriteOnly)){
        messageBox(QString::fromUtf8("打开文件失败!"));
        delete patchData;
        return;
    }
    QDataStream out(&fileout);
    out.writeRawData((const char*)patchData,size);
    fileout.close();
    delete patchData;
    progressBar_Down_fontBinPatchBuild->setValue(100);
}

void FontConvert::down_patchSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字库补丁"),
        ".",
        tr("PATCH(*.patch)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Down_fontBinPatchSrc->setText(filename);
        progressBar_Down_fontPatchSync->setValue(0);
    }
}

void FontConvert::down_doPatch()
{
    if(lineEdit_Down_fontBinPatchSrc->text().isEmpty())
        return;
    down_fontBinDownCheckPatch(2);
}

/*********************************************************************************/
void FontConvert::flash_comInit(){
    //refer to qextserialbase enum
    QStringList protlist;
    QStringList baudratelist;
    QStringList databitslist;
    QStringList stopbitslist;
    QStringList paritylist;
    QStringList flowcontrollist;

    protlist<<"/dev/ttyUSB0"<<"/dev/ttyUSB1"<<"/dev/ttyUSB2"<<"/dev/ttyUSB3"<<"/dev/ttyUSB4"<<"/dev/ttyUSB5"\
              <<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyS2"<<"/dev/ttyS3"<<"/dev/ttyS4"<<"/dev/ttyS5"<<"/dev/ttyS6";

    baudratelist<<"50"<<"75"<<"110"<<"134"<<"150"<<"200"<<"300"<<"600"<<"1200"<<"1800"\
               <<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"<<"56000"<<"57600"<<"76800"<<"115200"\
                <<"128000"<<"256000";

    databitslist<<"5"<<"6"<<"7"<<"8";

    stopbitslist<<"1"<<"1.5"<<"2";

    paritylist<<"None"<<"Odd"<<"Even";

    flowcontrollist<<"OFF"<<"HardWare"<<"Xon/Xoff";

    comboBox_Flash_comPort->addItems(protlist);
    comboBox_Flash_comPort->setCurrentIndex(0);

    comboBox_Flash_comBaudrate->addItems(baudratelist);
    comboBox_Flash_comBaudrate->setCurrentIndex(19);

    comboBox_Flash_comDatabits->addItems(databitslist);
    comboBox_Flash_comDatabits->setCurrentIndex(3);

    comboBox_Flash_comStopbits->addItems(stopbitslist);
    comboBox_Flash_comStopbits->setCurrentIndex(0);

    comboBox_Flash_comParity->addItems(paritylist);
    comboBox_Flash_comParity->setCurrentIndex(2);

    comboBox_Flash_comFlowcontrol->addItems(flowcontrollist);
    comboBox_Flash_comFlowcontrol->setCurrentIndex(0);

    pushButton_Flash_blMode->setEnabled(0);
    pushButton_Flash_firmwareDown->setEnabled(0);
    pushButton_Flash_blCmdSend->setEnabled(0);
}

void FontConvert::flash_blCmdSlt(int index){
    switch(index){
    case 0://Get
    case 1://GV
    case 2://GID
    case 9://GID
    case 10://GID
    case 11://GID
        label_Flash_blCmdArg1->hide();
        label_Flash_blCmdArg2->hide();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->hide();
        lineEdit_Flash_blCmdArg2->hide();
        lineEdit_Flash_blCmdArg3->hide();

        break;
    case 3://RM
        label_Flash_blCmdArg1->setText(QString("Address:"));
        label_Flash_blCmdArg2->setText(QString("N(Bytes-1):"));
        label_Flash_blCmdArg1->show();
        label_Flash_blCmdArg2->show();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->show();
        lineEdit_Flash_blCmdArg2->show();
        lineEdit_Flash_blCmdArg3->hide();
        break;
    case 4://GO
        label_Flash_blCmdArg1->setText(QString("Address:"));
        label_Flash_blCmdArg1->show();
        label_Flash_blCmdArg2->hide();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->show();
        lineEdit_Flash_blCmdArg2->hide();
        lineEdit_Flash_blCmdArg3->hide();
        break;
    case 5://WM
        label_Flash_blCmdArg1->setText(QString("Address:"));
        label_Flash_blCmdArg2->setText(QString("N(Bytes-1):"));
        label_Flash_blCmdArg3->setText(QString("Data:"));
        label_Flash_blCmdArg1->show();
        label_Flash_blCmdArg2->show();
        label_Flash_blCmdArg3->show();
        lineEdit_Flash_blCmdArg1->show();
        lineEdit_Flash_blCmdArg2->show();
        lineEdit_Flash_blCmdArg3->show();
        break;
    case 6://ER
    case 7://EE
        label_Flash_blCmdArg1->setText(QString("N(Nums-1)"));
        label_Flash_blCmdArg2->setText(QString("Pages:"));
        label_Flash_blCmdArg1->show();
        label_Flash_blCmdArg2->show();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->show();
        lineEdit_Flash_blCmdArg2->show();
        lineEdit_Flash_blCmdArg3->hide();
        break;
    case 8:
        label_Flash_blCmdArg1->setText(QString("N(Nums-1):"));
        label_Flash_blCmdArg2->setText(QString("Sectors:"));
        label_Flash_blCmdArg1->show();
        label_Flash_blCmdArg2->show();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->show();
        lineEdit_Flash_blCmdArg2->show();
        lineEdit_Flash_blCmdArg3->hide();
        break;
    default:
        label_Flash_blCmdArg1->hide();
        label_Flash_blCmdArg2->hide();
        label_Flash_blCmdArg3->hide();
        lineEdit_Flash_blCmdArg1->hide();
        lineEdit_Flash_blCmdArg2->hide();
        lineEdit_Flash_blCmdArg3->hide();

        break;
    }
}

void FontConvert::flash_comOpen(){
    comboBox_Flash_comOpen->setEnabled(0);
    if(!blCmd.get_com_status()){
        //get com setting
        flash_portSetting.BaudRate = (BaudRateType)(comboBox_Flash_comBaudrate->currentIndex());
        flash_portSetting.DataBits = (DataBitsType)(comboBox_Flash_comDatabits->currentIndex());
        flash_portSetting.StopBits = (StopBitsType)(comboBox_Flash_comStopbits->currentIndex());
        flash_portSetting.Parity = (ParityType)(comboBox_Flash_comParity->currentIndex());
        flash_portSetting.FlowControl = (FlowType)(comboBox_Flash_comFlowcontrol->currentIndex());
        flash_portSetting.Timeout_Millisec = 50;
        //get port
        QString port = comboBox_Flash_comPort->currentText();
        if(!blCmd.ComOpen(port,flash_portSetting))
        {
            connect(&(blCmd),SIGNAL(myprint_dump(QString,QByteArray)),this,SLOT(flash_logDump(QString,QByteArray)));
            connect(&(blCmd),SIGNAL(myprint_message(QString)),this,SLOT(flash_log(QString)));
            comboBox_Flash_comOpen->setText(QString("关闭串口"));
            pushButton_Flash_blMode->setEnabled(1);
            pushButton_Flash_blCmdSend->setEnabled(1);
            pushButton_Flash_firmwareDown->setEnabled(1);
            bool isenable = blCmd.get_log_transdata_isEnable();
            pushButton_Flash_dataLog->setText(isenable?QString("隐藏收发数据"):QString("显示收发数据"));
        }
        else{
            messageBox(QString::fromUtf8("打开串口失败!"));
            return;
        }
    }
    else{
        blCmd.ComClose();
        disconnect(&(blCmd),SIGNAL(myprint_dump(QString,QByteArray)),this,SLOT(flash_logDump(QString,QByteArray)));
        disconnect(&(blCmd),SIGNAL(myprint_message(QString)),this,SLOT(flash_log(QString)));
        disconnect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareDownPercent(qint64)));
        disconnect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareExportPercent(qint64)));
        comboBox_Flash_comOpen->setText(QString("打开串口"));
        pushButton_Flash_blMode->setEnabled(0);
        pushButton_Flash_firmwareDown->setEnabled(0);
        pushButton_Flash_blCmdSend->setEnabled(0);

    }
    sleep(1);
    comboBox_Flash_comOpen->setEnabled(1);
}

void FontConvert::flash_blMode(){
    if(0>blCmd.Command_Bootloader())
    {
        messageBox(QString::fromUtf8("进入fastboot模式失败!"));
        return;
    }
}

void FontConvert::flash_logDump(QString tile,QByteArray data){
    textBrowser_Flash_log->insertPlainText(tile);
    QByteArray temp;
    for(int i=0;i<data.count();i++){
        temp.append(hex2Ascii((data.at(i)>>4) & 0x0f));
        temp.append(hex2Ascii(data.at(i) & 0x0f));
        temp.append(' ');
    }
    temp.append('\n');
    textBrowser_Flash_log->insertPlainText(temp);
}

void FontConvert::flash_log(QString text){
    textBrowser_Flash_log->insertPlainText(text);
}

void FontConvert::flash_logClear(){
    textBrowser_Flash_log->clear();
}

void FontConvert::flash_logMoveEnd(){
    textBrowser_Flash_log->moveCursor(QTextCursor::End);
}

void FontConvert::flash_blCmdSend(){
    if(!blCmd.get_com_status()){
        messageBox(QString::fromUtf8("请先打开串口!"));
        return;
    }
    int _4bytes;
    char _1byte;

    QByteArray temp;
    char out[256];
    char in[256];
    memset(in,0xff,256);
    bool ok;
    switch(comboBox_Flash_blCmd->currentIndex()){
    case 0://Get
        blCmd.Command_Get(out);
        break;
    case 1://GV
        blCmd.Command_Get_Version_Protection(out);
        break;
    case 2://GID
        blCmd.Command_Get_ID(out);
        break;
    case 3://RM
        if(lineEdit_Flash_blCmdArg1->text().isEmpty()||lineEdit_Flash_blCmdArg2->text().isEmpty())
            break;
        _4bytes = lineEdit_Flash_blCmdArg1->text().toInt(&ok,16);
        _1byte = lineEdit_Flash_blCmdArg2->text().toInt(&ok,16);
        blCmd.Command_Read_Memory(_4bytes,_1byte,out);
        break;
    case 4://GO
        if(lineEdit_Flash_blCmdArg1->text().isEmpty())
            break;
        _4bytes = lineEdit_Flash_blCmdArg1->text().toInt(&ok,16);
        blCmd.Command_Go(_4bytes);
        break;
    case 5://WM
        if(lineEdit_Flash_blCmdArg1->text().isEmpty()||lineEdit_Flash_blCmdArg2->text().isEmpty()||lineEdit_Flash_blCmdArg3->text().isEmpty())
            break;
        _4bytes = lineEdit_Flash_blCmdArg1->text().toInt(&ok,16);
        _1byte = lineEdit_Flash_blCmdArg2->text().toInt(&ok,16);
        temp = lineEdit_Flash_blCmdArg3->text().toAscii();
        qDebug("text to ascii num %d \n",temp.count());
        formatHex(temp);
        if(temp.count()%2){
            messageBox(QString::fromUtf8(" 请输入完整字节(偶数倍)!"));
            return;
        }
        for(int i = 0; i < temp.count() ; i+=2 ){
            in[i/2] = ((ascii2Hex(temp.at(i)) << 4) & 0xf0) + (ascii2Hex(temp.at(i + 1)) & 0x0f);
        }
        blCmd.Command_Write_Memory(_4bytes,_1byte,in);
        break;
    case 6://ER
        if(lineEdit_Flash_blCmdArg1->text().isEmpty())
            break;
        _1byte = lineEdit_Flash_blCmdArg1->text().toInt(&ok,16);
        if(!lineEdit_Flash_blCmdArg2->text().isEmpty()){
            temp = lineEdit_Flash_blCmdArg2->text().toAscii();
            qDebug("text to ascii num %d \n",temp.count());
            formatHex(temp);
            if(temp.count()%2){
                messageBox(QString::fromUtf8(" 请输入完整字节(偶数倍)!"));
                return;
            }
            for(int i = 0; i < temp.count() ; i+=2 ){
                in[i/2] = ((ascii2Hex(temp.at(i)) << 4) & 0xf0) + (ascii2Hex(temp.at(i + 1)) & 0x0f);
            }
        }
        blCmd.Command_Erase(_1byte,in);
        break;
    case 7://EE
        if(lineEdit_Flash_blCmdArg1->text().isEmpty()||lineEdit_Flash_blCmdArg2->text().isEmpty())
            break;

        break;
    case 8://WP
        if(lineEdit_Flash_blCmdArg1->text().isEmpty()||lineEdit_Flash_blCmdArg2->text().isEmpty())
            break;
        _1byte = lineEdit_Flash_blCmdArg1->text().toInt(&ok,16);
        temp = lineEdit_Flash_blCmdArg2->text().toAscii();
        qDebug("text to ascii num %d \n",temp.count());
        formatHex(temp);
        if(temp.count()%2){
            messageBox(QString::fromUtf8(" 请输入完整字节(偶数倍)!"));
            return;
        }
        for(int i = 0; i < temp.count() ; i+=2 ){
            in[i/2] = ((ascii2Hex(temp.at(i)) << 4) & 0xf0) + (ascii2Hex(temp.at(i + 1)) & 0x0f);
        }
        blCmd.Command_Write_Protect(_1byte,in);
        break;
    case 9://WUP
        blCmd.Command_Write_Unprotect();
        break;
    case 10://RP
        blCmd.Command_Readout_Protect();
        break;
    case 11://RUP
        blCmd.Command_Readout_Unprotect();
        break;

    }
    //blCmd.ComClose();
}

void FontConvert::flash_firmwareSrcBrs(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字体"),
        ".",
        tr("HEX (*.hex)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Flash_firmwareSrc->setText(filename);
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)){
            messageBox(QString::fromUtf8("打开文件失败!"));
            return;
        }
        qint64 size = file.size();
        label_Flash_firmwareSize->setText(QString::number(size/1024)+QString('K'));
    }
}

void FontConvert::flash_firmwareDownPercent(qint64 percent){
    progressBar_Flash_firmwareDown->setValue(percent);
}

void FontConvert::flash_logDataShow(){
    bool isenable = blCmd.get_log_transdata_isEnable();
    pushButton_Flash_dataLog->setText(!isenable?QString("隐藏收发数据"):QString("显示收发数据"));
    blCmd.log_transdata_setEnable(!isenable);
}

void FontConvert::flash_firmwareDown(){
    if(!blCmd.get_com_status()){
        messageBox(QString::fromUtf8("请先打开串口!"));
        return;
    }
    if(blCmd.isRunning()){
        messageBox(QString::fromUtf8("任务正在运行!"));
        return;
    }
    /*
    if(lineEdit_Flash_binDownAddr->text().isEmpty()){
        qDebug("no flash address\n");
        return;
    }
    bool ok;
    int address = lineEdit_Flash_binDownAddr->text().toInt(&ok,16);
    */
    QString filename = lineEdit_Flash_firmwareSrc->text();
    blCmd.set_down_mode();
    disconnect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareExportPercent(qint64)));
    connect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareDownPercent(qint64)));
    blCmd.set_down_args(filename);
    blCmd.start();
}

void FontConvert::flash_firmwareExport(){
    if(!blCmd.get_com_status()){
        messageBox(QString::fromUtf8("请先打开串口!"));
        return;
    }
    if(blCmd.isRunning()){
        messageBox(QString::fromUtf8("任务正在运行!"));
        return;
    }
    if(lineEdit_Flash_exportName->text().isEmpty()){
        messageBox(QString::fromUtf8("请输入导出文件名!"));
        return;
    }
    if(lineEdit_Flash_exportAddr->text().isEmpty()){
        messageBox(QString::fromUtf8("请输入读取地址!"));
        return;
    }
    if(lineEdit_Flash_exportSize->text().isEmpty()){
        messageBox(QString::fromUtf8("请输入读取大小!"));
        return;
    }
    QDir dir;
    QString filename = dir.currentPath() + QString("/")+lineEdit_Flash_exportName->text();
    bool ok;
    int address = lineEdit_Flash_exportAddr->text().toInt(&ok,16);
    int size = lineEdit_Flash_exportSize->text().toInt(&ok,16);
    blCmd.set_read_mode();
    disconnect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareExportPercent(qint64)));
    connect(&(blCmd),SIGNAL(mypercent(qint64)),this,SLOT(flash_firmwareExportPercent(qint64)));
    blCmd.set_down_args(filename,address,size);
    blCmd.start();
}

void FontConvert::flash_firmwareExportPercent(qint64 percent){
    progressBar_Flash_firmwareExport->setValue(percent);
}

void FontConvert::merge_masterFontBinSrcBrs()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字体"),
        ".",
        tr("bin (*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Merge_masterFontBinSrc->setText(filename);

    }
}
void FontConvert::merge_slaveFontBinSrcBrs()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择字体"),
        ".",
        tr("bin (*.bin)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        lineEdit_Merge_slaveFontBinSrc->setText(filename);
    }
}

void FontConvert::merge_masterFontBinLoad()
{
    QString filename;
    int size;
    filename = lineEdit_Merge_masterFontBinSrc->text();
    if(filename.isEmpty())
        return;
    char* image = newFileImage(filename,size);
    if(!image)
        return;
    fontBinStructLoad(image,merge_masterCharsets,
                      lineEdit_Merge_masterFontBinVer,lineEdit_Merge_masterFontBinDesc,
                      lineEdit_Merge_masterFontBinOffset,lineEdit_Merge_masterFontBinMd5);
    freeFileImage(image);
    fontBinStructShow(tableWidget_Merge_masterFontBinStruct,merge_masterCharsets);
    merge_masterFontBinStructOpsSetEnable(1);
}

void FontConvert::merge_slaveFontBinLoad()
{
    QString filename;
    int size;
    filename = lineEdit_Merge_slaveFontBinSrc->text();
    if(filename.isEmpty())
        return;
    char* image = newFileImage(filename,size);
    if(!image)
        return;
    fontBinStructLoad(image,merge_slaveCharsets,
                      lineEdit_Merge_slaveFontBinVer,lineEdit_Merge_slaveFontBinDesc,
                      lineEdit_Merge_slaveFontBinOffset,lineEdit_Merge_slaveFontBinMd5);
    freeFileImage(image);
    fontBinStructShow(tableWidget_Merge_slaveFontBinStruct,merge_slaveCharsets);
}

void FontConvert::charset_reAddress(QList<CharSet_Unit>& charsets)
{
    int align = 1;
    int next_addr_start,addr_end;
    int unit,nums;
    QList<CharSet_Unit>::iterator iter = charsets.begin();
    for(next_addr_start = 0; iter != charsets.end(); iter++){
        unit = iter->charset_hdr.unit;
        nums = iter->charset_hdr.nums;
        addr_end = next_addr_start + unit*nums;
        iter->charset_hdr.addr_start = next_addr_start;
        iter->charset_hdr.addr_end = addr_end;
        next_addr_start = ROUND(addr_end,align);
    }
}

void FontConvert::merge_masterFontBinStructMoveUp()
{
    int cur_index = tableWidget_Merge_masterFontBinStruct->currentRow();
    if(cur_index){
        merge_masterCharsets.swap(cur_index,cur_index - 1);
        charset_reAddress(merge_masterCharsets);
        fontBinStructShow(tableWidget_Merge_masterFontBinStruct,merge_masterCharsets);
        tableWidget_Merge_masterFontBinStruct->selectRow(cur_index - 1);
    }
}

void FontConvert::merge_masterFontBinStructMoveDown()
{
    int cur_index = tableWidget_Merge_masterFontBinStruct->currentRow();
    if(cur_index != (merge_masterCharsets.count()-1)){
        merge_masterCharsets.swap(cur_index,cur_index + 1);
        charset_reAddress(merge_masterCharsets);
        fontBinStructShow(tableWidget_Merge_masterFontBinStruct,merge_masterCharsets);
        tableWidget_Merge_masterFontBinStruct->selectRow(cur_index + 1);
    }
}

void FontConvert::merge_masterFontBinStructAddFromSlave()
{
    CharSet_Unit merge_charset;
    QList<QTableWidgetSelectionRange>ranges = tableWidget_Merge_slaveFontBinStruct->selectedRanges();
    int count = ranges.count();
    for(int i = 0; i < count; i++){
        int toprow = ranges.at(i).topRow();
        int bottomrow = ranges.at(i).bottomRow();
        for(int j = toprow; j <= bottomrow; j++) {
            memcpy(&(merge_charset.charset_hdr), &(merge_slaveCharsets.at(j).charset_hdr), sizeof(Header_Charset));
            merge_charset.bitmap = merge_slaveCharsets.at(j).bitmap;
            merge_masterCharsets.append(merge_charset);
        }
    }
    charset_reAddress(merge_masterCharsets);
    merge_masterFontBinStructOpsSetEnable(1);
    fontBinStructShow(tableWidget_Merge_masterFontBinStruct,merge_masterCharsets);
}

void FontConvert::merge_masterFontBinStructOpsSetEnable(bool value)
{
    pushButton_Merge_masterFontBinStructMoveUp->setEnabled(value);
    pushButton_Merge_masterFontBinStructMoveDown->setEnabled(value);
    pushButton_Merge_masterFontBinStructDel->setEnabled(value);
    pushButton_Merge_masterFontBinStructSave->setEnabled(value);
}

void FontConvert::merge_masterFontBinStructDel()
{
    if(!merge_masterCharsets.empty()){
        QList<QTableWidgetSelectionRange>ranges = tableWidget_Merge_masterFontBinStruct->selectedRanges();
        int count = ranges.count();
        for(int i = 0; i < count; i++){
            int toprow = ranges.at(i).topRow();
            int bottomrow = ranges.at(i).bottomRow();
            for(int j = bottomrow; j >= toprow; j--) {
                qDebug("currentRow %d, merge_masterCharsets count %d",j,merge_masterCharsets.count());
                if(j>(merge_masterCharsets.count()-1))
                    return;
                merge_masterCharsets.removeAt(j);
                tableWidget_Merge_masterFontBinStruct->selectRow(j-1);
            }
        }
        if(!merge_masterCharsets.empty()){
            charset_reAddress(merge_masterCharsets);
            fontBinStructShow(tableWidget_Merge_masterFontBinStruct,merge_masterCharsets);
        }
        else{
           tableWidget_Merge_masterFontBinStruct->clear();
           merge_masterFontBinStructOpsSetEnable(0);
           return;
        }
    }
}

void FontConvert::merge_masterFontBinSave()
{
    if(merge_masterCharsets.isEmpty()){
        qDebug("空数据!");
        return;
    }
    QString filename = lineEdit_Merge_masterFontBinSrc->text();
    int pos = filename.lastIndexOf("_Md5_");
    pos = (pos==-1) ? filename.count() : pos;
    QString newfilename = filename.left(pos);
    bool ok;
    int bin_version = lineEdit_Merge_masterFontBinVer->text().toULong(&ok,16);
    QString describe = lineEdit_Merge_masterFontBinDesc->text();
    if(describe.count()>64)
    {
       describe = describe.left(64);
       lineEdit_Merge_masterFontBinDesc->setText(describe);
    }

    int i;
    Fontlib_Header ft_hdr;
    memset(&ft_hdr, 0 ,sizeof(Fontlib_Header));

    memcpy(ft_hdr.magic, "WiZaRpOs",8);
    ft_hdr.version = bin_version;
    memcpy(ft_hdr.describe, describe.toAscii().data(),64);

    size_t ss= sizeof(Fontlib_Header) + sizeof(Header_Charset) * (merge_masterCharsets.count()+1);
    ft_hdr.offset = ROUND(ss,0x1000);
    ft_hdr.size = merge_masterCharsets.last().charset_hdr.addr_end + ft_hdr.offset;

    unsigned char *bin = new unsigned char[ft_hdr.size];
    if (bin == NULL){
        qDebug("fail new bin");
        return;
    }
    memset(bin,0,ft_hdr.size);
    //memcpy(bin,c_hdr,sizeof(Fontlib_Header));
    unsigned char *bin_index = bin + sizeof(Fontlib_Header);
    unsigned char *bin_bitmap;
    unsigned long bytes;
    char checksum = 0;
    QList<CharSet_Unit>::iterator iter = merge_masterCharsets.begin();
    for(; iter != merge_masterCharsets.end(); iter++){

        memcpy(bin_index, &(iter->charset_hdr), sizeof(Header_Charset));
        for(i=0;i<(int)(sizeof(Header_Charset));i++)
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
    //copy header
    memcpy(bin,c_hdr,sizeof(Fontlib_Header));
    //save
    lineEdit_Merge_masterFontBinMd5->setText(QString(bb.toHex()));
    newfilename += (QString("_Md5_") + QString(bb.toHex()).left(4) + QString(".bin"));
    QFile file(newfilename);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug("打开文件失败!");
        delete bin;
        return;
    }
    QDataStream out(&file);
    out.writeRawData((const char*)bin,ft_hdr.size);
    file.close();
    delete bin;
    lineEdit_Merge_masterFontBinSrc->setText(newfilename);

}

