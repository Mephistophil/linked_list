/*------------------------------------------------------------------------
 * filename - strtod.c
 *
 * function(s)
 *        strtod   - converts string to a double value
 *        _strtold - converts string to a long double value
 *        Get      - gets a character from a string
 *        UnGet    - ungets a character from a string
 *        wcstod   - converts wide-character string to a double value
 *        _wcstold - converts wide-character string to a long double value
 *-----------------------------------------------------------------------*/

/*
 *      C/C++ Run Time Library - Version 11.0
 *
 *      Copyright (c) 1987, 2002 by Borland Software Corporation
 *      All Rights Reserved.
 *
 */

/* $Revision: 9.3 $        */

#include <errno.h>
#include <_scanf.h>
#include <stddef.h>

#include <_math.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <tchar.h>
#include <_tchar.h>

/*---------------------------------------------------------------------*

Name            Get - gets a character from a (wide-character)string

Usage           int Get (char **strPP)
                int Get (wchar_t **strPP)

Description     returns the next character in a string

Return value    the next character in a string.  if that character is
                the null character, Get returns -1.

*---------------------------------------------------------------------*/

static  int  Get (_TCHAR **strPP)
{
    unsigned    c;

    return  ((c = *((*strPP) ++)) == 0) ? -1 : c;
}

/*---------------------------------------------------------------------*

Name            UnGet  - ungets a character from a (wide-character) string

Usage           void UnGet (char c, char **strPP)
                void UnGet (wchar_t c, char **strPP)

Description     decrements a string pointer

Return value    Nothing

*---------------------------------------------------------------------*/

#pragma warn -par

static  void  UnGet (_TCHAR c, _TCHAR **strPP)
{
    --(*strPP);         /* ignore c, we don't allow the string to change */
}

#pragma warn .par

/*--------------------------------------------------------------------------*

Name            strtod   - converts string to a double value
                _strtold - converts string to a long double value
		wcstod   - converts wide-character string to a double value
		_wcstold - converts wide-character string to a long double value

Usage           double strtod(const char *strP, char **suffixPP);
                long double _strtold(const char *strP, char **suffixPP);
                double wcstod(const wchar_t *strP, wchar_t **suffixPP);
                long double _wcstold(const wchar_t *strP, wchar_t **suffixPP);

Prototype in    stdlib.h

Description     Convert a string to a double precision or long double real.
                The syntax of the string must be:

                float    ::= [isspace]* [sign] [realnum] [exponent]

                isspace  ::= as per <ctype.h>:isspace

                realnum  ::= {digit [digit]* ['.' [digit]* ]} |
                             {'.' digit [digit]*}

                exponent ::= 'e'|'E' [sign] digit [digit]*

                "strP" is a pointer to the ASCII string to be scanned.

                "suffixPP" is a pointer to  a string pointer to be updated.
                If it is not NULL then (*suffixPP) will be updated to point
                to the first character following  the section of the string
                which was consumed.

                The digits must be decimal.

Return value    strtod and _strtold return the converted value of the input
                string as a double or long double value, respectively.
                If the source string is  not a valid floating point numeral
                then the  result value is  zero and the  next char pointer
                will equal  the starting string  pointer. If the  number is
                too large or too tiny then the result is signed HUGE_VAL
                (strtod), _LHUGE_VAL (_strtold), or zero, and errno is
                set to ERANGE.

*--------------------------------------------------------------------------*/

long double _RTLENTRY _EXPFUNC _tcstold (const _TCHAR *strP, _TCHAR **suffixPP)
{
        int     charCt = 0;
        int     status;
        long double  result;

         _scanttod (
                &result,
                (int  (*) (void *))Get,
                (void (*) (int, void *))UnGet,
                &strP,
                0x7FFF,
                &charCt,
                &status
                );

        if (status <= 0)
                strP -= charCt;
        else if (status == 2)
                errno = ERANGE;

        if (suffixPP != NULL)
                *suffixPP = (_TCHAR *)strP;

        return (result);
}

double _RTLENTRY _EXPFUNC _tcstod (const _TCHAR *strP, _TCHAR **suffixPP)
{
        /*
          __ldtrunc sets 'errno' to ERANGE if the result
          is to become 0 or HUGE_VAL.
        */
        return __ldtrunc(DBL, _tcstold(strP, suffixPP), HUGE_VAL);
}
