/* TP */
#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
volatile int pixel_buffer_start;
volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // pixel controller
void draw_char(int x, int y, char c) {
    volatile char *character_buffer = (char *) (0x09000000 + (y<<7) + x);
    *character_buffer = c;
}
void clear_line(int y){
    for(int x = 0; x <= 79; x++) {
        volatile char *character_buffer = (char *) (0x09000000 + (y<<7) + x);
        *character_buffer = 0;
    }
}

int main(void){
    pixel_buffer_start = *pixel_ctrl_ptr;
    int xc = 10;
    while (1){
        clear_line(10);
        xc = 10;
        char* str = "***Speed Gun***";
        while (*str) draw_char(xc++, 10, *str++);
        clear_line(11);
        xc = 10;
        str = "Calibrated";
        while (*str) draw_char(xc++, 11, *str++);
        clear_line(12);
        xc = 10;
        str = "Speed Check";
        while (*str) draw_char(xc++, 12, *str++);
        clear_line(13);
        xc = 10;
        str = "Speed[m/s] : 4";
        while (*str) draw_char(xc++, 13, *str++);
    }
    return 0;
}

