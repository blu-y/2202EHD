#include "address_map_nios2.h"
#include "stdio.h"

static volatile int *HEX3_0_ptr = (int*) HEX3_HEX0_BASE;
//static volatile int *HEX5_4_ptr = (int*) HEX5_HEX4_BASE;
static volatile int *TIMER_ptr = (int*) TIMER_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
static int *TIMER_ctrl = (int*)(TIMER_BASE+4);
static int *TIMER_pl = (int*)(TIMER_BASE+8);
static int *TIMER_ph = (int*)(TIMER_BASE+12);
int read_key(void){
    return *KEY_ptr & 0b1111;
}

int power(int a, int b){
    int p = 1;
    for (int i=0; i<b; i++) p *= a;
    return p;
}

int HEX[11] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000
    }; // 0 1 2 3 4 5 6 7 8 9 NULL

void display_hex(int n){
    // display 6 digit number to hex
    int h[6] = {0};
    unsigned int hex0 = 0;
    unsigned int hex4 = 0;
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    }
    for (int i = 2; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    }
    *HEX3_0_ptr = hex0;
    for (int i = 0; i <= 1; i++){
        hex4 += HEX[h[i]]<<(8*(1-i));
    }
    //*HEX5_4_ptr = hex4;
}

void set_timer(int us){
    // input : period in microsec
    us = 100*us; // 100MHz > 0.01us
    *TIMER_pl = us;
    *TIMER_ph = us>>16;
    // low 16 bit to periodl, high 16 bit to periodh
    *TIMER_ctrl = 0b0110; // STOP 0, START 1, CONT 1, ITO 0
}

int timer(int sec){
    int t = *TIMER_ptr;
    if (t&1){
        sec+=1;
        *TIMER_ptr = 0;
    }
    if (sec == 3600) sec = 0;
    return sec;
}

int sec2time(int sec){
    int min = sec/60;
    sec = sec%60;
    return min*100+sec;
}

int main(void){
    int sec = 0;
    int run = 1;
    set_timer(1000000);
    while(1){
        if (read_key()) {
            while (read_key()) read_key();
            if (run){
                run = 0;
                *TIMER_ctrl = 0b1010; // STOP 0, START 1, CONT 1, ITO 0
            }
            else{
                run = 1;
                *TIMER_ctrl = 0b0110; // STOP 0, START 1, CONT 1, ITO 0
            }
        }
        if (run) sec = timer(sec);
        display_hex(sec2time(sec));
    }
}

