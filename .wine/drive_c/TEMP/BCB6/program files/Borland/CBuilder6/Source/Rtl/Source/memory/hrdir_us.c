/*-----------------------------------------------------------------------*
 * filename - hrdir_us.c
 *
 * function(s)
 *
 *     __CRTL_MEM_UseBorMM
 *
 *
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1998, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.6.2.1 $        */

#define __CRTL_MEM_FORCE_NAKED
#include <_hrdir.h>

/* This function is a stub and does nothing but satisfy the external call
 * from the startup code.  The user can link in USEBORMM.LIB which also
 * satisfies this call, and then forces in the borlndmm.dll
 */

void _RTLENTRY _EXPFUNC __declspec(naked) __CRTL_MEM_UseBorMM (void)
{
    __emit__(0xC3); /* asm ret */
}
