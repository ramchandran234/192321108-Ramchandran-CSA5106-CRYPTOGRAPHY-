#include <stdio.h>
#include <stdlib.h>

// Extended Euclidean Algorithm to find modular inverse
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

int main() {
    // --- BOB'S SETUP ---
    // Bob originally uses p = 61, q = 53 -> n = 3233, phi = 60 * 52 = 3120
    long long n = 3233;
    
    // Key Pair 1 (Leaked)
    long long e1 = 17;
    long long d1 = 2753; // Leaked private key

    // Bob tries to be clever and generates Key Pair 2 using the SAME n
    long long e2 = 7;
    long long d2 = 1783; // Bob thinks this new key is safe!

    printf("=== BOB'S INITIAL CONFIGURATION ===\n");
    printf("Modulus (n): %lld\n", n);
    printf("Leaked Key Pair: (e1 = %lld, d1 = %lld)\n", e1, d1);
    printf("Bob's New Public Key (e2): %lld\n\n", e2);

    // --- THE ATTACKER'S SIDE ---
    printf("=== ATTACKER EXPLOITING THE LEAK ===\n");
    
    // Step 1: The attacker uses the leaked d1 to factor n.
    // (A simplified deterministic representation of the probabilistic algorithm for small numbers)
    long long p = 0, q = 0;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            p = i;
            q = n / i;
            break;
        }
    }

    printf("1. Attacker successfully factors n using leaked data: p = %lld, q = %lld\n", p, q);

    // Step 2: Attacker reconstructs phi(n)
    long long cracked_phi = (p - 1) * (q - 1);
    printf("2. Attacker computes phi(n): %lld\n", cracked_phi);

    // Step 3: Attacker intercepts Bob's new public key e2 and calculates the new private key d2
    long long cracked_d2 = mod_inverse(e2, cracked_phi);
    
    printf("3. Attacker computes Bob's brand new private key (d2): %lld\n\n", cracked_d2);

    printf("=== VERIFICATION ===\n");
    if (cracked_d2 == d2) {
        printf("CRITICAL FAILURE: The attacker successfully deduced the new private key!\n");
    } else {
        printf("Attack failed.\n");
    }

    return 0;
}