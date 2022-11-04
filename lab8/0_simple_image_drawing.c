#include "address_map_nios2.h"
#define PIXEL(r, g, b) \
    (short int)((((r)&0x1F)<<11) | (((g)&0x3F)<<5) | (((b)&0x1F))) // pixel intensity : 5 6 5 bit
#define FILL_PIXEL(x, y, r, g, b) \
    *(short int *)(pixel_buffer_start + (((y)&0xFF)<<10) + (((x)&0x1FF)<<1)) = PIXEL(r,g,b) // M[address] (8, 9 bit) <- PIXEL
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int pixel_buffer_start;
void clear_screen(int r, int g, int b);
void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b);

int main(void){
    volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // pixel controller
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen(0xFF, 0xFF, 0xFF);
    draw_square(10, 10, 50, 50, 0xFF, 0x00, 0x00);
    draw_square(51, 51, 80, 80, 0x00, 0xFF, 0x00);
    draw_square(81, 81, 100, 100, 0x00, 0x00, 0xFF);

    while(1);
}

void clear_screen(int r, int g, int b){
    draw_square(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, r, g, b);
}

void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b){
    int x, y;
    for(x = x1; x <= x2; x++)
        for(y = y1; y <= y2; y++)
            FILL_PIXEL(x, y, r, g, b);
}