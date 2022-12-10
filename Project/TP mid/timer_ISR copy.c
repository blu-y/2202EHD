#include "address_map_nios2.h"
#include "ultrasonic_address.h"
#define cos 0.96592582628
extern volatile int *TIMER_ptr;
extern volatile int *TIMER_2_ptr;
extern volatile int *JP1_ptr;
extern int state, a, u0, dt, d, r;
void set_period(int us){
    us = 100*us; // unit time 0.01us so us*100
    *(TIMER_ptr+2) = us;
    *(TIMER_ptr+3) = us>>16;
    // low 16 bit to periodl, high 16 bit to periodh
}
void config_timer(int us){
    // input : period in microsec
    *TIMER_ptr = 0; // initialize
    set_period(us);
    *(TIMER_ptr+1) = 0b0111; // STOP 0, START 1, CONT 1, ITO 1
}
int root(int num) {
  int tmp = num;
  for(int i=0;i<15;i++) {
    tmp = 0.5*(tmp + num/tmp);
  }
  return tmp;
}

void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (u0 == 1) {
        *JP1_ptr = 0b1<<t0;
        u0 = 2;
    }
    else if (u0 == 2) {
        *JP1_ptr = 0;
        u0 = 3;
    }
    else if (u0 == 3) {
        int echo = *JP1_ptr;
        echo = 0b1 & (echo>>e0);
        //printf("%d ", echo);
        if (echo) {
            if (state==1) {
                a = count;
                a = a * 1.7;
                u0 = 0;
                state += 1;
            }
    }}
    else if (u0 == 4) {
        int echo = *JP1_ptr;
        echo = 0b1 & (echo>>e0);
        //printf("%d ", echo);
        if (echo) {
            if (state==1) {
                a = count - a;
                a = a * 1.7;
                u0 = 0;
                state += 1;
            }
    }}
    *TIMER_ptr = 0;
    count++;
}