#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern int HEX;     // access external variable
int HEX_CODE[11] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000
    }; // 0 1 2 3 4 5 6 7 8 9 NULL
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    int key = *(KEY_ptr);
    switch (key)
    {
    case 0b1:
        HEX = HEX_CODE[0];
        break;
    case 0b10:
        HEX = HEX_CODE[1]<<8;
        break;
    case 0b100:
        HEX = HEX_CODE[2]<<16;
        break;
    case 0b1000:
        HEX = HEX_CODE[3]<<24;
        break;
    default:
        break;
    }
}