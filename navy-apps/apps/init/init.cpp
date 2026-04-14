/*
 * this is the first process in the OS:
 *   display a splash screen
 *   display a boot menu and receive input
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ndl.h>

#define logo_size 64
const char *startup_app = "/bin/lua";

int W = 0, H = 0;
FILE *fbdev, *evtdev;

struct MenuItem {
  const char *name, *bin, *arg1;
} items[] = {
  {"Graphical Shell", "/bin/nwm", NULL},
  {"Lua shell", "/bin/lua", NULL},
  {"Vi the text editor (busybox)", "/bin/vi", NULL},
  {"Litenes (Super Mario Bros)", "/bin/litenes", "/share/games/nes/mario.nes"},
  {"Litenes (Yie Ar Kung Fu)", "/bin/litenes", "/share/games/nes/kungfu.nes"},
  {"PAL - Xian Jian Qi Xia Zhuan", "/bin/pal", NULL},
};

#define nitems (sizeof(items) / sizeof(items[0]))


static void open_display();
static void display_menu();

int main(int argc, char *argv[], char *envp[]) {

  if (!freopen("/dev/tty", "r", stdin) ||
      !freopen("/dev/tty", "w", stdout) ||
      !freopen("/dev/tty", "w", stderr)) {
    // the OS does not meet the spec
    exit(1);
  }

  NDL_Bitmap logo;
  NDL_LoadBitmap(&logo, "/share/pictures/projectn.bmp");

  open_display();

  if (fbdev) {
    for (int i = 0; i < W * H; i ++) {
      uint32_t col = 0xffffff;
      fwrite(&col, sizeof(uint32_t), 1, fbdev);
    }
    fflush(fbdev);

    int dx = (W - logo.w) / 2;
    int dy = (H - logo.h) / 2;
    for (int y = 0; y < logo.h; y ++) {
      fseek(fbdev, ((y + dy) * W + dx) * sizeof(uint32_t), SEEK_SET);
      fwrite(&logo.pixels[y * logo.w], sizeof(uint32_t), logo.w, fbdev);
    }
    fflush(fbdev);
    fclose(fbdev);
  }

  evtdev = fopen("/dev/events", "r");

  while (1) {
    display_menu();

    int i = 0;
    do {
      char cmd[32], data[32];
      if (fscanf(evtdev, "%s %s", cmd, data) != 2) {
        continue;
      }
      if (strcmp(cmd, "kd") == 0) {
        sscanf(data, "%d", &i);
      }
    } while (i == 0);

    if (i == 0) continue;

    if (i >= 1 && i <= nitems) {
      auto *item = &items[i - 1];
      const char *exec_argv[] = {
        item->bin,
        item->arg1,
        NULL,
      };
      execve(exec_argv[0], (char**)exec_argv, (char**)envp);
      fprintf(stderr, "\033[31m[ERROR]\033[0m Exec %s failed.\n\n", exec_argv[0]);
    } else {
      fprintf(stderr, "Choose a number between %d and %d\n\n", 1, nitems);
    }
  }
  return -1;
}

static void open_display() {
  FILE *dispinfo = fopen("/proc/dispinfo", "r");

  if (!dispinfo) {
    fprintf(stderr, "[IGN] Cannot open /proc/dispinfo.\n");
    return;
  }

  char buf[128], key[128], value[128], *delim;
  while (fgets(buf, 128, dispinfo)) {
    *(delim = strchr(buf, ':')) = '\0';
    sscanf(buf, "%s", key);
    sscanf(delim + 1, "%s", value);
    if (strcmp(key, "WIDTH") == 0) sscanf(value, "%d", &W);
    if (strcmp(key, "HEIGHT") == 0) sscanf(value, "%d", &H);
  }

  fclose(dispinfo);

  if (W == 0 || H == 0) {
    fprintf(stderr, "[IGN] Invalid display information.\n");
    return;
  }

  fbdev = fopen("/dev/fb", "w");
  if (!fbdev) {
    fprintf(stderr, "[IGN] Cannot open display.\n");
  }
}

static void display_menu() {
  printf("Available applications:\n");
  for (int i = 0; i < nitems; i ++) {
    auto *item = &items[i];
    printf("  [%d] %s\n", i + 1, item->name);
  }
  printf("========================================\n");
  printf("Please Choose.\n");
  fflush(stdout);
}
