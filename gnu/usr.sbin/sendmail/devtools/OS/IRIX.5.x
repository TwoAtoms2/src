#	$Sendmail: IRIX.5.x,v 8.15 2001/02/10 18:19:47 ca Exp $
define(`confCC', `cc -mips2 -OPT:Olimit=1400')
define(`confMAPDEF', `-DNDBM -DNIS')
define(`confLIBS', `-lmld -lmalloc')
define(`confSM_OS_HEADER', `sm_os_irix')
define(`confMBINDIR', `/usr/lib')
define(`confSBINDIR', `/usr/etc')
define(`confUBINDIR', `/usr/bsd')
define(`confEBINDIR', `/usr/lib')
define(`confSBINGRP', `sys')
define(`confSTDIR', `/var')
define(`confINSTALL', `${BUILDBIN}/install.sh')
define(`confDEPEND_TYPE', `CC-M')
