// Step 1: Generate small primes up to sqrt(10^12) = 10^6
// These are used for segmented sieving

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SMALL_LIMIT 1000000  // 10^6, enough for sqrt(10^12)

int main() {
    printf("Generating primes up to %d...\n", SMALL_LIMIT);

    // Simple sieve for small primes
    uint8_t *is_prime = malloc(SMALL_LIMIT + 1);
    memset(is_prime, 1, SMALL_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (int64_t i = 2; i * i <= SMALL_LIMIT; i++) {
        if (is_prime[i]) {
            for (int64_t j = i * i; j <= SMALL_LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    // Count and write to file
    FILE *f = fopen("small_primes.bin", "wb");
    int count = 0;

    for (int i = 2; i <= SMALL_LIMIT; i++) {
        if (is_prime[i]) {
            fwrite(&i, sizeof(int), 1, f);
            count++;
        }
    }

    fclose(f);
    free(is_prime);

    printf("Done! Wrote %d primes to small_primes.bin\n", count);
    printf("File size: %lu bytes\n", (unsigned long)(count * sizeof(int)));

    return 0;
}
