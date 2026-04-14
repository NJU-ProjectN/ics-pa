#ifndef FCE_H
#define FCE_H

#include <am.h>
#include <common.h>

#define FPS 60
#define W   256
#define H   240

int fce_load_rom(char *rom);
void fce_init();
void fce_run();
void fce_update_screen();

extern byte canvas[257][520];
extern int frame_cnt;

extern char rom_mario_nes[];

#endif
