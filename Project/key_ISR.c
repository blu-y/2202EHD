#include "address_map_nios2.h"
#define cos 0.96592582628
extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int count;
extern int r;
extern int d;
extern int a;
extern int b;
extern int state;     // access external variable
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    /* Calibrate */
    d = sqrt((r+a)*(r+a)+(r+b)*(r+b)-2*(r+a)*(r*b)*cos);
    state = 1;
}