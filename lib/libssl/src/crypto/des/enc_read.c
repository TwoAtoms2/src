/* crypto/des/enc_read.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#include <stdio.h>
#include <errno.h>
#include "des_locl.h"

/* This has some uglies in it but it works - even over sockets. */
/*extern int errno;*/
int des_rw_mode=DES_PCBC_MODE;

int des_enc_read(fd, buf, len, sched, iv)
int fd;
char *buf;
int len;
des_key_schedule sched;
des_cblock (*iv);
	{
	/* data to be unencrypted */
	int net_num=0;
	static unsigned char *net=NULL;
	/* extra unencrypted data 
	 * for when a block of 100 comes in but is des_read one byte at
	 * a time. */
	static char *unnet=NULL;
	static int unnet_start=0;
	static int unnet_left=0;
	static char *tmpbuf=NULL;
	int i;
	long num=0,rnum;
	unsigned char *p;

	if (tmpbuf == NULL)
		{
		tmpbuf=(char *)malloc(BSIZE);
		if (tmpbuf == NULL) return(-1);
		}
	if (net == NULL)
		{
		net=(unsigned char *)malloc(BSIZE);
		if (net == NULL) return(-1);
		}
	if (unnet == NULL)
		{
		unnet=(char *)malloc(BSIZE);
		if (unnet == NULL) return(-1);
		}
	/* left over data from last decrypt */
	if (unnet_left != 0)
		{
		if (unnet_left < len)
			{
			/* we still still need more data but will return
			 * with the number of bytes we have - should always
			 * check the return value */
			memcpy(buf,&(unnet[unnet_start]),
				(unsigned int)unnet_left);
			/* eay 26/08/92 I had the next 2 lines
			 * reversed :-( */
			i=unnet_left;
			unnet_start=unnet_left=0;
			}
		else
			{
			memcpy(buf,&(unnet[unnet_start]),(unsigned int)len);
			unnet_start+=len;
			unnet_left-=len;
			i=len;
			}
		return(i);
		}

	/* We need to get more data. */
	if (len > MAXWRITE) len=MAXWRITE;

	/* first - get the length */
	while (net_num < HDRSIZE) 
		{
		i=read(fd,&(net[net_num]),(unsigned int)HDRSIZE-net_num);
#ifdef EINTR
		if ((i == -1) && (errno == EINTR)) continue;
#endif
		if (i <= 0) return(0);
		net_num+=i;
		}

	/* we now have at net_num bytes in net */
	p=net;
	/* num=0;  */
	n2l(p,num);
	/* num should be rounded up to the next group of eight
	 * we make sure that we have read a multiple of 8 bytes from the net.
	 */
	if ((num > MAXWRITE) || (num < 0)) /* error */
		return(-1);
	rnum=(num < 8)?8:((num+7)/8*8);

	net_num=0;
	while (net_num < rnum)
		{
		i=read(fd,&(net[net_num]),(unsigned int)rnum-net_num);
#ifdef EINTR
		if ((i == -1) && (errno == EINTR)) continue;
#endif
		if (i <= 0) return(0);
		net_num+=i;
		}

	/* Check if there will be data left over. */
	if (len < num)
		{
		if (des_rw_mode & DES_PCBC_MODE)
			des_pcbc_encrypt((des_cblock *)net,(des_cblock *)unnet,
				num,sched,iv,DES_DECRYPT);
		else
			des_cbc_encrypt((des_cblock *)net,(des_cblock *)unnet,
				num,sched,iv,DES_DECRYPT);
		memcpy(buf,unnet,(unsigned int)len);
		unnet_start=len;
		unnet_left=(int)num-len;

		/* The following line is done because we return num
		 * as the number of bytes read. */
		num=len;
		}
	else
		{
		/* >output is a multiple of 8 byes, if len < rnum
		 * >we must be careful.  The user must be aware that this
		 * >routine will write more bytes than he asked for.
		 * >The length of the buffer must be correct.
		 * FIXED - Should be ok now 18-9-90 - eay */
		if (len < rnum)
			{

			if (des_rw_mode & DES_PCBC_MODE)
				des_pcbc_encrypt((des_cblock *)net,
					(des_cblock *)tmpbuf,
					num,sched,iv,DES_DECRYPT);
			else
				des_cbc_encrypt((des_cblock *)net,
					(des_cblock *)tmpbuf,
					num,sched,iv,DES_DECRYPT);

			/* eay 26/08/92 fix a bug that returned more
			 * bytes than you asked for (returned len bytes :-( */
			memcpy(buf,tmpbuf,(unsigned int)num);
			}
		else
			{
			if (des_rw_mode & DES_PCBC_MODE)
				des_pcbc_encrypt((des_cblock *)net,
					(des_cblock *)buf,num,sched,iv,
					DES_DECRYPT);
			else
				des_cbc_encrypt((des_cblock *)net,
					(des_cblock *)buf,num,sched,iv,
					DES_DECRYPT);
			}
		}
	return((int)num);
	}

