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

int main() {
    init_sieve();
    
    int64_t checkpoints[] = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    int num_cp = 6;
    
    int64_t c3 = 0, c5 = 0, c7 = 0;
    int64_t total = 0;
    int cp_idx = 0;
    
    printf("%10s %10s %10s %10s %10s %10s\n", 
           "N", "pmin=3", "pmin=5", "pmin=7", "pmin<=7", "ratio");
    printf("------------------------------------------------------------------\n");
    
    for (int64_t N = 6; N <= LIMIT; N += 2) {
        total++;
        
        if (is_prime[N - 3]) {
            c3++;
        } else if (is_prime[N - 5]) {
            c5++;
        } else if (is_prime[N - 7]) {
            c7++;
        }
        
        if (cp_idx < num_cp && N == checkpoints[cp_idx]) {
            double p3 = 100.0 * c3 / total;
            double p5 = 100.0 * c5 / total;
            double p7 = 100.0 * c7 / total;
            double p_le7 = p3 + p5 + p7;
            double ratio = p_le7 / p3;
            
            printf("%10ld %9.2f%% %9.2f%% %9.2f%% %9.2f%% %9.3f\n",
                   N, p3, p5, p7, p_le7, ratio);
            cp_idx++;
        }
    }
    
    return 0;
}
