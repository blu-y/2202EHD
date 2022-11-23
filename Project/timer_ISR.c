#include "address_map_nios2.h"
#define t0 6 // ultrasonic 0 trigger pin#
#define e0 7 // ultrasonic 0 echo pin#
#define t1 8 // ultrasonic 1 trigger pin#
#define e1 9 // ultrasonic 1 echo pin#
extern volatile int *TIMER_ptr;
extern volatile int *GPIO_ptr;
extern int count;
extern int state;     // access external variable
extern int a;
extern int b;
void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (state==2) {
        count++;
        if (count>10000){
            // if count > 10000, then initialize
            count = 0;
            state = 2;
            *(TIMER_ptr+1) = 0b1011; // STOP 1, START 0, CONT 1, ITO 1
        }
    }

    *TIMER_ptr = 0;
}