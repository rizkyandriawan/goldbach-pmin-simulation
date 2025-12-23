#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

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
    printf("Goldbach pmin finder - up to %ld\n\n", LIMIT);

    clock_t start = clock();
    printf("Initializing sieve...\n");
    init_sieve();
    double sieve_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Sieve done in %.3f seconds\n\n", sieve_time);

    FILE *csv = fopen("pmin_records.csv", "w");
    fprintf(csv, "N,pmin,prev_pmin,jump,ratio,n_mod_2310,pmin_mod_2310\n");

    start = clock();
    int64_t max_pmin = 0;
    double max_ratio = 0;
    int records = 0;

    for (int64_t N = 6; N <= LIMIT; N += 2) {
        int64_t pmin = find_pmin(N);

        if (pmin > max_pmin) {
            int64_t jump = pmin - max_pmin;
            double ratio = (max_pmin > 0) ? (double)pmin / max_pmin : 0;
            if (ratio > max_ratio && max_pmin > 0) max_ratio = ratio;

            fprintf(csv, "%ld,%ld,%ld,%ld,%.6f,%ld,%ld\n",
                    N, pmin, max_pmin, jump, ratio, N % 2310, pmin % 2310);

            printf("%12ld %6ld %6ld %4ld  %.4f  (mod2310: N=%4ld, p=%4ld)\n",
                   N, pmin, max_pmin, jump, ratio, N % 2310, pmin % 2310);

            max_pmin = pmin;
            records++;
        }
    }

    fclose(csv);
    double compute_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("\nFinal max pmin: %ld\n", max_pmin);
    printf("Total records: %d\n", records);
    printf("Max growth ratio: %.4f\n", max_ratio);
    printf("Compute time: %.3f seconds\n", compute_time);
    printf("Data saved to pmin_records.csv\n");

    free(is_prime);
    return 0;
}
