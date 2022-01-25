#include "assignment1.h"
#include <stdio.h>

// Function for freeing memory for the linked list
void deleteList(struct elt *head)
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

/*
Function for printing the linked list
*/
void printList(struct elt *head)
{
    while (head != NULL)
    {
        printf("%c\n", head->val);
        head = head->link;
    }
}

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
    printf("Expected Hex Result: %x\n", 0x00000000deadbeef);
    convert(HEX, 0xdeadbeef);
    printf("Binary value\n");
    convert(BIN, 0xdeadbeef);
    printf("Oct value\n");
    convert(OCT, 0xdeadbeef);

    printf("str_to_list\n");
    struct elt *result = str_to_list("hello");
    printList(result);
    // // printf("el 0: %i\n", result->val);
    // // printf("el 1: %i\n", result->link->val);
    deleteList(result);

    return 0;
}
