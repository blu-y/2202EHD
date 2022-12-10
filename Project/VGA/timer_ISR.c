#include "stdio.h"
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#define cos 0.96592582628
extern volatile int *TIMER_ptr;
extern volatile int *TIMER_2_ptr;
extern volatile int *JP1_ptr;
extern volatile int *JP2_ptr;
extern int state, r, a, b, a_, b_, speed, count, d;

void draw_char(int x, int y, char c);

void set_period2(int us){
    us = 100*us; // unit time 0.01us so us*100
    *(TIMER_2_ptr+2) = us;
    *(TIMER_2_ptr+3) = us>>16;
    // low 16 bit to periodl, high 16 bit to periodh
}
void config_timer2(int us){
    // input : period in microsec
    *TIMER_2_ptr = 0; // initialize
    set_period2(us);
    *(TIMER_2_ptr+1) = 0b0110; // STOP 0, START 1, CONT 1, ITO 0
}
int root(int num) {
    float tmp = 0;
    float ret = (float)num;
    while((tmp-ret)>0.1 || (tmp-ret)<-0.1) {
        tmp = ret;
        ret = 0.5*(ret + num/ret);
    }
    return (int)ret;
}
int measure(volatile int *sonic){
    int echo;
    int t = 0;
    int TO;
    int dist = 0;
    int cont = 1;
    config_timer2(10); // set timer2 10us
    *sonic = 0b1;    // set trigger 1
    while(1) {
        TO = (*TIMER_2_ptr)&0b1;
        if (TO==1) {
            *sonic = 0b0; // set trigger 0 after 10us
            *(TIMER_2_ptr+1) = 0b1010; // STOP 1, START 0, CONT 1, ITO 0
            *TIMER_2_ptr = 0;
            break;
        }
    }   // trigger ttl signal 10us
    while(1) {
        echo = *sonic;
        echo = (echo>>1) & 0b1;
        if (echo==1) {
            t = 0;
            break;
        }
        if (TO==1) {
            t += 1;
            *TIMER_2_ptr = 0;
        }
        if (t > 35000) {
            t = 0;
            cont = 0;
            break;
        }
    }   // wait until echo = 1 if no echo for 35ms, pass this turn
    config_timer2(1); // set timer2 1us
    while(cont) {
        TO = (*TIMER_2_ptr)&0b1;
        echo = *sonic;
        echo = (echo>>1) & 0b1;
        if (TO==1) {
            *TIMER_2_ptr = 0;
            t += 1;
        }
        if (echo==0) {
            dist = t/5.8; // echo pulse width(us) / 58 = distance(cm) 
            *(TIMER_2_ptr+1) = 0b1010; // STOP 1, START 0, CONT 1, ITO 0
            *TIMER_2_ptr = 0;
            break;
        }
    }   // wait until echo = 0, calculate distance a
    return dist;
}

void clearText(int j){
	volatile char * character_buffer = (char *) 0x09000000;	// VGA character buffer
	for (int i = 0;i <80;i++) {
		int offset;		
		offset = (j << 7) + (i); 			
		*(character_buffer + offset) = 0x00;	// 
		
	}
}

void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    /* state 1: measure sonic 1 distance = a */
    int xc;
    char* str;
    char* spd = "\0";
    switch (state) {
        case 1:
             
            a = measure(JP1_ptr);
            printf("a=%d\n", a);
            state++;
            clearText(11);
            clearText(12);
            clearText(13);
            str = "Calibrating..";
            xc = 10;
            while (*str) draw_char(xc++, 11, *str++);
            break;

        case 2:
            clearText(11);
            str = "Calibrating.....";
            xc = 10;
            while (*str) draw_char(xc++, 11, *str++);
            b = measure(JP2_ptr);
            printf("b=%d\n", b);
            d = root((r+a)*(r+a)+(r+b)*(r+b)-2*(r+a)*(r*b)*cos);
            printf("d=%d\n", d);
            state++;
           
            break;

        case 3:
            clearText(11);
            str = "Calibrated";
            xc = 10;
            while (*str) draw_char(xc++, 11, *str++);
            clearText(12);
            str = "Speed check";
            xc = 10;
            while (*str) draw_char(xc++, 12, *str++);
            if ((a_ = measure(JP1_ptr))==0) a_ = a;
            if (a-a_ > 30) {
                printf("a_=%d\n", a_);
                count = 0;
                state++;
            }
         
            break;

        case 4:
           
            if ((b_ = measure(JP2_ptr))==0) b_ = b;
            if (b-b_ > 30) {
                printf("b_=%d\n", b_);
                printf("cnt=%d\n", count);
                speed = d/count;    // speed(m/s) = distance(mm) / time(ms)
                printf("spd=%d\n", speed);
                state++;
            }
            break;
        
        default:
            clearText(13);
            str = ("Speed[m/s] : %s",speed);
            xc = 10;
            sprintf(str, "Speed[m/s] is : %d",speed);
            while (*str) draw_char(xc++, 13, *str++);
    }
    count++;
    *TIMER_ptr = 0;
}