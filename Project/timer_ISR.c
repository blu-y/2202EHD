#include "address_map_nios2.h"
#include "ultrasonic_address.h"
extern volatile int *TIMER_ptr;
extern volatile int *JP1_ptr;
extern int count;
extern int state;     // access external variable
extern int a;
extern int b;
extern int a_;
extern int b_;
extern int u0;
extern int u1;
extern int dt;
void timer_ISR(void){
    /* TIMER - Interrupt Service Routine */
    if (u0 == 1) {
        *JP1_ptr = 0b1<<t0;
        u0 = 2;
        if (state==1) a = count;
        if (state==3) a_ = count;
    }
    else if (u0 == 2) {
        *JP1_ptr = 0b0;
        u0 = 3;
    }
    else if (u0 == 3) {
        int echo = *JP1_ptr;
        echo = 0b1 & (echo>>e0);
        if (echo) {
            if (state==1) {
                a = count - a;
                a = a * 1.7;
                u0 = 0;
                state += 1;
            }
            if (state==3) {
                state += 1;
                a_ = count - a_;
                a_ = a_ * 1.7;
                u0 = 0;
                if ((a-a_) > 100) {
                    dt = count;
                    state += 1;
    }}}}
    else if (u1 == 1) {
        *JP1_ptr = 0b1<<t1;
        u1 = 2;
        if (state==2) b = count;
        if (state==4) b_ = count;
    }
    else if (u1 == 2) {
        *JP1_ptr = 0b0;
        u1 = 3;
    }
    else if (u1 == 3) {
        int echo = *JP1_ptr;
        echo = 0b1 & (echo>>e1);
        if (echo) {
            if (state==2) {
                b = count - b;
                b = b * 1.7;
                u1 = 0;
                state += 1;
            }
            if (state==4) {
                int tmp = count - dt;
                state += 1;
                b_ = count - b_;
                b_ = b_ * 1.7;
                u1 = 0;
                if ((b-b_) > 100) {
                    dt = tmp/100000;
                    state += 1;
    }}}}
    *TIMER_ptr = 0;
    count++;
}