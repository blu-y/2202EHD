#include <stdio.h>
#include "address_map_nios2.h"

extern volatile int *TIMER_Ptr;		//�ܺο��� �����Ͽ��� TIMER_ptr������ ����
extern int count;			//�ܺο��� �����Ͽ��� count������ ����
extern int run;				//�ܺο��� �����Ͽ��� run������ ����

void timer_ISR()
{
	if(run == 1)	//run�� 1�� �� ��, timer�� �۵� ���� ��
	{
		count++;	//count�� 1�� ���Ѵ�
	}
	*TIMER_Ptr=0b0;	//TIMER�� TO�� �ʱ�ȭ
    
	return;
}
