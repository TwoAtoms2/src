# $OpenBSD: Makefile,v 1.10 2006/04/29 12:35:56 biorn Exp $

KRB5DIR	= ${.CURDIR}/../../kerberosV
.include <${.CURDIR}/../libkrb5/Makefile.common.inc>

LIB=	gssapi
WANTLINT=

DIR_GSSAPI=	${KRB5SRC}/lib/gssapi
GENDIR=		${.CURDIR}/generated
HDR_GSSAPI=	gssapi.h
HDR_GEN=	${GENDIR}/spnego_asn1.h

.PATH:  ${GENDIR}

#${HDR_GEN}: ${DIR_GSSAPI}/spnego.asn1
#	asn1_compile $(DIR_GSSAPI)/spnego.asn1 spnego_asn1

includes: ${HDR_GEN}
	for i in ${HDR_GEN}; do\
		echo -n " $$i"; \
		cmp -s $$i ${DESTDIR}/usr/include/kerberosV/spnego_asn1.h || \
		${INSTALL} ${INSTALL_COPY} -m 444 \
		-o ${BINOWN} -g $(BINGRP) $$i \
		${DESTDIR}/usr/include/kerberosV; done; \

	@echo -n ${DIR_GSSAPI}:; \
	cd ${DIR_GSSAPI}; for i in ${HDR_GSSAPI}; do \
		echo -n " $$i"; \
		cmp -s $$i ${DESTDIR}/usr/include/kerberosV/$$i || \
			${INSTALL} ${INSTALL_COPY} -m 444 \
			-o ${BINOWN} -g $(BINGRP) $$i \
			${DESTDIR}/usr/include/kerberosV; done; \
		echo

SRCS=	8003.c			\
	accept_sec_context.c	\
	acquire_cred.c		\
	add_cred.c		\
	add_oid_set_member.c	\
        address_to_krb5addr.c	\
	arcfour.c		\
	canonicalize_name.c	\
	ccache_name.c		\
	cfx.c			\
	compare_name.c		\
	compat.c		\
	context_time.c		\
	copy_ccache.c		\
	create_emtpy_oid_set.c	\
	decapsulate.c		\
	delete_sec_context.c	\
	display_name.c		\
	display_status.c	\
	duplicate_name.c	\
	encapsulate.c		\
	export_name.c		\
	export_sec_context.c	\
	external.c		\
	get_mic.c		\
	gssapi.h		\
	gssapi_locl.h		\
	import_name.c		\
	import_sec_context.c	\
	indicate_mechs.c	\
	init.c			\
	init_sec_context.c	\
	inquire_context.c	\
	inquire_cred.c		\
	inquire_cred_by_mech.c	\
	inquire_mechs_for_name.c \
	inquire_names_for_mech.c \
	process_context_token.c \
	release_buffer.c	\
	release_cred.c		\
	release_name.c		\
	release_oid_set.c	\
	sequence.c		\
	test_oid_set_member.c	\
	ticket_flags.c		\
	unwrap.c		\
	v1.c			\
	verify_mic.c		\
        wrap.c

gen_files= \
	asn1_ContextFlags.c	\
	asn1_MechType.c		\
	asn1_MechTypeList.c	\
	asn1_NegTokenInit.c	\
	asn1_NegTokenTarg.c	\
	asn1_NegotiationToken.c

SRCS+=${gen_files}

MAN+=	gssapi.3

MAN+=	gss_acquire_cred.3
MLINKS+=gss_acquire_cred.3 gss_accept_sec_context.3 \
	gss_acquire_cred.3 gss_add_cred.3 \
	gss_acquire_cred.3 gss_add_oid_set_member.3 \
	gss_acquire_cred.3 gss_canonicalize_name.3 \
	gss_acquire_cred.3 gss_compare_name.3 \
	gss_acquire_cred.3 gss_context_time.3 \
	gss_acquire_cred.3 gss_create_empty_oid_set.3 \
	gss_acquire_cred.3 gss_delete_sec_context.3 \
	gss_acquire_cred.3 gss_display_name.3 \
	gss_acquire_cred.3 gss_display_status.3 \
	gss_acquire_cred.3 gss_duplicate_name.3 \
	gss_acquire_cred.3 gss_export_name.3 \
	gss_acquire_cred.3 gss_export_sec_context.3 \
	gss_acquire_cred.3 gss_get_mic.3 \
	gss_acquire_cred.3 gss_import_name.3 \
	gss_acquire_cred.3 gss_import_sec_context.3 \
	gss_acquire_cred.3 gss_indicate_mechs.3 \
	gss_acquire_cred.3 gss_init_sec_context.3 \
	gss_acquire_cred.3 gss_inquire_context.3 \
	gss_acquire_cred.3 gss_inquire_cred.3 \
	gss_acquire_cred.3 gss_inquire_cred_by_mech.3 \
	gss_acquire_cred.3 gss_inquire_mechs_for_name.3 \
	gss_acquire_cred.3 gss_inquire_names_for_mech.3 \
	gss_acquire_cred.3 gss_krb5_ccache_name.3 \
	gss_acquire_cred.3 gss_krb5_compat_des3_mic.3 \
	gss_acquire_cred.3 gss_krb5_copy_ccache.3 \
	gss_acquire_cred.3 gsskrb5_extract_authz_data_from_sec_context.3 \
	gss_acquire_cred.3 gss_krb5_get_tkt_flags.3 \
	gss_acquire_cred.3 gss_process_context_token.3 \
	gss_acquire_cred.3 gss_release_buffer.3 \
	gss_acquire_cred.3 gss_release_cred.3 \
	gss_acquire_cred.3 gss_release_name.3 \
	gss_acquire_cred.3 gss_release_oid_set.3 \
	gss_acquire_cred.3 gss_seal.3 \
	gss_acquire_cred.3 gss_sign.3 \
	gss_acquire_cred.3 gss_test_oid_set_member.3 \
	gss_acquire_cred.3 gss_unseal.3 \
	gss_acquire_cred.3 gss_unwrap.3 \
	gss_acquire_cred.3 gss_verify.3 \
	gss_acquire_cred.3 gss_verify_mic.3 \
	gss_acquire_cred.3 gss_wrap.3 \
	gss_acquire_cred.3 gss_wrap_size_limit.3

.PATH:	${KRB5SRC}/lib/gssapi
CFLAGS+=-I${KRB5SRC}/lib/krb5 -I${KRB5SRC}/lib/ \
	-I${KRB5SRC}/lib/asn1 -I. \
	-DSHA1_CTX=SHA_CTX

regen:
	cd ${GENDIR}; asn1_compile $(DIR_GSSAPI)/spnego.asn1 spnego_asn1; \
		for f in *.x; do mv $$f `echo $$f | sed s,.x$$,.c,g`; done

.include <bsd.lib.mk>
