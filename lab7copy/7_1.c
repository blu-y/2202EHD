#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_Ptr = (int*)KEY_BASE; 
volatile int *HEX3_HEX0 = (int*)HEX3_HEX0_BASE;

void config_KEYs(){ //모든 KEY가 interrupt 발생 가능하게 준비 하는 함수
    *(KEY_Ptr + 2) = 0b1111;
}

void enable_nios2_interrupts(void){ //interrput 준비 함수
    NIOS2_WRITE_IENABLE(0b10); //KEY에서 interrupt를 발생시키게 한다.
    NIOS2_WRITE_STATUS(0b1); //전체적으로 Nios 2가 interrupt 가능하게 한다.
}

int main(void){
    config_KEYs();  //모든 KEY가 interrput를 걸 수 있게 함
    enable_nios2_interrupts();  //nios2가 interrupt를 걸 수 있게 함
    *HEX3_HEX0 = 0;  //HEX 초기화
    while(1){   //main을 끝내지 않고 무한 루프에서 인터럽트가 발생하는 것을 계속 대기하고 있는다.
    }
}