#include <stdio.h>

struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
};

int main()
{
    struct Product initialInventory[] = {
        {1, "ProductA", 10.99, 50},
        {2, "ProductB", 5.99, 100},
        {3, "ProductC", 20.49, 30}

    };

    FILE *file = fopen("inventory.dat", "wb");

    if (file == NULL)
    {
        printf("Error creating the file.\n");
        return 1;
    }

    fwrite(initialInventory, sizeof(struct Product), sizeof(initialInventory) / sizeof(struct Product), file);

    fclose(file);

    printf("inventory.dat file created successfully with initial data.\n");

    return 0;
}
