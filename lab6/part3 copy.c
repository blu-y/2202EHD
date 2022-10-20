#include "address_map_nios2.h"
#include "stdio.h"

static volatile int *HEX3_0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_4_ptr = (int*) HEX5_HEX4_BASE;

int power(int a, int b){
    int p = 1;
    for (int i=0; i<b; i++) p *= a;
    return p;
}

int HEX[11] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000
    }; // 0 1 2 3 4 5 6 7 8 9 NULL

void display_hex(int n){
    int h[6] = {0};
    unsigned int hex0 = 0;
    unsigned int hex4 = 0;
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    }
    for (int i = 2; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    }
    //*HEX3_0_ptr = hex0;
    for (int i = 0; i <= 1; i++){
        hex4 += HEX[h[i]]<<(8*(1-i));
    }
    //*HEX5_4_ptr = hex4;
}

int longest1(unsigned int n){
    int l = 0;
    while(n!=0){
        n = n & (n>>1);
        l++;
    }
    return l;
    // returns continous 1 of binary
}

int longest(int n[], int num){
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
    // +1 is to detect first missing bit ex) 01 = 1+1
    // replace l10 if new value is bigger
    return l10*10000+l0*100+l1;  // 10 00 11
}

int main(void){
    int TEST_NUM[] = {
        0x0000e000, 0x3fabedef, 0x00000001, 0x00000002,
        0x75a5a5a5, 0x01ffC000, 0x03ffC000, 0x55555555,
        0x77777777, 0x08888888, 0x00000000
        };
    int max = longest(TEST_NUM, 10);
    printf("%d\n", max);
    display_hex(max);
}

