#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26

// English letter frequency (approximate percentages)
double englishFreq[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150,
    1.974, 0.074
};

// Function to decrypt with a given shift
void decrypt(const char *cipher, int shift, char *plain) {
    int len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        if (isalpha(cipher[i])) {
            int c = toupper(cipher[i]) - 'A';
            int p = (c - shift + MOD) % MOD;
            plain[i] = p + 'A';
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = '\0';
}

// Score plaintext based on letter frequency
double scoreText(const char *text) {
    int counts[26] = {0};
    int total = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            counts[toupper(text[i]) - 'A']++;
            total++;
        }
    }
    double score = 0.0;
    for (int i = 0; i < 26; i++) {
        double observed = (double)counts[i] / total * 100.0;
        score += (englishFreq[i] - observed) * (englishFreq[i] - observed);
    }
    return -score; // lower difference = better, so invert
}

int main() {
    char cipher[] = "WKH TXLFN EURZQ IRA MXPSV RYHU WKH ODCB GRJ"; // Example Caesar cipher
    int topN = 10;

    char candidate[200];
    double scores[26];
    char results[26][200];

    // Try all shifts
    for (int shift = 0; shift < 26; shift++) {
        decrypt(cipher, shift, candidate);
        scores[shift] = scoreText(candidate);
        strcpy(results[shift], candidate);
    }

    // Print top N results
    printf("Ciphertext: %s\n", cipher);
    printf("Top %d possible plaintexts:\n", topN);

    for (int n = 0; n < topN; n++) {
        int best = -1;
        for (int i = 0; i < 26; i++) {
            if (best == -1 || scores[i] > scores[best]) {
                best = i;
            }
        }
        printf("Shift %2d: %s\n", best, results[best]);
        scores[best] = -1e9; // mark used
    }

    return 0;
}
