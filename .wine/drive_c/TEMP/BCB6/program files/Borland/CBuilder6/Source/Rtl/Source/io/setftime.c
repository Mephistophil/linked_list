/*-----------------------------------------------------------------------*
 * filename - setftime.c
 *
 * function(s)
 *        setftime - sets file date and time
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1987, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.3 $        */

#include <dos.h>
#include <io.h>

/*-----------------------------------------------------------------------*

Name            setftime - sets file date and time

Usage           #include <io.h>
                int setftime(int handle, struct ftime *ftimep);

Prototype in    io.h

Description     see getftime

*------------------------------------------------------------------------*/

int _RTLENTRY _EXPFUNC setftime(int handle, struct ftime *ftimep)
{
    short *p;

    /* WARNING: The following code makes assumptions about the
     * ordering of bit fields.  This may not work on non-Intel CPUs.
     * Note that p[0] is the time, and p[1] is the date.
     */
    p = (short *)ftimep;

    return (_dos_setftime(handle, p[1], p[0]) == 0 ? 0 : -1);
}
