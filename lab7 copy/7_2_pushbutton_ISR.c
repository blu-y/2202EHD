#include <stdio.h>
#include "address_map_nios2.h"

extern volatile int *KEY_Ptr;		//외부에서 지정하였던 KEY_ptr변수를 참조
extern volatile int *TIMER_Ptr;		//외부에서 지정하였던 TIMER_ptr변수를 참조
extern int run;				//외부에서 지정하였던 run변수를 참조

void pushbutton_ISR()
{
	int press;		//press 변수 선언
	press=*(KEY_Ptr+3);	//edge capture register안의 값을 press에 저장
    *(KEY_Ptr+3)= press;	//edge capture register에 어떤 값을 다시 써서 초기화 함

	if(press==0b1000)		//눌러진 것이 KEY3라면
	{
		if(run==1)		//run=1이라면 즉, 이는 타이머가 작동 중이라는 의미
		{
			*(TIMER_Ptr+1)=0b1011;	//control register에 STOP, CONT, ITO에 1을 저장하여 타이머를 stop함
			run=0;		//run에 0을 주어 stop 상태임을 알림
		}
		else			//run이 1이 아니라면 0이므로 stop 상태임
		{
			*(TIMER_Ptr+1)=0b0111;	//control register에 start, cont, ito에 1을 저장하여 timer를 작동 시킴
			run=1;		//run에 1을 주어 타이머가 작동 상태임을 알림
		}
	}
    
	return;
}
