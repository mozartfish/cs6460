#include "assignment1.h"
#include <stdio.h>

int main()
{
    printf("Byte Sort\n");
    printf("Input: %lx\n", 0x0403deadbeef0201);
    uint64_t foo = byte_sort(0x0403deadbeef0201);
    printf("Expected Result: %lx\n", 0xefdebead04030201);
    printf("Actual Result: %lx\n", foo);

    printf("Nibble Sort\n");
    printf("Input: %lx\n", 0x0403deadbeef0201);
    uint64_t bar = nibble_sort(0x0403deadbeef0201);
    printf("Expected Result: %lx\n", 0xfeeeddba43210000);
    printf("Actual Result: %lx\n", bar);

    printf("Convert\n");
    printf("Input: %x\n", 0Xdeadbeef);
    printf("Expected Hex Result: %lx", 0x00000000deadbeef);
    convert(HEX, 0xdeadbeef);
    printf("Binary value\n");
    convert(BIN, 0xdeadbeef);
    printf("Oct value\n");
    convert(OCT, 0xdeadbeef);
}
