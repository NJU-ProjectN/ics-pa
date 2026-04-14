/* sys/signal.h */

/*
 * This file serves two purposes:
 * 1) Define the various signals and supported cast.
 * 2) Define _MAX_SIGNALS.
 *
 * _MAX_SIGNALS is needed by sys/reent.h which #define's "__need__MAX_SIGNALS"
 * before including this file.  We #undef it when we're done so further
 * includes of this file will get the regular contents.
 */

#ifdef __need__MAX_SIGNALS

#ifndef _MAX_SIGNALS
#ifdef ___AM29K__
#define _MAX_SIGNALS 2
#endif
#if defined (__i386__) && ! defined (__unix__)	/* go32 */
#define _MAX_SIGNALS 13
#endif
#ifndef _MAX_SIGNALS	/* 32 is used for __svr4__ as well (kiss) */
#define _MAX_SIGNALS 32
#endif
#endif /* ! _MAX_SIGNALS */

#undef __need__MAX_SIGNALS

#else /* ! __need__MAX_SIGNALS */

#ifndef _SYS_SIGNAL_H

#ifdef ___AM29K__
#define _SYS_SIGNAL_H
/* These all need to be defined for ANSI C, but I don't think they are
   meaningful.  */
#define SIGABRT 1
#define SIGFPE 1
#define SIGILL 1
#define SIGINT 1
#define SIGSEGV 1
#define SIGTERM 1
/* These need to be defined for POSIX, and some others do too.  */
#define SIGHUP 1
#define SIGQUIT 1
#define NSIG 2
#endif

#ifdef __GO32__
#define SIGINT  1
#define SIGKILL 2
#define SIGPIPE 3
#define SIGFPE  4
#define SIGHUP  5
#define SIGTERM 6
#define SIGSEGV 7
#define SIGTSTP 8
#define SIGQUIT 9
#define SIGTRAP 10
#define SIGILL  11
#define SIGEMT  12
#define SIGALRM 13
#define SIGBUS  14
#define SIGLOST 15
#define SIGSTOP 16

#define SIGABRT 17
#define SIGUSR1	18
#define SIGUSR2	19

#define NSIG    20

#define _SYS_SIGNAL_H
#endif

#ifndef _SYS_SIGNAL_H
#define _SYS_SIGNAL_H
#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt */
#define	SIGQUIT	3	/* quit */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGIOT	6	/* IOT instruction */
#define	SIGABRT 6	/* used by abort, replace SIGIOT in the future */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#ifndef __svr4__
#define	SIGURG	16	/* urgent condition on IO channel */
#define	SIGSTOP	17	/* sendable stop signal not from tty */
#define	SIGTSTP	18	/* stop signal from tty */
#define	SIGCONT	19	/* continue a stopped process */
#define	SIGCHLD	20	/* to parent on child stop or exit */
#define	SIGCLD	20	/* System V name for SIGCHLD */
#define	SIGTTIN	21	/* to readers pgrp upon background tty read */
#define	SIGTTOU	22	/* like TTIN for output if (tp->t_local&LTOSTOP) */
#define	SIGIO	23	/* input/output possible signal */
#define	SIGPOLL	SIGIO	/* System V name for SIGIO */
#define	SIGXCPU	24	/* exceeded CPU time limit */
#define	SIGXFSZ	25	/* exceeded file size limit */
#define	SIGVTALRM 26	/* virtual time alarm */
#define	SIGPROF	27	/* profiling time alarm */
#define	SIGWINCH 28	/* window changed */
#define	SIGLOST 29	/* resource lost (eg, record-lock lost) */
#define	SIGUSR1 30	/* user defined signal 1 */
#define	SIGUSR2 31	/* user defined signal 2 */

#define NSIG	32      /* signal 0 implied */
#else
/* svr4 specifics. different signals above 15, and sigaction. */
#define	SIGUSR1	16
#define SIGUSR2	17
#define SIGCLD	18
#define	SIGPWR	19
#define SIGWINCH 20

#define	SIGPOLL	22	/* 20 for x.out binaries!!!! */

#define	SIGSTOP	23	/* sendable stop signal not from tty */
#define	SIGTSTP	24	/* stop signal from tty */
#define	SIGCONT	25	/* continue a stopped process */
#define	SIGTTIN	26	/* to readers pgrp upon background tty read */
#define	SIGTTOU	27	/* like TTIN for output if (tp->t_local&LTOSTOP) */

#define NSIG	28	/* 1 - (NSIG-1) */

typedef unsigned long sigset_t;	/* yes, limited to 32 signals. *sigh* */

struct sigaction {
	void (*sa_handler)();
	sigset_t sa_mask;
	int sa_flags;
};

#define SA_NOCLDSTOP 1	/* only value supported now for sa_flags */

#define SIG_SETMASK 0	/* set mask with sigprocmask() */
#define SIG_BLOCK 1	/* set of signals to block */
#define SIG_UNBLOCK 2	/* set of signals to, well, unblock */

#endif /* __svr4__ */
#endif /* _SYS_SIGNAL_H */
#endif /* _SYS_SIGNAL_H */
#endif /* ! __need__MAX_SIGNALS */
