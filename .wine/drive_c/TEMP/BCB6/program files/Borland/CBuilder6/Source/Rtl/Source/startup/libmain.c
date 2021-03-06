/*-----------------------------------------------------------------------*
 * filename - LibMain.c
 *
 * function(s)
 *      DllEntryPoint     - default DLL user initialization
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1992, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.4 $ */

#include <ntbc.h>

/*---------------------------------------------------------------------*

Name            LibMain - default DLL user initialization

Usage           BOOL  WINAPI DllEntryPoint(HINSTANCE hInst,
                           DWORD ul_reason_being_called,
                           LPVOID lpReserved)

Prototype in    none

Description     DllEntryPoint is called by the startup code when a DLL is
                is initialized, after all other RTL initialization
                is performed.  This is the default version, which
                is linked if the user does not provide one.

Return value    If it is successful, DllEntryPoint returns 1; if an error
                occurs, it returns 0.

*---------------------------------------------------------------------*/

#pragma argsused

BOOL  WINAPI DllMain(HINSTANCE hInst, DWORD ul_reason_being_called, LPVOID lpReserved);

BOOL  WINAPI DllEntryPoint(HINSTANCE hInst,
                           DWORD ul_reason_being_called,
                           LPVOID lpReserved)
{
    return DllMain(hInst, ul_reason_being_called, lpReserved);
}

