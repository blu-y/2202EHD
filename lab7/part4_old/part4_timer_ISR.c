#include "address_map_nios2.h"

extern volatile int *TIMER_ptr;
extern int count;
extern int run;     // access external variable
void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (run==1) count++;
    if (count>5000){
        // if count > 5000, then initialize
        count = 0;
        run = 2;
        *(TIMER_ptr+1) = 0b1011; // STOP 1, START 0, CONT 1, ITO 1
    }
    *TIMER_ptr = 0;
}