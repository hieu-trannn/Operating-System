// PLEASE READ README.MD FIRST!!!

#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf("You haven't have any empty files in the folder\n");
        return -1; // exit the shell echo $? == -1
    }
    // show menu of empty files
    // if command argument: p: stop print empty file
    if (!strcmp(argv[1], "p"))
    {
        for (int i = 2; i < argc; i++)
        {
            printf("\t %d. %s\n", i - 1, argv[i]);
        }
        return 1;
    }
    else
    {
        printf("Please choose delete or not: type '1': delete, other: not delete\n"); //
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            // delete file
            return 0;
        }
        else
        {
            // keep file
            return 1;
        }
    }
}
