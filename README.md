# Chip8 Emulator
![GitHub](https://img.shields.io/github/license/hoot-w00t/chip8-emu?style=flat-square)

Chip8 Emulator made in C (https://en.wikipedia.org/wiki/CHIP-8)

## Compiling from source on Linux
This project requires the SDL2 library for rendering and sound.

```
git clone https://github.com/hoot-w00t/chip8-emu.git
cd chip8-emu
make
```

## Usage
Emulate a Chip8 ROM
```
$ ./chip8 pong.rom
```
Display the program help
```
$ ./chip8 -h
```

Keybinds:
* Exit: **Escape** or close the window
* Pause/Resume: **Space Bar**
* Dump Chip8 registers, stack, CPU information: **N**
* Hexadecimal keyboard input: Corresponding numbers and letters from **0-9**, **A-F**