#include "address_map_nios2.h"
extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int state, a, b, a_, b_, speed, count, d;
void initialize_variables2(void){
    state = 0;
    a = 0;
    b = 0;
    a_ = 0;
    b_ = 0;
    speed = 0;
    count = 0;
    d = 0;
}
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    initialize_variables2();
    state = 1;
}