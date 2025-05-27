#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main() {
    int m = 50000000;   //100000000
    int n = 100000000;     //2000000 
    int chunksize = 1000;

    clock_t start = clock();

    bool* result = (bool*)malloc((n - m + 1) * sizeof(bool));
    memset(result, true, (n - m + 1) * sizeof(bool));

    bool* primeArray = (bool*)malloc((sqrt(n) + 1) * sizeof(bool));
    memset(primeArray, true, (sqrt(n) + 1) * sizeof(bool));

    for (int i = 2; i * i <= n; i++) {
        for (int j = 2; j * j <= i; j++) {
            if (primeArray[j] == true && i % j == 0) {
                primeArray[i] = false;
                break;
            }
        }
    }

    #pragma omp parallel
    {
    #pragma omp for schedule(dynamic, chunksize) //static
        for (int i = m; i <= n; i++){
            for (int j = 2; j * j <= i; j++){
                if (primeArray[j] == true && i % j == 0) { 
                    result[i - m] = false;
                    break;
                }
            }
        }
    }

    // #pragma omp parallel
    // {
    // #pragma omp for schedule(guided, chunksize) 
    //     for (int i = m; i <= n; i++){
    //         for (int j = 2; j * j <= i; j++){
    //             if (primeArray[j] == true && i % j == 0) { 
    //                 result[i - m] = false;
    //                 break;
    //             }
    //         }
    //     }
    // }

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
    printf("Predkosc: %.5f liczb pierwszych na sekunde\n", (count / time_spent) / 1000000);

    free(result);
    free(primeArray);
    return 0;
}