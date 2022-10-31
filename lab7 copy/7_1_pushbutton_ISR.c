#include "address_map_nios2.h"

extern volatile int *KEY_Ptr;
extern int *HEX3_HEX0;  //*HEX3_HEX0�� �ܺο��� �����ϰڴٴ� ��
int HEX_ON_OFF[4] = {0,0,0,0};

void FOR_HEX(int key_num, int ON_OFF) {	//HEX ����� ���� ��ġ�� �Լ��̴�. current_num, key_num, ON_OFF�� ���ڷ� �޴´�
	int convert_num;	//convert_num ���� ����
	int HEX_NUM[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
			0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111 }; //0~9�� HEX�� ����ϱ� ���� 8��Ʈ�� ������� HEX_NUM�̶�� �迭�� ����
    int current_num = *(HEX3_HEX0); //HEX3_HEX0_BASE�� ���� ���ִ� ���� current_num ������ ���� ���� ���¸� ���� ���ؼ�
    if (ON_OFF == 0){   //���� ������ HEX�� �����־��ٸ� 
        int convert = HEX_NUM[key_num]; //convert = HEX_NUM[key_num] key�� ���ڿ� ���� 0,1,2,3�� ����� �ϱ⿡ HEX_NUM�� key_num ��°�� convert�� �����Ѵ�
	    convert_num = convert<<key_num*8; //convert�� (key_num * 8)��ŭ �������� shift ���� convert_num�� �ִ´� HEX �ڸ��� ���߱� ����
        current_num = current_num | convert_num;    //���� HEX ���� number�� convert_num�� or �Ͽ� ������Ʈ �Ѵ�.
	    *(HEX3_HEX0) = current_num; //������Ʈ �� number�� HEX �ڸ��� �־� HEX�� ����.
    }

    else if (ON_OFF == 1){  //�ݴ�� ������ HEX�� �����־��ٸ�
        int convert = HEX_NUM[key_num];  //convert = 0 �̴� �� �ڸ��� HEX�� 0�� �־� ���� ����
	    convert_num = convert<<key_num*8; //convert�� (key_num * 8)��ŭ �������� shift ���� convert_num�� �ִ´� HEX �ڸ��� ���߱� ����
        current_num = current_num ^ convert_num;    
        //���� HEX ���� number�� convert_num�� 0�� �ְ� and �Ͽ� ������ 0�� ������ ����� assmeply���� ���Ͽ��µ�, 
        //c������ int �����̹Ƿ� ���� HEX ���� number�� convert_num�� �� �ڸ��� �Ȱ��� number�� �־� xor�Ͽ� 0���� ������ ��.
        //assmbly���� �ߴ� ����� ��� ������ ��.
	    *(HEX3_HEX0) = current_num; //������Ʈ �� number�� HEX �ڸ��� �־� HEX�� ����.
    }
	
}


void pushbutton_ISR(void){
    
    int press = *(KEY_Ptr+3);   //edgecapture register�� �о press�� ����
    *(KEY_Ptr+3) = press; //edge capture�� � ���� ���� �ʱ�ȭ��
    if (press == 0b1){ //KEY0�� �������� ��

        if (HEX_ON_OFF[0]==0) //HEX0�� �����־��ٸ�
            {FOR_HEX(0,0);
            HEX_ON_OFF[0] = 1;}

        else if (HEX_ON_OFF[0]==1)
            {FOR_HEX(0,1);
            HEX_ON_OFF[0] = 0;}

    }
    else if (press ==0b10 ){    //KEY1�� �������� ��

        if (HEX_ON_OFF[1]==0) //HEX1�� �����־��ٸ�
            {FOR_HEX(1,0);
            HEX_ON_OFF[1]= 1;}

        else if (HEX_ON_OFF[1]==1)  //HEX1�� �����־��ٸ�
            {FOR_HEX(1,1);
            HEX_ON_OFF[1] = 0;}
    }
    else if (press == 0b100){    //KEY2�� �������� ��

        if (HEX_ON_OFF[2]==0) //HEX2�� ���� �־��ٸ�
            {FOR_HEX(2,0);
            HEX_ON_OFF[2] = 1;}

        else if (HEX_ON_OFF[2]==1)  //HEX2�� �����־��ٸ�
            {FOR_HEX(2,1);
            HEX_ON_OFF[2] = 0;}
    }
    else if (press == 0b1000){   //KEY3�� �������� ��

        if (HEX_ON_OFF[3]==0) //HEX3�� ���� �־��ٸ�
            {FOR_HEX(3,0);
            HEX_ON_OFF[3] = 1;}

        else if (HEX_ON_OFF[3]==1)  //HEX3�� �����־��ٸ�
            {FOR_HEX(3,1);
            HEX_ON_OFF[3] = 0;}
    }
}