#include "address_map_nios2.h"

extern volatile int *TIMER_ptr;
extern int count;
extern int run;     // access external variable
void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (run) count++;
    *TIMER_ptr = 0;
}