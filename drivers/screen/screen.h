#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

// Public kernel API functions
void clear_screen();
void print(char* message);
void print_at(char* message, int col, int row);
void print_int(int num);
void print_hex_ll(unsigned long long n);

#endif