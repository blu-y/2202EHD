#include <stdio.h>
#include "address_map_nios2.h"

extern volatile int *TIMER_Ptr;		//외부에서 지정하였던 TIMER_ptr변수를 참조
extern int count;			//외부에서 지정하였던 count변수를 참조
extern int run;				//외부에서 지정하였던 run변수를 참조

void timer_ISR()
{
	if(run == 1)	//run이 1일 때 즉, timer가 작동 중일 때
	{
		count++;	//count에 1을 더한다
	}
	*TIMER_Ptr=0b0;	//TIMER의 TO를 초기화
    
	return;
}
