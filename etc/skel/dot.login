# $OpenBSD: dot.login,v 1.4 2009/01/30 08:42:26 sobrado Exp $
#
# csh login file

if ( ! $?TERMCAP ) then
	tset -Q '-mdialup:?vt100' $TERM
endif

stty	newcrt crterase

set	savehist=100
set	ignoreeof

setenv	EXINIT		'set ai sm noeb'
setenv	HOSTALIASES	 $HOME/.hostaliases

if (-x /usr/games/fortune) /usr/games/fortune
