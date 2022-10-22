//#include "address_map_nios2.h"
#include "stdio.h"
#define SDRAM_BASE				0x00000000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE					0xFF200040
#define KEY_BASE				0xFF200050

static volatile int *HEX3_0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_4_ptr = (int*) HEX5_HEX4_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
static volatile int *SW_ptr = (int*) SW_BASE;
static int numbers;
// memmory mapped IO to volatile variable

int read_key(void){
    return *KEY_ptr & 0b1111;
} // read last 4 bit of KEY_BASE

int read_sw(void){
    return *SW_ptr & 0b1111111111;
} // read last 10 bit of SW_BASE

int signed10(int n) {
    if(n>=512) n = -((1023-n)+1);
    // n >= 0b1000000000 then it's minus
    // and 0b1111111111 - n + 1 it numberpart
    return n;
} // make 10bit signed number to 32bit

void append(int value){
    numbers += 1;
    // SDRAM_END contains length of set
    int *new = (int*) (SDRAM_BASE+(4*numbers));
    // new value is stored at SDRAM_BASE + 4*n
    *new = value;
}

int read_num(int n){
    // read n-th value
    int *ptr = (int*) (SDRAM_BASE+(4*n));
    return *ptr;
}

void show_num(void){
    for (int i = 1; i <= numbers; i++) printf("%d ", read_num(i));
    printf("\n");
} // print current element

int max_element(void){
    int max = -512;
    for (int i = 1; i <= numbers; i++){
        if (max < read_num(i)) max = read_num(i);
    }
    return max;
} // return max element

int power(int a, int b){
    int p = 1;
    for (int i=0; i<b; i++) p *= a;
    return p;
} // get a^b

int HEX[12] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01100111, 0b00000000, 0b01000000
    }; // BIT_CODES : 0 1 2 3 4 5 6 7 8 9 NULL -

void display_hex_signed(int n){
    // display 1 digit sign and 3 digit number to hex
    int minus = 0;
    if (n < 0){
        minus = 1;
        n = -n;
    } // separate sign and number
    int h[6] = {0}; // array for 6 digit
    unsigned int hex0 = 0; // bit codes for HEX 3-0
    // unsigned int hex4 = 0; // bit codes for HEX 5-4
    for (int i = 0; i < 6; i++){
        h[i] = n / power(10, 5-i);
        n = n - h[i] * power(10, 5-i);
    } // separate 6 digit by dividing 10^(5~0)
    for (int i = 3; i <= 5; i++){
        hex0 += HEX[h[i]]<<(8*(5-i));
    } // HEX2-0 = HEX2<<16 + HEX1<<8 + HEX0
    if (minus) hex0 += HEX[11]<<24; // put - if minus
    *HEX3_0_ptr = hex0;
}

void reset_hex(void){*HEX3_0_ptr = 0; *HEX5_4_ptr = 0;}

int main(void){
    reset_hex();
    numbers = 0;
    while(1){
        switch(read_key()){
            case 2: // if KEY1 pressed : add element
                while (read_key()) read_key(); // wait until key release
                append(signed10(read_sw())); // append signed num of sw
                show_num();
                break;
            case 4: // if KEY2 pressed : display max
                while (read_key()) read_key(); // wait until key release
                display_hex_signed(max_element());
                break;
            case 8: // if KEY3 pressed : reset hex and n
                while (read_key()) read_key(); // wait until key release
                reset_hex();
                numbers = 0;
                break;
            default:
                break;
        }

    }
}

