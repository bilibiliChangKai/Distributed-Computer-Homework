// Author : huziang
// This is a parallel program calculate
// the all prime number in 2 -> n

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <memory.h>

int n, k;
int sqrt_n;
double blockinv;
pthread_t *pids;
int *pidsindex;
int *beginindex;
bool *isprime;
int *message;

void init() {
    pids = new pthread_t[k];
    pidsindex = new int[k];
    beginindex = new int[k];
    isprime = new bool[n + 1];
    memset(isprime, true, n + 1);
    message = new int[sqrt_n];
    memset(message, -1, sizeof(int) * sqrt_n);

    // set pthread's process range
    for (int i = 0; i < k; i++) {
        beginindex[i] = blockinv * (i + 1) + 1;
    }
    beginindex[k - 1] = n + 1;  // last pthread process till n (n + 1 because half open)

    // set pids' self index
    for (int i = 0; i < k; i++) {
        pidsindex[i] = i;
    }
}

void release() {
    delete [] pids;
    delete [] beginindex;
    delete [] isprime;
}

// main thread like the not parallel function
void *mainThread(void *ptr) {
    int messageindex = 0;
    int end = beginindex[*((int*)ptr)];
    for (int i = 2; i <= sqrt_n; i++) {
        if (isprime[i]) {
            // send data message
            message[messageindex++] = i;
            // processon
            for (int j = i + i; j < end; j += i) {
                isprime[j] = false;
            }
        }
    }

    // send end control message
    message[messageindex++] = 0;
    return NULL;
}

// the process range is [,) (left close and right open)
void *processThread(void *ptr) {
    int pos = *((int*)ptr);
    int messageindex = 0;
    int num = 0;
    int begin = beginindex[*((int*)ptr) - 1];
    int end = beginindex[*((int*)ptr)];
    printf("Thread%d: range [%d, %d)\n", pos,begin, end);

    while (1) {
        while (message[messageindex] == -1);
        num = message[messageindex];
        messageindex++;
        printf("Thread:%d get the message %d\n", pos, num);
        // if recieve 0, mean end
        if (num == 0) {
            break;
        }

        // process
        int i = begin % num == 0 ? begin : begin + num - begin % num;
        for (; i < end; i += num) {
            isprime[i] = false;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Please input n and k: (k is the process' number)\n");
        scanf("%d %d", &n, &k);
    } else {
        n = atoi(argv[1]);
        k = atoi(argv[2]);
    }
    sqrt_n = sqrt(n);
    blockinv = (double)n / k;

    // first process must include [2, floor(√n)]
    if (n / sqrt_n < k) {
        perror("k must be less n / √n!\n");
        exit(1);
    }

    init();

    // create main thread
    int ret = pthread_create(&pids[0], NULL, mainThread, &pidsindex[0]);
    if (ret != 0) {
        printf("Create pthread error!\n");
        exit(1);
    }

    // create process threac
    for (int i = 1; i < k; i++) {
        ret = pthread_create(&pids[i], NULL, processThread, &pidsindex[i]);
        if (ret != 0) {
            printf("Create pthread error!\n");
            exit(1);
        }
    }

    // wait
    for (int i = 0; i < k; i++) {
        pthread_join(pids[i], NULL);
    }

    // write
    for (int i = 2; i <= n; i++) {
        printf("%d: %d\n", i, isprime[i]);
    }

    release();
    return 0;
}


