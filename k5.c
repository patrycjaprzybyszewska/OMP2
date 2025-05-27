#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main() {
    int m = 2;   //100000000
    int n = 50000000;     //2000000 

    clock_t start = clock();

    bool* result = (bool*)malloc((n - m + 1) * sizeof(bool));
    memset(result, true, (n - m + 1) * sizeof(bool));

    bool* primeArray = (bool*)malloc((sqrt(n) + 1) * sizeof(bool));
    memset(primeArray, true, (sqrt(n) + 1) * sizeof(bool));

    for (int i = 2; i*i*i*i <= n; i++) {
        if (primeArray[i] == true) {
            for (int j = i*i; j*j <= n; j+=i) {
                primeArray[j] = false;
            }
        }
    }

    int blockSize = 10000;  
    int numberOfBlocks = (n - m) / blockSize;
    if ((n - m) % blockSize != 0) {
        numberOfBlocks++;
    }

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numberOfBlocks; i++) {
        int low = m + i * blockSize;
        int high = m + i * blockSize + blockSize;
        if (high > n) {
            high = n;
        }
        for (int j = 2; j * j <= high; j++) {
            if (primeArray[j]) {
                int firstMultiple = (low / j);
                if (firstMultiple <= 1) {
                    firstMultiple = j + j;
                }
                else if (low % j) {
                    firstMultiple = (firstMultiple * j) + j;
                } else {
                    firstMultiple = (firstMultiple * j);
                }
                for (int k = firstMultiple; k <= high; k += j) {
                    result[k - m] = false;
                }
            }
        }
    }

    // for (int i = m; i <= n; i++) {
    //     if (result[i - m]) {
    //         printf("%d jest pierwsza\n", i);
    //     }
    // }

    int count = 0;
    for (int i = m; i <= n; i++) {
        if (result[i - m]) {
            count++;
        }
    }
    printf("Liczb pierwszych: %d\n", count);

    clock_t end = clock(); 
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Czas wykonania: %.3f sekund\n", time_spent);
    printf("Predkosc: %.2f liczb pierwszych na sekunde\n", (count / time_spent) / 1000000);


    free(result);
    free(primeArray);
    return 0;
}
