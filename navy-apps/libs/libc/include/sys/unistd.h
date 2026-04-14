#ifndef _SYS_UNISTD_H
#define _SYS_UNISTD_H
#ifdef __cplusplus
extern "C" {
#endif
#include <_ansi.h>
#include <sys/types.h>
#define __need_size_t
#include <stddef.h>

typedef int pid_t;

extern char **environ;

void	_EXFUN(_exit,(int _status ));
int	_EXFUN(access,(const char *_path, int _amode ));
unsigned  _EXFUN(alarm, (unsigned _secs ));
int     _EXFUN(chdir, (const char *_path ));
int     _EXFUN(chmod, (const char *_path, mode_t _mode ));
int     _EXFUN(chown, (const char *_path, uid_t _owner, gid_t _group ));
int     _EXFUN(close, (int _fildes ));
char    _EXFUN(*ctermid, (char *_s ));
char    _EXFUN(*cuserid, (char *_s ));
int     _EXFUN(dup, (int _fildes ));
int     _EXFUN(dup2, (int _fildes, int _fildes2 ));
int     _EXFUN(execl, (const char *_path, const char *, ... ));
int     _EXFUN(execle, (const char *_path, const char *, ... ));
int     _EXFUN(execlp, (const char *_file, const char *, ... ));
int     _EXFUN(execv, (const char *_path, char * const _argv[] ));
int     _EXFUN(execve, (const char *_path, char * const _argv[], char * const _envp[] ));
int     _EXFUN(execvp, (const char *_file, char * const _argv[] ));
pid_t   _EXFUN(fork, (void ));
pid_t   _EXFUN(vfork, (void ));
long    _EXFUN(fpathconf, (int _fd, int _name ));
char    _EXFUN(*getcwd, (char *_buf, size_t _size ));
gid_t   _EXFUN(getegid, (void ));
uid_t   _EXFUN(geteuid, (void ));
gid_t   _EXFUN(getgid, (void ));
int     _EXFUN(getgroups, (int _gidsetsize, gid_t _grouplist[] ));
char    _EXFUN(*getlogin, (void ));
pid_t   _EXFUN(getpgrp, (void ));
pid_t   _EXFUN(getpid, (void ));
pid_t   _EXFUN(getppid, (void ));
uid_t   _EXFUN(getuid, (void ));
int     _EXFUN(isatty, (int _fildes ));
int     _EXFUN(link, (const char *_path1, const char *_path2 ));
off_t   _EXFUN(lseek, (int _fildes, off_t _offset, int _whence ));
long    _EXFUN(pathconf, (char *_path, int _name ));
int     _EXFUN(pause, (void ));
int     _EXFUN(pipe, (int _fildes[2] ));
int     _EXFUN(read, (int _fildes, void *_buf, size_t _nbyte ));
int     _EXFUN(rmdir, (char *_path ));
void *  _EXFUN(sbrk,  (size_t incr));
int     _EXFUN(setgid, (gid_t _gid ));
int     _EXFUN(setpgid, (pid_t _pid, pid_t _pgid ));
pid_t   _EXFUN(setsid, (void ));
int     _EXFUN(setuid, (uid_t _uid ));
unsigned _EXFUN(sleep, (unsigned int _seconds ));
long    _EXFUN(sysconf, (int _name ));
pid_t   _EXFUN(tcgetpgrp, (int _fildes ));
int     _EXFUN(tcsetpgrp, (int _fildes, pid_t _pgrp_id ));
char    _EXFUN(*ttyname, (int _fildes ));
int     _EXFUN(unlink, (const char *_path ));
int     _EXFUN(write, (int _fildes, const void *_buf, size_t _nbyte ));

# define	F_OK	0
# define	R_OK	4
# define	W_OK	2
# define	X_OK	1

# define	SEEK_SET	0
# define	SEEK_CUR	1
# define	SEEK_END	2

#ifdef __svr4__
# define _POSIX_JOB_CONTROL	1
# define _POSIX_SAVED_IDS	1
# define _POSIX_VERSION	199009L
#endif

long _EXFUN(sysconf, (int _name));

# define	_SC_ARG_MAX	0
# define	_SC_CHILD_MAX	1
# define	_SC_CLK_TCK	2
# define	_SC_NGROUPS_MAX	3
# define	_SC_OPEN_MAX	4
/* no _SC_STREAM_MAX */
# define	_SC_JOB_CONTROL	5
# define	_SC_SAVED_IDS	6
# define	_SC_VERSION	7

# define	_PC_LINK_MAX	0
# define	_PC_MAX_CANON	1
# define	_PC_MAX_INPUT	2
# define	_PC_NAME_MAX	3
# define	_PC_PATH_MAX	4
# define	_PC_PIPE_BUF	5
# define	_PC_CHOWN_RESTRICTED	6
# define	_PC_NO_TRUNC	7
# define	_PC_VDISABLE	8

# ifndef	_POSIX_SOURCE
#  define	MAXNAMLEN	1024
# endif		/* _POSIX_SOURCE */

# define	MAXPATHLEN	1024

#ifdef __cplusplus
}
#endif
#endif
