/* Lab 7 Part I
   KEY0 -> Display HEX0 0
   KEY1 -> Display HEX1 1
   KEY2 -> Display HEX2 2
   KEY3 -> Display HEX3 3
   using interrupts
*/

#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
volatile int *KEY_ptr = (int *) KEY_BASE;

int HEX = 0;                    // initialize HEX CODE

int main(void){
    *(KEY_ptr + 2) = 0b1111;    // enable interrupts for KEY (2 means 8byte)
    NIOS2_WRITE_IENABLE(0b10);  // set interrupt mask bit for KEYs
    NIOS2_WRITE_STATUS(1);      // enable Nios II interrupts
    while (1){
        *HEX3_HEX0_ptr = HEX;
    }
}