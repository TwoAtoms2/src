#! /bin/sh
# cygwin.sh - hints for building perl using the Cygwin environment for Win32
#

# not otherwise settable
exe_ext='.exe'
firstmakefile='GNUmakefile'
case "$ldlibpthname" in
'') ldlibpthname=PATH ;;
esac
archobjs='cygwin.o'

# mandatory (overrides incorrect defaults)
test -z "$cc" && cc='gcc'
if test -z "$plibpth"
then
    plibpth=`gcc -print-file-name=libc.a`
    plibpth=`dirname $plibpth`
    plibpth=`cd $plibpth && pwd`
fi
so='dll'
# - eliminate -lc, implied by gcc and a symlink to libcygwin.a
libswanted=`echo " $libswanted " | sed -e 's/ c / /g'`
# - eliminate -lm, symlink to libcygwin.a
libswanted=`echo " $libswanted " | sed -e 's/ m / /g'`
# - eliminate -lutil, symbols are all in libcygwin.a
libswanted=`echo " $libswanted " | sed -e 's/ util / /g'`
# - add libgdbm_compat $libswanted
# - libcygipc doesn't work much at all with
#   the Perl SysV IPC tests so not adding it --jhi 2003-08-09
#   (with cygwin 1.5.7, cygipc is deprecated in favor of the builtin cygserver)
libswanted="$libswanted gdbm_compat"
test -z "$optimize" && optimize='-O3'
man3ext='3pm'
test -z "$use64bitint" && use64bitint='define'
test -z "$usethreads" && usethreads='define'
test -z "$usemymalloc" && usemymalloc='define'
ccflags="$ccflags -DPERL_USE_SAFE_PUTENV -U__STRICT_ANSI__"
# - otherwise i686-cygwin
archname='cygwin'

# dynamic loading
# - otherwise -fpic
cccdlflags=' '
lddlflags=' --shared'
ld='g++'

case "$osvers" in

# Configure gets these wrong if the IPC server isn't yet running:
# only use for 1.5.7 and onwards
[2-9]*|1.[6-9]*|1.[1-5][0-9]*|1.5.[7-9]*|1.5.[1-6][0-9]*)
        d_semctl_semid_ds='define'
        d_semctl_semun='define'
        ;;
esac;

# compile Win32CORE "module" as static. try to avoid the space.
if test -z "$static_ext"; then
  static_ext="Win32CORE"
else
  static_ext="$static_ext Win32CORE"
fi

# Win9x problem with non-blocking read from a closed pipe
d_eofnblk='define'

# suppress auto-import warnings
ldflags="$ldflags -Wl,--enable-auto-import -Wl,--export-all-symbols -Wl,--stack,8388608 -Wl,--enable-auto-image-base"
lddlflags="$lddlflags $ldflags"

# strip exe's and dll's, better do it afterwards
#ldflags="$ldflags -s"
#ccdlflags="$ccdlflags -s"
#lddlflags="$lddlflags -s"

# IPv6 not fully implemented under Cygwin
d_inetntop='undef'
d_inetpton='undef'
