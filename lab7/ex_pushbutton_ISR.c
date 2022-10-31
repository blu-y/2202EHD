#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern int LEDR_on;     // access external variable

void pushbutton_ISR(void){
    /* Pushbutton - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    LEDR_on ^= 1;   // toggle LEDR
    return;
}
