#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_Ptr = (int*)KEY_BASE;
volatile int *TIMER_Ptr = (int*)TIMER_BASE;
volatile int *LED_Ptr = (int*)LEDR_BASE;

int count = 0;  //global counter for red lights
int run = 1;    //global,used to increment/not the count variable
int period = 25000000; //period ���� ���� ������ ���� 10^8/4�� �ʱ�ȭ ó���� 0.25�� �ֱ� �̱� �����̴�.

void config_timer(void){    //timer setting�� ���� �Լ��̴�.
    int counter_start_value = period; //0.25���� to�� 1�� �ǰ� �ϱ� ���� periodl,h�� �־�� �ϴ� ��
    *(TIMER_Ptr+2) = counter_start_value;   //periodl�� ���� 16��Ʈ�� ����
    counter_start_value = counter_start_value >>16; //counter_start_value�� 16��Ʈ �������� shift�Ͽ� ���� 16��Ʈ�� ������ ���� ��
    *(TIMER_Ptr+3) = counter_start_value;   //periodh�� ���� 16��Ʈ�� ����
    *(TIMER_Ptr+1) = 0b0111;    //control register�� start, cont, ito�� 1�� �ִ� ����
}

void config_KEYs(void){ //KEY3,2,1�� interrupt �߻� �����ϰ� �غ� �ϴ� �Լ�
    *(KEY_Ptr + 2) = 0b1110;
}


void enable_nios2_interrupts(void){ //interrput �غ� �Լ�
    NIOS2_WRITE_IENABLE(0b11); //KEY�� TIMER���� interrupt�� �߻���Ű�� �Ѵ�.
    NIOS2_WRITE_STATUS(0b1); //��ü������ Nios 2�� interrupt �����ϰ� �Ѵ�.
}

void main(void){
    config_timer();  //timer setting
    config_KEYs();  //KEY3,2,1�� interrput�� �� �� �ְ� ��
    enable_nios2_interrupts();  //nios2�� interrupt�� �� �� �ְ� ��
    while(1){   //main�� ������ �ʰ� ���� �������� ���ͷ�Ʈ�� �߻��ϴ� ���� ��� ����ϰ� �ִ´�.
        *LED_Ptr = count;
    }
}
