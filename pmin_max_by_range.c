#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int64_t find_pmin(int64_t N) {
    for (int64_t p = 3; p < N; p += 2) {
        if (is_prime[p] && is_prime[N - p]) {
            return p;
        }
    }
    return -1;
}

int main() {
    init_sieve();
    
    // Output max pmin at regular log intervals
    FILE *f = fopen("max_pmin_by_log.csv", "w");
    fprintf(f, "log10_N,N,max_pmin,max_pmin_at_N\n");
    
    int64_t max_pmin = 0;
    int64_t max_pmin_N = 0;
    
    // Checkpoints at powers of 10 and intermediate points
    int64_t next_checkpoint = 100;
    double log_step = 0.1;  // log10 steps
    double current_log = 2.0;
    
    for (int64_t N = 6; N <= LIMIT; N += 2) {
        int64_t pm = find_pmin(N);
        if (pm > max_pmin) {
            max_pmin = pm;
            max_pmin_N = N;
        }
        
        // Check if we crossed a log threshold
        double log_N = N > 0 ? log10((double)N) : 0;
        if (log_N >= current_log) {
            fprintf(f, "%.2f,%ld,%ld,%ld\n", current_log, N, max_pmin, max_pmin_N);
            current_log += log_step;
        }
    }
    
    fclose(f);
    printf("Saved to max_pmin_by_log.csv\n");
    return 0;
}
