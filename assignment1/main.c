#include "assignment1.h"
#include <stdio.h>

int main()
{
    printf("Byte Sort\n");
    printf("original value passed in: %lx\n", 0x0403deadbeef0201);
    uint64_t foo = byte_sort(0x0403deadbeef0201);
    printf("value of foo is: %lx\n", foo);

    printf("Nibble Sort\n");
    printf("original value passed in: %lx\n", 0x0403deadbeef0201);
    uint64_t bar = nibble_sort(0x0403deadbeef0201);
    printf("value of bar is: %lx\n", bar);

    printf("Convert\n");
    printf("original value passed in: %x\n", 0Xdeadbeef);
    convert(HEX, 0Xdeadbeef);
    printf("Binary value\n");
    convert(BIN, 0Xdeadbeef);
    printf("Oct value\n");
    convert(OCT, 0Xdeadbeef);
    printf("hex result should be: 00000000deadbeef\n");
}
