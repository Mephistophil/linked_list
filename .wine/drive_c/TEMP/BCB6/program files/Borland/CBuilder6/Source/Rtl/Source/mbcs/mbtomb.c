/*-----------------------------------------------------------------------*
 * filename - mbtomb.c
 *
 * function(s)
 *        _mbbtombc - Converts 1-byte character to 2-bytes character.
 *        _mbctombb - Converts 2-bytes character to 1-byte character.
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

extern int _mbcsCodePage;

/*---------------------------------------------------------------------*

Name            _mbbtombc - Converts 1-byte character to 2-bytes character.
                _mbctombb - Converts 2-bytes character to 1-byte character.

Usage           unsigned int _mbbtombc(unsigned int c);
                unsigned int _mbctombb(unsigned int c);

Prototype in    mbstring.h

Description     _mbbtombc converts 1-byte character c to 2-byte character.
                The conversion occurs in the range of ASCII subset from
                0x20 to 0x7E, and JIS Kana subset from 0xA1 to 0xDF.
                _mbctombb converts 2-bytes character c to 1-byte character.
                The convertible characters are alphabetical, numeric,
                Katakana, Hiragana and a part of simbols.

Return value    Returns the character code after converted if it is
                convertible character. If the character is not
                convertible, these functions return original character,c.

(The followingis for just Japanese character)
Notes           A voiced and harh-voiced sound of Katakana and Hiragana
                is converted to a character without these symbol.

*---------------------------------------------------------------------*/

static unsigned short ascii_to_mbc_table[0x007E-0x0020+1] = {
    0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166,
    0x8169, 0x816A, 0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E,
    0x824F, 0x8250, 0x8251, 0x8252, 0x8253, 0x8254, 0x8255, 0x8256,
    0x8257, 0x8258, 0x8146, 0x8147, 0x8183, 0x8181, 0x8184, 0x8148,
    0x8197, 0x8260, 0x8261, 0x8262, 0x8263, 0x8264, 0x8265, 0x8266,
    0x8267, 0x8268, 0x8269, 0x826A, 0x826B, 0x826C, 0x826D, 0x826E,
    0x826F, 0x8270, 0x8271, 0x8272, 0x8273, 0x8274, 0x8275, 0x8276,
    0x8277, 0x8278, 0x8279, 0x816D, 0x818F, 0x816E, 0x814F, 0x8151,
    0x8165, 0x8281, 0x8282, 0x8283, 0x8284, 0x8285, 0x8286, 0x8287,
    0x8288, 0x8289, 0x828A, 0x828B, 0x828C, 0x828D, 0x828E, 0x828F,
    0x8290, 0x8291, 0x8292, 0x8293, 0x8294, 0x8295, 0x8296, 0x8297,
    0x8298, 0x8299, 0x829A, 0x816F, 0x8162, 0x8170, 0x8150
};

static unsigned short mbb_to_mbc_katakana_table[0x00DF-0x00A0+1] = {
    0x8140, 0x8142, 0x8175, 0x8176, 0x8141, 0x8145, 0x8392, 0x8340,
    0x8342, 0x8344, 0x8346, 0x8348, 0x8383, 0x8385, 0x8387, 0x8362,
    0x815B, 0x8341, 0x8343, 0x8345, 0x8347, 0x8349, 0x834A, 0x834C,
    0x834E, 0x8350, 0x8352, 0x8354, 0x8356, 0x8358, 0x835A, 0x835C,
    0x835E, 0x8360, 0x8363, 0x8365, 0x8367, 0x8369, 0x836A, 0x836B,
    0x836C, 0x836D, 0x836E, 0x8371, 0x8374, 0x8377, 0x837A, 0x837D,
    0x837E, 0x8380, 0x8381, 0x8382, 0x8384, 0x8386, 0x8388, 0x8389,
    0x838A, 0x838B, 0x838C, 0x838D, 0x838F, 0x8393, 0x814A, 0x814B
};

static unsigned char mbc_to_mbb_katakana_table[0x8396-0x8340+1] = {
    0xA7, 0xB1, 0xA8, 0xB2, 0xA9, 0xB3, 0xAA, 0xB4,
    0xAB, 0xB5, 0xB6, 0xB6, 0xB7, 0xB7, 0xB8, 0xB8,
    0xB9, 0xB9, 0xBA, 0xBA, 0xBB, 0xBB, 0xBC, 0xBC,
    0xBD, 0xBD, 0xBE, 0xBE, 0xBF, 0xBF, 0xC0, 0xC0,
    0xC1, 0xC1, 0xAF, 0xC2, 0xC2, 0xC3, 0xC3, 0xC4,
    0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCA,
    0xCA, 0xCB, 0xCB, 0xCB, 0xCC, 0xCC, 0xCC, 0xCD,
    0xCD, 0xCD, 0xCE, 0xCE, 0xCE, 0xCF, 0xD0, 0,
    0xD1, 0xD2, 0xD3, 0xAC, 0xD4, 0xAD, 0xD5, 0xAE,
    0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDC,
    0xB2, 0xB4, 0xA6, 0xDD, 0xB3, 0xB6, 0xB9
};

#define ZTOH_SYMBOLS 9
static unsigned short mbc_to_mbb_symbol_table_1[ZTOH_SYMBOLS] = {
    0x8142, 0x8175, 0x8176, 0x8141, 0x8145, 0x815B, 0x814A, 0x814B
};
static unsigned char mbc_to_mbb_symbol_table_2[ZTOH_SYMBOLS] = {
    0xA1,   0xA2,   0xA3,   0xA4,   0xA5,   0xB0,   0xDE,   0xDF
};

unsigned int _RTLENTRY _EXPFUNC _mbbtombc(unsigned int c)
{
    if ( _mbcsCodePage  != _KANJI_CP )
        return (c);
    if (c >= 0x0020 && c <= 0x007E)
        return ascii_to_mbc_table[c - 0x0020];
    else if (c >= 0x00A1 && c <= 0x00DF)
        return mbb_to_mbc_katakana_table[c - 0x00A0];
    else
        return c;
}

unsigned int _RTLENTRY _EXPFUNC _mbctombb(unsigned int c)
{
    int i;

    if ( _mbcsCodePage  != _KANJI_CP )
        return (c);

    if (c >= 0x8340 && c <= 0x8396)
    {
        if (c == 0x837F)
            return c;
        return mbc_to_mbb_katakana_table[c - 0x8340];
    }
    else if (c >= 0x829F && c <= 0x82F1)
    {
        c += (c <= 0x82DD) ? 0x00A1 : 0x00A2;
        return mbc_to_mbb_katakana_table[c - 0x8340];
    }
    else if (c <= 0x8396)
    {
        unsigned short *p;

        for (i = 0x0020, p = ascii_to_mbc_table; i <= 0x007E; i++, p++)
            if (*p == c)
                return i;
        for (i = 0; i < ZTOH_SYMBOLS; i++)
            if (mbc_to_mbb_symbol_table_1[i] == c)
                return mbc_to_mbb_symbol_table_2[i];
    }
    return c;
}
