/* $Id: debug.h,v 1.1 2001/11/19 19:41:14 me Exp $
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

#ifndef debug_h

#include <sys/types.h>

#if DEBUG
#define calloc(a,b) debug_calloc(a,b,__FILE__,__LINE__)
#define malloc(a) debug_malloc(a,__FILE__,__LINE__)
#ifdef strdup
#undef strdup
#endif
#define strdup(a) debug_strdup(a,__FILE__,__LINE__)
#define realloc(a,b) debug_realloc(a,b,__FILE__,__LINE__)
#define free(a) debug_free(a,__FILE__,__LINE__)
#endif

void *debug_calloc (size_t, size_t, const char *, unsigned int);
void *debug_realloc (void *, size_t, const char *, unsigned int);
void *debug_malloc (size_t, const char *, unsigned int);
void debug_free (void *, const char *, unsigned int);
char *debug_strdup (const char *, const char *, unsigned int);
void debug_cleanup (void);

#endif /*debug_h */
