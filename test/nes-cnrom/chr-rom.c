#include <nes.h>
#include <peekpoke.h>
#include <stdlib.h>

asm(".globl __chr_rom_size\n"
    "__chr_rom_size = 2048\n");

__attribute__((used, section(".chr_rom_0")))
const char cr0[8192] = {1, [8191] = 2};
__attribute__((used, section(".chr_rom_255")))
const char cr255[8192] = {3, [8191] = 4};

void set_bank(char b) {
  static const char rom_bytes[] = {0, [1] = 1, [255] = 255};
  POKE(&rom_bytes[b], b);
}

char read_ppu(unsigned ppu_addr) {
  (void)PPU.status;
  PPU.vram.address = (unsigned)ppu_addr >> 8;
  PPU.vram.address = (unsigned)ppu_addr;
  (void)PPU.vram.data;
  return PPU.vram.data;
}

int main(void) {
  set_bank(0);
  if (read_ppu(0) != 1)
    return EXIT_FAILURE;
  if (read_ppu(8191) != 2)
    return EXIT_FAILURE;

  set_bank(255);
  if (read_ppu(0) != 3)
    return EXIT_FAILURE;
  if (read_ppu(8191) != 4)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
