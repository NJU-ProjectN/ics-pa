#include <fce.h>
#include <cpu.h>
#include <memory.h>
#include <ppu.h>
#include <klib.h>

//#define NOGUI

int key_state[256];
int frame_cnt;
static byte *buf;

typedef struct {
    char signature[4];
    byte prg_block_count;
    byte chr_block_count;
    word rom_type;
    byte reserved[8];
} ines_header;

static ines_header *fce_rom_header;

// FCE Lifecycle

byte *romread(int size)
{
    byte *ret = buf;
    buf += size;
    return ret;
}

int fce_load_rom(char *rom)
{
    buf = (byte*)rom;
    fce_rom_header = (ines_header*)romread(sizeof(ines_header));

    if (memcmp(fce_rom_header->signature, "NES\x1A", 4)) {
        return -1;
    }

    mmc_id = ((fce_rom_header->rom_type & 0xF0) >> 4);

    int prg_size = fce_rom_header->prg_block_count * 0x4000;
    
    byte *blk = romread(prg_size);

    if (mmc_id == 0 || mmc_id == 3) {
        // if there is only one PRG block, we must repeat it twice
        if (fce_rom_header->prg_block_count == 1) {
            mmc_copy(0x8000, blk, 0x4000);
            mmc_copy(0xC000, blk, 0x4000);
        }
        else {
            mmc_copy(0x8000, blk, 0x8000);
        }
    }
    else {
        return -1;
    }

    // Copying CHR pages into MMC and PPU
    int i;
    for (i = 0; i < fce_rom_header->chr_block_count; i++) {
        byte *blk = romread(0x2000);
        mmc_append_chr_rom_page(blk);

        if (i == 0) {
            ppu_copy(0x0000, blk, 0x2000);
        }
    }

    return 0;
}

void fce_init()
{
    cpu_init();
    ppu_init();
    ppu_set_mirroring(fce_rom_header->rom_type & 1);
    cpu_reset();
}

static unsigned long gtime;

void wait_for_frame() {
#ifdef NOGUI
  return;
#endif
  unsigned long cur = _uptime();
  while (cur - gtime < 1000 / FPS) {
    cur = _uptime();
  }
  gtime = cur;
}

void fce_run()
{
    key_state[0] = 1;
    gtime = _uptime();
    while(1)
    {
        wait_for_frame();
        int scanlines = 262;
        while (scanlines-- > 0)
        {
            ppu_run(1);
            cpu_run(1364 / 12); // 1 scanline
        }

        int key = _read_key();
        if (key != _KEY_NONE) {
          int down = (key & 0x8000) != 0;
          int code = key & ~0x8000;
          key_state[code] = down;
        }
    }
}

// Rendering

static const uint32_t palette[64] = {
  0x808080, 0x0000BB, 0x3700BF, 0x8400A6, 0xBB006A, 0xB7001E,
  0xB30000, 0x912600, 0x7B2B00, 0x003E00, 0x00480D, 0x003C22,
  0x002F66, 0x000000, 0x050505, 0x050505, 0xC8C8C8, 0x0059FF,
  0x443CFF, 0xB733CC, 0xFF33AA, 0xFF375E, 0xFF371A, 0xD54B00,
  0xC46200, 0x3C7B00, 0x1E8415, 0x009566, 0x0084C4, 0x111111,
  0x090909, 0x090909, 0xFFFFFF, 0x0095FF, 0x6F84FF, 0xD56FFF,
  0xFF77CC, 0xFF6F99, 0xFF7B59, 0xFF915F, 0xFFA233, 0xA6BF00,
  0x51D96A, 0x4DD5AE, 0x00D9FF, 0x666666, 0x0D0D0D, 0x0D0D0D,
  0xFFFFFF, 0x84BFFF, 0xBBBBFF, 0xD0BBFF, 0xFFBFEA, 0xFFBFCC,
  0xFFC4B7, 0xFFCCAE, 0xFFD9A2, 0xCCE199, 0xAEEEB7, 0xAAF7EE,
  0xB3EEFF, 0xDDDDDD, 0x111111, 0x111111
}; 

byte canvas[257][520];

static int xmap[1024];
static uint32_t row[1024];

void fce_update_screen()
{
  int idx = ppu_ram_read(0x3F00);

  int w = _screen.width;
  int h = _screen.height;

  frame_cnt ++;
#ifdef NOGUI
  if (frame_cnt % 1000 == 0) printf("Frame %d (%d FPS)\n", frame_cnt, frame_cnt * 1000 / _uptime());
  return;
#endif
  if (frame_cnt % 3 != 0) return;

  int pad = (w - h) / 2;
  for (int y = 0; y < h; y ++) {
    int y1 = y * (H - 1) / h + 1;
    for (int x = pad; x < w - pad; x ++) {
      row[x] = palette[canvas[y1][xmap[x] + 0xff]];
    }
    _draw_rect(row + pad, pad, y, w - 2 * pad, 1);
  }

  _draw_sync();

  assert(sizeof(byte) == 1);
  memset(canvas, idx, sizeof(canvas));
}

void xmap_init() {
  int w = _screen.width;
  int h = _screen.height;
  int pad = (w - h) / 2;
  for (int x = pad; x < w - pad; x ++) {
    xmap[x] = (x - pad) * W / h;
  }
}

int main() {
  _ioe_init();

  xmap_init();
  fce_load_rom(rom_mario_nes);
  fce_init();
  fce_run();
  return 1;
}

