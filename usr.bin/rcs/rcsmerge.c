/*	$OpenBSD: rcsmerge.c,v 1.1 2005/10/18 16:16:39 xsa Exp $	*/
/*
 * Copyright (c) 2005 Xavier Santolaria <xsa@openbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "rcs.h"
#include "diff.h"
#include "rcsprog.h"


static int kflag = RCS_KWEXP_ERR;

int
rcsmerge_main(int argc, char **argv)
{
	int i, ch;
	char fpath[MAXPATHLEN];
	RCSFILE *file;
	RCSNUM *rev;

	rev = RCS_HEAD_REV;

	while ((ch = rcs_getopt(argc, argv, "k:qr:TV")) != -1) {
		switch (ch) {
		case 'k':
			kflag = rcs_kflag_get(rcs_optarg);
			if (RCS_KWEXP_INVAL(kflag)) {
				cvs_log(LP_ERR,
				    "invalid RCS keyword expansion mode");
				(usage)();
				exit(1);
			}
			break;
		case 'q':
			verbose = 0;
			break;
		case 'r':
			rcs_set_rev(rcs_optarg, &rev);
			break;
		case 'T':
			/*
			 * kept for compatibility
			 */
			break;
		case 'V':
			printf("%s\n", rcs_version);
			exit(0);
		default:
			break;
		}
	}

	argc -= rcs_optind;
	argv += rcs_optind;

	if (argc < 0) {
		cvs_log(LP_ERR, "no input file");
		(usage)();
		exit(1);
	}

	for (i = 0; i < argc; i++) {
		if (rcs_statfile(argv[i], fpath, sizeof(fpath)) < 0)
			continue;

		if ((file = rcs_open(fpath, RCS_READ)) == NULL)
			continue;

		rcs_close(file);
	}

	return (0);
}

void
rcsmerge_usage(void)
{
	fprintf(stderr,
	    "usage: rcsmerge [-qTV] [-kmode] [-rrev] file ...\n");
}
