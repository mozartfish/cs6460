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
