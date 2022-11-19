/* Lab 7 Part II
   set TIMER 0.25s
   toggle with KEY, display with LEDR
   using interrupts
*/

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *LEDR_ptr = (int *) LEDR_BASE;
volatile int *TIMER_ptr = (int *) TIMER_BASE;
volatile int *KEY_ptr = (int *) KEY_BASE;

int count = 0;                    // initialize HEX CODE
int run = 1;

void config_timer(int us){
    // input : period in microsec
    *TIMER_ptr = 0; // initialize
    us = 100*us; // unit time 0.01us so us*100
    *(TIMER_ptr+2) = us;
    *(TIMER_ptr+3) = us>>16;
    // low 16 bit to periodl, high 16 bit to periodh
    *(TIMER_ptr+1) = 0b0111;
}

void config_KEYs(){ *(KEY_ptr + 2) = 0b1111; }  // enable interrupts for KEY (2 means 8byte)

void enable_nios2_interrupts(void){
    NIOS2_WRITE_IENABLE(0b11);  // set interrupt mask bit for KEY, TIMER
    NIOS2_WRITE_STATUS(1);      // enable Nios II interrupts
}

int main(void){
    config_timer(250000);
    config_KEYs();
    enable_nios2_interrupts();
    while (1){
        *LEDR_ptr = count;
    }
}