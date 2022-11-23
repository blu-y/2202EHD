#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *LEDR_ptr = (int *) LEDR_BASE;
volatile int *KEY_ptr = (int *) KEY_BASE;

int LEDR_on = 0;

/* On pressing KEY0, LEDR0 is toggled */
int main(void){
    *(KEY_ptr + 8) = 0x1;   // enable interrupts for KEY0 (2 means 8byte)
    NIOS2_WRITE_IENABLE(0x2);   // set interrupt mask bit for level 1 (KEYs)
    NIOS2_WRITE_STATUS(1);  // enable Nios II interrupts

    while (1) *LEDR_ptr = LEDR_on;
}