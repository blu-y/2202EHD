#include "address_map_nios2.h"
#include <stdlib.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int pixel_buffer_start;
void plot_pixel(int x, int y, short int line_color);
void clear_screen(void);
void draw_line(int x0, int x1, int y0, int y1, short int line_color);

int main(void){
    volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // pixel controller
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    draw_line(0, 0, 150, 150, 0x001F);      // blue
    draw_line(150, 150, 319, 0, 0x07E0);    // green
    draw_line(0, 239, 319, 239, 0xF800);    // red
    draw_line(319, 0, 0, 239, 0xF81F);      // pink

    while(1);
}

void clear_screen(void){
    int x, y;
    for(x = 0; x < SCREEN_WIDTH; x++)
        for(y = 0; y < SCREEN_HEIGHT; y++)
            plot_pixel(x, y, 0xFFFF);
}

void plot_pixel(int x, int y, short int line_color){
    *(short int *)(pixel_buffer_start + (y<<10) + (x<<1)) = line_color;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color){
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
        if (is_steep) plot_pixel(y, x, line_color);
        else plot_pixel(x, y, line_color);
        error = error + deltay;
        if (error >= 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}