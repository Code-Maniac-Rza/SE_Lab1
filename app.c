#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "inventory.dat"

struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
};

void displayRole();
void displayMenu();
void addItem();
void updateItem();
void processOrder();
void generateReport();
void displayInventory();
void placeOrder();
void displayOrder();

int main()
{
    int role;

    int choice;
    int choiceRole;

    do
    {
        displayRole();
        printf("Choose your role: ");
        scanf("%d", &choiceRole);
        switch (choiceRole)
        {
        case 1:
            do
            {

                displayMenuSeller();
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    addItem();
                    break;
                case 2:
                    updateItem();
                    break;
                case 3:
                    processOrder();
                    break;
                case 4:
                    generateReport();
                    break;
                case 5:
                    displayInventory();
                    break;
                case 0:
                    printf("Exiting the application.\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 0);

            break;
        case 2:
            do
            {

                displayMenuBuyer();
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    displayInventory();
                    break;
                case 2:

                    placeOrder();
                    break;
                case 3:
                    displayOrder();
                    break;

                case 0:
                    printf("Exiting the application.\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 0);

            break;
        case 0:
            printf("Exiting the application.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

    } while (choiceRole != 0);

    return 0;
}

void displayRole()
{
    printf("\n===== Role Menu =====\n");
    printf("1. Seller\n");
    printf("2. Buyer\n");
    printf("0. Exit\n");
}

void displayMenuSeller()
{
    printf("\n===== E-Commerce Application Menu =====\n");
    printf("1. Add Item\n");
    printf("2. Update Item\n");
    printf("3. Process Order\n");
    printf("4. Generate Report\n");
    printf("5. Display Inventory\n");
    printf("0. Exit\n");
}

void displayMenuBuyer()
{
    printf("\n===== E-Commerce Application Menu =====\n");
    printf("1. Display Inventory\n");
    printf("2. Place Order\n");
    printf("3. Display Orders\n");
    printf("0. Exit\n");
}

void addItem()
{
    FILE *file = fopen(FILENAME, "ab"); // Open in "ab" mode to append to the file
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product newProduct;
    struct Product product;

    int productID;

    printf("Enter product details:\n");
    printf("ID: ");
    scanf("%d", &productID);

    // Check if the product ID already exists
    while (fread(&product, sizeof(struct Product), 1, file) == 1)
    {
        if (productID == product.id)
        {
            printf("The product ID %d already exists.\n", product.id);
            fclose(file);
            return;
        }
    }

    // Clear the file error and seek to the beginning for appending
    clearerr(file);
    fseek(file, 0, SEEK_END);

    newProduct.id = productID;
    printf("Name: ");
    scanf("%s", newProduct.name);
    printf("Price: ");
    scanf("%f", &newProduct.price);
    printf("Quantity: ");
    scanf("%d", &newProduct.quantity);

    fwrite(&newProduct, sizeof(struct Product), 1, file);

    fclose(file);
}

void placeOrder()
{
    int targetID, orderQuantity;
    printf("Enter the ID of the product to order: ");
    scanf("%d", &targetID);
    printf("Enter the quantity to order: ");
    scanf("%d", &orderQuantity);

    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;

    while (fread(&product, sizeof(struct Product), 1, file) == 1)
    {
        if (product.id == targetID)
        {

            FILE *orderFile = fopen("orders.dat", "ab");
            if (orderFile == NULL)
            {
                perror("Error opening order file");
                fclose(file);
                return;
            }

            if (product.quantity >= orderQuantity)
            {

                product.quantity -= orderQuantity;
                fseek(file, -sizeof(struct Product), SEEK_CUR);
                fwrite(&product, sizeof(struct Product), 1, file);

                struct Product order;
                order.id = targetID;
                strcpy(order.name, product.name);
                order.price = product.price;
                order.quantity = orderQuantity;
                fwrite(&order, sizeof(struct Product), 1, orderFile);

                printf("Order placed successfully.\n");
                fclose(file);
                fclose(orderFile);
                return;
            }
            else
            {
                printf("Insufficient quantity available for %s.\n", product.name);
                fclose(file);
                fclose(orderFile);
                return;
            }
        }
    }

    printf("Product with ID %d not found.\n", targetID);
    fclose(file);
}

void displayOrder()
{
    FILE *orderFile = fopen("orders.dat", "rb");
    if (orderFile == NULL)
    {
        perror("Error opening order file");
        return;
    }

    struct Product order;

    printf("===== Order Details =====\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Price", "Quantity");

    while (fread(&order, sizeof(struct Product), 1, orderFile) == 1)
    {
        printf("%-5d %-20s %-10.2f %-10d\n", order.id, order.name, order.price, order.quantity);
    }

    fclose(orderFile);
}

void updateItem()
{
    int targetID;
    printf("Enter the ID of the product to update: ");
    scanf("%d", &targetID);

    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;

    while (fread(&product, sizeof(struct Product), 1, file) == 1)
    {
        if (product.id == targetID)
        {
            printf("Enter new quantity for %s: ", product.name);
            scanf("%d", &product.quantity);

            fseek(file, -sizeof(struct Product), SEEK_CUR);
            fwrite(&product, sizeof(struct Product), 1, file);
            printf("Product updated successfully.\n");
            fclose(file);
            return;
        }
    }

    printf("Product with ID %d not found.\n", targetID);
    fclose(file);
}

void processOrder()
{
    int targetID, orderQuantity;
    printf("Enter the ID of the product to order: ");
    scanf("%d", &targetID);
    printf("Enter the quantity to order: ");
    scanf("%d", &orderQuantity);

    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;

    while (fread(&product, sizeof(struct Product), 1, file) == 1)
    {
        if (product.id == targetID)
        {
            if (product.quantity >= orderQuantity)
            {
                product.quantity -= orderQuantity;
                fseek(file, -sizeof(struct Product), SEEK_CUR);
                fwrite(&product, sizeof(struct Product), 1, file);
                printf("Order processed successfully.\n");
                fclose(file);
                return;
            }
            else
            {
                printf("Insufficient quantity available for %s.\n", product.name);
                fclose(file);
                return;
            }
        }
    }

    printf("Product with ID %d not found.\n", targetID);
    fclose(file);
}

void generateReport()
{
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;

    printf("===== Inventory Report =====\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Price", "Quantity");

    while (fread(&product, sizeof(struct Product), 1, file) == 1)
    {
        printf("%-5d %-20s %-10.2f %-10d\n", product.id, product.name, product.price, product.quantity);
    }

    fclose(file);
}

void displayInventory()
{
    generateReport();
}
