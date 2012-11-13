/*-----------------------------------------------------------------------*
 * filename - need_vcl.c
 *
 *   Provides a symbol that is only in the CP32MTx libs and not in
 *   the CW32xxx libs.  Forces a linker error if the wrong libs are
 *   mixed together
 *
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1997, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.4.2.1 $        */

char __Must_Link_VCL_Apps_With_The_CP32MT_Or_CP32MTI_RTL;
