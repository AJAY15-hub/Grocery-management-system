#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

#define ITEM_COUNT 8

struct Product {
         int id; 
         char name[30]; 
         int quantity; 
         float price; 
        };

struct Sale { 
              int id; 
              char name[30]; 
              int quantitySold; 
              float total; 
              char date[20]; 
              char customer[30]; 
            };

struct Product items[ITEM_COUNT] = { {1, "Rice", 50, 60.0}, 
                                     {2, "Sugar", 40, 45.0}, 
                                     {3, "Milk", 30, 25.0}, 
                                     {4, "Bread", 20, 30.0},
                                     {5, "Oil", 25, 120.0}, 
                                     {6, "Eggs", 100, 6.0}, 
                                     {7, "Salt", 35, 20.0}, 
                                     {8, "Tea", 15, 150.0} 
                                    };


int initialStock[ITEM_COUNT];

void displayStock(); 
void makeSale(); 
void viewSalesReport(); 
void getCurrentDate(char *buffer); 
void updateStock(); 
void viewDailyExpenseTable(); 
void generateBill(char customer[], int id, int qty, float total);


int main() {
     for (int i = 0; i < ITEM_COUNT; i++) { 
        initialStock[i] = items[i].quantity; 
    }

    int choice;
    do {
                printf("\n=== Grocery Shop System ===\n");
                printf("1. View Stock\n");
                printf("2. Make Sale\n");
                printf("3. View Daily Sales Report\n");
                printf("4. Add More Stock\n");
                printf("5. View Daily Expense Summary\n");
                printf("6. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

              switch(choice)
               {
                 case 1: displayStock(); break;
                 case 2: makeSale(); break;
                 case 3: viewSalesReport(); break;
                 case 4: updateStock(); break;
                 case 5: viewDailyExpenseTable(); break;
                 case 6: printf("Exiting...\n"); break;
                 default: printf("Invalid choice!\n");
                }
        } while(choice != 6);
        return 0;

}

void displayStock() {

     printf("\n--- Current Stock ---\n"); 
     printf("ID\tName\t\tQty\tPrice\n"); 
     
     for(int i = 0; i < ITEM_COUNT; i++){ 
        printf("%d\t%-10s\t%d\t%.2f\n", items[i].id, items[i].name, items[i].quantity, items[i].price); 
    } 
}


void makeSale(){ 
     int id,qty, found = 0; 
     char today[20], customer[30]; 
     getCurrentDate(today);

    FILE *fptr = fopen("sales.dat", "ab");
    if (!fptr) {
         printf("Error opening sales file.\n");
         return;
    }

    printf("Enter customer name: ");
    scanf("%s", customer);
    printf("Enter product ID to sell: ");
    scanf("%d", &id);
    for (int i = 0; i < ITEM_COUNT; i++){
          if (items[i].id == id)
           {
                printf("Enter quantity to sell: ");
                scanf("%d", &qty);
                   if (qty > items[i].quantity)
                   {
                         int extra;
                         printf("Not enough stock. Available: %d.\n", items[i].quantity);
                         printf("Do you want to add more stock to fulfill this sale? Enter additional quantity: ");
                         scanf("%d", &extra);
                         items[i].quantity += extra;
                         printf("Stock updated. New stock: %d\n", items[i].quantity);
                    }

                 items[i].quantity -= qty;

                  struct Sale s;
                  s.id = items[i].id;
                  strcpy(s.name, items[i].name);
                  s.quantitySold = qty;
                  s.total = qty * items[i].price;
                  strcpy(s.date, today);
                  strcpy(s.customer, customer);

                 fwrite(&s, sizeof(struct Sale), 1, fptr);
                 generateBill(customer, s.id, qty, s.total);
                 found = 1;
                break;
            }
    }

   if (!found) printf("Invalid product ID.\n");
   fclose(fptr);

}

void generateBill(char customer[], int id, int qty, float total){

       char today[20]; 
       getCurrentDate(today); 
       char filename[50]; 
       sprintf(filename, "%s_%s_bill.txt", customer, today);

    FILE *bill = fopen(filename, "a");
    if (!bill){
        printf("Error saving the bill.\n");
        return;
    }

    printf("\n--- Bill ---\n");
    fprintf(bill, "--- Bill for %s on %s ---\n", customer, today);
    printf("Customer: %s\n", customer);
    fprintf(bill, "Customer: %s\n", customer);

    for (int i = 0; i < ITEM_COUNT; i++)
    {
          if (items[i].id == id)
           {
              printf("Item: %s\n", items[i].name);
              printf("Quantity: %d\n", qty);
              printf("Unit Price: %.2f\n", items[i].price);
              printf("Total: Rs. %.2f\n", total);

              fprintf(bill, "Item: %s\n", items[i].name);
              fprintf(bill, "Quantity: %d\n", qty);
              fprintf(bill, "Unit Price: %.2f\n", items[i].price);
              fprintf(bill, "Total: Rs. %.2f\n", total);
              break;
            }
    }

   fprintf(bill, "-----------------------\n");
   fclose(bill);
   printf("Bill saved to %s\n", filename);

}

void viewSalesReport(){

    FILE *fptr = fopen("sales.dat", "rb"); 
    struct Sale s; 
    float totalRevenue = 0; 
    char today[20]; 
    getCurrentDate(today);

    if (!fptr) {
        printf("No sales records found.\n");
        return;
    }

    printf("\n--- Sales Report for %s ---\n", today);
    printf("Customer\tItem\tQty\tTotal\n");
    while (fread(&s, sizeof(struct Sale), 1, fptr)){
         if (strcmp(s.date, today) == 0) 
            {
              printf("%-10s\t%-10s\t%d\t%.2f\n", s.customer, s.name, s.quantitySold, s.total);
              totalRevenue += s.total;
            }
    }
    printf("Total Revenue: Rs. %.2f\n", totalRevenue);
    fclose(fptr);

}

void updateStock(){
     int id, qty, found = 0; 
     printf("Enter Product ID to add stock: "); 
     scanf("%d", &id); 
    for(int i = 0; i < ITEM_COUNT; i++){ 
        if (items[i].id == id){ 
            printf("Enter quantity to add: "); 
            scanf("%d", &qty); items[i].quantity += qty; 
            printf("Updated stock for %s: %d\n", items[i].name, items[i].quantity); found = 1; 
            break;
        } 
    } 
    if (!found) printf("Invalid product ID.\n"); 
}

void viewDailyExpenseTable(){ 
         char today[20]; 
         getCurrentDate(today); 
         FILE *fptr = fopen("sales.dat", "rb"); 
         struct Sale s; 
         int sold[ITEM_COUNT] = {0}; 
         float totalRevenue = 0;

    if (!fptr) {
      printf("No sales data found.\n");
      return;
    }

    while (fread(&s, sizeof(struct Sale), 1, fptr)) {
        if (strcmp(s.date, today) == 0) {
            for (int i = 0; i < ITEM_COUNT; i++) {
                if (items[i].id == s.id) {
                   sold[i] += s.quantitySold;
                   totalRevenue += s.total;
                   break;
                }
            }
        }
    }  
    fclose(fptr);

    printf("\n--- Daily Expense Summary for %s ---\n", today);
    printf("ID\tName\t\tStartQty\tSold\tRemain\tPrice\tSales(Rs)\n");
    for (int i = 0; i < ITEM_COUNT; i++) {
      int start = initialStock[i];
      int remaining = items[i].quantity;
      int soldQty = sold[i];
      float revenue = soldQty * items[i].price;
     printf("%d\t%-10s\t%d\t\t%d\t%d\t%.2f\t%.2f\n", items[i].id, items[i].name, start, soldQty, remaining, items[i].price, revenue);
    }
    printf("\nTotal Revenue Today: Rs. %.2f\n", totalRevenue);

}

void getCurrentDate(char *buffer){ 

     time_t t = time(NULL); 
     struct tm tm = *localtime(&t); 
     sprintf(buffer, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900); 
}