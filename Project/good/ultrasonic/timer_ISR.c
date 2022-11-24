#include "stdio.h"
#include "address_map_nios2.h"
#define cos 0.96592582628
extern volatile int *TIMER_ptr;
extern volatile int *TIMER_2_ptr;
extern volatile int *JP1_ptr;
extern volatile int *JP2_ptr;
extern int state, a, u0, dt, d, r;
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

void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (state==1) {
        int echo;
        int count = 0;
        int TO;
        config_timer2(10); // set timer2 10us
        *JP1_ptr = 0b1;    // set trigger 1
        while(1) {
            TO = (*TIMER_2_ptr)&0b1;
            if (TO==1) {
                *JP1_ptr = 0b0; // set trigger 0 after 10us
                *(TIMER_2_ptr+1) = 0b1010; // STOP 1, START 0, CONT 1, ITO 0
                *TIMER_2_ptr = 0;
                break;
            }
        }   // trigger ttl signal 10us
        while(1) {
            echo = *JP1_ptr;
            echo = (echo>>1) & 0b1;
            if (echo==1) break;
        }   // wait until echo = 1
        config_timer2(1); // set timer2 1us
        while(1) {
            TO = (*TIMER_2_ptr)&0b1;
            echo = *JP1_ptr;
            echo = (echo>>1) & 0b1;
            if (TO==1) {
                *TIMER_2_ptr = 0;
                count += 1;
            }
            if (echo==0) {
                a = count/58; // echo pulse width(us) / 58 = distance(cm) 
                *(TIMER_2_ptr+1) = 0b1010; // STOP 1, START 0, CONT 1, ITO 0
                *TIMER_2_ptr = 0;
                state = 2;
                break;
            }
        }   // wait until echo = 0, calculate distance a
    }
    *TIMER_ptr = 0;
    return;
}
