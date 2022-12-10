#include "address_map_nios2.h"
extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int count;
extern int state;     // access external variable
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    count = 0;
    state = 1;
}