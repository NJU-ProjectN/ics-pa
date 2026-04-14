#include <am.h>
#include <klib.h>

const int FPS = 30;
const int N = 32;

static inline uint32_t pixel(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}
static inline uint8_t R(uint32_t p) { return p >> 16; }
static inline uint8_t G(uint32_t p) { return p >> 8; }
static inline uint8_t B(uint32_t p) { return p; }

uint32_t canvas[N][N];
bool used[N][N];

uint32_t color_buf[32 * 32];

void redraw() {
  int w = _screen.width / N;
  int h = _screen.height / N;
  int  block_size = w * h;
  assert(block_size <= (int)(sizeof(color_buf) / sizeof(color_buf[0])));

  int x, y, k;
  for (y = 0; y < N; y ++) {
    for (x = 0; x < N; x ++) {
      for (k = 0; k < block_size; k ++) {
        color_buf[k] = canvas[y][x];
      }
      _draw_rect(color_buf, x * w, y * h, w, h);
    }
  }

  _draw_sync();
}

static uint32_t p(int tsc) {
  int b = tsc & 0xff;
  return pixel(b * 6, b * 7, b);
}

void update() {
  static int tsc = 0;
  static int dx[4] = {0, 1, 0, -1};
  static int dy[4] = {1, 0, -1, 0};

  tsc ++;

  for (int i = 0; i < N; i ++)
    for (int j = 0; j < N; j ++) {
      used[i][j] = false;
    }

  int init = tsc * 1;
  canvas[0][0] = p(init); used[0][0] = true;
  int x = 0, y = 0, d = 0;
  for (int step = 1; step < N * N; step ++) {
    for (int t = 0; t < 4; t ++) {
      int x1 = x + dx[d], y1 = y + dy[d];
      if (x1 >= 0 && x1 < N && y1 >= 0 && y1 < N && !used[x1][y1]) {
        x = x1; y = y1;
        used[x][y] = true;
        canvas[x][y] = p(init + step / 2);
        break;
      }
      d = (d + 1) % 4;
    }
  }
}


int main() {
  _ioe_init();

  unsigned long last = 0;

  while (true) {
    unsigned long upt = _uptime();
    if (upt - last > 1000 / FPS) {
      update();
      redraw();
      last = upt;
    }
  }

  return 0;
}
