#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define LIMIT 100000000LL

uint8_t *is_prime;

void init_sieve() {
    is_prime = malloc(LIMIT + 1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (int64_t i = 2; i * i <= LIMIT; i++) {
        if (is_prime[i]) {
            for (int64_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }
}

int main() {
    init_sieve();
    
    int64_t checkpoints[] = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    int num_cp = 6;
    
    int64_t count_pmin3 = 0;
    int64_t total = 0;
    int cp_idx = 0;
    
    printf("%12s %12s %12s %12s\n", "N", "pmin=3 %", "2/ln(N) %", "difference");
    printf("----------------------------------------------------\n");
    
    for (int64_t N = 6; N <= LIMIT; N += 2) {
        total++;
        // pmin = 3 iff N - 3 is prime
        if (is_prime[N - 3]) {
            count_pmin3++;
        }
        
        if (cp_idx < num_cp && N == checkpoints[cp_idx]) {
            double actual = 100.0 * count_pmin3 / total;
            double theory = 200.0 / log(N);
            printf("%12ld %11.2f%% %11.2f%% %+11.2f%%\n", 
                   N, actual, theory, actual - theory);
            cp_idx++;
        }
    }
    
    return 0;
}
