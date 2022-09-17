#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

char *ops[] = {"+", "-", "*", "/"};

int choose(int max) {
  return rand() % max;
}

void gen(char* e) {
    strcat(buf, e);
}

void gen_num() {
    char istr[4];
    sprintf(istr, "%d", choose(100));
    gen(istr);
}

void gen_op() {
    gen(ops[choose(4)]);
}

static void gen_rand_expr() {
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1:
      if (strlen(buf) > 30000) {
        gen_num();
      } else{
        gen("("); gen_rand_expr(); gen(")");
      }
      break;
    case 2:
      if (strlen(buf) > 30000) {
        gen_num();
      } else{
        gen_rand_expr(); gen_op(); gen_rand_expr();
      }
      break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[0] = '\0';
    code_buf[0] = '\0';
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    printf("exec code\n");
    int r = fscanf(fp, "%d", &result);
    printf("r is: %d.\n", r);
    if (r == -1) {
        continue;
    }
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
