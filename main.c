/* 
    Author: Atharva Joshi
    BT23CSE020
*/
#include <stdio.h>
#include "utils.h"

#define SUCCESS 1
#define FAILURE 0
/*
    basic architecture:
    user: unique key: user id
    family: unique key: family id
    expense: unique key: expense id
*/

// defining arrays of structures first
extern struct user Users[1000];
extern struct family Families[100];
extern struct expense Expenses[1000];


int user_dict[1001];
int expense_dict[1000];

int main() {

    // defining user directory
    // maps (user_id - 1) -> record_number_inside_Users[]
    int i;
    for(i = 0; i<=1000; i++) {
        // user_dict[user_id - 1] = rec_num_in_Users[]
        user_dict[i] = -1; // default value meaning that user_id i is not yet added
        expense_dict[i] = -1;
    }

    // initialising 
    Initialise_users(Users);
    Initialise_expenses(Expenses);
    Initialise_families(Families);

    int expense_status_code = read_expense_data(Expenses, 40);
    int user_status_code = read_user_data(Users, Families, Expenses, 25, user_dict);

    print_Users(Users, 25);

    
    printf("\nExpense Record:\n");
    print_Expenses(Expenses, 40);

    printf("\nFamily Record:\n");
    print_Families(Families, -1);

    int exit = 0;
    int op; // operation code for menu driven program

    // variable declarations for different cases of the menu driven program that follows
    // 1:
    struct user NewUser;
    int family_yes;
    int FamilyID;
    char FamilyName[NAME_SIZE];
    int user_record_num;
    int done;


    // 2:
    struct expense NewExpense;
    int exp_rec;

    // 3:
    int nature_of_rec;

    // 4: 
    int updation;
    int up_del_flag;
    int ExpenseID;

    // 51:
    // int updation as in 4
    int UserID;
    int status_code;

    // 7:
    char category[11];

    // 8:
    // FamilyID as in 1

    // 9:
    float total;
    //101:
    int user_rec;

    while( !exit ) {
        printf("\nEnter the number corresponding to the desired operation:\n");
        printf("0. Exit\n1. Add User\n2. Add expense\n3. Print Records\n4. Update/Delete expense\n51: Update/Delete User\n52. Delete Family\n6. Get total expense of a family\n7. Get categorical expense\n8. Get the highest expense day of a family\n9. Get individual expense\n101. buffer\n");
        scanf("%d", &op);

        switch(op) {
            case 0:
                exit = 1;
                break;
            case 1:
                printf("Enter username: ");
                scanf("%s", NewUser.user_name);
                printf("Enter income: ");
                scanf("%f", &NewUser.income);

                i = 0;
                done = 0;
                while( (i < 1000) && (!done) ) {
                // finding a unique user_id
                    if( user_dict[i] == -1 ) {
                        NewUser.user_id = i + 1;
                        done = 1;
                    }
                    else {
                        i = i + 1;
                    }
                }

                if( !done ) {
                    printf("\nUsers Register is full\n");
                }
                else {
                    printf("Want to be added to existing family?(1/0) ");
                    scanf("%d", &family_yes);
                    if( family_yes ) {
                        printf("enter the family id: " );
                        scanf("%d", &FamilyID);
                    }
                    else {
                        FamilyID = 0;
                    }
                    FamilyName[0] = '\0';
                    
                    user_record_num = AddUser(-1, NewUser, FamilyID, FamilyName);
                    // user_dict[NewUser.user_id - 1] = user_record_num;

                    print_Users(Users, CalcNumUsers(Users));
                }

                break;
            case 2:
                i = 0;
                done = 0;
                while( (i<1000) && (!done) ) {
                    if( expense_dict[i] == -1 ) {
                        // means the expense_id i + 1 is free
                        NewExpense.expense_id = i + 1;
                        done = 1;
                    }
                    else {
                        i = i + 1;
                    }
                }
                // reading input from user
                printf("\nEnter the new Expense, field wise, as follows:");
                printf("\nUserID: ");
                scanf("%d", &NewExpense.user_id);
                printf("\nExpense Category: ");
                scanf("%s", NewExpense.expense_category);
                printf("\nExpense Amount: ");
                scanf("%f", &NewExpense.expense_amount);
                printf("\nExpense date: ");
                scanf("%s", NewExpense.date_of_expense);

                // function call:
                exp_rec = AddExpense(NewExpense);
                // updation of expense_dict[] takes place inside AddExpense()
                if( exp_rec != -1) {
                    printf("\nExpense Added at record no. %d", exp_rec);
                }
                else {
                    printf("\nCannot add expense");
                }
                break;
            
            case 3:
                printf("enter:\n1 for User records\n2 for Family records\n3 for Expense records\n");
                scanf("%d", &nature_of_rec);
                if( nature_of_rec == 1 ) {
                    print_Users(Users, -1);
                }
                else if ( nature_of_rec == 2 ) {
                    print_Families(Families, -1);

                }
                else if ( nature_of_rec == 3 ) {   
                    print_Expenses(Expenses, -1);                    
                }
                else {
                    printf("\nEnter a valid option no.\n");
                }

                ClearExpense(&NewExpense);
                break;

            case 4:
                printf("\nEnter whether updation(1) or deletion(0) operation is to be done: ");
                scanf("%d", &updation);
                if( updation == 1 ) {
                    printf("Enter expense details as follows:\n");


                    printf("\nExpenseID: ");
                    scanf("%d", &NewExpense.expense_id);
                    printf("\nUserID: ");
                    scanf("%d", &NewExpense.user_id);
                    printf("\nExpense Category: ");
                    scanf("%s", NewExpense.expense_category);
                    printf("\nExpense Amount: ");
                    scanf("%f", &NewExpense.expense_amount);
                    printf("\nExpense date: ");
                    scanf("%s", NewExpense.date_of_expense);

                    up_del_flag = Update_delete_expense(NewExpense.expense_id, NewExpense, updation);
                    if( up_del_flag ) {
                        printf("\nSuccessfully Updated\n");
                    }

                }
                else {
                    printf("\nEnter ExpenseID to be deleted: ");
                    scanf("%d", &ExpenseID);

                    ClearExpense(&NewExpense);
                    up_del_flag = Update_delete_expense(ExpenseID, NewExpense, updation);
                    if( up_del_flag ) {
                        printf("ExpenseID: %d successfully deleted\n", ExpenseID);
                    }

                }
                
                break;
            case 51:
                printf("\nUpdate(1) or delete(0): ");
                scanf("%d", &updation);
                printf("Enter the UserID: ");
                scanf("%d", &UserID);
                if( updation ) {
                    printf("\nenter the new details of UserID: %d", UserID);
                    printf("\nUsername: ");
                    scanf("%s", NewUser.user_name);
                    printf("\nIncome: ");
                    scanf("%f", &NewUser.income);

                    status_code = update_user(NewUser);
                    if( status_code ) {
                        printf("UserID %d successfully updated", NewUser.user_id);
                    }


                }
                else {
                    status_code = delete_user(UserID);
                    if( status_code ) {
                        printf("\nUserID: %d has been deleted successfully", UserID);
                    }
                    else {
                        printf("\nUser cannot be deleted");
                    }
                }
                break;

            case 52:
                printf("\nEnter FamilyID: ");
                scanf("%d", &FamilyID);

                status_code = delete_family(FamilyID);
                if( status_code ) {
                    printf("\nFamilyID %d deleted", FamilyID);
                }
                break;
            
            case 6:
                printf("\nenter the family id: ");
                scanf("%d", &FamilyID);
                Get_total_expense(FamilyID);
                break;
            case 7:
                printf("\nEnter FamilyID: ");
                scanf("%d", &FamilyID);
                printf("\nEnter the category name: ");
                scanf("%s", category);
                Get_categorial_expense(category, FamilyID);
                break;
            case 8:
                printf("\nenter the FamilyID: ");
                scanf("%d", &FamilyID);
                printf("scanned FamilyID: %d\n", FamilyID);

                Get_highest_expense_day(FamilyID);
                // printing is a part of function itself
                break;
            case 9:
                printf("enter userid: ");
                scanf("%d", &UserID);
                Get_individual_expense(Expenses, UserID, 1);
                
                

                break;

            case 101:
                user_rec = 0;
                int count = 0;
                while( user_rec < NUM_USERS ) {
                    if( user_rec == user_dict[Users[user_rec].user_id - 1] ) {
                        printf("\nUser %d:%s exists in user_dict", Users[user_rec].user_id, Users[user_rec].user_name );
                        count = count + 1;
                    }
                    
                    user_rec = user_rec + 1;
                }
                printf("\ncount = %d", count);
                break;
            default:
                printf("\nenter valid operation number");
        }
    }

    return 0;
}


