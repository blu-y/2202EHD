#include "address_map_nios2.h"

static volatile int *HEX3_0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_4_ptr = (int*) HEX5_HEX4_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
static volatile int *SW_ptr = (int*) SW_BASE;
// memmory mapped IO to volatile variable

int read_key(void){
    return *KEY_ptr & 0b1111;
} // read last 4 bit of KEY_BASE

int read_sw(void){
    return *SW_ptr & 0b1111111111;
} // read last 10 bit of SW_BASE

int signed10(int n){
    int s = (n>>9)<<31;
    
}

int power(int a, int b){
    int p = 1;
    for (int i=0; i<b; i++) p *= a;
    return p;
} // get a^b

int HEX[11] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000
    }; // BIT_CODES : 0 1 2 3 4 5 6 7 8 9 NULL

void display_hex(int n){
    // display 6 digit number to hex
    int h[6] = {0}; // array for 6 digit
    unsigned int hex0 = 0; // bit codes for HEX 3-0
    // unsigned int hex4 = 0; // bit codes for HEX 5-4
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    } // separate 6 digit by dividing 10^(5~0)
    for (int i = 2; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    } // HEX3-0 = HEX3<<24 + HEX2<<16 + HEX1<<8 + HEX0
    *HEX3_0_ptr = hex0;
    // save HEX3-0 to HEX3_HEX0_BASE
    /*
    for (int i = 0; i <= 1; i++){
        hex4 += HEX[h[i]]<<(8*(1-i));
    } // HEX5-4 = HEX5<<8 + HEX4
    *HEX5_4_ptr = hex4;
    // save HEX5-4 to HEX5_HEX4_BASE
    */
    // no need to save HEX5-4
}

int main(void){
    int sec = 0;
    int run = 1;
    
}

