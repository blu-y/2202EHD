#include "address_map_nios2.h"
#define cos 0.96592582628
extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern int count;
extern int r;
extern float d;
extern int a;
extern int b;
extern int state;     // access external variable
float sqrt(int num) {
  float tmp = num;
  for(int i=0;i<15;i++) {
    tmp = 0.5*(tmp + num/tmp);
  }
  return tmp;
}
void key_ISR(void){
    /* KEY - Interrupt Service Routine */
    int press = *(KEY_ptr + 3);     // read the edgecapture register
    *(KEY_ptr + 3) = press;     // clear the edgecapture register turnoff IRQ
    /* Calibrate */
    d = sqrt((r+a)*(r+a)+(r+b)*(r+b)-2*(r+a)*(r*b)*cos);
    count = 0;
    state = 1;
}