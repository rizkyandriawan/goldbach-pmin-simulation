#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define LIMIT 1000000000LL  // 1 billion

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
    printf("Computing max pmin up to 10^9...\n");
    init_sieve();
    printf("Sieve done.\n");
    
    FILE *f = fopen("max_pmin_1B.csv", "w");
    fprintf(f, "log10_N,N,max_pmin\n");
    
    int64_t max_pmin = 0;
    double log_step = 0.1;
    double current_log = 1.0;
    
    for (int64_t N = 6; N <= LIMIT; N += 2) {
        int64_t pm = find_pmin(N);
        if (pm > max_pmin) {
            max_pmin = pm;
        }
        
        double log_N = log10((double)N);
        if (log_N >= current_log) {
            fprintf(f, "%.2f,%ld,%ld\n", current_log, N, max_pmin);
            if ((int)(current_log * 10) % 10 == 0) {
                printf("  log10(N) = %.1f, max_pmin = %ld\n", current_log, max_pmin);
            }
            current_log += log_step;
        }
    }
    
    fclose(f);
    printf("Done! Saved to max_pmin_1B.csv\n");
    return 0;
}
