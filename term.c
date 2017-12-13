#include "term.h"

//
// TODO
//
// should probably convert to NCurses...
// make it more portable

void setForegroundColor(Color c) {
    printf("\x1b[38;2;%u;%u;%um", c.r, c.g, c.b);
}

void setBackgroundColor(Color c) {
    printf("\x1b[48;2;%u;%u;%um", c.r, c.g, c.b);
}

void resetColor() {
    printf("\x1b[0m");
}

int getScreenWidth() {
    struct winsize screen;
    ioctl(0, TIOCGWINSZ, &screen);
    return screen.ws_col;
}

int getScreenHeight() {
    struct winsize screen;
    ioctl(0, TIOCGWINSZ, &screen);
    return screen.ws_row;
}

void clearScreen() {
    system("clear");
}

void hideCursor(bool h) {
    if (h) {
        printf("\x1b[?25l");
    }
    else {
        printf("\x1b[?25h");
    }
    
}

void moveTo(int coord[]) {
    printf("\x1b[%d;%df", coord[1], coord[0]);
}

struct termios* setRawTerminal(struct termios *orig) {
    struct termios *new = malloc(sizeof(struct termios));

    tcgetattr(fileno(stdin), orig);
    memcpy(new, orig, sizeof(struct termios));
    new->c_lflag &= ~(ECHO|ICANON);
    new->c_cc[VTIME] = 0;
    new->c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, new);

    return new;
}

void restoreTerminalMode(const struct termios *orig) {
    tcsetattr(fileno(stdin), TCSANOW, orig);
}

void testColor() {
    Color c;
    c.r = 255;
    c.g = 100;
    c.b = 0;

    setForegroundColor(c);
    printf("TRUECOLOR");
    resetColor();
    setBackgroundColor(c);
    printf("TRUECOLOR");
    resetColor();
    setForegroundColor(c);
    setBackgroundColor(c);
    printf("TRUECOLOR");
    resetColor();
    printf("\n");
    printf("\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m\n");
}

int testKeyPress() {
    unsigned char c;
    struct termios orig_term_attr;
    
    setRawTerminal(&orig_term_attr);
    while((c = fgetc(stdin)) != 0x1b) {
        if(c != (unsigned char)EOF) {
            printf("key pressed: %c,%u\n", c, c);
        }
    }
    restoreTerminalMode(&orig_term_attr);
    return 0;
}