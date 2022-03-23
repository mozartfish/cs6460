#include <stdio.h>

int main() {
    printf("-----                       -----\n"); // 35 bytes 1 
    printf("|   |                       |   |\n"); // 4 bytes 2
    printf("|   |                       |   |\n"); // 33 bytes 3
    printf("|   |                       |   |\n"); // 4
    printf("|   |                       |   |\n"); // 5
    printf("|   |                       |   |\n"); // 6
    printf("|   |                       |   |\n"); // 7
    printf("|   |                       |   |\n"); // 8
    printf("|   | _ _ _ _ _ _ _ _ _ _ _ |   |\n"); // 9
    printf("\\ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _/\n"); // 10
    return 0;
    }

