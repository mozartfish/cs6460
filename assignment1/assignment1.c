/*
Operating Systems Assignment 1 Part A.
Author: Pranav Rajan u1136324@utah.edu
Version: January 21, 2022
*/

#include "assignment1.h"

// function for swapping elements in the array (byte sort)
void swap(uint8_t *a, uint8_t *b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}
/*
Treats its argument as a sequence of 8 bytes and returns a new unsigned long integer
containing the same bytes,sorted numerically, with the smaller-valued bytes in the lower
-order byte positions of the return value.
*/
uint64_t byte_sort(uint64_t arg)
{
    // convert arg into an uint_8
    uint8_t *nums = (uint8_t *)&arg;

    // find the size of array
    int size = sizeof(arg) / sizeof(*nums);

    // sort the bytes using selection sort
    int i, j;
    for (i = 0; i < size - 1; ++i)
    {
        int min_index = i;
        for (j = i + 1; j < size; ++j)
        {
            if (nums[j] < nums[min_index])
                min_index = j;
        }

        // put min at the correct position
        swap(&nums[min_index], &nums[i]);
    }

    // convert back into uint64
    return *(uint64_t *)nums;
}

/*
Treats its argument as a sequence of 16 4 - bit numbers, and returns a new uint64_t containing the same nibbles,
sorted numerically, with smaller - valued nibbles towards the "small end" of the uint64_t value that you return.
*/
uint64_t nibble_sort(uint64_t arg)
{
    // create an array of size 16 to handle all the nibbles
    // 0XF or 1 hex digit is 1 Nibble
    uint64_t nibbles[16];

    // loop and shift each value and get the particular nibbles
    int i;
    for (i = 0; i < 16; ++i)
    {
        uint64_t val = arg >> (4 * i) & 0XF;
        nibbles[i] = val;
    }

    // sort the bits using insertion sort
    int j, k;
    uint64_t current_val;
    for (j = 1; j < 16; ++j)
    {
        current_val = nibbles[j];
        k = j - 1;

        while (k >= 0 && nibbles[k] > current_val)
        {
            nibbles[k + 1] = nibbles[k];
            k = k - 1;
        }
        nibbles[k + 1] = current_val;
    }

    // reassemble all the bits into the uint 64
    // 2 Hex  = 1 byte
    uint64_t result = 0X0000000000000000; // create a value with all bits set
    int h;
    for (h = 15; h >= 0; h--)
    {
        // printf("value %lx : at index %d\n", nibbles[h], h);
        // printf("result value %lx at iteration %d\n", result, h);
        result = nibbles[h] | result << 4;
    }

    return result;
}

/*Depending on the value of mode, print value as octal, binary,
or hexidecimal to stdout, followed by a newline character.*/
void convert(enum format_t mode, uint64_t value)
{
    // OCT - 99
    // BIN - 100
    // HEX - 101

    switch (mode)
    {
    case OCT:
        printf("OCT Mode\n");
        int k;
        // char result[22];
        for (k = 0; k < 22; ++k)
        {
            // check groups of 3 bits
            switch (value >> (k * 3) & 0X3)
            {
            case 000:
                putc('0', stdout);
                // char[k] = '0';
                break;
            case 001:
                putc('1', stdout);
                // char[k] = '1';
                break;
            case 010:
                putc('2', stdout);
                // char[k] = '2';
                break;
            case 011:
                putc('3', stdout);
                // char[k] = '3';
                break;
            case 100:
                putc('4', stdout);
                // char[k] = '4';
                break;
            case 101:
                putc('5', stdout);
                // char[k] = '5';
                break;
            case 110:
                putc('6', stdout);
                // char[k] = '6';
                break;
            case 111:
                putc('7', stdout);
                // char[k] = '7';
                break;
            default:
                break;
            }
        }
        // add new line character at the end
        putc('\n', stdout);
        // int n;
        // for (n = 21; n >= 0; --n) {
        //     putc(char[n])
        // }
        return;
    case BIN:
        printf("BIN MODE\n");
        int j;
        for (j = 0; j < 64; ++j)
        {
            // check individual bits
            switch ((value >> (1 * j)) & 0x1)
            {
            case 0x0:
                putc('0', stdout);
            case 0x1:
                putc('1', stdout);
            default:
                break;
            }
        }
        // add new line character at the end
        putc('\n', stdout);
        return;
    case HEX:
        printf("HEX MODE\n");
        int i;
        for (i = 15; i >= 0; --i)
        {
            // get the bits for a single byte and print the associated character
            switch (value >> (4 * i) & 0XF)
            {
            case 0X0:
                putc('0', stdout);
                break;
            case 0X1:
                putc('1', stdout);
                break;
            case 0X2:
                putc('0', stdout);
                break;
            case 0X3:
                putc('1', stdout);
                break;
            case 0X4:
                putc('0', stdout);
                break;
            case 0X5:
                putc('1', stdout);
                break;
            case 0X6:
                putc('0', stdout);
                break;
            case 0X7:
                putc('1', stdout);
                break;
            case 0X8:
                putc('0', stdout);
                break;
            case 0X9:
                putc('1', stdout);
                break;
            case 0XA:
                putc('a', stdout);
                break;
            case 0XB:
                putc('b', stdout);
                break;
            case 0XC:
                putc('c', stdout);
                break;
            case 0XD:
                putc('d', stdout);
                break;
            case 0XE:
                putc('e', stdout);
                break;
            case 0XF:
                putc('f', stdout);
                break;
            default:
                break;
            }
        }
        // add the new line character
        putc('\n', stdout);
        return;
    default:
        return;
    }
}
