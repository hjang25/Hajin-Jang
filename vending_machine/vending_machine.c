#include <stdio.h>

int main(void) {
    int num_cola = 5, num_candy = 5, num_pop = 5;
    float p_cola = 0.75, p_candy = 1.25, p_pop = 0.5; // Price of goods in dollars
    double money_made = 0.0;
    int menu = 5; // integer array used to store menu choice inputs
    float money; // float array used to store money inputs

    // Printing welcome message
    printf("Welcome to the Vending Machine!\n");

    // Printing menu
    printf("enter your choice by # and input cash amount, repeatedly (^d to end).\n");
    printf("[0] %d cola left: cost is $%.2f\n", num_cola, p_cola);
    printf("[1] %d candybar left: cost is $%.2f\n", num_candy, p_candy);
    printf("[2] %d popcorn left: cost is $%.2f\n", num_pop, p_pop);
    printf("Money made so far is $%.2lf\n", money_made);
    

    while(scanf(" %d %f", &menu, &money) == 2)
    {
        if(menu < 0 || money < 0) // Catch if input is a negative number
        {
            printf("malformed expression\n");
            return 1;
        }
        // Check which menu option user chose
        switch(menu) 
        {
            // Cola option
            case 0: 
                if(money >= p_cola && num_cola > 0)
                {
                    num_cola--; 
                    money_made += p_cola;
                    printf("cola is dispensed and $%.2f returned\n", money - p_cola);
                }
                else if(num_cola <= 0)
                {
                    printf("invalid item\n");  // No more cola left
                    return 2;
                }
                else if(money < p_cola) // Not enough money case
                {
                    printf("not enough money\n");
                    return 3;
                }
                break;
            // Candybar option
            case 1:
                if(money >= p_candy && num_candy > 0)
                {
                    num_candy--; 
                    money_made += p_candy;
                    printf("candybar is dispensed and $%.2f returned\n", money - p_candy);
                }
                else if(num_candy <= 0) // No more candybars left
                {
                    printf("invalid item\n"); 
                    return 2;
                }
                else if(money < p_candy) // Not enough money case
                {
                    printf("not enough money\n");
                    return 3;
                }
                break;
            // Popcorn option
            case 2:
                if(money >= p_pop && num_pop > 0)
                {
                    num_pop--; 
                    money_made += p_pop;
                    printf("popcorn is dispensed and $%.2f returned\n", money - p_pop);
                }
                else if(num_pop <= 0) // No more popcorn left
                {
                    printf("invalid item\n"); 
                    return 2;
                }
                else if(money < p_pop) // Not enough money case
                {
                    printf("not enough money\n");
                    return 3;
                }
                break;
            default: // Not on the menu (user chose menu other than 0, 1, 2)
                printf("invalid item\n"); 
                return 2;
        }

        // Print how many items are left + money made
        printf("[0] %d cola left: cost is $%.2f\n", num_cola, p_cola);
        printf("[1] %d candybar left: cost is $%.2f\n", num_candy, p_candy);
        printf("[2] %d popcorn left: cost is $%.2f\n", num_pop, p_pop);
        printf("Money made so far is $%.2lf\n", money_made);
    }

    // Catch malformed expressions (e.g. missing input)
    if(menu == 5) // If menu still holds its initialized value, it means while loop did not execute properly
    {
        printf("malformed expression\n");
        return 1;
    }
    printf("Thanks for your patronage!\n");
    return 0;
}
