#ifndef FONTCONVERT_H
#define FONTCONVERT_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QDir>

#include "ui_fontwidget.h"
#include "common.h"
#include "characterset.h"
#include "qextserialport.h"
#include "stm32flash.h"
#include "charset_convert.h"
#include "printerft.h"


class FontConvert : public QWidget,public Ui::Form
{
    Q_OBJECT

public:
    FontConvert(QWidget *parent = 0);

private slots:
    /*Preview*/
    void pre_ttfSrcBrs();
    void pre_setEnable(int);
    void pre_inPutSlt(int);
    void pre_setWeightWidthRange(QString);
    void pre_setWeightHeightRange(QString);
    void pre_doPreview();
    void pre_outPut(QString);
    /*Make*/
    void make_ttfSrcBrs();
    void make_charsetSlt(int);
    void make_setWeightWidthRange(QString);
    void make_setWeightHeightRange(QString);
    //user define
    void make_ruleSltShow(int,int);
    void make_usrDefShow();
    void make_ruleValueReset();
    void make_usrDefShowBytes(int);
    void make_addRule();
    //charset members
    void make_charsetMbrSlt(int index);
    void make_ruleSltDel();
    //convert
    void make_convDescSltShow(int,int);
    void make_convDescShow();
    void make_addConv();
    void make_delConv();
    void make_convMoveUp();
    void make_convMoveDown();
    void make_convStart();
    void make_convStop();
    void make_convDispIndex(int,int);
    void make_convDispNum(int,int);
    /*Trim*/
    void trim_fontBinSrcBrs();
    void trim_fontBinLoad();
    void trim_charsetSlt(int index);
    void trim_getPreChar();
    void trim_getNextChar();
    void trim_getChar();
    void trim_charClear(QString text);
    void trim_CharViewZoomIn();
    void trim_CharViewZoomOut();
    void trim_charViewPixelClick(int,int);
    void trim_charViewRecover();
    void trim_charSave();
    void trim_newFontBinSave();
    /*Down*/
    void down_fontBinSrcBrs();
    void down_fontBinDownProcShow(qint64);
    void down_log(QString);
    void down_logDump(QString,QByteArray);
    void down_fontBinDown();
    void down_fontBinCheck();
    void down_logClear();
    void down_logMoveEnd();
    //patch
    void down_patchFontBinSrcBrs();
    void down_patchFontBinDstBrs();
    void down_patchBuild();
    void down_patchSrcBrs();
    void down_patchDownProcShow(qint64);
    void down_doPatch();
    /*Flash*/
    void flash_comOpen();
    void flash_blMode();
    void flash_blCmdSlt(int index);
    void flash_blCmdSend();
    //print test log
    void flash_logDataShow();
    void flash_logDump(QString,QByteArray);
    void flash_log(QString);
    void flash_logClear();
    void flash_logMoveEnd();
    //down
    void flash_firmwareSrcBrs();
    void flash_firmwareDown();
    void flash_firmwareDownPercent(qint64);
    void flash_firmwareExport();
    void flash_firmwareExportPercent(qint64);
    /*Merge*/
    void merge_masterFontBinSrcBrs();
    void merge_slaveFontBinSrcBrs();
    void merge_masterFontBinLoad();
    void merge_slaveFontBinLoad();
    void merge_masterFontBinStructMoveUp();
    void merge_masterFontBinStructMoveDown();
    void merge_masterFontBinStructAddFromSlave();
    void merge_masterFontBinStructDel();
    void merge_masterFontBinSave();

private:
    void messageBox(QString);
    void my_drawbitmap_spare(QTableWidget*,int,int,QList<Draw_BitMap>&);
    void my_drawbitmap_extract(QTableWidget*,Trim_Char_Info*);
    void my_drawbitmap_modifySave(QTableWidget*,Trim_Char_Info*);
    char hex2Ascii(char);
    char ascii2Hex(char);
    void formatHex(QByteArray &);
    void blankPadding(QByteArray &);
    QString toDump(QByteArray);
    void fontBinStructLoad(char*,QList<CharSet_Unit>&,QLineEdit*,QLineEdit*,QLineEdit*,QLineEdit*);
    void fontBinStructShow(QTableWidget*,QList<CharSet_Unit>&);
    void charset_reAddress(QList<CharSet_Unit>&);
    char* newFileImage(QString,int&);
    void freeFileImage(char*);
    int patchHeaderCheck(char*);

    /*Preview*/
    void preview_Init();
    /*Make*/
    void make_Init();
    void make_charsetMbrInit();
    void make_usrDefInit();
    void make_charsetMbrShow();
    void make_ruleShow1Bytes();
    void make_ruleShow2Bytes();
    void make_ruleShow4Bytes();
    void make_convShow();
    void make_convOptSetEnable(bool);
    /*Trim*/
    void trim_Init();
    int  trim_getCharBitmap(Trim_Char_Info*);
    void trim_charShow();
    void trim_charDataShow(unsigned long,QByteArray);
    /*Down*/
    void down_comInit();
    void down_fontBinDownCheckPatch(int);
    /*Flash*/
    void flash_comInit();
    /*Merge*/
    void merge_masterFontBinStructOpsSetEnable(bool);

private:
    /*Preview*/
    Character_Set *pre_charset = NULL;
    /*Make*/
    QList<Convert> converts;
    Character_Set *make_charset = NULL;
    Charset_Convert *my_convThd = NULL;
    /*Trim*/
    Character_Set *trim_charset = NULL;
    Trim_Char_Info *new_trimChar = NULL;
    QList<Trim_Char_Info> trimChars;
    QList<CharSet_Unit> trim_charsets;
    Header_Charset trim_curCharsetHdr;
    /*flash*/
    PortSettings flash_portSetting;
    Bootloader_Commands blCmd;
    /*Down*/
    PrinterFont* my_fontThd = NULL;
    PortSettings down_portSetting;

    /*Merge*/
    QList<CharSet_Unit> merge_masterCharsets;
    QList<CharSet_Unit> merge_slaveCharsets;
};

#endif
