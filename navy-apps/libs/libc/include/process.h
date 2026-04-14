#ifndef __PROCESS_H_
#define __PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

int execl(const char *path, const char *argv0, ...);
int execle(const char *path, const char *argv0, ... /*, char * const *envp */);
int execlp(const char *path, const char *argv0, ...);
int execlpe(const char *path, const char *argv0, ... /*, char * const *envp */);

int execv(const char *path, char * const *argv);
int execve(const char *path, char * const *argv, char * const *envp);
int execvp(const char *path, char * const *argv);
int execvpe(const char *path, char * const *argv, char * const *envp);

int spawnl(int mode, const char *path, const char *argv0, ...);
int spawnle(int mode, const char *path, const char *argv0, ... /*, char * const *envp */);
int spawnlp(int mode, const char *path, const char *argv0, ...);
int spawnlpe(int mode, const char *path, const char *argv0, ... /*, char * const *envp */);

int spawnv(int mode, const char *path, char * const *argv);
int spawnve(int mode, const char *path, char * const *argv, char * const *envp);
int spawnvp(int mode, const char *path, char * const *argv);
int spawnvpe(int mode, const char *path, char * const *argv, char * const *envp);

#define P_WAIT		1
#define P_NOWAIT	2	/* always generates error */
#define P_OVERLAY	3

#ifdef __cplusplus
}
#endif

#endif


