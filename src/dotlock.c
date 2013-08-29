/* $Id: dotlock.c,v 1.5 2003/05/07 00:06:37 ossi Exp $
 *
 * isync - IMAP4 to maildir mailbox synchronizer
 * Copyright (C) 2002 Michael R. Elkins <me@mutt.org>
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
 *
 * As a special exception, isync may be linked with the OpenSSL library,
 * despite that library's more restrictive license.
 */

/*
 * this file contains routines to establish a mutex using a `dotlock' file
 */

#include "dotlock.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#if TESTING
#include <stdio.h>
#endif

static struct flock lck = { 0, SEEK_SET, 0, 0, 0 };

int dotlock_lock (const char *path, int *fd)
{
  *fd = open (path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (*fd == -1)
    return -1;
  lck.l_type = F_WRLCK;
  if (fcntl (*fd, F_SETLK, &lck))
  {
    close (*fd);
    *fd = -1;
    return -1;
  }
  return 0;
}

int dotlock_unlock (int *fd)
{
  int r = 0;

  if (*fd != -1)
  {
    lck.l_type = F_UNLCK;
    if (fcntl (*fd, F_SETLK, &lck))
      r = -1;
    close (*fd);
    *fd = -1;
  }
  return r;
}

#if TESTING
int main (void)
{
  int fd;

  if (dotlock_lock ("./lock", &fd))
  {
    perror ("dotlock_lock");
    goto done;
  }
  puts ("sleeping for 5 seconds");
  sleep(5);
  if (dotlock_unlock (&fd))
  {
    perror ("dotlock_unlock");
  }
done:
  exit (0);
}
#endif
