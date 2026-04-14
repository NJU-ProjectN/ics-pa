#include "game.h"

//#define SCREEN_STRETCH

static uint32_t canvas[H][W];

extern char font8x8_basic[128][8];

char *itoa(int n)  {  
  static char s[64];
  int i = sizeof(s) - 1;
  do {
    s[--i] = n % 10 + '0';  
    n /= 10;
  } while(n > 0);  
  return &s[i];
}  

static inline void draw_character(char ch, int x, int y, int color) {
  int i, j;
  char *p = font8x8_basic[(int)ch];
  for (i = 0; i < 8; i ++) 
    for (j = 0; j < 8; j ++) 
      if ((p[i] >> j) & 1)
        if (x + j < W && y + i < H)
          canvas[y + i][x + j] = color;
}

static inline void draw_string(const char *str, int x, int y, int color) {
  while (*str) {
    draw_character(*str ++, x, y, color);
    if (x + 8 >= W) {
      y += 8; x = 0;
    } else {
      x += 8;
    }
  }
}

void redraw_screen() {
  fly_t it;
  const char *hit, *miss;

  /* 绘制每个字符 */
  for (it = characters(); it != NULL; it = it->_next) {
    static char buf[2];
    buf[0] = it->text + 'A'; buf[1] = 0;
    draw_string(buf, it->x, it->y, 0xffffffff);
  }

  /* 绘制命中数、miss数、最后一次按键扫描码和fps */
  const char *key = itoa(last_key_code());
  draw_string(key, 0, H - 8, 0xffffffff);
  hit = itoa(get_hit());
  draw_string(hit, W - strlen(hit) * 8, 0, 0x00ff00);
  miss = itoa(get_miss());
  draw_string(miss, W - strlen(miss) * 8, H - 8, 0xfa5858);
  const char *fps = itoa(get_fps());
  draw_string(fps, 0, 0, 0xf3f781);
  draw_string("FPS", strlen(fps) * 8, 0, 0xf3f781);

#ifdef SCREEN_STRETCH
  int w = _screen.width, h = _screen.height;
  for (int x = 0; x < w; x ++)
    for (int y = 0; y < h; y ++) {
      _draw_rect(&canvas[y * H / h][x * W / w], x, y, 1, 1);
    }
#else
  assert(_screen.width >= W);
  assert(_screen.height >= H);
  int x = (_screen.width - W) / 2;
  int y = (_screen.height - H) / 2;
  _draw_rect(&canvas[0][0], x, y, W, H);
#endif

  _draw_sync();
  for (int y = 0; y < H; y ++)
    for (int x = 0; x < W; x ++)
      canvas[y][x] = 0x2a0a29;
}
