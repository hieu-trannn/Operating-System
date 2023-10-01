#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("You haven't added any dishes yet, please try again and pass dish(es) name.\n");
        return -1;
    }

    printf("Welcome to our program!\n");

    printf("You have added %d dish(es) to the menu:\n", argc - 1);
    for (int i = 1; i < argc; i++)
    {
        printf("\t %d. %s\n", i, argv[i]);
    }

    printf("=========================\n");
    printf("Please choose your dish: ");
    int choice;
    scanf("%d", &choice);
    while ((choice >= argc) || (choice < 1))
    {
        int flag = 0;
        printf("Out of range in the menu, choose again? \n");
        printf("[Enter 1 for Yes, others for No]\n");
        scanf("%d", &flag);
        if (flag != 1)
        {
            printf("Quit the program....\n");
            return 0;
        }
        printf("Choose your dish: ");
        scanf("%d", &choice);
    }

    printf("=========================\n");
    printf("You have choosed %s!\n", argv[choice]);
    printf("Thank you! Quit the program...\n");
    return choice;
}
