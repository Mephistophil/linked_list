/*-----------------------------------------------------------------------*
 * filename - mbsicmp.c
 *
 * function(s)
 *        _mbsicmp - Compares one string to another.
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1992, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

#include <ctype.h>
#include <mbctype.h>
#include <mbstring.h>

/*---------------------------------------------------------------------*

Name            _mbsicmp - Compares one string to another.

Usage           int _mbsicmp(const unsigned char *str1,
                             const unsigned char *str2);

Prototype in    mbstring.h

Description     Returns a value(<0, 0 or >0) based on the result of
                comparing str1(or part of it) to str2(or oart of it).

                The comparison is not case sensitive.

Return value    _mbsicmp returns a int value as following;

                        < 0  str1 is less than str2
                        = 0  str1 is the same as str2
                        > 0  str1 is the same as str2

                The relation for conparison is,

                ASCII < 1 byte Kana < 2 bytes character

                If the second byte of 2-bytes character is null,
                that character is regarded as null.

*---------------------------------------------------------------------*/

#if defined(__FARFUNCS__)
#include <_farfunc.h>
#include <_mbfarfn.h>
#endif

int _RTLENTRY _EXPFUNC _mbsicmp(const unsigned char *s1, const unsigned char *s2)
{
    unsigned int c1, c2;

    do {
        c1 = *s1++;
        if (_ismbblead(c1))
        {
            if (*s1 == '\0')
                c1 = 0;
            else
                c1 = (c1 << 8) | *s1++;
        }
        else
            c1 = _ltoupper(c1);
        c2 = *s2++;
        if (_ismbblead(c2))
        {
            if (*s2 == '\0')
                c2 = 0;
            else
                c2 = (c2 << 8) | *s2++;
        }
        else
            c2 = _ltoupper(c2);
        if (c1 != c2)
            return (c1 < c2) ? -1 : 1;
    } while (c1 != 0) ;
    return 0;
}
