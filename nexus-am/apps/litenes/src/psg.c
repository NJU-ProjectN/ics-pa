#include <psg.h>

static byte prev_write;
static int p = 10;

static int MAP[256] = {
  0, // On/Off
  _KEY_G, // A
  _KEY_H, // B
  _KEY_T, // SELECT
  _KEY_Y, // START
  _KEY_W,
  _KEY_S,
  _KEY_A, 
  _KEY_D, 
  255,
};

extern int key_state[];

inline byte psg_io_read(word address)
{
    // Joystick 1
    if (address == 0x4016) {
        if (p++ < 9) {
          return key_state[MAP[p]];
        }
    }
    return 0;
}

inline void psg_io_write(word address, byte data)
{
    if (address == 0x4016) {
        if ((data & 1) == 0 && prev_write == 1) {
            // strobe
            p = 0;
        }
    }
    prev_write = data & 1;
}
