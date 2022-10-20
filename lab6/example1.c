#include <stdio.h>

int add(int a, int b){
    return a+b;
}

int main(void){
    printf("1 + 2 = %d", add(1,2));
}