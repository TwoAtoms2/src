/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.11 1997/02/13 19:45:27 niklas Exp 
 */

char *svr4_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"read",			/* 3 = read */
	"write",			/* 4 = write */
	"open",			/* 5 = open */
	"close",			/* 6 = close */
	"wait",			/* 7 = wait */
	"creat",			/* 8 = creat */
	"link",			/* 9 = link */
	"unlink",			/* 10 = unlink */
	"execv",			/* 11 = execv */
	"chdir",			/* 12 = chdir */
	"time",			/* 13 = time */
	"mknod",			/* 14 = mknod */
	"chmod",			/* 15 = chmod */
	"chown",			/* 16 = chown */
	"break",			/* 17 = break */
	"stat",			/* 18 = stat */
	"lseek",			/* 19 = lseek */
	"getpid",			/* 20 = getpid */
	"#21 (unimplemented old_mount)",		/* 21 = unimplemented old_mount */
	"#22 (unimplemented System V umount)",		/* 22 = unimplemented System V umount */
	"setuid",			/* 23 = setuid */
	"getuid",			/* 24 = getuid */
	"#25 (unimplemented stime)",		/* 25 = unimplemented stime */
	"#26 (unimplemented ptrace)",		/* 26 = unimplemented ptrace */
	"alarm",			/* 27 = alarm */
	"fstat",			/* 28 = fstat */
	"pause",			/* 29 = pause */
	"utime",			/* 30 = utime */
	"#31 (unimplemented was stty)",		/* 31 = unimplemented was stty */
	"#32 (unimplemented was gtty)",		/* 32 = unimplemented was gtty */
	"access",			/* 33 = access */
	"#34 (unimplemented nice)",		/* 34 = unimplemented nice */
	"#35 (unimplemented statfs)",		/* 35 = unimplemented statfs */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"#38 (unimplemented fstatfs)",		/* 38 = unimplemented fstatfs */
	"pgrpsys",			/* 39 = pgrpsys */
	"#40 (unimplemented xenix)",		/* 40 = unimplemented xenix */
	"dup",			/* 41 = dup */
	"pipe",			/* 42 = pipe */
	"times",			/* 43 = times */
	"#44 (unimplemented profil)",		/* 44 = unimplemented profil */
	"#45 (unimplemented plock)",		/* 45 = unimplemented plock */
	"setgid",			/* 46 = setgid */
	"getgid",			/* 47 = getgid */
	"signal",			/* 48 = signal */
#ifdef SYSVMSG
	"msgsys",			/* 49 = msgsys */
#else
	"#49 (unimplemented msgsys)",		/* 49 = unimplemented msgsys */
#endif
	"sysarch",			/* 50 = sysarch */
	"#51 (unimplemented acct)",		/* 51 = unimplemented acct */
#ifdef SYSVSHM
	"shmsys",			/* 52 = shmsys */
#else
	"#52 (unimplemented shmsys)",		/* 52 = unimplemented shmsys */
#endif
#ifdef SYSVSEM
	"semsys",			/* 53 = semsys */
#else
	"#53 (unimplemented semsys)",		/* 53 = unimplemented semsys */
#endif
	"ioctl",			/* 54 = ioctl */
	"#55 (unimplemented uadmin)",		/* 55 = unimplemented uadmin */
	"#56 (unimplemented exch)",		/* 56 = unimplemented exch */
	"utssys",			/* 57 = utssys */
	"fsync",			/* 58 = fsync */
	"execve",			/* 59 = execve */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"fcntl",			/* 62 = fcntl */
	"ulimit",			/* 63 = ulimit */
	"#64 (unimplemented reserved for unix/pc)",		/* 64 = unimplemented reserved for unix/pc */
	"#65 (unimplemented reserved for unix/pc)",		/* 65 = unimplemented reserved for unix/pc */
	"#66 (unimplemented reserved for unix/pc)",		/* 66 = unimplemented reserved for unix/pc */
	"#67 (unimplemented reserved for unix/pc)",		/* 67 = unimplemented reserved for unix/pc */
	"#68 (unimplemented reserved for unix/pc)",		/* 68 = unimplemented reserved for unix/pc */
	"#69 (unimplemented reserved for unix/pc)",		/* 69 = unimplemented reserved for unix/pc */
	"#70 (obsolete advfs)",		/* 70 = obsolete advfs */
	"#71 (obsolete unadvfs)",		/* 71 = obsolete unadvfs */
	"#72 (obsolete rmount)",		/* 72 = obsolete rmount */
	"#73 (obsolete rumount)",		/* 73 = obsolete rumount */
	"#74 (obsolete rfstart)",		/* 74 = obsolete rfstart */
	"#75 (obsolete sigret)",		/* 75 = obsolete sigret */
#ifdef COMPAT_SVR4_NCR
	"rdebug",			/* 76 = rdebug */
#else
	"#76 (obsolete rdebug)",		/* 76 = obsolete rdebug */
#endif
	"#77 (obsolete rfstop)",		/* 77 = obsolete rfstop */
	"#78 (unimplemented rfsys)",		/* 78 = unimplemented rfsys */
	"rmdir",			/* 79 = rmdir */
	"mkdir",			/* 80 = mkdir */
	"getdents",			/* 81 = getdents */
	"#82 (obsolete libattach)",		/* 82 = obsolete libattach */
	"#83 (obsolete libdetach)",		/* 83 = obsolete libdetach */
	"#84 (unimplemented sysfs)",		/* 84 = unimplemented sysfs */
	"getmsg",			/* 85 = getmsg */
	"putmsg",			/* 86 = putmsg */
	"poll",			/* 87 = poll */
	"lstat",			/* 88 = lstat */
	"symlink",			/* 89 = symlink */
	"readlink",			/* 90 = readlink */
	"getgroups",			/* 91 = getgroups */
	"setgroups",			/* 92 = setgroups */
	"fchmod",			/* 93 = fchmod */
	"fchown",			/* 94 = fchown */
	"sigprocmask",			/* 95 = sigprocmask */
	"sigsuspend",			/* 96 = sigsuspend */
	"sigaltstack",			/* 97 = sigaltstack */
	"sigaction",			/* 98 = sigaction */
	"sigpending",			/* 99 = sigpending */
	"context",			/* 100 = context */
	"#101 (unimplemented evsys)",		/* 101 = unimplemented evsys */
	"#102 (unimplemented evtrapret)",		/* 102 = unimplemented evtrapret */
	"statvfs",			/* 103 = statvfs */
	"fstatvfs",			/* 104 = fstatvfs */
	"#105 (unimplemented)",		/* 105 = unimplemented */
	"#106 (unimplemented nfssvc)",		/* 106 = unimplemented nfssvc */
	"waitsys",			/* 107 = waitsys */
	"#108 (unimplemented sigsendsys)",		/* 108 = unimplemented sigsendsys */
	"hrtsys",			/* 109 = hrtsys */
	"#110 (unimplemented acancel)",		/* 110 = unimplemented acancel */
	"#111 (unimplemented async)",		/* 111 = unimplemented async */
	"#112 (unimplemented priocntlsys)",		/* 112 = unimplemented priocntlsys */
	"pathconf",			/* 113 = pathconf */
	"mincore",			/* 114 = mincore */
	"mmap",			/* 115 = mmap */
	"mprotect",			/* 116 = mprotect */
	"munmap",			/* 117 = munmap */
	"fpathconf",			/* 118 = fpathconf */
	"vfork",			/* 119 = vfork */
	"fchdir",			/* 120 = fchdir */
	"readv",			/* 121 = readv */
	"writev",			/* 122 = writev */
	"xstat",			/* 123 = xstat */
	"lxstat",			/* 124 = lxstat */
	"fxstat",			/* 125 = fxstat */
	"xmknod",			/* 126 = xmknod */
	"#127 (unimplemented clocal)",		/* 127 = unimplemented clocal */
	"setrlimit",			/* 128 = setrlimit */
	"getrlimit",			/* 129 = getrlimit */
	"lchown",			/* 130 = lchown */
	"memcntl",			/* 131 = memcntl */
	"#132 (unimplemented getpmsg)",		/* 132 = unimplemented getpmsg */
	"#133 (unimplemented putpmsg)",		/* 133 = unimplemented putpmsg */
	"rename",			/* 134 = rename */
	"uname",			/* 135 = uname */
	"setegid",			/* 136 = setegid */
	"sysconfig",			/* 137 = sysconfig */
	"adjtime",			/* 138 = adjtime */
	"systeminfo",			/* 139 = systeminfo */
	"#140 (unimplemented)",		/* 140 = unimplemented */
	"seteuid",			/* 141 = seteuid */
	"#142 (unimplemented vtrace)",		/* 142 = unimplemented vtrace */
	"#143 (unimplemented fork1)",		/* 143 = unimplemented fork1 */
	"#144 (unimplemented sigtimedwait)",		/* 144 = unimplemented sigtimedwait */
	"#145 (unimplemented lwp_info)",		/* 145 = unimplemented lwp_info */
	"#146 (unimplemented yield)",		/* 146 = unimplemented yield */
	"#147 (unimplemented lwp_sema_wait)",		/* 147 = unimplemented lwp_sema_wait */
	"#148 (unimplemented lwp_sema_post)",		/* 148 = unimplemented lwp_sema_post */
	"#149 (unimplemented)",		/* 149 = unimplemented */
	"#150 (unimplemented)",		/* 150 = unimplemented */
	"#151 (unimplemented)",		/* 151 = unimplemented */
	"#152 (unimplemented modctl)",		/* 152 = unimplemented modctl */
	"fchroot",			/* 153 = fchroot */
	"utimes",			/* 154 = utimes */
	"vhangup",			/* 155 = vhangup */
	"gettimeofday",			/* 156 = gettimeofday */
	"getitimer",			/* 157 = getitimer */
	"setitimer",			/* 158 = setitimer */
	"#159 (unimplemented lwp_create)",		/* 159 = unimplemented lwp_create */
	"#160 (unimplemented lwp_exit)",		/* 160 = unimplemented lwp_exit */
	"#161 (unimplemented lwp_suspend)",		/* 161 = unimplemented lwp_suspend */
	"#162 (unimplemented lwp_continue)",		/* 162 = unimplemented lwp_continue */
	"#163 (unimplemented lwp_kill)",		/* 163 = unimplemented lwp_kill */
	"#164 (unimplemented lwp_self)",		/* 164 = unimplemented lwp_self */
	"#165 (unimplemented lwp_getprivate)",		/* 165 = unimplemented lwp_getprivate */
	"#166 (unimplemented lwp_setprivate)",		/* 166 = unimplemented lwp_setprivate */
	"#167 (unimplemented lwp_wait)",		/* 167 = unimplemented lwp_wait */
	"#168 (unimplemented lwp_mutex_unlock)",		/* 168 = unimplemented lwp_mutex_unlock */
	"#169 (unimplemented lwp_mutex_lock)",		/* 169 = unimplemented lwp_mutex_lock */
	"#170 (unimplemented lwp_cond_wait)",		/* 170 = unimplemented lwp_cond_wait */
	"#171 (unimplemented lwp_cond_signal)",		/* 171 = unimplemented lwp_cond_signal */
	"#172 (unimplemented lwp_cond_broadcast)",		/* 172 = unimplemented lwp_cond_broadcast */
	"#173 (unimplemented pread)",		/* 173 = unimplemented pread */
	"#174 (unimplemented pwrite)",		/* 174 = unimplemented pwrite */
	"#175 (unimplemented llseek)",		/* 175 = unimplemented llseek */
	"#176 (unimplemented inst_sync)",		/* 176 = unimplemented inst_sync */
	"#177 (unimplemented)",		/* 177 = unimplemented */
	"#178 (unimplemented kaio)",		/* 178 = unimplemented kaio */
	"#179 (unimplemented)",		/* 179 = unimplemented */
	"#180 (unimplemented)",		/* 180 = unimplemented */
	"#181 (unimplemented)",		/* 181 = unimplemented */
	"#182 (unimplemented)",		/* 182 = unimplemented */
	"#183 (unimplemented)",		/* 183 = unimplemented */
	"#184 (unimplemented tsolsys)",		/* 184 = unimplemented tsolsys */
	"acl",			/* 185 = acl */
	"#186 (unimplemented auditsys)",		/* 186 = unimplemented auditsys */
	"#187 (unimplemented processor_bind)",		/* 187 = unimplemented processor_bind */
	"#188 (unimplemented processor_info)",		/* 188 = unimplemented processor_info */
	"#189 (unimplemented p_online)",		/* 189 = unimplemented p_online */
	"#190 (unimplemented sigqueue)",		/* 190 = unimplemented sigqueue */
	"#191 (unimplemented clock_gettime)",		/* 191 = unimplemented clock_gettime */
	"#192 (unimplemented clock_settime)",		/* 192 = unimplemented clock_settime */
	"#193 (unimplemented clock_getres)",		/* 193 = unimplemented clock_getres */
	"#194 (unimplemented timer_create)",		/* 194 = unimplemented timer_create */
	"#195 (unimplemented timer_delete)",		/* 195 = unimplemented timer_delete */
	"#196 (unimplemented timer_settime)",		/* 196 = unimplemented timer_settime */
	"#197 (unimplemented timer_gettime)",		/* 197 = unimplemented timer_gettime */
	"#198 (unimplemented timer_getoverrun)",		/* 198 = unimplemented timer_getoverrun */
	"#199 (unimplemented nanosleep)",		/* 199 = unimplemented nanosleep */
	"facl",			/* 200 = facl */
	"#201 (unimplemented door)",		/* 201 = unimplemented door */
	"setreuid",			/* 202 = setreuid */
	"setregid",			/* 203 = setregid */
	"#204 (unimplemented)",		/* 204 = unimplemented */
	"#205 (unimplemented)",		/* 205 = unimplemented */
	"#206 (unimplemented)",		/* 206 = unimplemented */
	"#207 (unimplemented)",		/* 207 = unimplemented */
	"#208 (unimplemented)",		/* 208 = unimplemented */
	"#209 (unimplemented)",		/* 209 = unimplemented */
	"#210 (unimplemented signotifywait)",		/* 210 = unimplemented signotifywait */
	"#211 (unimplemented lwp_sigredirect)",		/* 211 = unimplemented lwp_sigredirect */
	"#212 (unimplemented lwp_alarm)",		/* 212 = unimplemented lwp_alarm */
};
