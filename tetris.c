#include <signal.h>
#include "term.h"

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20
#define BOARD_OFFSET 1


static struct termios *orig_term_attr;

static void signalHandler(int _)
{
    (void)_;
    clearScreen();
    restoreTerminalMode(orig_term_attr);
    hideCursor(false);
    free(orig_term_attr);
    printf("Dying...\n");
    exit(0);
}

typedef enum {
    S0,S1,S2,S3,
    R,G,B,
} pDef;

typedef enum {
    UP       = 1,
    DOWN     = 2,
    LEFT     = 3,
    RIGHT    = 4,
    ROTATE_L = 5,
    ROTATE_R = 6
} Key;

uint32_t pieces[][7] = {{0x004E, 0x08C8, 0x00E4, 0x04C4, 0x7F,0x00,0x7F},
                        {0x00CC, 0x00CC, 0x00CC, 0x00CC, 0xFF,0x00,0xFF},
                        {0x000F, 0x8888, 0x000F, 0x8888, 0x00,0xFF,0xFF},
                        {0x044C, 0x008E, 0x0C88, 0x00E2, 0x00,0x00,0xFF},
                        {0x088C, 0x00E8, 0x0C44, 0x002E, 0xFF,0x7F,0x00},
                        {0x00C6, 0x04C8, 0x00C6, 0x04C8, 0xFF,0x00,0x00},
                        {0x006C, 0x08C4, 0x006C, 0x08C4, 0x00,0xFF,0x00}};
uint8_t playfield[BOARD_HEIGHT][BOARD_WIDTH];

void drawPiece(int* coord, uint32_t piece);

bool printBoard() {
    int sh = getScreenHeight();
    int sw = getScreenWidth();
    if (sh < BOARD_HEIGHT || sw < BOARD_WIDTH) {
        printf("Your terminal isn\'t big enough");
        return false;
    }
    else {
        printf("\n");
        printf("╔══════╦══════════╦══════╗\n");
        printf("║STORED║          ║ NEXT ║\n");
        printf("╠══════╣          ╠══════╣\n");
        printf("║ ████ ║          ║ ████ ║\n");
        printf("║ ████ ║          ║ ████ ║\n");
        printf("║ ████ ║          ║ ████ ║\n");
        printf("║ ████ ║          ║ ████ ║\n");
        printf("╚══════╣          ╠══════╝\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("       ║          ║\n");
        printf("╔══════╣          ║\n");
        printf("║POINTS║          ║\n");
        printf("╠══════╣          ║\n");
        printf("║ 0000 ║          ║\n");
        printf("╚══════╩══════════╝");
        return true;
    }

}
void setError(char * err) {
    int coord[] = {20,22+BOARD_OFFSET};
    moveTo(coord);
    printf("%s", err);
}

void setNext(uint32_t piece) {
    //printf("%x", piece);
    int coord[] = {3,4+BOARD_OFFSET};
    drawPiece(coord, piece);
}

void setStored(uint32_t piece) {
    //printf("%x", piece);
    int coord[] = {21,4+BOARD_OFFSET};
    drawPiece(coord, piece);
}

void drawPiece(int* coord, uint32_t piece) {
    for(int i = 3; i >= 0; i--) {
        //uint8_t t = (piece & (0xF << (i*4))) >> (i*4);
        uint8_t t = (piece >> (i*4)) & 0xF;
        moveTo(coord);
        for(int j = 3; j >= 0; j--) {
            printf((t & (1 << j)) ? "█" : " "); 
        }
        coord[1]++;
        //printf("%x", t);
    }
}

void renderPlayfield() {

    int coord[] = {9,2 + BOARD_OFFSET};
    for(int i=0; i < BOARD_HEIGHT;i++) {
        moveTo(coord);
        for(int j=0; j < BOARD_WIDTH;j++) {
            printf(playfield[i][j] ? "X" : " "); 
        }
        coord[1]++;
    }
}

//for tetris guidelines
// http://tetris.wikia.com/wiki/Tetris_Guideline

int main() {
    signal(SIGINT, signalHandler);
    clearScreen();
    hideCursor(true);
    int c[] = {1,1};
    uint8_t comm_in;
    char excomm_in[8] = {0};
    Key curr;

    orig_term_attr = malloc(sizeof(struct termios));
    setRawTerminal(orig_term_attr);

    moveTo(c);
    printBoard();
    while(1) {
        curr = -1;
        comm_in = fgetc(stdin);
        if(comm_in == 0x1b) {
            *excomm_in = comm_in;
            
            int i = 0;
            while ((comm_in = fgetc(stdin)) != (unsigned char)EOF) {
                if(i < 16){
                    excomm_in[++i] = comm_in;
                }
                else {
                    fflush(stdin);
                    memset(excomm_in, 0, 8);
                    curr = -1;
                    break;
                }
            }
            if(*excomm_in) {
                if(!strcmp(excomm_in, "\x1b[A")) {
                    //up
                    curr = UP;
                }
                else if(!strcmp(excomm_in, "\x1b[B")) {
                    //down
                    curr = DOWN;
                }
                else if(!strcmp(excomm_in, "\x1b[C")) {
                    //right
                    curr = RIGHT;
                }
                else if(!strcmp(excomm_in, "\x1b[D")) {
                    //left
                    curr = LEFT;
                }
                else {
                    fflush(stdin);
                    memset(excomm_in, 0, 8);
                    curr = -1;
                }
            }
        }
        else {
            switch(comm_in) {
                case 'z':
                        curr = ROTATE_L;
                    break;
                case 'x':
                        curr = ROTATE_R;
                    break;
                default:
                    break;
            }
        }
        if (curr) {
            switch(curr) {
                case UP:
                break;
                case DOWN:
                break;
                case RIGHT:
                break;
                case LEFT:
                break;
                case ROTATE_L:
                break;
                case ROTATE_R:
                break;
                default:
                break;
            }
        }
        setError("Test Error");
        //memset(playfield, 1, 200);
        //renderPlayfield();
        //setNext(pieces[0][0]);
    }
    getchar();
}