/*-----------------------------------------------------------------------*
 * filename - mbsnbcmp.c
 *
 * function(s)
 *        _mbsnbcmp - Compares a portrion of one string to a portion
 *                    of another.
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1992, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */


#include <mbctype.h>
#include <mbstring.h>

/*---------------------------------------------------------------------*

Name            _mbsnbcmp - Compares a portion of one string to a portion
                            of another.

Usage           int _mbsnbcmp(const unsigned char *str1,
                              const unsigned char *str2, size_t maxlen);

Prototype in    mbstring.h

Description     Compares str1 to str2, it starts with the first character
                in each string and continues with subsequent characters
                differ or until it has examined maxlen.

Return value    _mbsnbcmp returns an int value based on the result of
                comparing str1 to str2;

                        < 0  if str1 is less than str2
                        = 0  if str1 is the same as str2
                        > 0  if str1 is greater than str2

                The relation for conparison is,

                        ASCII <  1 byte Kana <  2 bytes character

                If the second byte of 2-bytes character is null,
                that character is regarded as null.

*---------------------------------------------------------------------*/

#if defined(__FARFUNCS__)
#include <_farfunc.h>
#include <_mbfarfn.h>
#endif

int _RTLENTRY _EXPFUNC _mbsnbcmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
    unsigned int c1, c2;

    while (n > 0)
    {
        n--;
        c1 = *s1++;
        if (_ismbblead(c1))
        {
            if (n <= 0)
                break;
            if (*s1 == '\0')
                c1 = 0;
            else
                c1 = (c1 << 8) | *s1++;
        }
        c2 = *s2++;
        if (_ismbblead(c2))
        {
            if (n <= 0)
                break;
            n--;
            if (*s2 == '\0')
                c2 = 0;
            else
                c2 = (c2 << 8) | *s2++;
        }
        if (c1 != c2)
            return (c1 < c2) ? -1 : 1;
        if (c1 == 0)
            break;
    }
    return 0;
}
