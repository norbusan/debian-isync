/* $Id: debug.c,v 1.1 2001/11/19 19:41:14 me Exp $
 *
 * isync - IMAP4 to maildir mailbox synchronizer
 * Copyright (C) 2000 Michael R. Elkins <me@mutt.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef DEBUG
#include "debug.h"

struct allocation
{
    unsigned int magic;
    const char *file;
    unsigned int line;
    size_t bytes;
    struct allocation *next;
    struct allocation *prev;
};

static struct allocation *sAllocations;

static struct allocation *
debug_check (void *p, const char *file, unsigned int line)
{
    struct allocation *a;

    if (p == NULL)
    {
	printf ("%s:%u : NULL pointer\n", file, line);
	abort ();
    }
    
    if ((unsigned int) p == 0xffffffff)
    {
	printf ("%s:%u : reference to pointer in free'd struct?\n", file, line);
	abort ();
    }

    a = (struct allocation *) ((unsigned char *) p - sizeof (struct allocation));

    if (a->magic != 0xdeadbeef)
    {
	printf ("%s:%u : bogus pointer\n", file, line);
	abort ();
    }

    if (*(unsigned int *)((unsigned char *) p + a->bytes) != 0xaaaaaaaa)
    {
	printf ("%s:%u : buffer overrun in memory allocated at %s:%u\n",
		file, line, a->file, a->line);
	abort ();
    }

    return a;
}

void *
debug_malloc (size_t nbytes, const char *file, unsigned int line)
{
    struct allocation *a = malloc (nbytes + sizeof (struct allocation) + 4);
    void *p;

    a->magic = 0xdeadbeef;
    a->bytes = nbytes;
    a->next = sAllocations;
    a->prev = NULL;
    a->file = file;
    a->line = line;
    if (sAllocations)
	sAllocations->prev = a;
    sAllocations = a;

    p = (void *) ((unsigned char *) a + sizeof (struct allocation));
    memset (p, 0xaa, nbytes + 4);

    return p;
}

void
debug_free (void *ptr, const char *file, unsigned int line)
{
    struct allocation *a = debug_check (ptr, file, line);

    if (a->next)
	a->next->prev = a->prev;
    if (a->prev)
	a->prev->next = a->next;
    else
	sAllocations = a->next;

    memset (a, 0xff, a->bytes + sizeof (struct allocation) + 4);

    free (a);
}

void *
debug_calloc (size_t nelem, size_t nsize, const char *file, unsigned int line)
{
    void *p = debug_malloc (nelem * nsize, file, line);
    memset (p, 0x00, nelem * nsize);
    return p;
}

char *
debug_strdup (const char *s, const char *file, unsigned int line)
{
    char *p = debug_malloc (strlen (s) + 1, file, line);
    return strcpy (p, s);
}

void *
debug_realloc (void *ptr, size_t nbytes, const char *file, unsigned int line)
{
    struct allocation *a;
    struct allocation *b;
    void *p;

    a = debug_check (ptr, file, line);
    p = debug_malloc (nbytes, file, line);
    b = (void *) ((unsigned char *) p - sizeof (struct allocation));

    if (nbytes <= a->bytes)
	memcpy (p, ptr, nbytes);
    else
    {
	memcpy (p, ptr, a->bytes);
	memset ((unsigned char *) p + a->bytes, 0xaa, nbytes - a->bytes);
    }

    debug_free (ptr, file, line);

    return p;
}

void
debug_cleanup (void)
{
    struct allocation *p;

    for (p = sAllocations; p; p = p->next)
    {
	debug_check ((unsigned char*)p + sizeof (struct allocation), __FILE__, __LINE__);
	printf ("%s:%u : %d bytes allocated\n", p->file, p->line, p->bytes);
    }
}
