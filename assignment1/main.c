#include "assignment1.h"
#include <stdio.h>

int main()
{
    // printf("hello, world\n");
    // return 0;
    printf("original value passed in: %lx\n", 0x0403deadbeef0201);
    uint64_t foo = byte_sort(0x0403deadbeef0201);
    printf("value of foo is: %lx\n", foo);
}
