#include "binfile.h"
#include "bitmap.h"
#include <QList>



void bitmap_2_binfile(QList<Stand_BitMap> bl, QList<Charset_Data> pl, int block_align){
   QList<Stand_BitMap>::iterator iter = bl.begin();
   for(; iter != bl.end(); iter++){
       iter->mode = pl->;


   }




}



