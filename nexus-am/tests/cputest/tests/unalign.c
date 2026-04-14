#include "trap.h"

unsigned x = 0xffffffff;
unsigned char buf[16];

int main() {

	*((volatile unsigned*)(buf + 3)) = 0xaabbccdd;

	x = *((volatile unsigned*)(buf + 3));
	nemu_assert(x == 0xaabbccdd);

	return 0;
}
