#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_Ptr = (int*)KEY_BASE; 
volatile int *HEX3_HEX0 = (int*)HEX3_HEX0_BASE;

void config_KEYs(){ //��� KEY�� interrupt �߻� �����ϰ� �غ� �ϴ� �Լ�
    *(KEY_Ptr + 2) = 0b1111;
}

void enable_nios2_interrupts(void){ //interrput �غ� �Լ�
    NIOS2_WRITE_IENABLE(0b10); //KEY���� interrupt�� �߻���Ű�� �Ѵ�.
    NIOS2_WRITE_STATUS(0b1); //��ü������ Nios 2�� interrupt �����ϰ� �Ѵ�.
}

int main(void){
    config_KEYs();  //��� KEY�� interrput�� �� �� �ְ� ��
    enable_nios2_interrupts();  //nios2�� interrupt�� �� �� �ְ� ��
    *HEX3_HEX0 = 0;  //HEX �ʱ�ȭ
    while(1){   //main�� ������ �ʰ� ���� �������� ���ͷ�Ʈ�� �߻��ϴ� ���� ��� ����ϰ� �ִ´�.
    }
}