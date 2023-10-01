#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100

// information of each header elements
typedef struct
{
    char name[50];
    int logicalAddress;
} HeaderElement;

// information of each contact
typedef struct
{
    char name[50];
    char phone[15];
} Contact;

// write Phonebook to Binary File
void WriteToFile(Contact contacts[], int numContacts, char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Cannot open file %s to write \n", filename);
        return;
    }

    // write header to file (header with fixed size: MAX_CONTACTS * sizeof(HeaderElement))
    HeaderElement header[MAX_CONTACTS];
    for (int i = 0; i < numContacts; i++)
    {
        strcpy(header[i].name, contacts[i].name);
        // logical address of Contact with given name
        header[i].logicalAddress = sizeof(Contact) * i + sizeof(HeaderElement) * MAX_CONTACTS;
    }
    fwrite(header, sizeof(HeaderElement), MAX_CONTACTS, file);

    // write contact to file
    for (int i = 0; i < numContacts; i++)
    {
        fwrite(&contacts[i], sizeof(Contact), 1, file);
    }

    fclose(file);
}

// find contact with name in header part
void FindContact(const char *name, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Cannot open file %s to read \n", filename);
        return;
    }

    // read header from file
    HeaderElement header[MAX_CONTACTS];
    fread(header, sizeof(HeaderElement), MAX_CONTACTS, file);

    // find Contact with name
    int found = 0;
    for (int i = 0; i < MAX_CONTACTS; i++)
    {
        if (strcmp(header[i].name, name) == 0)
        {
            // read Contact from its logical address
            Contact contact;
            fseek(file, header[i].logicalAddress, SEEK_SET);
            fread(&contact, sizeof(Contact), 1, file);
            printf("Contact with name %s exist in PhoneBook \n",contact.name);
            printf("Name: %s\nPhone number: %s\n", contact.name, contact.phone);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Cannot find Contact with given name %s in PhoneBook!!\n", name);
    }

    fclose(file);
}

int main()
{
    Contact contacts[MAX_CONTACTS];
    int numContacts = 0;

    // get Contact from PhoneBook information
    strcpy(contacts[0].name, "Tran Trung Hieu");
    strcpy(contacts[0].phone, "0123456789");
    numContacts++;

    strcpy(contacts[1].name, "Nguyen Phuong Linh");
    strcpy(contacts[1].phone, "0123456788");
    numContacts++;

    strcpy(contacts[1].name, "Nguyen Ngoc Duong");
    strcpy(contacts[1].phone, "0123456787");
    numContacts++;

    // write PhoneBook file
    WriteToFile(contacts, numContacts, "PhoneBook.bin");

    // find Contact with name and print phone number
    FindContact("Tran Trung Hieu", "PhoneBook.bin");
    // find Contact not exist in PhoneBook
    FindContact("Tran Chung Hieu", "PhoneBook.bin");

    return 0;
}
