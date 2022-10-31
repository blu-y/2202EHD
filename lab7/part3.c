/* Lab 7 Part III
   set TIMER 0.25s
   toggle with KEY, display with LEDR
   the rate of the increment is given by SW1-0
   11:x4, 10:x2, 01:x1, 00:0.5x
   using interrupts
*/

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include "stdio.h"

volatile int *LEDR_ptr = (int *) LEDR_BASE;
volatile int *TIMER_ptr = (int *) TIMER_BASE;
volatile int *KEY_ptr = (int *) KEY_BASE;
volatile int *SW_ptr = (int *) SW_BASE;

int count = 0;                    // initialize HEX CODE
int run = 1;

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

void config_KEYs(){ *(KEY_ptr + 2) = 0b1000; }  // enable interrupts for KEY3 (2 means 8byte)

int period_rate(int sw1){
    int sw2 = *SW_ptr & 0b11;
    if (sw1!=sw2){
        *(TIMER_ptr+1) = 0b1010; // STOP 1, START 0, CONT 1, ITO 0
        printf("switch changed %d -> %d\n", sw1, sw2);
        if(sw2==0) set_period(250000*2);
        else if(sw2==1) set_period(250000);
        else if(sw2==2) set_period(250000/2);
        else if(sw2==3) set_period(250000/4);
        *TIMER_ptr = 0; // Make TO 0
        *(TIMER_ptr+1) = 0b0111; // STOP 0, START 1, CONT 1, ITO 1
    }
    return sw2;
}

int main(void){
    config_timer(250000);
    config_KEYs();
    NIOS2_WRITE_IENABLE(0b11);  // set interrupt mask bit for KEY, TIMER
    NIOS2_WRITE_STATUS(1);      // enable Nios II interrupts
    int sw = *SW_ptr & 0b11;
    sw = period_rate(sw);
    printf("hi");
    while (1){
        sw = period_rate(sw);
        *LEDR_ptr = count;
    }
}