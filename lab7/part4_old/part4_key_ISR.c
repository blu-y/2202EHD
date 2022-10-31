#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int count;
extern int run;     // access external variable
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    if (run == 1){
        // second pressing key : timer stop
        *(TIMER_ptr+1) = 0b0111; // STOP 0, START 1, CONT 1, ITO 1
        run = 0;
    }
    else {
        // first pressing key : timer start
        count = 0;
        *(TIMER_ptr+1) = 0b0111; // STOP 0, START 1, CONT 1, ITO 1
        run = 1;
    }
}