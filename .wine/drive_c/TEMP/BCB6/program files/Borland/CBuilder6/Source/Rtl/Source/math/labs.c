/*-----------------------------------------------------------------------*
 * filename - labs.c
 *
 * function(s)
 *        labs - gives long absolute value
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1987, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.3 $        */

#include <stdlib.h>

/*---------------------------------------------------------------------*

Name            labs - gives long absolute value

Usage           long labs(long n);

Prototype in    stdlib.h

Description     calculates the absolute value of n, a long integer.

Return value    the absolute value of n.

*---------------------------------------------------------------------*/

long _RTLENTRY _EXPFUNC labs(long n)
{
    return(n < 0 ? -n : n);
}
