/*
Operating Systems Assignment 1 Part A.
Author: Pranav Rajan u1136324@utah.edu
Version: January 21, 2022
*/

#include "assignment1.h"

// function for swapping elements in the array
void swap(uint8_t *a, uint8_t *b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

// function for swapping elements in selection sort
/*
Treats its argument as a sequence of 8 bytes and returns a new unsigned long integer
containing the same bytes,sorted numerically, with the smaller-valued bytes in the lower
-order byte positions of the return value.
*/
uint64_t byte_sort(uint64_t arg)
{
    // convert arg into an uint_8
    uint8_t *nums = (uint8_t*)&arg;

    // find the size of array
    int size = sizeof(arg) / sizeof(*nums);

    // printf("The number of elements: %d\n", size);

    // sort the bytes using insertion sort
    // int i, j;
    // uint8_t value;
    // for (i = 1; i < size; ++i)
    // {
    //     value = nums[i];
    //     j = i - 1;

    //     while (j >= 0 && nums[j] > value)
    //     {
    //         nums[j + 1] = nums[j];
    //         j = j - 1;
    //     }
    //     nums[j + 1] = value;
    // }

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

    // reassemble the long array into a uint64_t
    return *(uint64_t *)nums;
}
