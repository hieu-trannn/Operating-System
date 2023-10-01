#include <stdio.h>

int main(int argc, char **argv)
{
    int idx;

    // Exception when you don't choose any file
    if (argc == 1) {
        printf("No files have size of 0!\n");
        return -1;
    }

    // Print all the files
    printf("\nFiles have size of 0:\n\n");
    for (int i = 1; i < argc; i++) {
        printf("\t%d. %s \n", i, argv[i]);
    }

    // Ask if user want to delete the next file 
    do {
        printf("\nDo you want to delete: '%s'\n 1. Yes \n 2. No\n", argv[1]);
        scanf("%d", &idx);
    } while ((idx != 1 && idx != 2));

    return idx;
}

