#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LIMIT 100000000LL  // 100 million first (faster)

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

int64_t find_pmin(int64_t N) {
    for (int64_t p = 3; p < N; p += 2) {
        if (is_prime[p] && is_prime[N - p]) {
            return p;
        }
    }
    return -1;
}

int main() {
    printf("Computing pmin distribution up to %ld\n\n", LIMIT);
    
    init_sieve();
    printf("Sieve done.\n\n");
    
    // Count at checkpoints
    int64_t checkpoints[] = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    int num_checkpoints = 6;
    
    int64_t count_le7 = 0, count_le31 = 0, count_le103 = 0;
    int64_t total = 0;
    int cp_idx = 0;
    
    printf("%12s %10s %10s %10s %10s\n", "N", "total", "<=7 %", "<=31 %", "<=103 %");
    printf("--------------------------------------------------------------\n");
    
    for (int64_t N = 6; N <= LIMIT; N += 2) {
        int64_t pm = find_pmin(N);
        total++;
        if (pm <= 7) count_le7++;
        if (pm <= 31) count_le31++;
        if (pm <= 103) count_le103++;
        
        if (cp_idx < num_checkpoints && N == checkpoints[cp_idx]) {
            printf("%12ld %10ld %9.2f%% %9.2f%% %9.2f%%\n",
                   N, total,
                   100.0 * count_le7 / total,
                   100.0 * count_le31 / total,
                   100.0 * count_le103 / total);
            cp_idx++;
        }
    }
    
    free(is_prime);
    return 0;
}
