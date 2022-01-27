/*
Operating Systems Assignment 1
Author: Pranav Rajan u1136324@utah.edu
Version: January 26, 2022
*/
#include "assignment1.h"
#include <fcntl.h>

// Operating Systems Assignment 1 Part A.

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
        uint64_t val = arg >> (4 * i) & 0xF;
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
    // | bit operator adds bits
    uint64_t result = 0x0000000000000000; // create a value with all bits set
    int h;
    for (h = 15; h >= 0; h--)
    {
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
    int i, j, k;
    switch (mode)
    {
    case OCT:
        for (k = 21; k >= 0; --k)
        {
            // check groups of 3 bits
            // octal - base 8
            switch (value >> (k * 3) & 0x7)
            {
            case 0x0:
                putc('0', stdout);
                break;
            case 0x1:
                putc('1', stdout);
                break;
            case 0x2:
                putc('2', stdout);
                break;
            case 0x3:
                putc('3', stdout);
                break;
            case 0x4:
                putc('4', stdout);
                break;
            case 0x5:
                putc('5', stdout);
                break;
            case 0x6:
                putc('6', stdout);
                break;
            case 0x7:
                putc('7', stdout);
                break;
            default:
                break;
            }
        }
        // add new line character at the end
        putc('\n', stdout);
        return;
    case BIN:
        // binary - base 2
        for (j = 63; j >= 0; --j)
        {
            // check individual bits
            switch (value >> (1 * j) & 0x1)
            {
            case 0x0:
                putc('0', stdout);
                break;
            case 0x1:
                putc('1', stdout);
                break;
            default:
                break;
            }
        }
        // add new line character at the end
        putc('\n', stdout);
        return;
    case HEX:
        // base - 16
        for (i = 15; i >= 0; --i)
        {
            // get the bits for a single byte and print the associated character
            switch (value >> (4 * i) & 0xF)
            {
            case 0x0:
                putc('0', stdout);
                break;
            case 0x1:
                putc('1', stdout);
                break;
            case 0x2:
                putc('0', stdout);
                break;
            case 0x3:
                putc('1', stdout);
                break;
            case 0x4:
                putc('0', stdout);
                break;
            case 0x5:
                putc('1', stdout);
                break;
            case 0X6:
                putc('0', stdout);
                break;
            case 0x7:
                putc('1', stdout);
                break;
            case 0x8:
                putc('0', stdout);
                break;
            case 0x9:
                putc('1', stdout);
                break;
            case 0xA:
                putc('a', stdout);
                break;
            case 0xB:
                putc('b', stdout);
                break;
            case 0xC:
                putc('c', stdout);
                break;
            case 0xD:
                putc('d', stdout);
                break;
            case 0xE:
                putc('e', stdout);
                break;
            case 0xF:
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

/*
Allocate and return a pointer to a linked list of struct elts.
*/
struct elt *str_to_list(const char *str)
{
    // declare a new node variable
    struct elt *head = NULL;
    int i = 0;

    // read in characters and build list
    while (str[i] != '\0')
    {
        // create a new node of size elt
        struct elt *new_elt = (struct elt *)malloc(sizeof(struct elt));

        // check if malloc fails and free memory
        if (new_elt == NULL)
        {
            struct elt *start_elt = head;
            struct elt *next_elt;
            while (start_elt != NULL)
            {
                next_elt = start_elt->link;
                free(start_elt);
                start_elt = next_elt;
            }
            // set head node null
            head = NULL;
        }

        // set the head node
        else if (i == 0)
        {
            new_elt->val = str[i];
            new_elt->link = NULL;
            head = new_elt;
        }
        // add a new node to the list
        else
        {
            new_elt->val = str[i];
            new_elt->link = NULL;
            struct elt *last_elt = head;
            while (last_elt->link != NULL)
            {
                last_elt = last_elt->link;
            }
            last_elt->link = new_elt;
        }
        ++i;
    }
    return head;
}

/**
 * Creates a file called u.txt in the current directory that contains an ASCII-art letter "U" (it does not need to be very big).
 * The file must be marked as owner-executable and owner-readable but not owner-writable;
 * other groups and users should have no access to the file.
 */
void draw_u(void)
{
    // Create a File
    int cdp = syscall(SYS_creat, "u.txt", S_IXUSR | S_IRUSR);

    // Check if File Descriptor failed
    // > -1 - success
    // -1 - failure
    if (cdp == -1)
    {
        // close resources
        syscall(SYS_close, cdp);

        // free memory
        syscall(SYS_unlink, "u.txt");
    }

    // write to the file
    const char *buf1 = {"-----                       -----\n"};
    const char *buf2 = {"|   |                       |   |\n"};
    const char *buf3 = {"|   |                       |   |\n"};
    const char *buf4 = {"|   |                       |   |\n"};
    const char *buf5 = {"|   |                       |   |\n"};
    const char *buf6 = {"|   |                       |   |\n"};
    const char *buf7 = {"|   |                       |   |\n"};
    const char *buf8 = {"|   |                       |   |\n"};
    const char *buf9 = {"|   | _ _ _ _ _ _ _ _ _ _ _ |   |\n"};
    const char *buf10 = {"\\ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _/\n"};

    // define file descriptors
    int fd1 = syscall(SYS_write, cdp, buf1, 34);
    int fd2 = syscall(SYS_write, cdp, buf2, 34);
    int fd3 = syscall(SYS_write, cdp, buf3, 34);
    int fd4 = syscall(SYS_write, cdp, buf4, 34);
    int fd5 = syscall(SYS_write, cdp, buf5, 34);
    int fd6 = syscall(SYS_write, cdp, buf6, 34);
    int fd7 = syscall(SYS_write, cdp, buf7, 34);
    int fd8 = syscall(SYS_write, cdp, buf8, 34);
    int fd9 = syscall(SYS_write, cdp, buf9, 34);
    int fd10 = syscall(SYS_write, cdp, buf10, 33);

    // check all the file descriptors work
    // if not, free memory
    if (fd1 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd2 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd3 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd4 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd5 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd6 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd7 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd8 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd9 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }
    else if (fd10 == -1)
    {
        // close resources here
        syscall(SYS_close, cdp);
        // free the memory
        syscall(SYS_unlink, "u.txt");
    }

    // close resources here
    syscall(SYS_close, cdp);
    // printf("file opened successfully\n");
    return;
}

// Operating Systems Assignment 1 Part B.

/**
 * Part B Question 1
 * %CR4 value before PSE is enabled: 0000000
 * %CR4 value after PSE is enabled: 00000010
 */

/**
 * Part B Question 2
 * Address of Initial Kernel Stack: 0x8010b5c0
 *
 */

/**
 * Part B Question 3
 * "initcode\000\000\000\000\000\000\000"
 *
 */

/**
 * Part B Question 4
 * NPROC = 64 as defined in param.h
 * The process table data structure limits this
 * because the for loop on line 35 iterates until the maximum
 * number of process specified in param.H which is 64.
 */

/**
 * Part B Question 5
 * I triggered the exit function by running the "ls" command to list the file systems. When ls
 * is executed it creates a new process and then when the process finishes exit is called. My guess
 * is that in the exit function the process is reaped.
 */

/**
 * Part B Question 6
 * The process goes from a Running Process to a Zombie Process as stated n line 265 in proc.c
 */

/**
 * Part B Question 7
 * XV6 Process: ls, p *myProc()
 * name = "ls"
 * pid = 4
 *
 * Parent Process (p * myProc()->parent)
 * name = "sh"
 * pid = 2
 */