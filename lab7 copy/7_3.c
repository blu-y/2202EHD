#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_Ptr = (int*)KEY_BASE;
volatile int *TIMER_Ptr = (int*)TIMER_BASE;
volatile int *LED_Ptr = (int*)LEDR_BASE;

int count = 0;  //global counter for red lights
int run = 1;    //global,used to increment/not the count variable
int period = 25000000; //period 변수 전역 변수로 선언 10^8/4로 초기화 처음엔 0.25초 주기 이기 때문이다.

void config_timer(void){    //timer setting을 위한 함수이다.
    int counter_start_value = period; //0.25마다 to가 1이 되게 하기 위해 periodl,h에 넣어야 하는 값
    *(TIMER_Ptr+2) = counter_start_value;   //periodl에 하위 16비트를 넣음
    counter_start_value = counter_start_value >>16; //counter_start_value를 16비트 왼쪽으로 shift하여 상위 16비트가 밑으로 오게 함
    *(TIMER_Ptr+3) = counter_start_value;   //periodh에 상위 16비트를 넣음
    *(TIMER_Ptr+1) = 0b0111;    //control register의 start, cont, ito에 1을 주는 것임
}

void config_KEYs(void){ //KEY3,2,1이 interrupt 발생 가능하게 준비 하는 함수
    *(KEY_Ptr + 2) = 0b1110;
}


void enable_nios2_interrupts(void){ //interrput 준비 함수
    NIOS2_WRITE_IENABLE(0b11); //KEY와 TIMER에서 interrupt를 발생시키게 한다.
    NIOS2_WRITE_STATUS(0b1); //전체적으로 Nios 2가 interrupt 가능하게 한다.
}

void main(void){
    config_timer();  //timer setting
    config_KEYs();  //KEY3,2,1이 interrput를 걸 수 있게 함
    enable_nios2_interrupts();  //nios2가 interrupt를 걸 수 있게 함
    while(1){   //main을 끝내지 않고 무한 루프에서 인터럽트가 발생하는 것을 계속 대기하고 있는다.
        *LED_Ptr = count;
    }
}
