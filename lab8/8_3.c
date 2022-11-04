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
void wait_for_vsync();
short int front_buffer[512*256];
short int back_buffer[512*256];

int main(void){
    pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // pixel controller
    *(pixel_ctrl_ptr+1) = front_buffer;
    wait_for_vsync();

    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0x00, 0x00, 0x00);
    *(pixel_ctrl_ptr+1) = back_buffer;

    int x[8], y[8], dx[8], dy[8], i;
    for (i = 0; i < 8; i++){
        x[i] = rand() % SCREEN_WIDTH;
        y[i] = rand() % SCREEN_HEIGHT;
        if (rand() % 2) dx[i] = 1;
        else dx[i] = -1;
        if (rand() % 2) dy[i] = 1;
        else dy[i] = -1;
    }
    while(1){
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        clear_screen(0x00, 0x00, 0x00);
        for (i = 0; i < 8; i++){
            if (x[i] == 0 || x[i] == (SCREEN_WIDTH-1)) dx[i] = -dx[i];
            if (y[i] == 0 || y[i] == ((SCREEN_HEIGHT-1))) dy[i] = -dy[i];
            x[i] = x[i] + dx[i];
            y[i] = y[i] + dy[i];
            draw_square(x[i]-2, y[i]-2, x[i]+2, y[i]+2, 0xFF, 0xFF, 0xFF);
            if (i == 7) draw_line(x[7], y[7], x[0], y[0], 0xFF, 0xFF, 0xFF);
            else draw_line(x[i], y[i], x[i+1], y[i+1], 0xFF, 0xFF, 0xFF);
        }
        wait_for_vsync();
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

void wait_for_vsync(){
    register int status;
    *pixel_ctrl_ptr = 1; // start the synchronization process

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0)
        status = *(pixel_ctrl_ptr + 3);
}