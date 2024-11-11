#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#if __has_include("fileutil.h")
#include "fileutil.h"
#endif

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    int size;
    char (*hashes)[HASH_LEN] = loadFile2D(argv[1], &size);
    
    
    // CHALLENGE1: Sort the hashes using qsort.
    qsort(hashes, size, sizeof(hashes[0]), (int (*)(const void *, const void *))strcmp);

    // Open the password file for reading.
    FILE *dict = fopen(argv[2], "r");
    if (!dict)
    {
        perror("Could not open dictionary file");
        exit(1);
    }

    // CHALLENGE1: Use binary search instead of linear search.
    char word[PASS_LEN];
    int cracked = 0;

    while (fgets(word, sizeof(word), dict))
    {
        // trim newline
        word[strcspn(word, "\n")] = '\0';
        char *hash = md5(word, strlen(word));

        // binary search for hash in sorted array
        if (bsearch(hash, hashes, size, sizeof(hashes[0]), (int (*)(const void *, const void *))strcmp))
        {
            printf("Found: %s -> %s\n", hash, word);
            cracked++;
        }
        // free the hash after use
        free(hash);
    }

    // Close the file
    fclose(dict);

    // Display the number of hashes found.
    printf("%d hashes cracked.\n", cracked);
    
    // Free up memory.
    free2D(hashes);
}