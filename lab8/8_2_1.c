#include "address_map_nios2.h"
#include <stdlib.h>
#define PIXEL(r, g, b) \
    (short int)((((r)&0x1F)<<11) | (((g)&0x3F)<<5) | (((b)&0x1F)))
#define FILL_PIXEL(x, y, r, g, b) \
    *(short int *)(pixel_buffer_start + (((y)&0xFF)<<10) + (((x)&0x1FF)<<1)) = PIXEL(r,g,b)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int pixel_buffer_start;
volatile int *pixel_ctrl_ptr;
void clear_screen(int r, int g, int b);
void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b);
void draw_line(int x0, int x1, int y0, int y1, int r, int g, int b);

int main(void){
    pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // pixel controller
    pixel_buffer_start = *pixel_ctrl_ptr;

    int x0 = 100, x1 = 219, y = 239, a = 1;
    while(1){
        clear_screen(0x00, 0x00, 0x00);
        if (y == 0 || a == 239) a = -a;
        y = y + a;
        draw_line(x0, y, x1, y, 0xFF, 0xFF, 0xFF);
    }
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

void draw_line(int x0, int y0, int x1, int y1, int r, int g, int b){
    int is_steep = abs(y1-y0) > abs(x1 - x0); // bool
    int s;
    if (is_steep){
        s = x0; x0 = y0; y0 = s; // swap(x0, y0)
        s = x1; x1 = y1; y1 = s; // swap(x1, y1)
    }
    if (x0 > x1){
        s = x0; x0 = x1; x1 = s; // swap(x0, x1)
        s = y0; y0 = y1; y1 = s; // swap(y0, y1)
    }
    int deltax = x1 - x0;
    int deltay = abs(y1-y0);
    int error = -(deltax/2);
    int y = y0;
    int y_step;
    if (y0 < y1) y_step = 1;
    else y_step = -1;

    for (int x = x0; x < x1; x++){
        if (is_steep) plot_pixel(y, x, r, g, b);
        else plot_pixel(x, y, r, g, b);
        error = error + deltay;
        if (error >= 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}