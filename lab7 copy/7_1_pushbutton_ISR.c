#include "address_map_nios2.h"

extern volatile int *KEY_Ptr;
extern int *HEX3_HEX0;  //*HEX3_HEX0를 외부에서 참조하겠다는 것
int HEX_ON_OFF[4] = {0,0,0,0};

void FOR_HEX(int key_num, int ON_OFF) {	//HEX 출력을 위해 거치는 함수이다. current_num, key_num, ON_OFF를 인자로 받는다
	int convert_num;	//convert_num 변수 선언
	int HEX_NUM[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
			0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111 }; //0~9를 HEX에 출력하기 위한 8비트를 순서대로 HEX_NUM이라는 배열에 넣음
    int current_num = *(HEX3_HEX0); //HEX3_HEX0_BASE에 저장 돼있던 값을 current_num 변수에 담음 현재 상태를 보기 위해서
    if (ON_OFF == 0){   //만약 이전에 HEX가 꺼져있었다면 
        int convert = HEX_NUM[key_num]; //convert = HEX_NUM[key_num] key의 숫자에 따라 0,1,2,3을 띄워야 하기에 HEX_NUM의 key_num 번째를 convert에 저장한다
	    convert_num = convert<<key_num*8; //convert를 (key_num * 8)만큼 왼쪽으로 shift 시켜 convert_num에 넣는다 HEX 자리를 맞추기 위함
        current_num = current_num | convert_num;    //현재 HEX 저장 number와 convert_num을 or 하여 업데이트 한다.
	    *(HEX3_HEX0) = current_num; //업데이트 된 number를 HEX 자리에 넣어 HEX를 띄운다.
    }

    else if (ON_OFF == 1){  //반대로 이전에 HEX가 켜져있었다면
        int convert = HEX_NUM[key_num];  //convert = 0 이는 그 자리의 HEX를 0을 주어 끄기 위함
	    convert_num = convert<<key_num*8; //convert를 (key_num * 8)만큼 왼쪽으로 shift 시켜 convert_num에 넣는다 HEX 자리를 맞추기 위함
        current_num = current_num ^ convert_num;    
        //현재 HEX 저장 number와 convert_num에 0을 넣고 and 하여 무조건 0이 나오는 방법을 assmeply에서 행하였는데, 
        //c언어에서는 int 단위이므로 현재 HEX 저장 number와 convert_num에 그 자리에 똑같은 number를 주어 xor하여 0으로 만들어야 함.
        //assmbly에서 했던 방식은 모두 꺼지게 됨.
	    *(HEX3_HEX0) = current_num; //업데이트 된 number를 HEX 자리에 넣어 HEX를 띄운다.
    }
	
}


void pushbutton_ISR(void){
    
    int press = *(KEY_Ptr+3);   //edgecapture register를 읽어서 press에 저장
    *(KEY_Ptr+3) = press; //edge capture에 어떤 값을 쓰면 초기화됨
    if (press == 0b1){ //KEY0가 눌러졌을 때

        if (HEX_ON_OFF[0]==0) //HEX0가 꺼져있었다면
            {FOR_HEX(0,0);
            HEX_ON_OFF[0] = 1;}

        else if (HEX_ON_OFF[0]==1)
            {FOR_HEX(0,1);
            HEX_ON_OFF[0] = 0;}

    }
    else if (press ==0b10 ){    //KEY1이 눌러졌을 때

        if (HEX_ON_OFF[1]==0) //HEX1이 꺼져있었다면
            {FOR_HEX(1,0);
            HEX_ON_OFF[1]= 1;}

        else if (HEX_ON_OFF[1]==1)  //HEX1이 켜져있었다면
            {FOR_HEX(1,1);
            HEX_ON_OFF[1] = 0;}
    }
    else if (press == 0b100){    //KEY2가 눌러졌을 때

        if (HEX_ON_OFF[2]==0) //HEX2가 꺼져 있었다면
            {FOR_HEX(2,0);
            HEX_ON_OFF[2] = 1;}

        else if (HEX_ON_OFF[2]==1)  //HEX2가 켜져있었다면
            {FOR_HEX(2,1);
            HEX_ON_OFF[2] = 0;}
    }
    else if (press == 0b1000){   //KEY3가 눌러졌을 때

        if (HEX_ON_OFF[3]==0) //HEX3이 꺼져 있었다면
            {FOR_HEX(3,0);
            HEX_ON_OFF[3] = 1;}

        else if (HEX_ON_OFF[3]==1)  //HEX3이 켜져있었다면
            {FOR_HEX(3,1);
            HEX_ON_OFF[3] = 0;}
    }
}