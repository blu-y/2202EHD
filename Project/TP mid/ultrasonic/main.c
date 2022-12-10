/* TP */
#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
volatile int *HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
volatile int *TIMER_ptr = (int *) TIMER_BASE;
volatile int *TIMER_2_ptr = (int *) TIMER_2_BASE;
volatile int *KEY_ptr = (int *) KEY_BASE;
volatile int *SW_ptr = (int *) SW_BASE;
volatile int *JP1_ptr = (int *) JP1_BASE;
volatile int *JP2_ptr = (int *) JP2_BASE;

int count = 0;              // initialize HEX CODE
int state = 0;              // calibrated = 1
int start = 0;
int r = 1;                  // radius(mm)
float d = 0;                // calibrated distance(m)
int a = 0;                  // first sonic distance(mm)
int u0 = 0;                 // first sonic state
int dt = 0;                 // time difference
int speed = 0;              // speed(mm/s)

int HEX[11] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000
    }; // 0 1 2 3 4 5 6 7 8 9 NULL
int power(int a, int b){
    int p = 1;
    for (int i=0; i<b; i++) p *= a;
    return p;
} // get a^b
void display_hex4(int n){
    // display 6 digit number to hex
    int h[6] = {0}; // array for 6 digit
    unsigned int hex0 = 0; // bit codes for HEX 3-0
    // unsigned int hex4 = 0; // bit codes for HEX 5-4
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    } // separate 6 digit by dividing 10^(5~0)
    for (int i = 2; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    } // HEX3-0 = HEX3<<24 + HEX2<<16 + HEX1<<8 + HEX0
    *HEX3_HEX0_ptr = hex0;
    // save HEX3-0 to HEX3_HEX0_BASE
    /*
    for (int i = 0; i <= 1; i++){
        hex4 += HEX[h[i]]<<(8*(1-i));
    } // HEX5-4 = HEX5<<8 + HEX4
    *HEX5_4_ptr = hex4;
    // save HEX5-4 to HEX5_HEX4_BASE
    */
    // no need to save HEX5-4
}
void display_loading(int n){
    if (n==1) *HEX3_HEX0_ptr = 16843009;        // display '£þ£þ£þ£þ'
    else if (n==2) *HEX3_HEX0_ptr = 101058054;  // display ' ||||'
    else if (n==3) *HEX3_HEX0_ptr = 134744072;  // display '____'
    else if (n==4) *HEX3_HEX0_ptr = 808464432;  // display '|||| '
    else *HEX3_HEX0_ptr =0;
}
void set_period(int us){
    us = 100*us; // unit time 0.01us so us*100
    *(TIMER_ptr+2) = us;
    *(TIMER_ptr+3) = us>>16;
    // low 16 bit to periodl, high 16 bit to periodh
}
void config_timer(int us){
    // input : period in microsec
    *TIMER_ptr = 0; // initialize
    set_period(us);
    *(TIMER_ptr+1) = 0b0111; // STOP 0, START 1, CONT 1, ITO 1
}
void config_KEYs(){ *(KEY_ptr + 2) = 0b0001; }  // enable interrupts for KEY0 (2 means 8byte)
void config_GPIO(){
    *(JP1_ptr+1) = 0b1001;   //set JP1 pin #2(output mode) #1(input mode)
    *JP1_ptr = 0b0;
}
void enable_nios2_interrupts(void){
    NIOS2_WRITE_IENABLE(0b11);  // set interrupt mask bit for KEY, TIMER
    NIOS2_WRITE_STATUS(1);      // enable Nios II interrupts
}

int main(void){
    config_timer(1000);   //period 1ms
    config_KEYs();
    config_GPIO();
    enable_nios2_interrupts();
    while (1){
        if (state==1) display_loading(1);
        else if (state==2) display_hex4(a%10000);
        else display_loading(3);
    }
    return 0;
}