#include <stdio.h>
#include <stdlib.h>

// Extended Euclidean Algorithm to find the multiplicative inverse
// It solves: a*x + b*y = gcd(a, b)
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    
    long long x1, y1;
    long long gcd = extended_gcd(b, a % b, &x1, &y1);
    
    // Update x and y using results of recursive call
    *x = y1;
    *y = x1 - (a / b) * y1;
    
    return gcd;
}

// Function to find the modular inverse of e modulo phi
long long mod_inverse(long long e, long long phi) {
    long long x, y;
    long long gcd = extended_gcd(e, phi, &x, &y);
    
    if (gcd != 1) {
        printf("Multiplicative inverse does not exist (gcd != 1).\n");
        return -1;
    }
    
    // x might be negative, convert it to a positive value modulo phi
    return (x % phi + phi) % phi;
}

int main() {
    long long n = 3599;
    long long e = 31;
    long long p = 0, q = 0;
    
    printf("Given Public Key: (e = %lld, n = %lld)\n\n", e, n);

    // Step 1: Trial-and-error (factorization) to find p and q
    // We look for odd factors starting from 3 up to sqrt(n)
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            p = i;
            q = n / i;
            break;
        }
    }

    if (p == 0 || q == 0) {
        printf("Error: Could not factorize n.\n");
        return 1;
    }

    printf("Step 1: Factorizing n...\n");
    printf("Found prime factors: p = %lld, q = %lld\n", p, q);
    printf("Verification: %lld * %lld = %lld\n\n", p, q, p * q);

    // Step 2: Compute phi(n)
    long long phi = (p - 1) * (q - 1);
    printf("Step 2: Computing phi(n)...\n");
    printf("phi(n) = (p - 1) * (q - 1) = %lld * %lld = %lld\n\n", p - 1, q - 1, phi);

    // Step 3: Compute private key d using the Extended Euclidean Algorithm
    printf("Step 3: Finding multiplicative inverse of e = %lld mod phi(n) = %lld...\n", e, phi);
    long long d = mod_inverse(e, phi);

    if (d != -1) {
        printf("Found private key: d = %lld\n\n", d);
        printf("=== Final Result ===\n");
        printf("The private key of the user is d = %lld\n", d);
        printf("Verification: (e * d) mod phi(n) -> (%lld * %lld) mod %lld = %lld\n", 
               e, d, phi, (e * d) % phi);
    }

    return 0;
}