/*------------------------------------------------------------------------
 * filename - insline.c
 *
 * function(s)
 *        insline  -  inserts a line of text
 *        delline  -  deletes a line of text
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1987, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.4.2.1 $        */

#include <_video.h>
#include <conio.h>

/*---------------------------------------------------------------------*

Name            insline  - inserts a line of text

Usage           void insline(void);

Prototype in    conio.h

Description     inserts a line of text at the current row position.

*---------------------------------------------------------------------*/

void _RTLENTRY _EXPFUNC insline(void)
{
    __scroll(DOWN, _video.windowx1, _wherey(),
                   _video.windowx2, _video.windowy2, 1);
}


/*---------------------------------------------------------------------*

Name            delline  - deletes a line of text

Usage           void delline(void);

Prototype in    conio.h

Description     deletes the current line of text.

*---------------------------------------------------------------------*/

void _RTLENTRY _EXPFUNC delline(void)
{
    __scroll(UP, _video.windowx1, _wherey(),
                 _video.windowx2, _video.windowy2, 1);
}
