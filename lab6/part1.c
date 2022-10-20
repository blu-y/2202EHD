#include <stdio.h>

int max_element(int a[]){
    int max = 0;
    for (int i=1; i<=a[0]; i++){
        if (a[i]>max) max = a[i];
    }
    return max;
}

int main(void){
    int LIST[8] = {7,4,5,3,6,1,8,2};
    printf("MAX = %d\n", max_element(LIST));
}