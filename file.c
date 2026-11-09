#include <stdio.h>
#include "file.h"

void loadContactsFromFile(AddressBook *addressBook)
{
    // open the file in read mode
    FILE *fptr = fopen("contacts.csv", "r");

    // read line 1 and store into contactCount
    fscanf(fptr, "%d\n", &addressBook->contactCount);

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        // read each contact's name, phone and email
        fscanf(fptr, "%[^,],%[^,],%[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    // close the file
    fclose(fptr);
}

void saveContactsToFile(AddressBook *addressBook)
{
    // open the file in write mode
    FILE *fptr = fopen("contacts.csv", "w");

    // write the contact count on line 1
    fprintf(fptr, "%d\n", addressBook->contactCount);

    // loop contactCount times
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        // write each contact on a new line
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    // close the file
    fclose(fptr);
}
