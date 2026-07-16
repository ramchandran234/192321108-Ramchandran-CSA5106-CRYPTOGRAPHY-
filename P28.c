#include <stdio.h>

// Modular Exponentiation: Computes (base^exp) % mod safely for large numbers
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
    // Publicly known parameters: q (a prime number) and a (a primitive root modulo q)
    long long q = 353; 
    long long a = 3;   

    printf("=== PUBLIC PARAMETERS ===\n");
    printf("Prime modulus (q): %lld\n", q);
    printf("Generator/Base (a): %lld\n\n", a);

    // --- ALICE'S SIDE ---
    long long x_A = 97; // Alice's secret private key
    // Alice computes her public value: Y_A = (a^x_A) % q
    long long Y_A = power(a, x_A, q); 
    printf("=== ALICE ===\n");
    printf("Private Secret (x_A): %lld\n", x_A);
    printf("Sends Public Value (Y_A): %lld\n\n", Y_A);

    // --- BOB'S SIDE ---
    long long x_B = 233; // Bob's secret private key
    // Bob computes his public value: Y_B = (a^x_B) % q
    long long Y_B = power(a, x_B, q); 
    printf("=== BOB ===\n");
    printf("Private Secret (x_B): %lld\n", x_B);
    printf("Sends Public Value (Y_B): %lld\n\n", Y_B);

    // --- KEY COMPUTATION AFTER EXCHANGE ---
    // Alice receives Bob's Y_B and computes: K_Alice = (Y_B ^ x_A) % q
    long long K_Alice = power(Y_B, x_A, q);
    
    // Bob receives Alice's Y_A and computes: K_Bob = (Y_A ^ x_B) % q
    long long K_Bob = power(Y_A, x_B, q);

    printf("=== KEY AGREEMENT VERIFICATION ===\n");
    printf("Key calculated by Alice: %lld\n", K_Alice);
    printf("Key calculated by Bob:   %lld\n", K_Bob);

    if (K_Alice == K_Bob) {
        printf("Success! Both parties agreed on the same shared secret key.\n");
    } else {
        printf("Failure! The keys do not match.\n");
    }

    return 0;
}