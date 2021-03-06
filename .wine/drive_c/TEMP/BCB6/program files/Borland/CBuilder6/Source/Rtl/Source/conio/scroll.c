/*-----------------------------------------------------------------------*
 * filename - scroll.c
 *
 * function(s)
 *        __scroll - scrolls the specified window in text mode
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1991, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.5.2.1 $        */

#define INCL_CON
#include <ntbc.h>

#include <_video.h>
#include <conio.h>

/*---------------------------------------------------------------------*

Name            __scroll - scrolls the specified window in text mode

Usage           void __scroll(uchar dir, uchar x1, uchar y1,
                       uchar x2, uchar y2, uchar lines);

Prototype in    _video.h

Return value    None

*---------------------------------------------------------------------*/

void _RTLENTRY _EXPFUNC __scroll(uchar dir, uchar x1, uchar y1,
                   uchar x2,  uchar y2, uchar lines)
{
    CHAR_INFO   fill;
    SMALL_RECT  r;      /* scroll rectangle */
    COORD       c;      /* destination coordinates */

    fill.Char.AsciiChar = ' ';     /* Fill with blanks */
    fill.Attributes = _video.attribute;

    /* If lines is zero, scroll the entire rectangle.
     */
    if (lines == 0)
        lines = y2 - y1 + 1;

    /* The scroll rectangle is the same as the clipping rectangle.
     */
    r.Left = x1;
    r.Top  = y1;
    r.Right = x2;
    r.Bottom = y2;

    /* Define the destination location as the scroll origin
     * plus (DOWN) or minus (UP) the the number of lines to scroll.
     */
    c.X = x1;
    if (dir == UP)
        c.Y = y1 - lines;
    else
        c.Y = y1 + lines;

    ScrollConsoleScreenBuffer(_hout, &r, &r, c, &fill);
}
