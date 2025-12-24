# Goldbach pmin Simulation

**TL;DR:** Goldbach says every even number N = prime + prime. We ask: *how small can the first prime be?* Turns out, almost always tiny. This explains why Oliveira e Silva et al. were able to verify Goldbach up to 4×10¹⁸.

---

## What's Goldbach's Conjecture?

```
Every even integer > 2 is the sum of two primes.

Examples:
  10 = 3 + 7  = 5 + 5
  30 = 7 + 23 = 11 + 19 = 13 + 17
  98 = 19 + 79
```

Unproven since 1742, but verified computationally up to 4×10¹⁸ by [Oliveira e Silva, Herzog, and Pardi (2014)](https://www.ams.org/journals/mcom/2014-83-288/S0025-5718-2013-02787-1/). That's 4,000,000,000,000,000,000 numbers checked. We're not proving it here—we're asking a different question that explains *why* their verification was even feasible.

---

## The Key Question

**What's the smallest prime that works?**

We call this `pmin(N)`. For N=30, even though 30 = 7+23 = 11+19 = 13+17, the smallest working prime is 7, so `pmin(30) = 7`.

This matters because:
- If pmin is always small, Goldbach pairs are **easy to find**
- If pmin could be huge, verification would be **computationally impossible**

Spoiler: pmin is almost always tiny.

---

## The Diagonal Search Visualization

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

**Notice:** pmin is almost always 3, 5, or 7! You rarely need to go far right in the grid.

---

## The Main Result: How Does pmin Grow?

We computed pmin for **all 500 million** even numbers up to 10⁹, plus extended runs to 10¹⁰.

### Distribution of pmin Values

| Percentile | pmin ≤ | Primes to test |
|------------|--------|----------------|
| 95% | 103 | 26 |
| 99% | 191 | 42 |
| 99.9% | 331 | 66 |
| 99.999% | 631 | 114 |

**99.999% of even numbers need pmin ≤ 631.** That's only 114 primes to check!

### Maximum pmin at Each Scale

| N | Max pmin | Primes tested | pmin / ln(N)³ |
|---|----------|---------------|---------------|
| 10² | 19 | 8 | 0.195 |
| 10³ | 73 | 20 | 0.221 |
| 10⁴ | 173 | 40 | 0.221 |
| 10⁵ | 293 | 62 | 0.192 |
| 10⁶ | 523 | 97 | 0.198 |
| 10⁷ | 751 | 132 | 0.179 |
| 10⁸ | 1,093 | 183 | 0.175 |
| 10⁹ | 1,789 | 282 | 0.201 |
| 10¹⁰ | 1,877 | 287 | 0.154 |

### The Formula

```
max pmin(N) ~ 0.2 × ln(N)³
```

The ratio pmin/ln(N)³ stays remarkably stable around 0.15-0.20 across **10 orders of magnitude**. This is our main empirical finding.

---

## Why ln(N)³? A Heuristic Argument

The cubic growth isn't coincidental. Here's an intuitive explanation:

**Step 1: Prime density.** By the Prime Number Theorem, roughly 1 in ln(N) integers near N is prime.

**Step 2: Pair probability.** For N = p + q to work, both p AND (N-p) must be prime. Treating these as roughly independent:
```
P(valid pair) ≈ 1/ln(N) × 1/ln(N) = 1/ln(N)²
```

**Step 3: Expected search depth.** If each candidate has ~1/ln(N)² chance of success, we expect to test ~ln(N)² primes before finding one. So:
```
typical pmin ~ ln(N)²
```

**Step 4: Maximum vs typical.** We're not looking for the *typical* pmin, but the *maximum* across N/2 even integers. This is an extreme value problem.

Analogy: If you flip a coin until you get heads, expected flips = 2. But if you repeat this experiment a million times, the longest streak is much larger than 2.

For geometric distributions, the maximum of n samples grows as log(n) × mean:
```
max pmin ~ log(N/2) × ln(N)² ~ ln(N) × ln(N)² = ln(N)³
```

The constant 0.2 is determined empirically from our data.

---

## Connection to Oliveira e Silva's Verification

This is where it gets exciting.

Oliveira e Silva et al. verified Goldbach up to 4×10¹⁸ using what they called the **"minimal partition" strategy**—for each N, find the *smallest* prime p such that N-p is also prime.

**That's exactly pmin!** Our study provides empirical evidence for *why* their approach was computationally feasible:

- At 10¹⁸, if our formula holds, max pmin ~ 14,000 (testing ~1,700 primes)
- At 4×10¹⁸, max pmin ~ 15,000 (testing ~1,800 primes)

Even at astronomical scales, you only need to test a few thousand small primes. The computational cost grows **logarithmically**, not linearly, with N.

### Extrapolation (assuming the pattern persists)

| Scale | Predicted max pmin | Primes to test |
|-------|-------------------|----------------|
| 10¹⁸ (current record) | ~14,000 | ~1,700 |
| 10²⁴ | ~25,000 | ~2,800 |
| 10³⁰ | ~38,000 | ~4,000 |
| 10¹⁰⁰ (googol) | ~2,400,000 | ~180,000 |

**Caveat:** At 10¹⁰⁰, primality testing itself becomes expensive (~hours per number using Miller-Rabin). But the *number of primes to test* stays manageable.

---

## Record-Breaking pmin Values

All 33 cases where pmin set a new record up to 10⁹:

```
N                pmin    log₁₀(N)
────────────────────────────────
6                3       0.78
12               5       1.08
30               7       1.48
98               19      1.99
220              23      2.34
308              31      2.49
556              47      2.75
992              73      3.00
2,642            103     3.42
5,372            139     3.73
7,426            173     3.87
43,532           211     4.64
54,244           233     4.73
63,274           293     4.80
113,672          313     5.06
128,168          331     5.11
194,428          359     5.29
194,470          383     5.29
413,572          389     5.62
503,222          523     5.70
1,077,422        601     6.03
3,526,958        727     6.55
3,807,404        751     6.58
10,759,922       829     7.03
24,106,882       929     7.38
27,789,878       997     7.44
37,998,938       1039    7.58
60,119,912       1093    7.78
113,632,822      1163    8.06
187,852,862      1321    8.27
335,070,838      1427    8.53
419,911,924      1583    8.62
721,013,438      1789    8.86
```

At 10¹⁰, the record extends to:
```
7,473,202,036    1877    9.87
```

---

## What Would a Goldbach Counterexample Require?

A counterexample would be an even N where **no** prime p makes N-p also prime. That means pmin(N) doesn't exist, or equivalently, pmin(N) > N/2.

At N = 10⁹:
- Observed max pmin = 1,789
- N/2 = 500,000,000
- Ratio: 1 : 280,000

For Goldbach to fail, pmin would need to jump by a factor of **280,000** beyond its expected value. Our data shows no hint of such behavior—the ratio pmin/ln(N)³ is rock solid.

This doesn't prove Goldbach, but it quantifies how dramatic a deviation would be required.

---

## Files

| File | Description |
|------|-------------|
| `pmin_max_1B.c` | Simple sieve + pmin computation up to 10⁹ (~18 sec, 1GB RAM) |
| `pmin_segmented.c` | Segmented sieve for 10¹⁰+ (low memory, ~2MB) |
| `generate_primes.c` | Generates prime list for segmented sieve |
| `goldbach_pmin_arxiv.tex` | Formal paper (LaTeX, arXiv-ready) |
| `goldbach_pmin_arxiv.pdf` | Compiled PDF |
| `goldbach_pmin_paper.md` | Markdown version of the paper |
| `fig_*.pdf` | Figures for the paper |

---

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

---

## Performance

Benchmarked on Intel Core i7-13700H (14 cores, 5.0 GHz), single-threaded:

| N | Time | Memory | Notes |
|---|------|--------|-------|
| 10⁹ | 18 sec | 1 GB | Simple sieve fits in RAM |
| 10¹⁰ | 2 min | 2 MB | Segmented sieve |
| 10¹² | ~3 hrs | 2 MB | Segmented sieve |

The fast runtime is because:
1. Sieve provides O(1) primality lookup
2. Most numbers have tiny pmin (95% need ≤26 primes)
3. Even worst cases only need ~300 primes

---

## References

- **Hardy, G.H. & Littlewood, J.E. (1923).** "Some problems of 'Partitio numerorum'; III: On the expression of a number as a sum of primes." *Acta Mathematica*, 44, 1-70.

- **Oliveira e Silva, T., Herzog, S., & Pardi, S. (2014).** "Empirical verification of the even Goldbach conjecture and computation of prime gaps up to 4×10¹⁸." *Mathematics of Computation*, 83(288), 2033-2060.

---

## Citation

```bibtex
@misc{andriawan2025goldbach,
  author = {Andriawan, Rizky},
  title = {On the Growth Rate of the Minimal Goldbach Prime:
           A Computational Study of pmin(N) for Even Integers up to 10^9},
  year = {2025},
  url = {https://github.com/rizkyandriawan/goldbach-pmin-simulation}
}
```

---

## License

MIT
