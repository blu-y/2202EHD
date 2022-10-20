#define LEDR_BASE 0xFF200000
#define KEY_BASE 0xFF200050

static volatile int *LEDR_ptr = (int*) LEDR_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
// ��������
// volatile : �ܺ� ���ο� ���� ���� ����� ���
//            memory mapped io�� �ַ� ���

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
        val1 = key_value & 0x3;     // ������ 2��Ʈ��
        val2 = (key_value >> 2) & 0x3; // ������ 3~4��°��Ʈ
        display_led(val1 + val2);
    }
}