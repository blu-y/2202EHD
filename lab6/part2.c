#include "address_map_nios2.h"

static volatile int *LEDR_ptr = (int*) LEDR_BASE;

int max_element(int a[]){
    int max = 0;
    for (int i=1; i<=a[0]; i++){
        if (a[i]>max) max = a[i];
    }
    return max;
}

void display_led(int number){
    *LEDR_ptr = number;
}

int main(void){
    int LIST[8] = {7,4,5,3,6,1,8,2};
    int max = max_element(LIST);
    while(1){
        display_led(max);
    }
}

