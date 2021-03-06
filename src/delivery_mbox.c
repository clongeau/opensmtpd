/*	$OpenBSD: delivery_mbox.c,v 1.6 2012/10/03 17:58:03 gilles Exp $	*/

/*
 * Copyright (c) 2011 Gilles Chehade <gilles@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "includes.h"

#include <sys/types.h>
#include "sys-queue.h"
#include "sys-tree.h"
#include <sys/param.h>
#include <sys/socket.h>

#include <ctype.h>
#include <err.h>
#include <event.h>
#include <fcntl.h>
#include <imsg.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "smtpd.h"
#include "log.h"

#ifndef PATH_MAILLOCAL
#define	PATH_MAILLOCAL	"/usr/libexec/mail.local"
#endif

extern char	**environ;

/* mbox backend */
static void delivery_mbox_open(struct deliver *);

struct delivery_backend delivery_backend_mbox = {
	1, delivery_mbox_open
};


static void
delivery_mbox_open(struct deliver *deliver)
{
	char	*environ_new[2];
	
	environ_new[0] = "PATH=" _PATH_DEFPATH;
	environ_new[1] = (char *)NULL;
	environ = environ_new;
	execle(PATH_MAILLOCAL, PATH_MAILLOCAL, "-f", deliver->from,
	    deliver->to, (char *)NULL, environ_new);
	perror("execle");
	_exit(1);
}
