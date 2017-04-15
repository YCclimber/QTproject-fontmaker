#ifndef FREETYPE_EX_H
#define FREETYPE_EX_H


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

FT_Error Old_FT_Outline_Embolden( FT_Outline*  outline, FT_Pos strength )
{
    FT_Vector*    points;
    FT_Vector    v_prev, v_first, v_next, v_cur;
    FT_Angle    rotate, angle_in, angle_out;
    FT_Int        c, n, first;
    FT_Int        orientation;

    if ( !outline )
        return FT_Err_Invalid_Argument;

    strength /= 2;
    if ( strength == 0 )
        return FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    if ( orientation == FT_ORIENTATION_NONE )
    {
        if ( outline->n_contours )
            return FT_Err_Invalid_Argument;
        else
            return FT_Err_Ok;
    }

    if ( orientation == FT_ORIENTATION_TRUETYPE )
        rotate = -FT_ANGLE_PI2;
    else
        rotate = FT_ANGLE_PI2;

    points = outline->points;

    first = 0;
    for ( c = 0; c < outline->n_contours; c++ )
    {
        int  last = outline->contours[c];

        v_first = points[first];
        v_prev  = points[last];
        v_cur   = v_first;

        for ( n = first; n <= last; n++ )
        {
            FT_Vector    in, out;
            FT_Angle    angle_diff;
            FT_Pos        d;
            FT_Fixed    scale;

            if ( n < last )
                v_next = points[n + 1];
            else
                v_next = v_first;

            /* compute the in and out vectors */
            in.x = v_cur.x - v_prev.x;
            in.y = v_cur.y - v_prev.y;

            out.x = v_next.x - v_cur.x;
            out.y = v_next.y - v_cur.y;

            angle_in   = FT_Atan2( in.x, in.y );
            angle_out  = FT_Atan2( out.x, out.y );
            angle_diff = FT_Angle_Diff( angle_in, angle_out );
            scale      = FT_Cos( angle_diff / 2 );

            if ( scale < 0x4000L && scale > -0x4000L )
                in.x = in.y = 0;
            else
            {
                d = FT_DivFix( strength, scale );

                FT_Vector_From_Polar( &in, d, angle_in + angle_diff / 2 - rotate );
            }

            outline->points[n].x = v_cur.x + strength + in.x;

            //outline->points[n].y = v_cur.y + strength + in.y;

            v_prev = v_cur;
            v_cur  = v_next;
        }

        first = last + 1;
    }

    return FT_Err_Ok;
}

// 垂直加粗
FT_Error Vert_FT_Outline_Embolden( FT_Outline*  outline, FT_Pos strength )
{
    FT_Vector*    points;
    FT_Vector    v_prev, v_first, v_next, v_cur;
    FT_Angle    rotate, angle_in, angle_out;
    FT_Int        c, n, first;
    FT_Int        orientation;

    if ( !outline )
        return FT_Err_Invalid_Argument;

    strength /= 2;
    if ( strength == 0 )
        return FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    if ( orientation == FT_ORIENTATION_NONE )
    {
        if ( outline->n_contours )
            return FT_Err_Invalid_Argument;
        else
            return FT_Err_Ok;
    }

    if ( orientation == FT_ORIENTATION_TRUETYPE )
        rotate = -FT_ANGLE_PI2;
    else
        rotate = FT_ANGLE_PI2;

    points = outline->points;

    first = 0;
    for ( c = 0; c < outline->n_contours; c++ )
    {
        int  last = outline->contours[c];

        v_first = points[first];
        v_prev  = points[last];
        v_cur   = v_first;

        for ( n = first; n <= last; n++ )
        {
            FT_Vector    in, out;
            FT_Angle    angle_diff;
            FT_Pos        d;
            FT_Fixed    scale;

            if ( n < last )
                v_next = points[n + 1];
            else
                v_next = v_first;

            /* compute the in and out vectors */
            in.x = v_cur.x - v_prev.x;
            in.y = v_cur.y - v_prev.y;

            out.x = v_next.x - v_cur.x;
            out.y = v_next.y - v_cur.y;

            angle_in   = FT_Atan2( in.x, in.y );
            angle_out  = FT_Atan2( out.x, out.y );
            angle_diff = FT_Angle_Diff( angle_in, angle_out );
            scale      = FT_Cos( angle_diff / 2 );

            if ( scale < 0x4000L && scale > -0x4000L )
                in.x = in.y = 0;
            else
            {
                d = FT_DivFix( strength, scale );

                FT_Vector_From_Polar( &in, d, angle_in + angle_diff / 2 - rotate );
            }

            //outline->points[n].x = v_cur.x + strength + in.x;

            outline->points[n].y = v_cur.y + strength + in.y;

            v_prev = v_cur;
            v_cur  = v_next;
        }

        first = last + 1;
    }

    return FT_Err_Ok;
}

// 新的加粗函数
FT_Error New_FT_Outline_Embolden( FT_Outline*  outline, FT_Pos str_h, FT_Pos str_v )
{
    if ( !outline ) return FT_Err_Invalid_Argument;
    int orientation = FT_Outline_Get_Orientation( outline );
    if ( orientation == FT_ORIENTATION_NONE )
        if ( outline->n_contours ) return FT_Err_Invalid_Argument;
    Vert_FT_Outline_Embolden( outline, str_v );
    Old_FT_Outline_Embolden( outline, str_h );
    return FT_Err_Ok;
}
#if 0
// 让一个字体槽加粗，并且填充其他的大小属性
void New_GlyphSlot_Embolden( FT_GlyphSlot  slot , const Vector2Float &embolden)
{
    if(embolden == Vector2Float::ZERO)
        return;
    FT_Library  library = slot->library;
    FT_Face     face    = slot->face;
    FT_Error    error;
    FT_Pos      xstr = embolden.x, ystr = embolden.y;


    if ( slot->format != FT_GLYPH_FORMAT_OUTLINE &&
        slot->format != FT_GLYPH_FORMAT_BITMAP )
        return;

    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
        FT_BBox oldBox;
        FT_Outline_Get_CBox(&slot->outline , &oldBox);
        error = New_FT_Outline_Embolden( &slot->outline, xstr , ystr);
        if ( error )
            return;

        FT_BBox newBox;
        FT_Outline_Get_CBox(&slot->outline , &newBox);
        xstr = (newBox.xMax - newBox.xMin) - (oldBox.xMax - oldBox.xMin);
        ystr = (newBox.yMax - newBox.yMin) - (oldBox.yMax - oldBox.yMin);
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
        xstr = FT_PIX_FLOOR( xstr );
        if ( xstr == 0 )
            xstr = 1 << 6;
        ystr = FT_PIX_FLOOR( ystr );

        error = FT_Bitmap_Embolden( library, &slot->bitmap, xstr, ystr );
        if ( error )
            return;
    }

    if ( slot->advance.x )
        slot->advance.x += xstr;

    if ( slot->advance.y )
        slot->advance.y += ystr;

    slot->metrics.width        += xstr;
    slot->metrics.height       += ystr;
    slot->metrics.horiBearingY += ystr;
    slot->metrics.horiAdvance  += xstr;
    slot->metrics.vertBearingX -= xstr / 2;
    slot->metrics.vertBearingY += ystr;
    slot->metrics.vertAdvance  += ystr;

    if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += ystr >> 6;
}
#endif

#endif // FREETYPE_EX_H
