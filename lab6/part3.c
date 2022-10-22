#include "address_map_nios2.h"

static volatile int *HEX3_0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_4_ptr = (int*) HEX5_HEX4_BASE;
// memmory mapped IO to volatile variable

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
    unsigned int hex4 = 0; // bit codes for HEX 5-4
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    } // separate 6 digit by dividing 10^(5~0)
    for (int i = 2; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    } // HEX3-0 = HEX3<<24 + HEX2<<16 + HEX1<<8 + HEX0
    *HEX3_0_ptr = hex0;
    // save HEX3-0 to HEX3_HEX0_BASE
    for (int i = 0; i <= 1; i++){
        hex4 += HEX[h[i]]<<(8*(1-i));
    } // HEX5-4 = HEX5<<8 + HEX4
    *HEX5_4_ptr = hex4;
    // save HEX5-4 to HEX5_HEX4_BASE
}

int longest1(unsigned int n){
    // continous 1 of binary
    int l = 0;
    while(n!=0){
        n = n & (n>>1);
        l++;
    }
    return l;
    // count if n&(n>>1) is not 0 
}

int longest(int n[], int num){
    // input : array, length of array
    int l1 = 0;
    int l0 = 0;
    int l10 = 0;
    for (int i = 0; i < num; i++)
        if (l1 < longest1(n[i])) l1 = longest1(n[i]);
    // replace l1 if new value is bigger
    for (int i = 0; i < num; i++){
        n[i] = ~n[i];
        if (l0 < longest1(n[i])) l0 = longest1(n[i]);
        n[i] = ~n[i];
    }
    // continous 0 means continous 1 of inversion
    // replace l0 if new value is bigger
    for (int i = 0; i < num; i++){
        if (l10 <= (longest1(n[i] ^ (n[i]>>1))))
            l10 = longest1(n[i] ^ (n[i]>>1)) + 1;
    }
    // use xor gate to detect alternating bits
    // +1 is to detect first missing bit by shifting
    // ex) 01 = 1 +1
    // replace l10 if new value is bigger
    return l10*10000+l0*100+l1;  // 10 00 11
}

int main(void){
    int TEST_NUM[] = {
        0x0000e000, 0x3fabedef, 0x00000001, 0x00000002,
        0x75a5a5a5, 0x01ffC000, 0x03ffC000, 0x55555555,
        0x77777777, 0x08888888, 0x00000000
        };
    int max = longest(TEST_NUM, 10); // get longest 10 00 11 in 6 digit
    while(1){
        display_hex(max); // display max to HEX
    }
}

