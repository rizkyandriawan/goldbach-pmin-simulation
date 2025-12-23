# Goldbach pmin Simulation

Computational exploration of pmin(N) - the smallest prime p such that N - p is also prime (Goldbach decomposition).

## Key Finding

**max pmin(N) ~ 0.2 × ln(N)³**

This means finding Goldbach pairs is computationally easy - even for very large N, you only need to test a small number of primes from the beginning of the sequence.

## Files

### Core Programs

| File | Description |
|------|-------------|
| `pmin_max_1B.c` | Simple sieve, computes pmin up to 10^9 (~18 seconds) |
| `generate_primes.c` | Generates small primes file for segmented sieve |
| `pmin_segmented.c` | Segmented sieve for 10^10+ (handles any size) |

### Data

| File | Description |
|------|-------------|
| `max_pmin_1B.csv` | Max pmin at each log₁₀ interval up to 10^9 |
| `small_primes.bin` | Pre-computed primes up to 10^6 |

### Paper

| File | Description |
|------|-------------|
| `goldbach_pmin_paper.md` | Full paper (markdown) |
| `goldbach_pmin_paper.pdf` | Full paper (PDF) |

## Usage

```bash
# Simple version (up to 10^9, needs 1GB RAM)
gcc -O3 -o goldbach_pmin pmin_max_1B.c -lm
./goldbach_pmin

# Segmented version (any size, low memory)
gcc -O3 -o generate_primes generate_primes.c -lm
gcc -O3 -o pmin_segmented pmin_segmented.c -lm

./generate_primes                    # Generate small primes (once)
./pmin_segmented 1000000000000       # Run up to 10^12
```

## Performance

| N | Method | Time | Memory |
|---|--------|------|--------|
| 10^9 | Simple sieve | 18 sec | 1 GB |
| 10^10 | Segmented | 2 min | 2 MB |
| 10^12 | Segmented | ~3 hrs | 2 MB |

## Results Summary

| log₁₀(N) | max pmin | Ratio to 0.2×ln³ |
|----------|----------|------------------|
| 9 | 1,789 | 0.201 |
| 10 | 1,877 | 0.185 |

## Reference

Andriawan, R. (2025). "On the Growth Rate of the Minimal Goldbach Prime."

## License

MIT
