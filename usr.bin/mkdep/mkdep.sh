#!/bin/sh -
#
#	$OpenBSD: mkdep.sh,v 1.6 1997/01/25 14:27:44 niklas Exp $
#	$NetBSD: mkdep.sh,v 1.3 1994/12/23 07:35:02 jtc Exp $
#
# Copyright (c) 1991, 1993
#	The Regents of the University of California.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#	This product includes software developed by the University of
#	California, Berkeley and its contributors.
# 4. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
#	@(#)mkdep.sh	8.1 (Berkeley) 6/6/93
#

D=.depend			# default dependency file is .depend
append=0

while :
	do case "$1" in
		# -a appends to the depend file
		-a)
			append=1
			shift ;;

		# -f allows you to select a makefile name
		-f)
			D=$2
			shift; shift ;;

		# the -p flag produces "program: program.c" style dependencies
		# so .o's don't get produced
		-p)
			SED='s;\.o ; ;'
			shift ;;
		*)
			break ;;
	esac
done

if [ $# = 0 ] ; then
	echo 'usage: mkdep [-p] [-f depend_file] [cc_flags] file ...'
	exit 1
fi

DTMP=/tmp/_mkdep$$
TMP=$DTMP/mkdep

um=`umask`
umask 022
if ! mkdir $DTMP ; then
	echo failed to create tmp dir $DTMP
	exit 1
fi
umask $um
trap 'rm -rf $DTMP ; exit 1' 1 2 3 13 15

${CC:-cc} -M $* |
sed "
	s; \./; ;g
	/\.c:$/d
	$SED" |
awk '{
	if ($1 != prev) {
		if (rec != "")
			print rec;
		rec = $0;
		prev = $1;
	}
	else {
		if (length(rec $2) > 78) {
			print rec;
			rec = $0;
		}
		else
			rec = rec " " $2
	}
}
END {
	print rec
}' > $TMP

if [ $? != 0 ]; then
	echo 'mkdep: compile failed.'
	rm -rf $DTMP
	exit 1
fi

if [ $append = 1 ]; then
	cat $TMP >> $D
	rm -rf $DTMP
else
	mv $TMP $D
	rm -rf $DTMP
fi

exit 0




