/*-----------------------------------------------------------------------*
 * filename - tmpfile.c
 *
 * function(s)
 *        tmpfile - creates a unique temporary file
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
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <_stdio.h>
#include <_io.h>

/*---------------------------------------------------------------------*

Name            tmpfile - creates a unique temporary file

Usage           #include <stdio.h>
                FILE *tmpfile(void);

Prototype in    stdio.h

Return value    On successful completion, the associated stream.
                In the event of error, NULL is returned.

*---------------------------------------------------------------------*/

FILE * _RTLENTRY _EXPFUNC tmpfile(void)
{
    FILE    *stream;
    int     handle;

    _lock_all_streams();

    /* Keep trying different filenames until we come up with one that
     * doesn't match an existing file.
     */
    while ((handle = __open(__tmpnam(NULL, &_tmpnum),
                            O_CREAT | O_EXCL | O_RDWR | O_BINARY,
                            S_IREAD | S_IWRITE)) == -1)
    {
        if (errno != EEXIST)
        {
            _unlock_all_streams();
            return (NULL);
        }
    }
    if ((stream = _fdopen(handle, "w+b")) != NULL)
        stream->istemp = _tmpnum;

    _unlock_all_streams();
    return (stream);
}
