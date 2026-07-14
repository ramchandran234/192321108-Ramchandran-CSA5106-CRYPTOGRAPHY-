#include <stdio.h>
#include <math.h>

// Function to compute log2(n!)
double log2_factorial(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += log2((double)i);
    }
    return sum;
}

int main() {
    int n = 25;
    double log2_total = log2_factorial(n);
    double log2_unique = log2_total - 3; // approx adjustment for duplicates

    printf("Total possible keys (ignoring duplicates): ~2^%.0f\n", log2_total);
    printf("Effectively unique keys (considering duplicates): ~2^%.0f\n", log2_unique);

    return 0;
}
