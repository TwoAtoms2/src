/*	$OpenBSD: pfkey.c,v 1.6 2010/06/26 18:32:34 reyk Exp $	*/
/*	$vantronix: pfkey.c,v 1.11 2010/06/03 07:57:33 reyk Exp $	*/

/*
 * Copyright (c) 2010 Reyk Floeter <reyk@vantronix.net>
 * Copyright (c) 2004, 2005 Hans-Joerg Hoexer <hshoexer@openbsd.org>
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2003, 2004 Markus Friedl <markus@openbsd.org>
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

#include <sys/types.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/uio.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip_ipsp.h>
#include <net/pfkeyv2.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <event.h>

#include "iked.h"
#include "ikev2.h"

#define ROUNDUP(x) (((x) + (PFKEYV2_CHUNK - 1)) & ~(PFKEYV2_CHUNK - 1))
#define IOV_CNT 20

#define PFKEYV2_CHUNK sizeof(u_int64_t)

static u_int32_t sadb_msg_seq = 1;
static u_int sadb_decoupled = 0;

struct pfkey_constmap {
	u_int8_t	 pfkey_id;
	u_int		 pfkey_ikeid;
	u_int		 pfkey_fixedkey;
};

static const struct pfkey_constmap pfkey_encr[] = {
	{ SADB_X_EALG_DES_IV64,	IKEV2_XFORMENCR_DES_IV64 },
	{ SADB_EALG_DESCBC,	IKEV2_XFORMENCR_DES },
	{ SADB_EALG_3DESCBC,	IKEV2_XFORMENCR_3DES },
	{ SADB_X_EALG_RC5,	IKEV2_XFORMENCR_RC5 },
	{ SADB_X_EALG_IDEA,	IKEV2_XFORMENCR_IDEA },
	{ SADB_X_EALG_CAST,	IKEV2_XFORMENCR_CAST },
	{ SADB_X_EALG_BLF,	IKEV2_XFORMENCR_BLOWFISH },
	{ SADB_X_EALG_3IDEA,	IKEV2_XFORMENCR_3IDEA },
	{ SADB_X_EALG_DES_IV32,	IKEV2_XFORMENCR_DES_IV32 },
	{ SADB_X_EALG_RC4,	IKEV2_XFORMENCR_RC4 },
	{ SADB_EALG_NULL,	IKEV2_XFORMENCR_NULL },
	{ SADB_X_EALG_AES,	IKEV2_XFORMENCR_AES_CBC },
	{ SADB_X_EALG_AESCTR,	IKEV2_XFORMENCR_AES_CTR },
	{ 0 }
};

static const struct pfkey_constmap pfkey_integr[] = {
	{ SADB_AALG_MD5HMAC,	IKEV2_XFORMAUTH_HMAC_MD5_96 },
	{ SADB_AALG_SHA1HMAC,	IKEV2_XFORMAUTH_HMAC_SHA1_96 },
	{ SADB_X_AALG_DES,	IKEV2_XFORMAUTH_DES_MAC },
	{ SADB_X_AALG_SHA2_256,	IKEV2_XFORMAUTH_HMAC_SHA2_256_128 },
	{ SADB_X_AALG_SHA2_384,	IKEV2_XFORMAUTH_HMAC_SHA2_384_192 },
	{ SADB_X_AALG_SHA2_512,	IKEV2_XFORMAUTH_HMAC_SHA2_512_256 },
	{ 0 }
};

static const struct pfkey_constmap pfkey_satype[] = {
	{ SADB_SATYPE_AH,	IKEV2_SAPROTO_AH },
	{ SADB_SATYPE_ESP,	IKEV2_SAPROTO_ESP },
	{ 0 }
};

int	pfkey_map(const struct pfkey_constmap *, u_int16_t, u_int8_t *);
int	pfkey_flow(int, u_int8_t, u_int8_t, struct iked_flow *);
int	pfkey_sa(int, u_int8_t, u_int8_t, struct iked_childsa *);
int	pfkey_sa_getspi(int, u_int8_t, struct iked_childsa *, u_int32_t *);
int	pfkey_sagroup(int, u_int8_t, u_int8_t,
	    struct iked_childsa *, struct iked_childsa *);
int	pfkey_write(int sd, struct sadb_msg *, struct iovec *, int,
	    u_int8_t **, ssize_t *);
int	pfkey_reply(int, u_int8_t **, ssize_t *);

struct sadb_ident *
	pfkey_id2ident(struct iked_id *, u_int);

int
pfkey_couple(int sd, struct iked_sas *sas, int couple)
{
	struct iked_sa		*sa;
	struct iked_flow	*flow;
	struct iked_childsa	*csa;
	u_int			 old;
	const char		*mode[] = { "coupled", "decoupled" };

	/* Socket is not ready */
	if (sd == -1)
		return (-1);

	old = sadb_decoupled ? 1 : 0;
	sadb_decoupled = couple ? 0 : 1;

	if (old == sadb_decoupled)
		return (0);

	log_debug("%s: kernel %s -> %s", __func__,
	    mode[old], mode[sadb_decoupled]);

	RB_FOREACH(sa, iked_sas, sas) {
		TAILQ_FOREACH(csa, &sa->sa_childsas, csa_entry) {
			if (!csa->csa_loaded && !sadb_decoupled)
				(void)pfkey_sa_add(sd, csa, NULL);
			else if (csa->csa_loaded && sadb_decoupled)
				(void)pfkey_sa_delete(sd, csa);
		}
		TAILQ_FOREACH(flow, &sa->sa_flows, flow_entry) {
			if (!flow->flow_loaded && !sadb_decoupled)
				(void)pfkey_flow_add(sd, flow);
			else if (flow->flow_loaded && sadb_decoupled)
				(void)pfkey_flow_delete(sd, flow);
		}
	}

	return (0);
}

int
pfkey_map(const struct pfkey_constmap *map, u_int16_t alg, u_int8_t *pfkalg)
{
	int	 i;

	for (i = 0; map[i].pfkey_id != 0; i++)
		if (map[i].pfkey_ikeid == alg) {
			*pfkalg = map[i].pfkey_id;
			return (0);
		}
	return (-1);
}

int
pfkey_flow(int sd, u_int8_t satype, u_int8_t action, struct iked_flow *flow)
{
	struct sadb_msg		 smsg;
	struct sadb_address	 sa_src, sa_dst, sa_local, sa_peer, sa_smask,
				 sa_dmask;
	struct sadb_protocol	 sa_flowtype, sa_protocol;
	struct sadb_ident	*sa_srcid, *sa_dstid;
	struct sockaddr_storage	 ssrc, sdst, slocal, speer, smask, dmask;
	struct iovec		 iov[IOV_CNT];
	int			 iov_cnt, ret = -1;
	in_port_t		 sport, dport;

	sport = dport = 0;
	sa_srcid = sa_dstid = NULL;

	bzero(&ssrc, sizeof(ssrc));
	bzero(&smask, sizeof(smask));
	memcpy(&ssrc, &flow->flow_src.addr, sizeof(ssrc));
	memcpy(&smask, &flow->flow_src.addr, sizeof(smask));
	if ((sport = flow->flow_src.addr_port) != 0)
		dport = 0xffff;
	socket_af((struct sockaddr *)&ssrc, sport);
	socket_af((struct sockaddr *)&smask, dport);

	switch (flow->flow_src.addr_af) {
	case AF_INET:
		((struct sockaddr_in *)&smask)->sin_addr.s_addr =
		    prefixlen2mask(flow->flow_src.addr_net ?
		    flow->flow_src.addr_mask : 32);
		break;
	case AF_INET6:
		prefixlen2mask6(flow->flow_src.addr_net ?
		    flow->flow_src.addr_mask : 128,
		    (u_int32_t *)((struct sockaddr_in6 *)
		    &smask)->sin6_addr.s6_addr);
		break;
	default:
		log_warnx("%s: unsupported address family %d",
		    __func__, flow->flow_src.addr_af);
		return (-1);
	}
	smask.ss_len = ssrc.ss_len;

	bzero(&sdst, sizeof(sdst));
	bzero(&dmask, sizeof(dmask));
	memcpy(&sdst, &flow->flow_dst.addr, sizeof(sdst));
	memcpy(&dmask, &flow->flow_dst.addr, sizeof(dmask));
	if ((sport = flow->flow_dst.addr_port) != 0)
		dport = 0xffff;
	socket_af((struct sockaddr *)&sdst, sport);
	socket_af((struct sockaddr *)&dmask, dport);

	switch (flow->flow_dst.addr_af) {
	case AF_INET:
		((struct sockaddr_in *)&dmask)->sin_addr.s_addr =
		    prefixlen2mask(flow->flow_dst.addr_net ?
		    flow->flow_dst.addr_mask : 32);
		break;
	case AF_INET6:
		prefixlen2mask6(flow->flow_dst.addr_net ?
		    flow->flow_dst.addr_mask : 128,
		    (u_int32_t *)((struct sockaddr_in6 *)
		    &dmask)->sin6_addr.s6_addr);
		break;
	default:
		log_warnx("%s: unsupported address family %d",
		    __func__, flow->flow_dst.addr_af);
		return (-1);
	}
	dmask.ss_len = sdst.ss_len;

	bzero(&slocal, sizeof(slocal));
	bzero(&speer, sizeof(speer));
	if (action != SADB_X_DELFLOW) {
		memcpy(&slocal, &flow->flow_local->addr, sizeof(slocal));
		socket_af((struct sockaddr *)&slocal, 0);

		memcpy(&speer, &flow->flow_peer->addr, sizeof(speer));
		socket_af((struct sockaddr *)&speer, 0);
	}

	bzero(&smsg, sizeof(smsg));
	smsg.sadb_msg_version = PF_KEY_V2;
	smsg.sadb_msg_seq = sadb_msg_seq++;
	smsg.sadb_msg_pid = getpid();
	smsg.sadb_msg_len = sizeof(smsg) / 8;
	smsg.sadb_msg_type = action;
	smsg.sadb_msg_satype = satype;

	bzero(&sa_flowtype, sizeof(sa_flowtype));
	sa_flowtype.sadb_protocol_exttype = SADB_X_EXT_FLOW_TYPE;
	sa_flowtype.sadb_protocol_len = sizeof(sa_flowtype) / 8;
	sa_flowtype.sadb_protocol_direction = flow->flow_dir;
	sa_flowtype.sadb_protocol_proto =
	    flow->flow_dir == IPSP_DIRECTION_IN ?
	    SADB_X_FLOW_TYPE_USE : SADB_X_FLOW_TYPE_REQUIRE;

	bzero(&sa_protocol, sizeof(sa_protocol));
	sa_protocol.sadb_protocol_exttype = SADB_X_EXT_PROTOCOL;
	sa_protocol.sadb_protocol_len = sizeof(sa_protocol) / 8;
	sa_protocol.sadb_protocol_direction = 0;
	sa_protocol.sadb_protocol_proto = flow->flow_ipproto;

	bzero(&sa_src, sizeof(sa_src));
	sa_src.sadb_address_exttype = SADB_X_EXT_SRC_FLOW;
	sa_src.sadb_address_len = (sizeof(sa_src) + ROUNDUP(ssrc.ss_len)) / 8;

	bzero(&sa_smask, sizeof(sa_smask));
	sa_smask.sadb_address_exttype = SADB_X_EXT_SRC_MASK;
	sa_smask.sadb_address_len =
	    (sizeof(sa_smask) + ROUNDUP(smask.ss_len)) / 8;

	bzero(&sa_dst, sizeof(sa_dst));
	sa_dst.sadb_address_exttype = SADB_X_EXT_DST_FLOW;
	sa_dst.sadb_address_len = (sizeof(sa_dst) + ROUNDUP(sdst.ss_len)) / 8;

	bzero(&sa_dmask, sizeof(sa_dmask));
	sa_dmask.sadb_address_exttype = SADB_X_EXT_DST_MASK;
	sa_dmask.sadb_address_len =
	    (sizeof(sa_dmask) + ROUNDUP(dmask.ss_len)) / 8;

	if (action != SADB_X_DELFLOW) {
		/* local address */
		bzero(&sa_local, sizeof(sa_local));
		sa_local.sadb_address_exttype = SADB_EXT_ADDRESS_SRC;
		sa_local.sadb_address_len =
		    (sizeof(sa_local) + ROUNDUP(slocal.ss_len)) / 8;

		/* peer address */
		bzero(&sa_peer, sizeof(sa_peer));
		sa_peer.sadb_address_exttype = SADB_EXT_ADDRESS_DST;
		sa_peer.sadb_address_len =
		    (sizeof(sa_peer) + ROUNDUP(speer.ss_len)) / 8;

		/* local id */
		sa_srcid = pfkey_id2ident(flow->flow_srcid,
		    SADB_EXT_IDENTITY_SRC);

		/* peer id */
		sa_dstid = pfkey_id2ident(flow->flow_dstid,
		    SADB_EXT_IDENTITY_DST);
	}

	iov_cnt = 0;

	/* header */
	iov[iov_cnt].iov_base = &smsg;
	iov[iov_cnt].iov_len = sizeof(smsg);
	iov_cnt++;

	/* add flow type */
	iov[iov_cnt].iov_base = &sa_flowtype;
	iov[iov_cnt].iov_len = sizeof(sa_flowtype);
	smsg.sadb_msg_len += sa_flowtype.sadb_protocol_len;
	iov_cnt++;

	if (action != SADB_X_DELFLOW) {
#if 0
		/* local ip */
		iov[iov_cnt].iov_base = &sa_local;
		iov[iov_cnt].iov_len = sizeof(sa_local);
		iov_cnt++;
		iov[iov_cnt].iov_base = &slocal;
		iov[iov_cnt].iov_len = ROUNDUP(slocal.ss_len);
		smsg.sadb_msg_len += sa_local.sadb_address_len;
		iov_cnt++;
#endif

		/* remote peer */
		iov[iov_cnt].iov_base = &sa_peer;
		iov[iov_cnt].iov_len = sizeof(sa_peer);
		iov_cnt++;
		iov[iov_cnt].iov_base = &speer;
		iov[iov_cnt].iov_len = ROUNDUP(speer.ss_len);
		smsg.sadb_msg_len += sa_peer.sadb_address_len;
		iov_cnt++;
	}

	/* src addr */
	iov[iov_cnt].iov_base = &sa_src;
	iov[iov_cnt].iov_len = sizeof(sa_src);
	iov_cnt++;
	iov[iov_cnt].iov_base = &ssrc;
	iov[iov_cnt].iov_len = ROUNDUP(ssrc.ss_len);
	smsg.sadb_msg_len += sa_src.sadb_address_len;
	iov_cnt++;

	/* src mask */
	iov[iov_cnt].iov_base = &sa_smask;
	iov[iov_cnt].iov_len = sizeof(sa_smask);
	iov_cnt++;
	iov[iov_cnt].iov_base = &smask;
	iov[iov_cnt].iov_len = ROUNDUP(smask.ss_len);
	smsg.sadb_msg_len += sa_smask.sadb_address_len;
	iov_cnt++;

	/* dest addr */
	iov[iov_cnt].iov_base = &sa_dst;
	iov[iov_cnt].iov_len = sizeof(sa_dst);
	iov_cnt++;
	iov[iov_cnt].iov_base = &sdst;
	iov[iov_cnt].iov_len = ROUNDUP(sdst.ss_len);
	smsg.sadb_msg_len += sa_dst.sadb_address_len;
	iov_cnt++;

	/* dst mask */
	iov[iov_cnt].iov_base = &sa_dmask;
	iov[iov_cnt].iov_len = sizeof(sa_dmask);
	iov_cnt++;
	iov[iov_cnt].iov_base = &dmask;
	iov[iov_cnt].iov_len = ROUNDUP(dmask.ss_len);
	smsg.sadb_msg_len += sa_dmask.sadb_address_len;
	iov_cnt++;

	/* add protocol */
	iov[iov_cnt].iov_base = &sa_protocol;
	iov[iov_cnt].iov_len = sizeof(sa_protocol);
	smsg.sadb_msg_len += sa_protocol.sadb_protocol_len;
	iov_cnt++;

	if (sa_srcid) {
		/* src identity */
		iov[iov_cnt].iov_base = sa_srcid;
		iov[iov_cnt].iov_len = sa_srcid->sadb_ident_len * 8;
		smsg.sadb_msg_len += sa_srcid->sadb_ident_len;
		iov_cnt++;
	}
	if (sa_dstid) {
		/* dst identity */
		iov[iov_cnt].iov_base = sa_dstid;
		iov[iov_cnt].iov_len = sa_dstid->sadb_ident_len * 8;
		smsg.sadb_msg_len += sa_dstid->sadb_ident_len;
		iov_cnt++;
	}

	ret = pfkey_write(sd, &smsg, iov, iov_cnt, NULL, NULL);

	if (sa_srcid)
		free(sa_srcid);
	if (sa_dstid)
		free(sa_dstid);

	return (ret);
}

int
pfkey_sa(int sd, u_int8_t satype, u_int8_t action, struct iked_childsa *sa)
{
	struct sadb_msg		 smsg;
	struct sadb_sa		 sadb;
	struct sadb_address	 sa_src, sa_dst;
	struct sadb_key		 sa_authkey, sa_enckey;
	struct sadb_x_udpencap	 udpencap;
	struct sadb_x_tag	 sa_tag;
	struct sockaddr_storage	 ssrc, sdst;
	struct sadb_ident	*sa_srcid, *sa_dstid;
	struct iovec		 iov[IOV_CNT];
	int			 iov_cnt;
	char			*tag = NULL;

	sa_srcid = sa_dstid = NULL;

	bzero(&ssrc, sizeof(ssrc));
	memcpy(&ssrc, &sa->csa_local->addr, sizeof(ssrc));
	if (socket_af((struct sockaddr *)&ssrc, 0) == -1) {
		log_warn("%s: invalid address", __func__);
		return (-1);
	}

	bzero(&sdst, sizeof(sdst));
	memcpy(&sdst, &sa->csa_peer->addr, sizeof(sdst));
	if (socket_af((struct sockaddr *)&sdst, 0) == -1) {
		log_warn("%s: invalid address", __func__);
		return (-1);
	}

	bzero(&smsg, sizeof(smsg));
	smsg.sadb_msg_version = PF_KEY_V2;
	smsg.sadb_msg_seq = sadb_msg_seq++;
	smsg.sadb_msg_pid = getpid();
	smsg.sadb_msg_len = sizeof(smsg) / 8;
	smsg.sadb_msg_type = action;
	smsg.sadb_msg_satype = satype;

	bzero(&sadb, sizeof(sadb));
	sadb.sadb_sa_len = sizeof(sadb) / 8;
	sadb.sadb_sa_exttype = SADB_EXT_SA;
	sadb.sadb_sa_spi = htonl(sa->csa_spi.spi);
	sadb.sadb_sa_state = SADB_SASTATE_MATURE;
	sadb.sadb_sa_replay = 16;

	/* XXX we don't support transport mode, yet */
	sadb.sadb_sa_flags |= SADB_X_SAFLAGS_TUNNEL;

	bzero(&sa_src, sizeof(sa_src));
	sa_src.sadb_address_len = (sizeof(sa_src) + ROUNDUP(ssrc.ss_len)) / 8;
	sa_src.sadb_address_exttype = SADB_EXT_ADDRESS_SRC;

	bzero(&sa_dst, sizeof(sa_dst));
	sa_dst.sadb_address_len = (sizeof(sa_dst) + ROUNDUP(sdst.ss_len)) / 8;
	sa_dst.sadb_address_exttype = SADB_EXT_ADDRESS_DST;

	bzero(&sa_authkey, sizeof(sa_authkey));
	bzero(&sa_enckey, sizeof(sa_enckey));
	bzero(&udpencap, sizeof udpencap);

	if (action == SADB_DELETE)
		goto send;

	/* XXX handle NULL encryption or NULL auth or combined encr/auth */
	if (action == SADB_ADD &&
	    !ibuf_length(sa->csa_integrkey) && !ibuf_length(sa->csa_encrkey) &&
	    satype != SADB_X_SATYPE_IPCOMP && satype != SADB_X_SATYPE_IPIP) {
		log_warnx("%s: no key specified", __func__);
		return (-1);
	}

	if (sa->csa_ikesa->sa_udpencap && sa->csa_ikesa->sa_natt) {
		sadb.sadb_sa_flags |= SADB_X_SAFLAGS_UDPENCAP;
		udpencap.sadb_x_udpencap_exttype = SADB_X_EXT_UDPENCAP;
		udpencap.sadb_x_udpencap_len = sizeof(udpencap) / 8;
		udpencap.sadb_x_udpencap_port =
		    sa->csa_ikesa->sa_peer.addr_port;

		log_debug("%s: udpencap port %d", __func__, 
		    ntohs(udpencap.sadb_x_udpencap_port),
		    udpencap.sadb_x_udpencap_port);
	}

	if (sa->csa_integrxf)
		if (pfkey_map(pfkey_integr,
		    sa->csa_integrxf->xform_id, &sadb.sadb_sa_auth) == -1) {
			log_warnx("%s: unsupported integrity algorithm %s",
			    __func__, print_map(sa->csa_integrxf->xform_id,
			    ikev2_xformauth_map));
			return (-1);
		}

	if (sa->csa_encrxf)
		if (pfkey_map(pfkey_encr,
		    sa->csa_encrxf->xform_id, &sadb.sadb_sa_encrypt) == -1) {
			log_warnx("%s: unsupported encryption algorithm %s",
			    __func__, print_map(sa->csa_encrxf->xform_id,
			    ikev2_xformencr_map));
			return (-1);
		}

	if (ibuf_length(sa->csa_integrkey)) {
		sa_authkey.sadb_key_len = (sizeof(sa_authkey) +
		    ((ibuf_size(sa->csa_integrkey) + 7) / 8) * 8) / 8;
		sa_authkey.sadb_key_exttype = SADB_EXT_KEY_AUTH;
		sa_authkey.sadb_key_bits =
		    8 * ibuf_size(sa->csa_integrkey);
	}

	if (ibuf_length(sa->csa_encrkey)) {
		sa_enckey.sadb_key_len = (sizeof(sa_enckey) +
		    ((ibuf_size(sa->csa_encrkey) + 7) / 8) * 8) / 8;
		sa_enckey.sadb_key_exttype = SADB_EXT_KEY_ENCRYPT;
		sa_enckey.sadb_key_bits =
		    8 * ibuf_size(sa->csa_encrkey);
	}

	/* local id */
	sa_srcid = pfkey_id2ident(sa->csa_srcid, SADB_EXT_IDENTITY_SRC);

	/* peer id */
	sa_dstid = pfkey_id2ident(sa->csa_dstid, SADB_EXT_IDENTITY_DST);

	tag = sa->csa_ikesa->sa_tag;
	if (tag != NULL && *tag != '\0') {
		bzero(&sa_tag, sizeof(sa_tag));
		sa_tag.sadb_x_tag_exttype = SADB_X_EXT_TAG;
		sa_tag.sadb_x_tag_len =
		    (ROUNDUP(strlen(tag) + 1) + sizeof(sa_tag)) / 8;
		sa_tag.sadb_x_tag_taglen = strlen(tag) + 1;
	} else
		tag = NULL;

 send:
	iov_cnt = 0;

	/* header */
	iov[iov_cnt].iov_base = &smsg;
	iov[iov_cnt].iov_len = sizeof(smsg);
	iov_cnt++;

	/* sa */
	iov[iov_cnt].iov_base = &sadb;
	iov[iov_cnt].iov_len = sizeof(sadb);
	smsg.sadb_msg_len += sadb.sadb_sa_len;
	iov_cnt++;

	/* src addr */
	iov[iov_cnt].iov_base = &sa_src;
	iov[iov_cnt].iov_len = sizeof(sa_src);
	iov_cnt++;
	iov[iov_cnt].iov_base = &ssrc;
	iov[iov_cnt].iov_len = ROUNDUP(ssrc.ss_len);
	smsg.sadb_msg_len += sa_src.sadb_address_len;
	iov_cnt++;

	/* dst addr */
	iov[iov_cnt].iov_base = &sa_dst;
	iov[iov_cnt].iov_len = sizeof(sa_dst);
	iov_cnt++;
	iov[iov_cnt].iov_base = &sdst;
	iov[iov_cnt].iov_len = ROUNDUP(sdst.ss_len);
	smsg.sadb_msg_len += sa_dst.sadb_address_len;
	iov_cnt++;

	if (udpencap.sadb_x_udpencap_len) {
		iov[iov_cnt].iov_base = &udpencap;
		iov[iov_cnt].iov_len = sizeof(udpencap);
		smsg.sadb_msg_len += udpencap.sadb_x_udpencap_len;
		iov_cnt++;
	}

	if (sa_enckey.sadb_key_len) {
		/* encryption key */
		iov[iov_cnt].iov_base = &sa_enckey;
		iov[iov_cnt].iov_len = sizeof(sa_enckey);
		iov_cnt++;
		iov[iov_cnt].iov_base = ibuf_data(sa->csa_encrkey);
		iov[iov_cnt].iov_len =
		    ((ibuf_size(sa->csa_encrkey) + 7) / 8) * 8;
		smsg.sadb_msg_len += sa_enckey.sadb_key_len;
		iov_cnt++;
	}
	if (sa_authkey.sadb_key_len) {
		/* authentication key */
		iov[iov_cnt].iov_base = &sa_authkey;
		iov[iov_cnt].iov_len = sizeof(sa_authkey);
		iov_cnt++;
		iov[iov_cnt].iov_base = ibuf_data(sa->csa_integrkey);
		iov[iov_cnt].iov_len =
		    ((ibuf_size(sa->csa_integrkey) + 7) / 8) * 8;
		smsg.sadb_msg_len += sa_authkey.sadb_key_len;
		iov_cnt++;
	}

	if (sa_srcid) {
		/* src identity */
		iov[iov_cnt].iov_base = sa_srcid;
		iov[iov_cnt].iov_len = sa_srcid->sadb_ident_len * 8;
		smsg.sadb_msg_len += sa_srcid->sadb_ident_len;
		iov_cnt++;
	}
	if (sa_dstid) {
		/* dst identity */
		iov[iov_cnt].iov_base = sa_dstid;
		iov[iov_cnt].iov_len = sa_dstid->sadb_ident_len * 8;
		smsg.sadb_msg_len += sa_dstid->sadb_ident_len;
		iov_cnt++;
	}

	if (tag != NULL) {
		/* tag identity */
		iov[iov_cnt].iov_base = &sa_tag;
		iov[iov_cnt].iov_len = sizeof(sa_tag);
		iov_cnt++;
		iov[iov_cnt].iov_base = tag;
		iov[iov_cnt].iov_len = ROUNDUP(strlen(tag) + 1);
		smsg.sadb_msg_len += sa_tag.sadb_x_tag_len;
		iov_cnt++;
	}

	return (pfkey_write(sd, &smsg, iov, iov_cnt, NULL, NULL));
}

int
pfkey_sa_getspi(int sd, u_int8_t satype, struct iked_childsa *sa,
    u_int32_t *spip)
{
	struct sadb_msg		*msg, smsg;
	struct sadb_address	 sa_src, sa_dst;
	struct sadb_sa		*sa_ext;
	struct sadb_ext		*ext;
	struct sadb_spirange	 sa_spirange;
	struct sockaddr_storage	 ssrc, sdst;
	struct iovec		 iov[IOV_CNT];
	u_int8_t		*data;
	ssize_t			 n;
	int			 iov_cnt, ret = 0;

	bzero(&ssrc, sizeof(ssrc));
	memcpy(&ssrc, &sa->csa_local->addr, sizeof(ssrc));
	if (socket_af((struct sockaddr *)&ssrc, 0) == -1) {
		log_warn("%s: invalid address", __func__);
		return (-1);
	}

	bzero(&sdst, sizeof(sdst));
	memcpy(&sdst, &sa->csa_peer->addr, sizeof(sdst));
	if (socket_af((struct sockaddr *)&sdst, 0) == -1) {
		log_warn("%s: invalid address", __func__);
		return (-1);
	}

	bzero(&smsg, sizeof(smsg));
	smsg.sadb_msg_version = PF_KEY_V2;
	smsg.sadb_msg_seq = sadb_msg_seq++;
	smsg.sadb_msg_pid = getpid();
	smsg.sadb_msg_len = sizeof(smsg) / 8;
	smsg.sadb_msg_type = SADB_GETSPI;
	smsg.sadb_msg_satype = satype;

	bzero(&sa_spirange, sizeof(sa_spirange));
	sa_spirange.sadb_spirange_exttype = SADB_EXT_SPIRANGE;
	sa_spirange.sadb_spirange_len = sizeof(sa_spirange) / 8;
	sa_spirange.sadb_spirange_min = 0x100;
	sa_spirange.sadb_spirange_max = 0xffffffff;
	sa_spirange.sadb_spirange_reserved = 0;

	bzero(&sa_src, sizeof(sa_src));
	sa_src.sadb_address_len = (sizeof(sa_src) + ROUNDUP(ssrc.ss_len)) / 8;
	sa_src.sadb_address_exttype = SADB_EXT_ADDRESS_SRC;

	bzero(&sa_dst, sizeof(sa_dst));
	sa_dst.sadb_address_len = (sizeof(sa_dst) + ROUNDUP(sdst.ss_len)) / 8;
	sa_dst.sadb_address_exttype = SADB_EXT_ADDRESS_DST;

	iov_cnt = 0;

	/* header */
	iov[iov_cnt].iov_base = &smsg;
	iov[iov_cnt].iov_len = sizeof(smsg);
	iov_cnt++;

	/* SPI range */
	iov[iov_cnt].iov_base = &sa_spirange;
	iov[iov_cnt].iov_len = sizeof(sa_spirange);
	smsg.sadb_msg_len += sa_spirange.sadb_spirange_len;
	iov_cnt++;

	/* src addr */
	iov[iov_cnt].iov_base = &sa_src;
	iov[iov_cnt].iov_len = sizeof(sa_src);
	iov_cnt++;
	iov[iov_cnt].iov_base = &ssrc;
	iov[iov_cnt].iov_len = ROUNDUP(ssrc.ss_len);
	smsg.sadb_msg_len += sa_src.sadb_address_len;
	iov_cnt++;

	/* dst addr */
	iov[iov_cnt].iov_base = &sa_dst;
	iov[iov_cnt].iov_len = sizeof(sa_dst);
	iov_cnt++;
	iov[iov_cnt].iov_base = &sdst;
	iov[iov_cnt].iov_len = ROUNDUP(sdst.ss_len);
	smsg.sadb_msg_len += sa_dst.sadb_address_len;
	iov_cnt++;

	*spip = 0;	

	if ((ret = pfkey_write(sd, &smsg, iov, iov_cnt, &data, &n)) != 0)
		return (-1);

	msg = (struct sadb_msg *)data;
	for (ext = (struct sadb_ext *)(msg + 1);
	    (size_t)((u_int8_t *)ext - (u_int8_t *)msg) <
	    msg->sadb_msg_len * PFKEYV2_CHUNK;
	    ext = (struct sadb_ext *)((u_int8_t *)ext +
	    ext->sadb_ext_len * PFKEYV2_CHUNK)) {
		if (ext->sadb_ext_type == SADB_EXT_SA) {
			sa_ext = (struct sadb_sa *)ext;
			*spip = ntohl(sa_ext->sadb_sa_spi);
			break;
		}
	}

	bzero(data, n);
	free(data);

	log_debug("%s: spi 0x%08x", __func__, *spip);

	return (ret);
}

int
pfkey_sagroup(int sd, u_int8_t satype1, u_int8_t action,
    struct iked_childsa *sa1, struct iked_childsa *sa2)
{
	struct sadb_msg		smsg;
	struct sadb_sa		sadb1, sadb2;
	struct sadb_address	sa_dst1, sa_dst2;
	struct sockaddr_storage	sdst1, sdst2;
	struct sadb_protocol	sa_proto;
	struct iovec		iov[IOV_CNT];
	int			iov_cnt;
	u_int8_t		satype2;

	if (pfkey_map(pfkey_satype, sa2->csa_saproto, &satype2) == -1)
		return (-1);

	bzero(&sdst1, sizeof(sdst1));
	memcpy(&sdst1, &sa1->csa_peer->addr, sizeof(sdst1));
	if (socket_af((struct sockaddr *)&sdst1, 0) == -1) {
		log_warnx("%s: unsupported address family %d",
		    __func__, sdst1.ss_family);
		return (-1);
	}

	bzero(&sdst2, sizeof(sdst2));
	memcpy(&sdst2, &sa2->csa_peer->addr, sizeof(sdst2));
	if (socket_af((struct sockaddr *)&sdst2, 0) == -1) {
		log_warnx("%s: unsupported address family %d",
		    __func__, sdst2.ss_family);
		return (-1);
	}

	bzero(&smsg, sizeof(smsg));
	smsg.sadb_msg_version = PF_KEY_V2;
	smsg.sadb_msg_seq = sadb_msg_seq++;
	smsg.sadb_msg_pid = getpid();
	smsg.sadb_msg_len = sizeof(smsg) / 8;
	smsg.sadb_msg_type = action;
	smsg.sadb_msg_satype = satype1;

	bzero(&sadb1, sizeof(sadb1));
	sadb1.sadb_sa_len = sizeof(sadb1) / 8;
	sadb1.sadb_sa_exttype = SADB_EXT_SA;
	sadb1.sadb_sa_spi = htonl(sa1->csa_spi.spi);
	sadb1.sadb_sa_state = SADB_SASTATE_MATURE;

	bzero(&sadb2, sizeof(sadb2));
	sadb2.sadb_sa_len = sizeof(sadb2) / 8;
	sadb2.sadb_sa_exttype = SADB_X_EXT_SA2;
	sadb2.sadb_sa_spi = htonl(sa2->csa_spi.spi);
	sadb2.sadb_sa_state = SADB_SASTATE_MATURE;
	iov_cnt = 0;

	bzero(&sa_dst1, sizeof(sa_dst1));
	sa_dst1.sadb_address_exttype = SADB_EXT_ADDRESS_DST;
	sa_dst1.sadb_address_len =
	    (sizeof(sa_dst1) + ROUNDUP(sdst1.ss_len)) / 8;

	bzero(&sa_dst2, sizeof(sa_dst2));
	sa_dst2.sadb_address_exttype = SADB_X_EXT_DST2;
	sa_dst2.sadb_address_len =
	    (sizeof(sa_dst2) + ROUNDUP(sdst2.ss_len)) / 8;

	bzero(&sa_proto, sizeof(sa_proto));
	sa_proto.sadb_protocol_exttype = SADB_X_EXT_PROTOCOL;
	sa_proto.sadb_protocol_len = sizeof(sa_proto) / 8;
	sa_proto.sadb_protocol_direction = 0;
	sa_proto.sadb_protocol_proto = satype2;

	/* header */
	iov[iov_cnt].iov_base = &smsg;
	iov[iov_cnt].iov_len = sizeof(smsg);
	iov_cnt++;

	/* sa */
	iov[iov_cnt].iov_base = &sadb1;
	iov[iov_cnt].iov_len = sizeof(sadb1);
	smsg.sadb_msg_len += sadb1.sadb_sa_len;
	iov_cnt++;

	/* dst addr */
	iov[iov_cnt].iov_base = &sa_dst1;
	iov[iov_cnt].iov_len = sizeof(sa_dst1);
	iov_cnt++;
	iov[iov_cnt].iov_base = &sdst1;
	iov[iov_cnt].iov_len = ROUNDUP(sdst1.ss_len);
	smsg.sadb_msg_len += sa_dst1.sadb_address_len;
	iov_cnt++;

	/* second sa */
	iov[iov_cnt].iov_base = &sadb2;
	iov[iov_cnt].iov_len = sizeof(sadb2);
	smsg.sadb_msg_len += sadb2.sadb_sa_len;
	iov_cnt++;

	/* second dst addr */
	iov[iov_cnt].iov_base = &sa_dst2;
	iov[iov_cnt].iov_len = sizeof(sa_dst2);
	iov_cnt++;
	iov[iov_cnt].iov_base = &sdst2;
	iov[iov_cnt].iov_len = ROUNDUP(sdst2.ss_len);
	smsg.sadb_msg_len += sa_dst2.sadb_address_len;
	iov_cnt++;

	/* SA type */
	iov[iov_cnt].iov_base = &sa_proto;
	iov[iov_cnt].iov_len = sizeof(sa_proto);
	smsg.sadb_msg_len += sa_proto.sadb_protocol_len;
	iov_cnt++;

	return (pfkey_write(sd, &smsg, iov, iov_cnt, NULL, NULL));
}

int
pfkey_write(int sd, struct sadb_msg *smsg, struct iovec *iov, int iov_cnt,
    u_int8_t **datap, ssize_t *lenp)
{
	ssize_t n, len = smsg->sadb_msg_len * 8;

	if (sadb_decoupled) {
		switch (smsg->sadb_msg_type) {
		case SADB_GETSPI:
			/* we need to get a new SPI from the kernel */
			break;
		default:
			if (datap || lenp) {
				log_warnx("%s: pfkey not coupled", __func__);
				return (-1);
			}
			/* ignore request */
			return (0);
		}
	}

	if ((n = writev(sd, iov, iov_cnt)) == -1) {
		log_warn("%s: writev failed", __func__);
		return (-1);
	} else if (n != len) {
		log_warn("%s: short write", __func__);
		return (-1);
	}

	return (pfkey_reply(sd, datap, lenp));
}

int
pfkey_reply(int sd, u_int8_t **datap, ssize_t *lenp)
{
	struct sadb_msg	 hdr;
	ssize_t		 len;
	u_int8_t	*data;

	if (recv(sd, &hdr, sizeof(hdr), MSG_PEEK) != sizeof(hdr)) {
		log_warnx("%s: short recv", __func__);
		return (-1);
	}
	len = hdr.sadb_msg_len * PFKEYV2_CHUNK;
	if ((data = malloc(len)) == NULL) {
		log_warn("%s: malloc", __func__);
		return (-1);
	}
	if (read(sd, data, len) != len) {
		log_warnx("%s: short read", __func__);
		bzero(data, len);
		free(data);
		return (-1);
	}
	if (datap) {
		*datap = data;
		if (lenp)
			*lenp = len;
	} else {
		bzero(data, len);
		free(data);
	}
	if (datap == NULL && hdr.sadb_msg_errno != 0) {
		errno = hdr.sadb_msg_errno;
		if (errno != EEXIST) {
			log_warn("%s: message", __func__);
			return (-1);
		}
	}
	return (0);
}

int
pfkey_flow_add(int fd, struct iked_flow *flow)
{
	u_int8_t	 satype;

	if (flow->flow_loaded)
		return (0);

	if (pfkey_map(pfkey_satype, flow->flow_saproto, &satype) == -1)
		return (-1);

	if (pfkey_flow(fd, satype, SADB_X_ADDFLOW, flow) == -1)
		return (-1);

	flow->flow_loaded = 1;
	return (0);
}

int
pfkey_flow_delete(int fd, struct iked_flow *flow)
{
	u_int8_t	satype;

	if (!flow->flow_loaded)
		return (0);

	if (pfkey_map(pfkey_satype, flow->flow_saproto, &satype) == -1)
		return (-1);

	if (pfkey_flow(fd, satype, SADB_X_DELFLOW, flow) == -1)
		return (-1);

	flow->flow_loaded = 0;
	return (0);
}

int
pfkey_sa_init(int fd, struct iked_childsa *sa, u_int32_t *spi)
{
	u_int8_t	satype;

	if (pfkey_map(pfkey_satype, sa->csa_saproto, &satype) == -1)
		return (-1);

	if (pfkey_sa_getspi(fd, satype, sa, spi) == -1)
		return (-1);

	log_debug("%s: new spi 0x%08x", __func__, *spi);

	return (0);
}

int
pfkey_sa_add(int fd, struct iked_childsa *sa, struct iked_childsa *last)
{
	u_int8_t	 satype;
	u_int		 cmd;

	if (pfkey_map(pfkey_satype, sa->csa_saproto, &satype) == -1)
		return (-1);

	if (sa->csa_allocated || sa->csa_loaded)
		cmd = SADB_UPDATE;
	else
		cmd = SADB_ADD;

	log_debug("%s: %s spi %s", __func__, cmd == SADB_ADD ? "add": "update",
	    print_spi(sa->csa_spi.spi, 4));

	if (pfkey_sa(fd, satype, cmd, sa) == -1) {
		if (cmd == SADB_ADD)
			(void)pfkey_sa_delete(fd, sa);
		return (-1);
	}

	if (last && cmd == SADB_ADD) {
		if (pfkey_sagroup(fd, satype,
		    SADB_X_GRPSPIS, sa, last) == -1) {
			(void)pfkey_sa_delete(fd, sa);
			return (-1);
		}
	}

	sa->csa_loaded = 1;
	return (0);
}

int
pfkey_sa_delete(int fd, struct iked_childsa *sa)
{
	u_int8_t	satype;

	if (!sa->csa_loaded || sa->csa_spi.spi == 0)
		return (0);

	if (pfkey_map(pfkey_satype, sa->csa_saproto, &satype) == -1)
		return (-1);

	if (pfkey_sa(fd, satype, SADB_DELETE, sa) == -1)
		return (-1);

	sa->csa_loaded = 0;
	return (0);
}

int
pfkey_flush(int sd)
{
	struct sadb_msg smsg;
	struct iovec	iov[IOV_CNT];
	int		iov_cnt;

	bzero(&smsg, sizeof(smsg));
	smsg.sadb_msg_version = PF_KEY_V2;
	smsg.sadb_msg_seq = sadb_msg_seq++;
	smsg.sadb_msg_pid = getpid();
	smsg.sadb_msg_len = sizeof(smsg) / 8;
	smsg.sadb_msg_type = SADB_FLUSH;
	smsg.sadb_msg_satype = SADB_SATYPE_UNSPEC;

	iov_cnt = 0;

	iov[iov_cnt].iov_base = &smsg;
	iov[iov_cnt].iov_len = sizeof(smsg);
	iov_cnt++;

	return (pfkey_write(sd, &smsg, iov, iov_cnt, NULL, NULL));
}

struct sadb_ident *
pfkey_id2ident(struct iked_id *id, u_int exttype)
{
	char			 idstr[IKED_ID_SIZE];
	u_int			 type;
	size_t			 len;
	struct sadb_ident	*sa_id;

	switch (id->id_type) {
	case IKEV2_ID_FQDN:
		type = SADB_IDENTTYPE_FQDN;
		break;
	case IKEV2_ID_UFQDN:
		type = SADB_IDENTTYPE_USERFQDN;
		break;
	case IKEV2_ID_IPV4:
	case IKEV2_ID_IPV6:
		type = SADB_IDENTTYPE_PREFIX;
		break;
	case IKEV2_ID_ASN1_DN:
	case IKEV2_ID_ASN1_GN:
	case IKEV2_ID_KEY_ID:
	case IKEV2_ID_NONE:
	default:
		/* XXX not implemented/supported by PFKEY */
		return (NULL);
	}

	bzero(&idstr, sizeof(idstr));

	if (print_id(id, idstr, sizeof(idstr)) == -1)
		return (NULL);

	len = ROUNDUP(strlen(idstr) + 1) + sizeof(*sa_id);
	if ((sa_id = calloc(1, len)) == NULL)
		return (NULL);

	strlcpy((char *)(sa_id + 1), idstr, ROUNDUP(strlen(idstr) + 1));
	sa_id->sadb_ident_type = type;
	sa_id->sadb_ident_len = len / 8;
	sa_id->sadb_ident_exttype = exttype;

	return (sa_id);
}

int
pfkey_init(void)
{
	int	 fd;

	if ((fd = socket(PF_KEY, SOCK_RAW, PF_KEY_V2)) == -1)
		fatal("pfkey_init: failed to open PF_KEY socket");

	pfkey_flush(fd);

	return (fd);
}