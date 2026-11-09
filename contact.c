#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook, int sortCriteria)
{
    printf("Name\t\tPhone_no\tEmail_id\n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf("%s\t%s\t%s\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    // populateAddressBook(addressBook);

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void createContact(AddressBook *addressBook)
{
    char string[50];

    printf("Enter Name: ");
    scanf(" %49[^\n]", string);
    strcpy(addressBook->contacts[addressBook->contactCount].name, string);

    printf("Enter Phone Number: ");
    scanf("%49s", string);

    while (validate_phone_no(addressBook, string) == 0)
    {
        printf("Invalid phone number. Enter again: ");
        scanf("%49s", string);
    }

    strcpy(addressBook->contacts[addressBook->contactCount].phone, string);

    printf("Enter Email ID: ");
    scanf("%49s", string);

    while (validate_email(addressBook, string) == 0)
    {
        printf("Invalid email. Enter again: ");
        scanf("%49s", string);
    }

    strcpy(addressBook->contacts[addressBook->contactCount].email, string);

    addressBook->contactCount++;
}

int searchContact(AddressBook *addressBook)
{
    int choice;
    int flag = 0;
    char str[50];

    printf("\nSearch Contact By:\n");
    printf("1. Search by name\n");
    printf("2. Search by phone number\n");
    printf("3. Search by email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice\n");
        return 0;
    }

    printf("Enter the search value: ");
    scanf(" %49[^\n]", str);

    printf("Index\tName\t\tPhone_no\tEmail_id\n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        int match = 0;

        if (choice == 1 &&
            strcasestr(addressBook->contacts[i].name, str) != NULL)
        {
            match = 1;
        }
        else if (choice == 2 &&
                 strcasestr(addressBook->contacts[i].phone, str) != NULL)
        {
            match = 1;
        }
        else if (choice == 3 &&
                 strcasestr(addressBook->contacts[i].email, str) != NULL)
        {
            match = 1;
        }

        if (match == 1)
        {
            printf("%d\t%s\t%s\t%s\n",
                   i,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            flag = 1;
        }
    }

    if (flag == 0)
    {
        printf("Contact not found\n");
    }

    return flag;
}

void editContact(AddressBook *addressBook)
{
    int ret;
    int index;
    int choice;
    char str[50];

    ret = searchContact(addressBook);

    if (ret == 0)
    {
        return;
    }

    printf("Enter the index you want to edit: ");
    scanf("%d", &index);

    if (index < 0 || index >= addressBook->contactCount)
    {
        printf("Invalid index\n");
        return;
    }

    printf("1. Edit name\n");
    printf("2. Edit phone\n");
    printf("3. Edit email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Enter new name: ");
        scanf(" %49[^\n]", str);

        strcpy(addressBook->contacts[index].name, str);
        printf("Contact edited successfully\n");
        break;

    case 2:
        printf("Enter new phone number: ");
        scanf("%19s", str);

        while (validate_phone_no(addressBook, str) == 0)
        {
            printf("Invalid phone number or number already exists. Enter again: ");
            scanf("%19s", str);
        }

        strcpy(addressBook->contacts[index].phone, str);
        printf("Contact edited successfully\n");
        break;

    case 3:
        printf("Enter new email: ");
        scanf("%49s", str);

        while (validate_email(addressBook, str) == 0)
        {
            printf("Invalid email or email already exists. Enter again: ");
            scanf("%49s", str);
        }

        strcpy(addressBook->contacts[index].email, str);
        printf("Contact edited successfully\n");
        break;

    default:
        printf("Invalid choice\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
    int ret;
    int index;
    char confirm;

    ret = searchContact(addressBook);

    if (ret == 0)
    {
        return;
    }

    printf("Enter the index you want to delete: ");
    scanf("%d", &index);

    if (index < 0 || index >= addressBook->contactCount)
    {
        printf("Invalid index\n");
        return;
    }

    printf("Are you sure you want to delete index %d? (y/n): ", index);
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y')
    {
        for (int i = index; i < addressBook->contactCount - 1; i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }

        addressBook->contactCount--;

        printf("Contact deleted successfully\n");
    }
    else
    {
        printf("Contact deletion cancelled\n");
    }
}

int validate_phone_no(AddressBook *addressBook, char *phone)
{
    if (strlen(phone) != 10)
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (phone[i] < '0' || phone[i] > '9')
        {
            return 0;
        }
    }

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return 0;
        }
    }

    return 1;
}

int validate_email(AddressBook *addressBook, char *email)
{
    int atCount = 0;
    int atPos = -1;
    int dotPos = -1;

    for (int i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            atCount++;
            atPos = i;
        }

        if (email[i] == '.' && atPos != -1 && dotPos == -1)
        {
            dotPos = i;
        }

        if (email[i] >= 'A' && email[i] <= 'Z')
        {
            return 0;
        }
    }

    if (atCount != 1)
    {
        return 0;
    }

    if (atPos <= 0 || dotPos <= atPos + 1 ||
        dotPos >= strlen(email) - 1)
    {
        return 0;
    }

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0)
        {
            return 0;
        }
    }

    return 1;
}