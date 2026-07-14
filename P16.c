#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26

// English letter frequency order (most to least common)
char englishOrder[26] = {
    'E','T','A','O','I','N','S','H','R','D','L','C',
    'U','M','W','F','G','Y','P','B','V','K','J','X','Q','Z'
};

// Count frequencies in ciphertext
void countFreq(const char *cipher, int counts[26]) {
    for (int i=0;i<26;i++) counts[i]=0;
    for (int i=0; cipher[i]; i++) {
        if (isalpha(cipher[i])) {
            counts[toupper(cipher[i])-'A']++;
        }
    }
}

// Sort letters by frequency in ciphertext
void sortByFreq(int counts[26], char order[26]) {
    int used[26]={0};
    for (int k=0;k<26;k++) {
        int best=-1;
        for (int i=0;i<26;i++) {
            if (!used[i] && (best==-1 || counts[i]>counts[best])) {
                best=i;
            }
        }
        order[k] = 'A'+best;
        used[best]=1;
    }
}

// Attempt decryption using frequency mapping
void attemptDecrypt(const char *cipher, char cipherOrder[26], char plainOrder[26], char *output) {
    int map[26];
    for (int i=0;i<26;i++) {
        map[cipherOrder[i]-'A'] = plainOrder[i]-'A';
    }
    int len=strlen(cipher);
    for (int i=0;i<len;i++) {
        if (isalpha(cipher[i])) {
            int c=toupper(cipher[i])-'A';
            output[i] = map[c]+'A';
        } else {
            output[i] = cipher[i];
        }
    }
    output[len]='\0';
}

int main() {
    char cipher[] = "ZIT JXOEA WKGVF YGB PXDHL GCTK ZIT SQMN RGU"; // Example ciphertext
    int counts[26];
    char cipherOrder[26];
    char candidate[200];

    // Count frequencies
    countFreq(cipher, counts);
    sortByFreq(counts, cipherOrder);

    printf("Ciphertext: %s\n", cipher);
    printf("Top 10 possible plaintexts:\n");

    // Generate top N candidates by shifting mapping
    int topN=10;
    for (int n=0;n<topN;n++) {
        // Rotate englishOrder slightly for different guesses
        char shifted[26];
        for (int i=0;i<26;i++) {
            shifted[i] = englishOrder[(i+n)%26];
        }
        attemptDecrypt(cipher, cipherOrder, shifted, candidate);
        printf("Guess %2d: %s\n", n+1, candidate);
    }

    return 0;
}
