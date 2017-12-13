#ifndef TERM_H
#define TERM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include <sys/ioctl.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

extern void setForegroundColor(Color c);
extern void setBackgroundColor(Color c);
extern void resetColor();
extern int getScreenWidth();
extern int getScreenHeight();
extern void clearScreen();
extern void hideCursor(bool h);
extern void moveTo(int coord[]);
extern struct termios* setRawTerminal(struct termios *orig);
extern void restoreTerminalMode(const struct termios *orig);

#endif