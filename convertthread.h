#ifndef CONVERTTHREAD_H
#define CONVERTTHREAD_H

#include <QObject>
#include <QThread>
#include "characterset.h"
#include "common.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <QFile>

class convertthread : public QThread
{
    Q_OBJECT

public:
    convertthread(QList<Convert> &convertlist,qint64 maxsize);
    ~convertthread();

signals:
    void convertdone();
    void convertnum(int cur,int total);
    void convertcharnum(int cur,int total);

protected:
    void run();

public:
    void get_charsets(QList<struct Charset_Data> &out);

private:
    int do_convert_bat(struct Convert *convert_obj);

private:
    qint64 MAX_SIZE;
    FT_Library pFTLib;
    FT_Face pFTFace;
    FT_Error pFTError;
    QList<Convert> converts;
    QList<struct Stand_BitMap> bitmaps;
    QList<struct Charset_Data> charsets;
    class Character_Set *convert_character_set = NULL;
    char* new_memory_face=NULL;
};

#endif // CONVERTTHREAD_H
