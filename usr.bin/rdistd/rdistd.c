/*	$OpenBSD: rdistd.c,v 1.6 2003/06/03 02:56:15 millert Exp $	*/

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "defs.h"

#ifndef lint
#if 0
static char RCSid[] __attribute__((__unused__)) =
"$From: rdistd.c,v 1.2 1999/08/04 15:57:33 christos Exp $";
#else
static char RCSid[] __attribute__((__unused__)) =
"$OpenBSD: rdistd.c,v 1.6 2003/06/03 02:56:15 millert Exp $";
#endif

static char sccsid[] __attribute__((__unused__)) =
"@(#)rdistd.c";

static char copyright[] __attribute__((__unused__)) =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

int main(int, char **, char **);
static void usage(void);

/*
 * Print usage message
 */
static void
usage(void)
{
	fprintf(stderr, "usage: %s -S [ -DV ]\n", progname);
	exit(1);
}

char	localmsglist[] = "syslog=ferror";

/*
 * The Beginning
 */
int
main(int argc, char **argv, char **envp)
{
	extern char *__progname;
	char *cp;
	int c;

	progname = __progname;

	if (init(argc, argv, envp) < 0)
		exit(1);

	while ((c = getopt(argc, argv, "SDV")) != -1)
		switch (c) {
		case 'S':
			isserver++;
			break;

		case 'D':
			debug++;
			break;

		case 'V':
			printf("%s\n", getversion());
			exit(0);

		case '?':
		default:
			error("Bad command line option.");
			usage();
		}

	if (!isserver) {
		error("Use the -S option to run this program in server mode.");
		exit(1);
	}

	/* Use stdin and stdout for remote descriptors */
	rem_r = fileno(stdin);
	rem_w = fileno(stdout);

	/* Set logging */
	if ((cp = msgparseopts(localmsglist, TRUE)) != NULL)
		fatalerr("Bad message logging option (%s): %s", 
			 localmsglist, cp);

	/*
	 * Main processing function
	 */
	server();

	exit(nerrs != 0);
}
