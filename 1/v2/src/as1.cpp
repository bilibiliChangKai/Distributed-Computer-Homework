// Author : huziang
// This is a parallel program calculate
// the all prime number in 2 -> n

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <memory.h>
#include "test.h"

int n, k;
int sqrt_n;
double blockinv;
int *pidsindex;
int *beginindex;
bool *isprime;
int *message;
long *processtimes;
pthread_t *pids;
pthread_rwlock_t *rwlocks;

void init() {
    pids = new pthread_t[k];
    pidsindex = new int[k];
    beginindex = new int[k];
    processtimes = new long[k];
    isprime = new bool[n + 1];
    memset(isprime, true, n + 1);
    message = new int[sqrt_n];
    memset(message, -1, sizeof(int) * sqrt_n);
    rwlocks = new pthread_rwlock_t[sqrt_n];

    // set pthread's process range
    for (int i = 0; i < k; i++) {
        beginindex[i] = blockinv * (i + 1) + 1;
    }
    beginindex[k - 1] = n + 1;  // last pthread process till n (n + 1 because half open)

    // set pids' self index
    for (int i = 0; i < k; i++) {
        pidsindex[i] = i;
    }

    // set rwlocks, to protect data
    for (int i = 0; i < sqrt_n; i++) {
        pthread_rwlock_init(&rwlocks[i], NULL);
        // all locks lock at begin
        pthread_rwlock_wrlock(&rwlocks[i]);
    }
}

void release() {
    for (int i = 0; i < sqrt_n; i++) {
        pthread_rwlock_destroy(&rwlocks[i]);
    }

    delete [] pids;
    delete [] pidsindex;
    delete [] beginindex;
    delete [] processtimes;
    delete [] isprime;
    delete [] message;
    delete [] rwlocks;
}

// main thread like the not parallel function
void *mainThread(void *ptr) {
    int id = *((int*)ptr);
    // time begin
    processtimes[id] = getDiffTime();
    int messageindex = 0;
    int end = beginindex[id];
    for (int i = 2; i <= sqrt_n; i++) {
        if (isprime[i]) {
            // send data message
            message[messageindex] = i;
            pthread_rwlock_unlock(&rwlocks[messageindex++]);
            // processon
            for (int j = i + i; j < end; j += i) {
                isprime[j] = false;
            }
        }
    }

    // send end control message
    message[messageindex] = 0;
    pthread_rwlock_unlock(&rwlocks[messageindex++]);
    // time end
    processtimes[id] = getDiffTime() - processtimes[id];
    return NULL;
}

// the process range is [,) (left close and right open)
void *processThread(void *ptr) {
    int id = *((int*)ptr);
    // time begin
    processtimes[id] = getDiffTime();
    int messageindex = 0;
    int num = 0;
    int begin = beginindex[*((int*)ptr) - 1];
    int end = beginindex[*((int*)ptr)];

#ifdef DEBUG
    printf("Thread%d: range [%d, %d)\n", id, begin, end);
#endif

    while (1) {
        pthread_rwlock_rdlock(&rwlocks[messageindex++]);
        num = message[messageindex - 1];

#ifdef DEBUG
        printf("Thread:%d get the message %d\n", id, num);
#endif

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
    // time end
    processtimes[id] = getDiffTime() - processtimes[id];
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc == 3) {
        n = atoi(argv[1]);
        k = atoi(argv[2]);
    } else if (argc == 2) {
        n = atoi(argv[1]);
        k = ceil(sqrt(n) / 2);
    } else {
        printf("Please input n and k: (k is the process' number)\n");
        scanf("%d %d", &n, &k);
    }
    sqrt_n = sqrt(n);
    blockinv = (double)n / k;

    // first process must include [2, floor(√n)]
    if (n / sqrt_n < k) {
        perror("k must be less n / √n!\n");
        exit(1);
    }

    // start timer
    long diftime;
    initTime();
    init();

    // timer once
    diftime = getDiffTime();
    printf("Init time: %ld us\n", diftime);

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

    // write to file
    char filename[20];
    sprintf(filename, "../output/answer%d_%d.txt", n, k);
    FILE *fp = fopen(filename, "w");
    for (int i = 2; i <= n; i++) {
        if (isprime[i]) {
            fprintf(fp, "%d: is prime!\n", i);
        }
    }
    fclose(fp);

    // print all processon time
    printf("Main time: %ld us\n", processtimes[0]);
    for (int i = 1; i < k; i++) {
        printf("Process %d time: %ld us\n", i, processtimes[i]);
    }

    release();
    return 0;
}
