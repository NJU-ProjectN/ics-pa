#include <am.h>
#include <SDL2/SDL.h>

#define W 400
#define H 300

_Screen _screen;

#define KEYDOWN_MASK 0x8000

static inline uint32_t pixel(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}
static inline uint8_t R(uint32_t p) { return p >> 16; }
static inline uint8_t G(uint32_t p) { return p >> 8; }
static inline uint8_t B(uint32_t p) { return p; }

static SDL_Window *window;
static SDL_Renderer *renderer;

static int event_thread(void *args);

#define KEY_QUEUE_LEN 1024
static int key_queue[KEY_QUEUE_LEN];
static int key_f = 0, key_r = 0;
static SDL_mutex *key_queue_lock;

static SDL_Texture *texture;
static uint32_t fb[W * H];

void gui_init() {
  _screen.width = W;
  _screen.height = H;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(W * 2, H * 2, 0, &window, &renderer);
  SDL_SetWindowTitle(window, "Native Application");
  SDL_CreateThread(event_thread, "event thread", NULL);
  texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, W, H);
  memset(fb, 0, W * H * sizeof(uint32_t));
  _draw_sync();
  key_queue_lock = SDL_CreateMutex();
}

static inline int min(int x, int y) {
  return (x < y) ? x : y;
}

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  int cp_bytes = sizeof(uint32_t) * min(w, _screen.width - x);
  for (int j = 0; j < h && y + j < _screen.height; j ++) {
    memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
    pixels += w;
  }
}

void _draw_sync() {
  SDL_UpdateTexture(texture, NULL, fb, W * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

int _read_key() {
  int ret = _KEY_NONE;
  SDL_LockMutex(key_queue_lock);
  if (key_f != key_r) {
    ret = key_queue[key_f];
    key_f = (key_f + 1) % KEY_QUEUE_LEN;
  }
  SDL_UnlockMutex(key_queue_lock);
  return ret;
}

#define XX(k) [SDL_SCANCODE_##k] = _KEY_##k,
static int keymap[256] = {
  _KEYS(XX)
};

static int event_thread(void *args) {
  SDL_Event event;
  while (1) {
    SDL_WaitEvent(&event);
    switch (event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN: 
      case SDL_KEYUP:
        {
          SDL_Keysym k = event.key.keysym;
          int keydown = event.key.type == SDL_KEYDOWN;
          if (event.key.repeat == 0) {
            int scancode = k.scancode;
            if (keymap[scancode] != 0) {
              int am_code = keymap[scancode] | (keydown ? KEYDOWN_MASK : 0);
              SDL_LockMutex(key_queue_lock);
              key_queue[key_r] = am_code;
              key_r = (key_r + 1) % KEY_QUEUE_LEN;
              SDL_UnlockMutex(key_queue_lock);
            }
          }
        }
        break;
    }
  }
}

