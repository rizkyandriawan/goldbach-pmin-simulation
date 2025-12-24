# Goldbach pmin Simulation

**TL;DR:** Goldbach says every even number N = prime + prime. We ask: *how small can the first prime be?* Turns out, almost always tiny.

## What's Goldbach's Conjecture?

```
Every even integer > 2 is the sum of two primes.

Examples:
  10 = 3 + 7  = 5 + 5
  30 = 7 + 23 = 11 + 19 = 13 + 17
  98 = 19 + 79
```

Unproven since 1742, but verified up to 4×10¹⁸. We're not proving it here - we're asking a different question.

## The Key Question

**What's the smallest prime that works?**

We call this `pmin(N)`. For N=30, even though 30 = 7+23 = 11+19 = 13+17, the smallest working prime is 7, so `pmin(30) = 7`.

## The Diagonal Search

Imagine a grid. Rows = even numbers N. Columns = small primes p. Mark a cell if both p and N-p are prime:

```
     N │  3  5  7 11 13 17 19 23 29 31 37 41  │ pmin
───────┼──────────────────────────────────────┼─────
    6  │ ◆                                    │  3
    8  │ ◆  ·                                 │  3
   10  │ ◆  ·  ·                              │  3
   12  │    ◆  ·                              │  5
   14  │ ◆     ·  ·                           │  3
   16  │ ◆  ·     ·  ·                        │  3
   18  │    ◆  ·  ·  ·                        │  5
   20  │ ◆     ·     ·  ·                     │  3
   22  │ ◆  ·     ·     ·  ·                  │  3
   24  │    ◆  ·  ·  ·  ·  ·                  │  5
   26  │ ◆     ·     ·     ·  ·               │  3
   28  │    ◆     ·     ·     ·               │  5
   30  │       ◆  ·  ·  ·  ·  ·               │  7
   32  │ ◆           ·     ·     ·            │  3
   34  │ ◆  ·     ·     ·     ·  ·  ·         │  3
   36  │    ◆  ·     ·  ·  ·  ·  ·  ·         │  5
   38  │       ◆           ·        ·         │  7
   40  │ ◆        ·     ·     ·  ·     ·      │  3
   42  │    ◆     ·  ·     ·  ·  ·  ·  ·      │  5
   44  │ ◆     ·     ·              ·  ·  ·   │  3
   46  │ ◆  ·           ·     ·  ·        ·   │  3
   48  │    ◆  ·  ·     ·  ·     ·  ·  ·  ·   │  5
   50  │ ◆     ·     ·     ·        ·  ·      │  3

◆ = pmin (first valid prime we find)
· = other valid Goldbach pairs
```

**Notice:** pmin is almost always 3, 5, or 7! You rarely need to go far.

## But What About HUGE Numbers?

We computed pmin for **all** even numbers up to 10 billion. The "hardest" cases:

```
N = 721,013,438      → pmin = 1,789  (need to check 282 primes)
N = 7,473,202,036    → pmin = 1,877  (need to check 287 primes)
```

Even for N in the billions, you only need ~300 primes. That's nothing!

## The Formula

```
max pmin(N) ~ 0.2 × ln(N)³
```

| N | Max pmin | Primes to test | Time to find |
|---|----------|----------------|--------------|
| 10⁶ | 523 | 97 | instant |
| 10⁹ | 1,789 | 282 | instant |
| 10¹⁰ | 1,877 | 287 | instant |
| 10¹⁸ | ~14,000 | ~1,700 | still instant |

**99.999% of even numbers need pmin ≤ 631** (only 114 primes to check).

## Why Does This Matter?

1. **Goldbach verification is cheap** - even for astronomically large N, just test a few hundred small primes
2. **Counterexample would be insane** - pmin would need to jump from ~2000 to ~500,000,000,000 (a factor of 250 million)
3. **The pattern is rock solid** - ratio pmin/ln³ stays around 0.15-0.20 across 10 orders of magnitude

## Files

| File | What it does |
|------|--------------|
| `pmin_max_1B.c` | Simple sieve, computes up to 10⁹ (~18 sec) |
| `pmin_segmented.c` | Segmented sieve for 10¹⁰+ (low memory) |
| `goldbach_pmin_arxiv.tex` | The formal paper (LaTeX, arXiv-ready) |

## Quick Start

```bash
# Compile and run (needs ~1GB RAM for 10^9)
gcc -O3 -o goldbach_pmin pmin_max_1B.c -lm
./goldbach_pmin

# For larger N (10^10+), use segmented sieve:
gcc -O3 -o generate_primes generate_primes.c -lm
gcc -O3 -o pmin_segmented pmin_segmented.c -lm
./generate_primes
./pmin_segmented 10000000000  # 10^10, takes ~2 min
```

## Performance

| N | Time | Memory |
|---|------|--------|
| 10⁹ | 18 sec | 1 GB |
| 10¹⁰ | 2 min | 2 MB |
| 10¹² | ~3 hrs | 2 MB |

## Citation

```
Andriawan, R. (2025). "On the Growth Rate of the Minimal Goldbach Prime:
A Computational Study of pmin(N) for Even Integers up to 10^9"
```

## License

MIT
