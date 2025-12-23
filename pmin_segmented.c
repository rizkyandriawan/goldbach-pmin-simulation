// Step 2: Segmented sieve for pmin computation up to 10^12
// Uses pre-generated small primes for sieving

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define SEGMENT_SIZE (1 << 20)  // 1M per segment (1 MB memory)
#define MAX_SMALL_PRIME 1000000

int *small_primes;
int num_small_primes;
uint8_t *segment;

// Load pre-generated small primes
void load_small_primes() {
    FILE *f = fopen("small_primes.bin", "rb");
    if (!f) {
        fprintf(stderr, "Error: small_primes.bin not found. Run generate_primes first.\n");
        exit(1);
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    num_small_primes = size / sizeof(int);
    small_primes = malloc(size);
    fread(small_primes, sizeof(int), num_small_primes, f);
    fclose(f);

    printf("Loaded %d small primes (largest: %d)\n", num_small_primes, small_primes[num_small_primes-1]);
}

// Sieve a segment [lo, hi)
void sieve_segment(int64_t lo, int64_t hi) {
    int64_t size = hi - lo;
    memset(segment, 1, size);

    // Mark 0 and 1 as not prime if in range
    if (lo == 0) { segment[0] = 0; segment[1] = 0; }
    if (lo == 1) { segment[0] = 0; }

    for (int i = 0; i < num_small_primes; i++) {
        int64_t p = small_primes[i];
        if (p * p >= hi) break;

        // Find first multiple of p >= lo
        int64_t start = ((lo + p - 1) / p) * p;
        if (start == p) start += p;  // Don't mark p itself
        if (start < lo) start += p;

        for (int64_t j = start; j < hi; j += p) {
            segment[j - lo] = 0;
        }
    }
}

// Check if n is prime using current segment (assumes n is in [lo, hi))
static inline int is_prime_in_segment(int64_t n, int64_t lo) {
    return segment[n - lo];
}

int main(int argc, char *argv[]) {
    int64_t LIMIT = 1000000000000LL;  // 10^12 default

    if (argc > 1) {
        LIMIT = atoll(argv[1]);
    }

    printf("Computing max pmin up to %lld (10^%.1f)\n", LIMIT, log10((double)LIMIT));

    load_small_primes();
    segment = malloc(SEGMENT_SIZE);

    // Also keep a small sieve for checking small p values
    uint8_t *small_sieve = malloc(MAX_SMALL_PRIME + 1);
    memset(small_sieve, 1, MAX_SMALL_PRIME + 1);
    small_sieve[0] = small_sieve[1] = 0;
    for (int i = 2; i * i <= MAX_SMALL_PRIME; i++) {
        if (small_sieve[i]) {
            for (int j = i * i; j <= MAX_SMALL_PRIME; j += i) {
                small_sieve[j] = 0;
            }
        }
    }

    FILE *out = fopen("max_pmin_10e12.csv", "w");
    fprintf(out, "log10_N,N,max_pmin\n");

    int64_t max_pmin = 0;
    int64_t max_pmin_at = 0;
    double log_step = 0.1;
    double current_log = 1.0;
    int64_t processed = 0;

    clock_t start_time = clock();
    clock_t last_report = start_time;

    // Process in segments
    for (int64_t seg_lo = 0; seg_lo < LIMIT; seg_lo += SEGMENT_SIZE) {
        int64_t seg_hi = seg_lo + SEGMENT_SIZE;
        if (seg_hi > LIMIT + 10000) seg_hi = LIMIT + 10000;  // Extra buffer for N-p

        sieve_segment(seg_lo, seg_hi);

        // Process even N in this segment
        int64_t N_start = (seg_lo < 6) ? 6 : ((seg_lo + 1) / 2 * 2);  // First even >= max(6, seg_lo)
        int64_t N_end = seg_hi - 2;  // Leave room for small p
        if (N_end > LIMIT) N_end = LIMIT;

        for (int64_t N = N_start; N <= N_end; N += 2) {
            // Find pmin for this N
            // We need N-p to be in segment, so p >= N - seg_hi + 1
            // But pmin is usually small, so check small primes first

            int64_t pmin = 0;
            for (int i = 1; i < num_small_primes && small_primes[i] < N; i++) {  // Skip 2
                int64_t p = small_primes[i];
                int64_t q = N - p;

                // Check if q is prime
                int q_prime = 0;
                if (q < MAX_SMALL_PRIME) {
                    q_prime = small_sieve[q];
                } else if (q >= seg_lo && q < seg_hi) {
                    q_prime = segment[q - seg_lo];
                } else {
                    // q not in our current segment - need to check differently
                    // For simplicity, use trial division for large q not in segment
                    // This is rare since pmin is usually very small
                    q_prime = 1;
                    for (int j = 0; j < num_small_primes && (int64_t)small_primes[j] * small_primes[j] <= q; j++) {
                        if (q % small_primes[j] == 0) {
                            q_prime = 0;
                            break;
                        }
                    }
                }

                if (q_prime) {
                    pmin = p;
                    break;
                }
            }

            if (pmin > max_pmin) {
                max_pmin = pmin;
                max_pmin_at = N;
            }

            processed++;

            // Log at intervals
            double log_N = log10((double)N);
            if (log_N >= current_log) {
                fprintf(out, "%.2f,%lld,%lld\n", current_log, N, max_pmin);
                fflush(out);

                if ((int)(current_log * 10) % 10 == 0) {
                    clock_t now = clock();
                    double elapsed = (double)(now - start_time) / CLOCKS_PER_SEC;
                    double rate = processed / elapsed / 1e6;
                    printf("  log10(N) = %.1f, max_pmin = %lld (at N=%lld), %.1fM N/sec, %.1fs elapsed\n",
                           current_log, max_pmin, max_pmin_at, rate, elapsed);
                }
                current_log += log_step;
            }
        }

        // Progress report every 10 seconds
        clock_t now = clock();
        if ((now - last_report) / CLOCKS_PER_SEC >= 10) {
            double elapsed = (double)(now - start_time) / CLOCKS_PER_SEC;
            double pct = 100.0 * seg_lo / LIMIT;
            printf("  Progress: %.2f%%, %.1fs elapsed\n", pct, elapsed);
            last_report = now;
        }
    }

    clock_t end_time = clock();
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nDone!\n");
    printf("Max pmin = %lld at N = %lld\n", max_pmin, max_pmin_at);
    printf("Total time: %.1f seconds (%.1f minutes)\n", total_time, total_time / 60);
    printf("Saved to max_pmin_10e12.csv\n");

    fclose(out);
    free(segment);
    free(small_primes);
    free(small_sieve);

    return 0;
}
