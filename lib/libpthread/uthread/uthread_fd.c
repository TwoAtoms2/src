/*	$OpenBSD: uthread_fd.c,v 1.33 2010/01/03 23:05:35 fgsch Exp $	*/
/*
 * Copyright (c) 1995-1998 John Birrell <jb@cimlogic.com.au>
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by John Birrell.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOHN BIRRELL AND CONTRIBUTORS ``AS IS'' AND
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
 * $FreeBSD: uthread_fd.c,v 1.13 1999/08/28 00:03:31 peter Exp $
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#ifdef _THREAD_SAFE
#include <pthread.h>
#include "pthread_private.h"

/* Static variables: */

static struct fd_table_entry	*_thread_fd_entries;
static struct fs_flags		*_thread_fd_flags;

static SLIST_HEAD(, fd_table_entry)	_thread_fd_entries_head;
static SLIST_HEAD(, fs_flags)		_thread_fd_flags_head;

int
_thread_fd_init_mem(void)
{
	int fd;

	_thread_fd_entries = calloc((size_t)_thread_max_fdtsize,
				  sizeof(struct fd_table_entry));
	if (_thread_fd_entries == NULL)
		return (-1);

	_thread_fd_flags = calloc((size_t)_thread_max_fdtsize,
				  sizeof(struct fs_flags));
	if (_thread_fd_flags == NULL) {
		free(_thread_fd_entries);
		_thread_fd_entries = NULL;
		return (-1);
	}

	/* add pre-allocated entries to freelists */
	for (fd = 0; fd < _thread_max_fdtsize; fd++)
	{
		SLIST_INSERT_HEAD(&_thread_fd_entries_head, &_thread_fd_entries[fd], fe);
		SLIST_INSERT_HEAD(&_thread_fd_flags_head, &_thread_fd_flags[fd], fe);
        }

	return (0);
}

/*
 * Build a new fd entry and return it.
 */
static struct fs_flags *
_thread_fs_flags_entry(void)
{
	struct fs_flags *entry;

        _thread_kern_sig_defer();
        entry = SLIST_FIRST(&_thread_fd_flags_head);
	if (entry != NULL) {
		SLIST_REMOVE_HEAD(&_thread_fd_flags_head, fe);
		memset(entry, 0, sizeof *entry);
	}
        _thread_kern_sig_undefer();
	return entry;
}

static void
_thread_fs_flags_free(struct fs_flags *entry)
{
        _thread_kern_sig_defer();
	SLIST_INSERT_HEAD(&_thread_fd_flags_head, entry, fe);
        _thread_kern_sig_undefer();
}

/*
 * Initialize a new status_flags entry and set system
 * file descriptor non-blocking.
 */
static int
_thread_fs_flags_init(struct fs_flags *status_flags, int fd)
{
	int ret = 0;
	int saved_errno;

	status_flags->flags = _thread_sys_fcntl(fd, F_GETFL, 0);
	if (status_flags->flags == -1)
		/* use the errno fcntl returned */
		ret = -1;
	else {
		/*
		 * Make the file descriptor non-blocking.
		 * This might fail if the device driver does
		 * not support non-blocking calls, or if the
		 * driver is naturally non-blocking.
		 */
		if ((status_flags->flags & O_NONBLOCK) == 0) {
			saved_errno = errno;
			_thread_sys_fcntl(fd, F_SETFL,
				  status_flags->flags | O_NONBLOCK);
			errno = saved_errno;
		}
	}

	return (ret);
}

/*
 * If existing entry's status_flags don't match new one,
 * then replace the current status flags with the new one.
 * It is assumed the entry is locked with a FD_RDWR_CLOSE
 * lock when this function is called.
 */
void
_thread_fs_flags_replace(int fd, struct fs_flags *new_status_flags)
{
	struct fd_table_entry *entry = _thread_fd_table[fd];
	struct fs_flags *old_status_flags;
	struct stat sb;
	int flags;
	int saved_errno;

        _thread_kern_sig_defer();
	if (entry->status_flags != new_status_flags) {
		if (entry->status_flags != NULL) {
			old_status_flags = entry->status_flags;
			old_status_flags->refcnt -= 1;
			if (old_status_flags->refcnt <= 0) {
				/*
				 * Check if the file should be left as blocking.
				 *
				 * This is so that the file descriptors shared with a parent
				 * process aren't left set to non-blocking if the child
				 * closes them prior to exit.  An example where this causes
				 * problems with /bin/sh is when a child closes stdin.
				 *
				 * Setting a file as blocking causes problems if a threaded
				 * parent accesses the file descriptor before the child exits.
				 * Once the threaded parent receives a SIGCHLD then it resets
				 * all of its files to non-blocking, and so it is then safe
				 * to access them.
				 *
				 * Pipes are not set to blocking when they are closed, as
				 * the parent and child will normally close the file
				 * descriptor of the end of the pipe that they are not
				 * using, which would then cause any reads to block
				 * indefinitely. However, stdin/out/err will be reset
				 * to avoid leaving them as non-blocking indefinitely.
				 *
				 * Files that we cannot fstat are probably not regular
				 * so we don't bother with them.
				 *
				 * Also don't reset fd to blocking if we are replacing
				 * the status flags with a shared version.
				 */
				saved_errno = errno;
				if (new_status_flags == NULL &&
				    (old_status_flags->flags & O_NONBLOCK) == 0 &&
				    (fd < 3 || (_thread_sys_fstat(fd, &sb) == 0 &&
				    (S_ISREG(sb.st_mode) || S_ISCHR(sb.st_mode)))))
				{
					/* Get the current flags: */
					flags = _thread_sys_fcntl(fd, F_GETFL, NULL);
					/* Clear the nonblocking file descriptor flag: */
					_thread_sys_fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
				}
				_thread_fs_flags_free(old_status_flags);
				errno = saved_errno;
			}
		}
		/* replace with new status flags */
		if (new_status_flags != NULL) {
			new_status_flags->refcnt += 1;
		}
		entry->status_flags = new_status_flags;
	}
        _thread_kern_sig_undefer();
}

/*
 * Build a new fd entry and return it.
 */
static struct fd_table_entry *
_thread_fd_entry(void)
{
	struct fd_table_entry *entry;

        _thread_kern_sig_defer();
        entry = SLIST_FIRST(&_thread_fd_entries_head);
	if (entry != NULL) {
		SLIST_REMOVE_HEAD(&_thread_fd_entries_head, fe);
		memset(entry, 0, sizeof *entry);
		TAILQ_INIT(&entry->r_queue);
		TAILQ_INIT(&entry->w_queue);
		entry->state = FD_ENTRY_CLOSED;
		entry->init_mode = FD_INIT_UNKNOWN;
	}
        _thread_kern_sig_undefer();
	return entry;
}

static void
_thread_fs_entry_free(struct fd_table_entry *entry)
{
        _thread_kern_sig_defer();
	SLIST_INSERT_HEAD(&_thread_fd_entries_head, entry, fe);
        _thread_kern_sig_undefer();
}

/*
 * Initialize the thread fd table for dup-ed fds, typically the stdio
 * fds.
 */

void
_thread_fd_init(void)
{
	int saved_errno;
	int fd;
	int fd2;
	int flag;
	int *flags;
	struct fd_table_entry *entry1, *entry2;
	struct fs_flags *status_flags;

        _thread_fd_init_mem();

	saved_errno = errno;
	flags = calloc((size_t)_thread_init_fdtsize, sizeof *flags);
	if (flags == NULL)
		PANIC("Cannot allocate memory for flags table");

	/* read the current file flags */
	for (fd = 0; fd < _thread_init_fdtsize; fd += 1)
		flags[fd] = _thread_sys_fcntl(fd, F_GETFL, 0);

	/*
	 * Now toggle the sync flags and see what other fd's
	 * change.   Those are the dup-ed fd's.   Dup-ed fd's are
	 * added to the table, all others are NOT added to the
	 * table.  They MUST NOT be added as the fds may belong
	 * to dlopen.   As dlclose doesn't go through the thread code
	 * so the entries would never be cleaned.
	 */

        _thread_kern_sig_defer();
	for (fd = 0; fd < _thread_init_fdtsize; fd += 1) {
		if (flags[fd] == -1)
			continue;
		entry1 = _thread_fd_entry();
		status_flags = _thread_fs_flags_entry();
		if (entry1 != NULL && status_flags != NULL) {
			_thread_sys_fcntl(fd, F_SETFL,
					  flags[fd] ^ O_SYNC);
			for (fd2 = fd + 1; fd2 < _thread_init_fdtsize; fd2 += 1) {
				if (flags[fd2] == -1)
					continue;
				flag = _thread_sys_fcntl(fd2, F_GETFL, 0);
				if (flag != flags[fd2]) {
					entry2 = _thread_fd_entry();
					if (entry2 != NULL) {
						status_flags->refcnt += 1;
						entry2->status_flags = status_flags;
						entry2->state = FD_ENTRY_OPEN;
						entry2->init_mode = FD_INIT_DUP2;
						_thread_fd_table[fd2] = entry2;
					} else
						PANIC("Cannot allocate memory for flags table");
					flags[fd2] = -1;
				}
			}
			if (status_flags->refcnt) {
				status_flags->refcnt += 1;
				status_flags->flags = flags[fd];
				entry1->status_flags = status_flags;
				entry1->state = FD_ENTRY_OPEN;
				entry1->init_mode = FD_INIT_DUP2;
				_thread_fd_table[fd] = entry1;
				flags[fd] |= O_NONBLOCK;
			} else {
				_thread_fs_entry_free(entry1);
				_thread_fs_flags_free(status_flags);
			}
		} else {
			PANIC("Cannot allocate memory for flags table");
		}
	}
        _thread_kern_sig_undefer();

	/* lastly, restore the file flags.   Flags for files that we
	   know to be duped have been modified so set the non-blocking'
	   flag.  Other files will be set to non-blocking when the
	   thread code is forced to take notice of the file. */
	for (fd = 0; fd < _thread_init_fdtsize; fd += 1)
		if (flags[fd] != -1)
			_thread_sys_fcntl(fd, F_SETFL, flags[fd]);

	free(flags);
	errno = saved_errno;
}

/*
 * Initialize the fd_table entry for the given fd.
 *
 * This function *must* return -1 and set the thread specific errno
 * as a system call. This is because the error return from this
 * function is propagated directly back from thread-wrapped system
 * calls.
 */
int
_thread_fd_table_init(int fd, enum fd_entry_mode init_mode, struct fs_flags *status_flags)
{
	int	ret = 0;
	int	saved_errno;
	struct fd_table_entry *entry;
	struct fs_flags *new_status_flags;

	if (fd < 0 || fd >= _thread_max_fdtsize) {
		/*
		 * file descriptor is out of range, Return a bad file
		 * descriptor error:
		 */ 
		errno = EBADF;
		return (-1);
	}
	
	if (_thread_fd_table[fd] == NULL) {
		/* First time for this fd, build an entry */
		entry = _thread_fd_entry();
		if (entry == NULL) {
			/* use _thread_fd_entry errno */
			ret = -1;
		} else {
			/* Protect the file descriptor table: */
        		_thread_kern_sig_defer();

			/*
			 * Check if another thread allocated the
			 * file descriptor entry while this thread
			 * was doing the same thing. The table wasn't
			 * kept locked during this operation because
			 * it has the potential to recurse.
			 */
			if (_thread_fd_table[fd] == NULL) {
				/* This thread wins: */
				_thread_fd_table[fd] = entry;
				entry = NULL;
			}

			/* Unprotect the file descriptor table: */
        		_thread_kern_sig_undefer();

			/*
			 * If another thread initialized the table entry
			 * throw the new entry away.
			 */
			if (entry != NULL)
				_thread_fs_entry_free(entry);
		}
	}

	if (ret == 0) {
		entry = _thread_fd_table[fd];
       		_thread_kern_sig_defer();
		switch (init_mode) {
		case FD_INIT_UNKNOWN:
			/*
			 * If the entry is closed, try to open it
			 * anyway since we may have inherited it or
			 * it may have been created by an unwrapped
			 * call such as openpty(3). Since we allow
			 * FD_RDWR_CLOSE locks on closed entries,
			 * we ignore EBADF status flags errors and
			 * return a closed entry. If the entry is
			 * not closed then there's nothing to do.
			 */
			if (entry->state == FD_ENTRY_CLOSED) {
				new_status_flags = _thread_fs_flags_entry();
				if (new_status_flags == NULL) {
					/* use _thread_fs_flags_entry errno */
					ret = -1;
				} else {
					saved_errno = errno;
					ret = _thread_fs_flags_init(new_status_flags, fd);
					if (ret == 0) {
						errno = saved_errno;
						new_status_flags->refcnt = 1;
						entry->status_flags = new_status_flags;
						new_status_flags = NULL;
						entry->state = FD_ENTRY_OPEN;
						entry->init_mode = init_mode;
					} else if (errno == EBADF) {
						errno = saved_errno;
						ret = 0;
					}
				}
				/* if flags init failed free new flags */
				if (new_status_flags != NULL)
					_thread_fs_flags_free(new_status_flags);
			}
			break;
		case FD_INIT_NEW:
			/*
			 * If the entry was initialized and opened
			 * by another thread (i.e. FD_INIT_DUP2 or
			 * FD_INIT_UNKNOWN), the status flags will
			 * be correct.
			 */
			if (entry->state == FD_ENTRY_CLOSED) {
				new_status_flags = _thread_fs_flags_entry();
				if (new_status_flags == NULL) {
					/* use _thread_fs_flags_entry errno */
					ret = -1;
				} else {
					ret = _thread_fs_flags_init(new_status_flags, fd);
				}
				if (ret == 0) {
					new_status_flags->refcnt = 1;
					entry->status_flags = new_status_flags;
					new_status_flags = NULL;
					entry->state = FD_ENTRY_OPEN;
					entry->init_mode = init_mode;
				}
				/* if flags init failed free new flags */
				if (new_status_flags != NULL)
					_thread_fs_flags_free(new_status_flags);
			}
			break;
		case FD_INIT_BLOCKING:
			/*
			 * If the entry was initialized and opened
			 * by another thread with FD_INIT_DUP2, the
			 * status flags will be correct. However,
			 * if FD_INIT_UNKNOWN raced in before us
			 * it means the app is not well behaved and
			 * tried to use the fd before it was returned
			 * to the client.
			 */
			if (entry->state == FD_ENTRY_CLOSED) {
				new_status_flags = _thread_fs_flags_entry();
				if (new_status_flags == NULL) {
					/* use _thread_fs_flags_entry errno */
					ret = -1;
				} else {
					ret = _thread_fs_flags_init(new_status_flags, fd);
				}
				if (ret == 0) {
					/* set user's view of status flags to blocking */
					new_status_flags->flags &= ~O_NONBLOCK;
					new_status_flags->refcnt = 1;
					entry->status_flags = new_status_flags;
					new_status_flags = NULL;
					entry->state = FD_ENTRY_OPEN;
					entry->init_mode = init_mode;
				}
				/* if flags init failed free new flags */
				if (new_status_flags != NULL)
					_thread_fs_flags_free(new_status_flags);
			} else if (entry->state == FD_ENTRY_OPEN &&
			    entry->init_mode == FD_INIT_UNKNOWN) {
				entry->status_flags->flags &= ~O_NONBLOCK;
			}
			break;
		case FD_INIT_DUP:
			/*
			 * If the entry was initialized and opened
			 * by another thread with FD_INIT_DUP2 then
			 * keep it. However, if FD_INIT_UNKNOWN raced
			 * in before us it means the app is not well
			 * behaved and tried to use the fd before it
			 * was returned to the client.
			 */
			if (entry->state == FD_ENTRY_CLOSED) {
				_thread_fs_flags_replace(fd, status_flags);
				entry->state = FD_ENTRY_OPEN;
				entry->init_mode = init_mode;
			} else if (entry->state == FD_ENTRY_OPEN &&
			    entry->init_mode == FD_INIT_UNKNOWN) {
				_thread_fs_flags_replace(fd, status_flags);
			}
			break;
		case FD_INIT_DUP2:
			/*
			 * This is only called when FD_RDWR_CLOSE
			 * is held and in state FD_ENTRY_CLOSING.
			 * Just replace flags and open entry.
			 * FD_INIT_UNKNOWN can't race in since we
			 * are in state FD_ENTRY_CLOSING before
			 * the _thread_sys_dup2 happens.
			 */
			_thread_fs_flags_replace(fd, status_flags);
			entry->state = FD_ENTRY_OPEN;
			entry->init_mode = init_mode;
			break;
		}
       		_thread_kern_sig_undefer();
	}

	/* Return the completion status: */
	return (ret);
}

/*
 * Close an fd entry. Replace existing status flags
 * with NULL. The entry is assummed to be locked with
 * a FD_RDWR_CLOSE lock and in state FD_ENTRY_CLOSING.
 */
void
_thread_fd_entry_close(int fd)
{
	_thread_fs_flags_replace(fd, NULL);
	_thread_fd_table[fd]->state = FD_ENTRY_CLOSED;
}

/*
 * Unlock an fd table entry for the given fd and lock type.
 */
void
_thread_fd_unlock(int fd, int lock_type)
{
	struct pthread *thread = _get_curthread();
	struct fd_table_entry *entry;

	/*
	 * If file descriptor is out of range or uninitialized,
	 * do nothing.
	 */ 
	if (fd >= 0 && fd < _thread_max_fdtsize && _thread_fd_table[fd] != NULL) {
		entry = _thread_fd_table[fd];

		/*
		 * Defer signals to protect the scheduling queues from
		 * access by the signal handler:
		 */
		_thread_kern_sig_defer();

		/* Check if the running thread owns the read lock: */
		if (entry->r_owner == thread &&
		    (lock_type & FD_READ)) {
			/*
			 * Decrement the read lock count for the
			 * running thread: 
			 */
			entry->r_lockcount--;
			if (entry->r_lockcount == 0) {
				/*
				 * no read locks, dequeue any threads
				 * waiting for a read lock
				 */
				entry->r_owner = TAILQ_FIRST(&entry->r_queue);
				if (entry->r_owner != NULL) {
					TAILQ_REMOVE(&entry->r_queue,
						     entry->r_owner, qe);

					/*
					 * Set the state of the new owner of
					 * the thread to running:  
					 */
					PTHREAD_NEW_STATE(entry->r_owner,
							  PS_RUNNING);

					/*
					 * Reset the number of read locks.
					 * This will be incremented by the new
					 * owner of the lock when it sees that
					 *it has the lock.
					 */
					entry->r_lockcount = 0;
				}
			}

		}
		/* Check if the running thread owns the write lock: */
		if (entry->w_owner == thread &&
		    (lock_type & FD_WRITE)) {
			/*
			 * Decrement the write lock count for the
			 * running thread: 
			 */
			entry->w_lockcount--;
			if (entry->w_lockcount == 0) {
				/*
				 * no write locks, dequeue any threads
				 * waiting on a write lock.
				 */
				entry->w_owner = TAILQ_FIRST(&entry->w_queue);
				if (entry->w_owner != NULL) {
					/* Remove this thread from the queue: */
					TAILQ_REMOVE(&entry->w_queue,
						     entry->w_owner, qe);

					/*
					 * Set the state of the new owner of
					 * the thread to running: 
					 */
					PTHREAD_NEW_STATE(entry->w_owner,
							  PS_RUNNING);

					/*
					 * Reset the number of write locks.
					 * This will be incremented by the
					 * new owner of the lock when it  
					 * sees that it has the lock.
					 */
					entry->w_lockcount = 0;
				}
			}
		}

		/*
		 * Undefer and handle pending signals, yielding if
		 * necessary:
		 */
		_thread_kern_sig_undefer();
	}
}

/*
 * Lock an fd table entry for the given fd and lock type.
 */
int
_thread_fd_lock(int fd, int lock_type, struct timespec * timeout)
{
	struct pthread	*curthread = _get_curthread();
	struct fd_table_entry *entry;
	int	ret;

	/*
	 * Check that the file descriptor table is initialised for this
	 * entry: 
	 */
	ret = _thread_fd_table_init(fd, FD_INIT_UNKNOWN, NULL);
	if (ret == 0) {
		entry = _thread_fd_table[fd];

		/*
		 * Protect the file descriptor table entry to prevent
		 * other threads for clashing with the current
		 * thread's accesses:
		 */
		_thread_kern_sig_defer();

		/* reject all new locks on entries that are closing */
		if (entry->state == FD_ENTRY_CLOSING) {
			ret = -1;
			errno = EBADF;
		} else if (lock_type == FD_RDWR_CLOSE) {
			/* allow closing locks on open and closed entries */
			entry->state = FD_ENTRY_CLOSING;
		} else if (entry->state == FD_ENTRY_CLOSED) {
			ret = -1;
			errno = EBADF;
		}

		/* Handle read locks */
		if (ret == 0 && (lock_type & FD_READ)) {
			/*
			 * Enter a loop to wait for the file descriptor to be
			 * locked    for read for the current thread: 
			 */
			while (entry->r_owner != curthread) {
				/*
				 * Check if the file descriptor is locked by
				 * another thread: 
				 */
				if (entry->r_owner != NULL) {
					/*
					 * Another thread has locked the file
					 * descriptor for read, so join the
					 * queue of threads waiting for a  
					 * read lock on this file descriptor: 
					 */
					TAILQ_INSERT_TAIL(&entry->r_queue,
							  curthread, qe);

					/*
					 * Save the file descriptor details
					 * in the thread structure for the
					 * running thread: 
					 */
					curthread->data.fd.fd = fd;

					/* Set the timeout: */
					_thread_kern_set_timeout(timeout);

					/*
					 * Unprotect the file descriptor
					 * table entry:
					 */
					_thread_kern_sig_undefer();

					/*
					 * Schedule this thread to wait on
					 * the read lock. It will only be
					 * woken when it becomes the next in
					 * the queue and is granted access
					 * to the lock by the thread that is
					 * unlocking the file descriptor.
					 */
					_thread_kern_sched_state(PS_FDLR_WAIT,
								 __FILE__,
								 __LINE__);

					/*
					 * Protect the file descriptor
					 * table entry again:
					 */
					_thread_kern_sig_defer();

				} else {
					/*
					 * The running thread now owns the
					 * read lock on this file descriptor: 
					 */
					entry->r_owner = curthread;

					/*
					 * Reset the number of read locks for
					 * this file descriptor: 
					 */
					entry->r_lockcount = 0;
				}
			}

			/* Increment the read lock count: */
			entry->r_lockcount++;
		}

		/* Handle write locks */
		if (ret == 0 && (lock_type & FD_WRITE)) {
			/*
			 * Enter a loop to wait for the file descriptor to be
			 * locked for write for the current thread: 
			 */
			while (entry->w_owner != curthread) {
				/*
				 * Check if the file descriptor is locked by
				 * another thread: 
				 */
				if (entry->w_owner != NULL) {
					/*
					 * Another thread has locked the file
					 * descriptor for write, so join the
					 * queue of threads waiting for a 
					 * write lock on this file
					 * descriptor: 
					 */
					TAILQ_INSERT_TAIL(&entry->w_queue,
							  curthread, qe);

					/*
					 * Save the file descriptor details
					 * in the thread structure for the
					 * running thread: 
					 */
					curthread->data.fd.fd = fd;

					/* Set the timeout: */
					_thread_kern_set_timeout(timeout);

					/*
					 * Unprotect the file descriptor
					 * table entry:
					 */
					_thread_kern_sig_undefer();

					/*
					 * Schedule this thread to wait on
					 * the write lock. It will only be
					 * woken when it becomes the next in
					 * the queue and is granted access to
					 * the lock by the thread that is
					 * unlocking the file descriptor.
					 */
					_thread_kern_sched_state(PS_FDLW_WAIT,
								 __FILE__,
								 __LINE__);

					/*
					 * Unprotect the file descriptor
					 * table entry again:
					 */
					_thread_kern_sig_defer();
				} else {
					/*
					 * The running thread now owns the
					 * write lock on this file descriptor: 
					 */
					entry->w_owner = curthread;

					/*
					 * Reset the number of write locks
					 * for this file descriptor: 
					 */
					entry->w_lockcount = 0;
				}
			}

			/* Increment the write lock count: */
			entry->w_lockcount++;
		}

		/* Unprotect the file descriptor table entry: */
		_thread_kern_sig_undefer();
	}

	/* Return the completion status: */
	return (ret);
}

struct timespec *
_thread_fd_timeout(int fd, int which)
{
	struct timeval tv;
	socklen_t len;
	int saved_errno;
	
	/* Avoid calling getsockopt if fd is not a socket. */
	if (!(_thread_fd_table[fd]->status_flags->flags & _FD_NOTSOCK)) {
		len = sizeof(tv);
		saved_errno = errno;
		if (_thread_sys_getsockopt(fd, SOL_SOCKET, which ?
		    SO_SNDTIMEO : SO_RCVTIMEO, &tv, &len) == 0) {
			if (timerisset(&tv)) {
				static struct timespec ts;
				TIMEVAL_TO_TIMESPEC(&tv, &ts);
				return (&ts);
			}
		} else if (errno == ENOTSOCK)
			_thread_fd_table[fd]->status_flags->flags |=
			    _FD_NOTSOCK;
		errno = saved_errno;
	}
	return (NULL);
}

#endif
