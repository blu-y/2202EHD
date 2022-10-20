#define LEDR_BASE 0xFF200000
#define KEY_BASE 0xFF200050

static volatile int *LEDR_ptr = (int*) LEDR_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
// 전역변수
// volatile : 외부 요인에 의해 값이 변경될 경우
//            memory mapped io에 주로 사용

void display_led(int number){
    *LEDR_ptr = number;
}

int read_key(void){
    return *KEY_ptr;
}

int main(void){
    int val1 = 0;
    int val2 = 0;
    while(1){
        int key_value = read_key();
        val1 = key_value & 0x3;     // 마지막 2비트만
        val2 = (key_value >> 2) & 0x3; // 마지막 3~4번째비트
        display_led(val1 + val2);
    }
}