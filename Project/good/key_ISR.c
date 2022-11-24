#include "address_map_nios2.h"
extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int state;     // access external variable
extern int state, r, a, b, a_, b_, speed, count, d;
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    initialize_variables();
    state = 1;
}