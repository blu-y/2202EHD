#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_Ptr = (int*)KEY_BASE; //KEY_BASE의 포인터를 KEY_Ptr 포인터로 선언
volatile int *TIMER_Ptr = (int*)TIMER_BASE; //TIMER_BASE의 포인터를 TIMER_Ptr 포인터로 선언
volatile int *LED_Ptr = (int*)LEDR_BASE;    //LEDR_BASE의 포인터를 LED_Ptr 포인터로 선언

int count = 0;  //global counter for red lights
int run = 1;    //global,used to increment/not the count variable

void config_timer(void){    //timer setting을 위한 함수이다.
    int counter_start_value = 25000000; //0.25마다 to가 1이 되게 하기 위해 periodl,h에 넣어야 하는 값
    *(TIMER_Ptr+2) = counter_start_value;   //periodl에 하위 16비트를 넣음
    counter_start_value = counter_start_value >>16; //counter_start_value를 16비트 왼쪽으로 shift하여 상위 16비트가 밑으로 오게 함
    *(TIMER_Ptr+3) = counter_start_value;   //periodh에 상위 16비트를 넣음
    *(TIMER_Ptr+1) = 0b0111;    //control register의 start, cont, ito에 1을 주는 것임
}

void config_KEYs(void){ //KEY3가 interrupt 발생 가능하게 준비 하는 함수
    *(KEY_Ptr + 2) = 0b1000;  //interruptmask register에 0b1000을 넣음 이는 KEY3가 interrupt를 발생 가능하게 하겠다는 의미임.
}


void enable_nios2_interrupts(void){ //interrput 준비 함수
    NIOS2_WRITE_IENABLE(0b11); //KEY와 TIMER에서 interrupt를 발생시키게 한다.
    NIOS2_WRITE_STATUS(0b1); //전체적으로 Nios 2가 interrupt 가능하게 한다.
}

void main(void){
    config_timer(); //timer setting
    config_KEYs();  //KEY interrupt 준비
    enable_nios2_interrupts();  //nios2가 interrupt를 걸 수 있게 함
    while(1){   //무한루프
        *LED_Ptr = count;   //*LED_Ptr에 count를 넣는다.
    }
}
