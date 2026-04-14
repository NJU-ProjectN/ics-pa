#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);

__attribute__((section(".text.unlikely"))) void _start(int argc, char *argv[], char *envp[]) {
  int ret = main(argc, argv, envp);
  exit(ret);
  assert(0);
}
