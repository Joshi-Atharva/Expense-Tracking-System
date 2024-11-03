#ifndef UTILS_H
#define UTILS_H

#define NAME_SIZE 100
#define NUM_USERS 1000
#define NUM_EXPENSES 1000
#define NUM_FAMILIES 100
#define EXPENSE_CATEGORIES 11


struct user {
    int user_id;
    char user_name[NAME_SIZE];
    float income; // doubt: think about the two decimal point constraint
};

struct family {
    int family_id;
    char family_name[NAME_SIZE];
    int members[4];
    float total_family_income;
    float monthly_family_expense;
};

struct expense {
    int expense_id;
    int user_id;
    char expense_category[EXPENSE_CATEGORIES];
    float expense_amount;
    char date_of_expense[11]; // in "dd/mm/yyyy\0" format
};

struct UserExp {
    int cat;
    float exp;
};

// declaration of record arrays
struct user Users[1000];
struct family Families[100];
struct expense Expenses[1000];

// create dummy struct variables:
struct user create_dummy_user();

// initialisation to default values:
void Initialise_users(struct user Users[NUM_USERS]);
void Initialise_expenses(struct expense Expenses[NUM_EXPENSES]);
void Initialise_families(struct family Families[NUM_FAMILIES]);
void ClearExpense(struct expense *Exp);
void copy_family(struct family *F1, struct family *F2);

// functionality 9:
int enumerate_exp_cat(char exp_name[11]);
void inverse_enumerate_exp_cat(char ret_str[11], int cat_num);
float Get_individual_expense(struct expense Expenses[NUM_EXPENSES], int UserID, int print); 

// functionality 3:
int FindFirstEmpty(struct family Families[NUM_FAMILIES]);
int FamilySearch(int FamilyID);
int max(int a, int b);
int UserExpenseSearch(struct expense Expenses[NUM_EXPENSES], int UserID);
int CreateFamily(struct family Families[], struct expense Expenses[], struct user NewUser, char FamilyName[NAME_SIZE], int FamilyID);


// functionality 1 begin:
void strcpy(char s1[], char s2[]);
int strcmp(char s1[], char s2[]);
int strlen( char s1[] );
void strcat(char s1[], char s2[]);
void copy_user(struct user *A, struct user *B);
int FindUserInFamilies(struct family Families[], int UserID);
int CalcNumUsers(struct user Users[NUM_USERS]);
int new_user_family_update(struct family Families[NUM_FAMILIES], struct expense Expenses[], struct user NewUser, int FamilyID, int family_rec);
int AddUser(int num_records, struct user NewUser, int FamilyID, char FamilyName[]);

// functionality 2:
void copy_expense(struct expense *E1, struct expense *E2);
int compare_expense(struct expense E1, struct expense E2);
int CalcNumExpenses();
int _AddExpense(struct expense Expenses[NUM_EXPENSES], int num_records, struct expense NewExpense);
int AddExpense(struct expense NewExpense);
   

// reading data
int string_to_int(char s[]);
float string_to_float(char s[]);
int read_user_data(struct user Users[], struct family Families[], struct expense Expenses[], int n, int user_dict[1000]);
int read_expense_data(struct expense Expenses[NUM_EXPENSES], int num_rec);

// display function
void print_Users(struct user Users[NUM_USERS], int n);
void print_Expenses(struct expense Expenses[NUM_EXPENSES], int n);
void print_Families(struct family Families[NUM_FAMILIES], int n);


// functionality 4:
int delete_user(int UserID);
int update_user(struct user NewUser);
int delete_family(int FamilyID);

// functionality 5
int Update_delete_expense(int ExpenseID, struct expense NewExpense, int updation);

// functionality 6
void Get_total_expense(int FamilyID);

// functionality 7:
float Get_categorial_expense(char category[11], int FamilyID);

// functionality 8:
int Get_highest_expense_day(int FamilyID);

#endif

