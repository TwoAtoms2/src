/*	$OpenBSD: safile.c,v 1.19 2009/11/05 08:36:48 ratchov Exp $	*/
/*
 * Copyright (c) 2008 Alexandre Ratchov <alex@caoua.org>
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

#include <sys/time.h>
#include <sys/types.h>

#include <poll.h>
#include <sndio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aparams.h"
#include "aproc.h"
#include "conf.h"
#include "dev.h"
#include "file.h"
#include "safile.h"

struct safile {
	struct file file;
	struct sio_hdl *hdl;
	int started;
};

void safile_close(struct file *);
unsigned safile_read(struct file *, unsigned char *, unsigned);
unsigned safile_write(struct file *, unsigned char *, unsigned);
void safile_start(struct file *);
void safile_stop(struct file *);
int safile_nfds(struct file *);
int safile_pollfd(struct file *, struct pollfd *, int);
int safile_revents(struct file *, struct pollfd *);

struct fileops safile_ops = {
	"sio",
	sizeof(struct safile),
	safile_close,
	safile_read,
	safile_write,
	safile_start,
	safile_stop,
	safile_nfds,
	safile_pollfd,
	safile_revents
};

void
safile_cb(void *addr, int delta)
{
	struct safile *f = (struct safile *)addr;
	struct aproc *p;

	if (delta != 0) {
		p = f->file.wproc;
		if (p && p->ops->opos)
			p->ops->opos(p, NULL, delta);
	}
	if (delta != 0) {
		p = f->file.rproc;
		if (p && p->ops->ipos)
			p->ops->ipos(p, NULL, delta);
	}
}

/*
 * Open the device.
 */
struct safile *
safile_new(struct fileops *ops, char *path,
    struct aparams *ipar, struct aparams *opar,
    unsigned *bufsz, unsigned *round)
{
	struct sio_par par;
	struct sio_hdl *hdl;
	struct safile *f;
	int mode;

	mode = 0;
	if (ipar)
		mode |= SIO_REC;
	if (opar)
		mode |= SIO_PLAY;
	hdl = sio_open(path, mode, 1);
	if (hdl == NULL)
		return NULL;
	sio_initpar(&par);
	if (ipar) {
		par.bits = ipar->bits;
		par.bps = ipar->bps;
		par.sig = ipar->sig;
		par.le = ipar->le;
		par.msb = ipar->msb;
		par.rate = ipar->rate;
		par.rchan = ipar->cmax - ipar->cmin + 1;
	} else {
		par.bits = opar->bits;
		par.bps = opar->bps;
		par.sig = opar->sig;
		par.le = opar->le;
		par.msb = opar->msb;
		par.rate = opar->rate;
	}
	if (opar)
		par.pchan = opar->cmax - opar->cmin + 1;
	par.appbufsz = *bufsz;
	par.round = *round;
	if (!sio_setpar(hdl, &par))
		goto bad_close;
	if (!sio_getpar(hdl, &par))
		goto bad_close;
	if (ipar) {
		ipar->bits = par.bits;
		ipar->bps = par.bps;
		ipar->sig = par.sig;
		ipar->le = par.le;
		ipar->msb = par.msb;
		ipar->rate = par.rate;
		ipar->cmax = ipar->cmin + par.rchan - 1;
	}
	if (opar) {
		opar->bits = par.bits;
		opar->bps = par.bps;
		opar->sig = par.sig;
		opar->le = par.le;
		opar->msb = par.msb;
		opar->rate = par.rate;
		opar->cmax = opar->cmin + par.pchan - 1;
	}
	*bufsz = par.bufsz;
	*round = par.round;
	if (path == NULL)
		path = "default";
	f = (struct safile *)file_new(ops, path, sio_nfds(hdl));
	if (f == NULL)
		goto bad_close;
	f->hdl = hdl;
	f->started = 0;
	sio_onmove(f->hdl, safile_cb, f);
	return f;
 bad_close:
	sio_close(hdl);
	return NULL;
}

void
safile_start(struct file *file)
{
	struct safile *f = (struct safile *)file;

	if (!sio_start(f->hdl)) {
		file_close(file);
		return;
	}
	f->started = 1;
}

void
safile_stop(struct file *file)
{
	struct safile *f = (struct safile *)file;

	f->started = 0;
	if (!sio_eof(f->hdl) && !sio_stop(f->hdl)) {
		file_close(file);
		return;
	}
}

unsigned
safile_read(struct file *file, unsigned char *data, unsigned count)
{
	struct safile *f = (struct safile *)file;
	unsigned n;

	n = sio_read(f->hdl, data, count);
	if (n == 0) {
		f->file.state &= ~FILE_ROK;
		if (sio_eof(f->hdl)) {
			file_eof(&f->file);
		} else {
		}
		return 0;
	}
	return n;

}

unsigned
safile_write(struct file *file, unsigned char *data, unsigned count)
{
	struct safile *f = (struct safile *)file;
	unsigned n;

	n = sio_write(f->hdl, data, count);
	if (n == 0) {
		f->file.state &= ~FILE_WOK;
		if (sio_eof(f->hdl)) {
			file_hup(&f->file);
		} else {
		}
		return 0;
	}
	return n;
}

int
safile_nfds(struct file *file)
{
	return sio_nfds(((struct safile *)file)->hdl);
}

int
safile_pollfd(struct file *file, struct pollfd *pfd, int events)
{
	return sio_pollfd(((struct safile *)file)->hdl, pfd, events);
}

int
safile_revents(struct file *file, struct pollfd *pfd)
{
	return sio_revents(((struct safile *)file)->hdl, pfd);
}

void
safile_close(struct file *file)
{
	struct safile *f = (struct safile *)file;

	if (f->started)
		safile_stop(&f->file);
	return sio_close(((struct safile *)file)->hdl);
}
