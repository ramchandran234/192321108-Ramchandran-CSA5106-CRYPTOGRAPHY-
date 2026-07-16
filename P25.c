#include <stdio.h>

// Standard Euclidean Algorithm to compute the Greatest Common Divisor
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Extended Euclidean Algorithm to find the modular multiplicative inverse
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

long long mod_inverse(long long e, long long phi) {
    long long x, y;
    long long g = extended_gcd(e, phi, &x, &y);
    if (g != 1) return -1;
    return (x % phi + phi) % phi;
}

// Modular Exponentiation: Computes (base^exp) % mod safely
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    // Public Parameters
    long long n = 2443; // Formed by secret primes p=37, q=661
    long long e = 17;   // Public Encryption Exponent

    // Suppose we intercept a Ciphertext block. 
    // The original Plaintext was M = 74 (which shares a factor of 37 with n).
    // M^e % n -> 74^17 % 2443 = 2035
    long long ciphertext_block = 2035;

    printf("--- ATTACKING RSA KEY USING WEAK CIPHERTEXT BLOCK ---\n");
    printf("Public Key: (e = %lld, n = %lld)\n", e, n);
    printf("Intercepted Ciphertext Block: %lld\n\n", ciphertext_block);

    // Step 1: Compute GCD of the ciphertext and n to extract a prime factor
    printf("Step 1: Calculating gcd(Ciphertext, n)...\n");
    long long p = gcd(ciphertext_block, n);

    if (p > 1 && p < n) {
        long long q = n / p;
        printf("Success! Found prime factor p = %lld\n", p);
        printf("Calculated second prime factor q = n / p = %lld\n\n", q);

        // Step 2: Reconstruct phi(n)
        long long phi = (p - 1) * (q - 1);
        printf("Step 2: Computing phi(n) = %lld\n", phi);

        // Step 3: Compute the Private Key (d)
        long long d = mod_inverse(e, phi);
        printf("Step 3: Calculating Private Key d = %lld\n\n", d);

        // Step 4: Decrypt the block to show it works
        long long decrypted_plaintext = power(ciphertext_block, d, n);
        
        printf("=== Final Results ===\n");
        printf("Cracked Private Key (d): %lld\n", d);
        printf("Decrypted Plaintext Block: %lld\n", decrypted_plaintext);
    } else {
        printf("Failed to find a common factor using this block.\n");
    }

    return 0;
}