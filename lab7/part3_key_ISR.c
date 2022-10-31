#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern int run;     // access external variable
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    int key = *(KEY_ptr);
    if (key == 0b1000) run ^= 1;
}