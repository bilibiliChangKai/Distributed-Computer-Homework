// Author : huziang
// This is a file to check whether anwser is true

#include <string.h>
#include <cmath>
#include <iostream>

int isPrime(int n) {
    int sqrt_n = sqrt(n);
    for (int i = 2; i <= sqrt_n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char **argv) {
    char dicname[30] = "output/";
    char filename[30];
    int n, isprime;

    if (argc == 2) {
        strcpy(filename, argv[1]);
    } else {
        printf("Please input filename:\n");
        scanf("%s", filename);
    }

    FILE *fp = fopen(strcat(dicname, filename), "r");
    if (fp == NULL) {
        perror("Not such file!");
        exit(1);
    }

    while (fscanf(fp, "%d: %d", &n, &isprime) != -1) {
        if (isPrime(n) != isprime) {
            fprintf(stderr, "ERROR: %d -> %d is wrong!", n, isprime);
        }
    }

    printf("All number test over!\n");

    fclose(fp);
}