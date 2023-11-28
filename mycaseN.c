#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printpid(int cN) {
    printf("\n\n case %d - pid=%d\n", cN, getpid());
}

int main() {
    int N = 1;
    int i, j;

    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            int pid = fork();
            if (pid == 0) {
                printpid(i + (j - 1) * N);
                exit(0);
            }
        }
    }

    for (j = 1; j <= N; j++) {
        wait(NULL);
    }

    return 0;
}
