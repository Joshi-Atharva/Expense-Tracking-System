/* 
    Author: Atharva Joshi
    BT23CSE020
*/
#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 100
#define NUM_USERS 1000
#define NUM_EXPENSES 1000
#define NUM_FAMILIES 100
#define EXPENSE_CATEGORIES 5
#define SUCCESS 1
#define FAILURE 0

// structure declarations
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
    char expense_category[11];
    float expense_amount;
    char date_of_expense[11]; // in "dd/mm/yyyy\0" format
};

struct UserExp {
    int cat;
    float exp;
};

// extern keyword 'declares' variables accross files,
// instructing the compiler that definition of those variables exist somewhere in the compiled files
// without the extern keyword, variables are declared as well as defined
// we can have multiple declarations ( no memory is allocated ) but only one definition ( memory is allocated )
// refer the GFG article on extern keyword for better insights
extern struct user Users[1000];
extern struct family Families[100];
extern struct expense Expenses[1000];

extern int user_dict[1000];
extern int expense_dict[1000];

// create dummy struct variables:
struct user create_dummy_user() {
    struct user dummy;
    dummy.user_id = 0;
    dummy.user_name[0] = '\0';
    dummy.income = 0;

    return dummy;
}

// initialisation to default values:
void Initialise_users(struct user Users[NUM_USERS]) {
    int i = 0;

    while(i < NUM_USERS) {
        Users[i].user_id = 1001;
        Users[i].user_name[0] = '\0';
        Users[i].income = 0;

        i = i + 1;
    }
}
void Initialise_expenses(struct expense Expenses[NUM_EXPENSES]) {
    int i = 0;

    while(i < NUM_EXPENSES) {
        Expenses[i].expense_id = 1001;
        Expenses[i].user_id = 1001;
        Expenses[i].expense_category[0] = '\0';
        Expenses[i].expense_amount = 0;
        Expenses[i].date_of_expense[0] = '\0';
         
        i = i + 1;
    }
}
void Initialise_families(struct family Families[NUM_FAMILIES]) {
    int i = 0;
    int j = 0;
    while( i < NUM_FAMILIES ) {
        Families[i].family_id = 1001;
        Families[i].family_name[0] = '\0';
        j = 0;
        while( j < 4 ) {
            Families[i].members[j] = 0;
            j = j + 1;
        }
        Families[i].total_family_income = 0;
        Families[i].monthly_family_expense = 0;

        i = i + 1;
    }
}

void ClearExpense(struct expense *Exp) {
    (*Exp).expense_id = 1001;
    (*Exp).user_id = 1001;
    (*Exp).expense_category[0] = '\0';
    (*Exp).expense_amount = 0;
    (*Exp).date_of_expense[0] = '\0';
}
void copy_family(struct family *F1, struct family *F2) {
    (*F1).family_id = (*F2).family_id;
    strcpy((*F1).family_name, (*F2).family_name);

    int i = 0;
    while( i < 4 ) {
        (*F1).members[i] = (*F2).members[i];
        i = i + 1;
    }

    (*F1).total_family_income = (*F2).total_family_income;
    (*F1).monthly_family_expense = (*F2).monthly_family_expense;
}

// functionality 9:
// function for encoding category strings as integer values
int enumerate_exp_cat(char exp_name[11]) {
    int ret_val = -1;

    if( strcmp(exp_name, "Rent") == 0 ) ret_val = 0;
    else if( strcmp(exp_name, "Utility") == 0 ) ret_val = 1;
    else if( strcmp(exp_name, "Grocery") == 0 ) ret_val = 2;
    else if( strcmp(exp_name, "Stationary") == 0 ) ret_val = 3;
    else if( strcmp(exp_name, "Leisure") == 0 ) ret_val = 4;

    return ret_val;
}
// function for decoding category integers as category strings
void inverse_enumerate_exp_cat(char ret_str[11], int cat_num) {
    switch( cat_num ) {
        case 0:
            strcpy(ret_str, "Rent");
            break;
        case 1:
            strcpy(ret_str, "Utility");
            break;
        case 2:
            strcpy(ret_str, "Grocery");
            break;
        case 3:
            strcpy(ret_str, "Stationary");
            break;
        case 4:
            strcpy(ret_str, "Leisure");
            break;
        default:
            strcpy(ret_str, "\0");
    }

}
// returns total expense of the user, prints in sorted order user's expense if print = 1
float Get_individual_expense(struct expense Expenses[NUM_EXPENSES], int UserID, int print) {
    // binary search for even user_id and then linear traversal to get all entries for one user
    /*
    int exp_rec = 0;
    int found = 0;
    while( (exp_rec < NUM_EXPENSES) && !found) {
        if( Expenses[exp_rec].user_id == UserID ) {
            found = 1;
        }
        else {
            exp_rec = exp_rec + 1;
        }
    }
    */
    float total = 0;
    int exp_rec = UserExpenseSearch(Expenses, UserID);
    int found;
    if( exp_rec == -1 ) {
        found = 0;
    }
    else {
        found = 1;
    }
    struct UserExp user_expenses[5]; // each entry in the array represents each expense category
    struct UserExp temp;
    int i;
    i = 0;

    // zero intialisation
    i = 0;
    while( i < 5 ) {
        user_expenses[i].cat = i;
        user_expenses[i].exp = 0;
        i++;
    }

    int done;
    int j;
    
    // adding expenses as found
    // real time sorting will be more expensive hence sorting is performed after the entry loop
    // this is because no entry can be assumed to be complete unless the complete traversal is over
    if( found ) {
        done = 0;
        while( (exp_rec < NUM_EXPENSES) && !done ) {
            if( Expenses[exp_rec].user_id == UserID ) {
                j = enumerate_exp_cat(Expenses[exp_rec].expense_category);
                user_expenses[j].exp += Expenses[exp_rec].expense_amount;
                total += Expenses[exp_rec].expense_amount;
                exp_rec = exp_rec + 1;

            }
            else {
                done = 1;
            }
        }

        if( print ) {
            // putting min at position 0
            i = 1; 
            struct UserExp min;

            min.cat = user_expenses[0].cat;
            min.exp = user_expenses[0].exp;
            j = 0;

            while( i < 5 ) {
                if( min.exp > user_expenses[i].exp ) {
                    min.exp = user_expenses[i].exp;
                    min.cat = user_expenses[i].cat;
                    j = i;
                }
                i = i + 1;
            }
            // assigning minimum
            user_expenses[j].cat = user_expenses[0].cat;
            user_expenses[j].exp = user_expenses[0].exp;

            user_expenses[0].cat = min.cat;
            user_expenses[0].exp = min.exp;




            // insertion sort
            i = 2;
            // loop invariant: post iteration, first 'i' elements are sorted => index 0 -> i-1 sorted
            while( i < 5 ) {
                temp.cat = user_expenses[i].cat;
                temp.exp = user_expenses[i].exp;
                j = i - 1;
                while( user_expenses[j].exp > temp.exp ) {
                    
                    user_expenses[j+1].cat = user_expenses[j].cat;
                    user_expenses[j+1].exp = user_expenses[j].exp;
                    j = j - 1;
                    
                }
                user_expenses[j+1].cat = temp.cat;
                user_expenses[j+1].exp = temp.exp;
                i = i + 1;
            }

        
            i = 0;
            char category[11];
            while( i < 5 ) {
                inverse_enumerate_exp_cat(category, user_expenses[i].cat);
                printf("\n%s: %.2f\n", category, user_expenses[i].exp);
                i = i + 1;
            }
        }

    }
    if( print ) {
        printf("\ntotal = %.2f", total);
    }
    return total;
}





// functionality 3 begin:

// returns -1 if records are full (operation unsuccessful)
// else returns the family_id created
int FindFirstEmpty(struct family Families[NUM_FAMILIES]) {
    int ans;

    int i = 0;
    int found = 0;
    while( (i < NUM_FAMILIES) && (!found) ) {
        if( Families[i].family_id == 1001 ) {
            found = 1;
        }
        else {
            i = i + 1;
        }
    }

    if( found == 1 ) {
        ans = i;
    }
    else {
        ans = -1; // indicating no empty space
    }

    return ans;
}

// Searches the family_record index in Families[] array using FamilyID
int FamilySearch(int FamilyID) {
    // Searches the Families[] array for 'FamilyID'
    // assumes that family records are sorted according to family ID
    // performs binary search
    int index;

    int low = 0;
    int high = NUM_FAMILIES - 1;
    int found = 0;

    int mid;
    while( (low <= high) && (!found) ) {
        mid = (low + high)/2;
        
        if( Families[mid].family_id == FamilyID ) {
            found = 1;
        }
        else if( Families[mid].family_id < FamilyID ) {
            low = mid + 1;
        }
        else if( Families[mid].family_id > FamilyID ) {
            high = mid - 1;
        }
    }
    
    if( found == 1 ) {
        index = mid;
    }
    else {
        index = -1; // indicating not found
    }

    return index;
}

// define Get_individual_expense(UserID)
int max(int a, int b) {
    int ret_val;
    if( a >= b ) {
        ret_val = a;
    }
    else {
        ret_val = b;
    }
    return ret_val;
}

// finds the one of the occurences of UserID using binary search
// then FINALLY RETURNS THE FIRST OCCURENCE by running a loop backward
// required Expenses[] to be sorted according to user_id
int UserExpenseSearch(struct expense Expenses[NUM_EXPENSES], int UserID) {
  
    int index;

    int low, high, mid;
    low = 0;
    high = NUM_EXPENSES - 1;
    
    int found = 0;

    while( (low <= high) && (!found) ) {
        mid = (low + high)/2;

        if( Expenses[mid].user_id == UserID ) {
            found = 1;
        }
        else if( UserID < Expenses[mid].user_id ) {
            // UserID lies in Expenses[low, mid - 1]
            high = mid - 1;
        }
        else if( Expenses[mid].user_id < UserID ) {
            low = mid + 1;
        }
    }

    if( found == 1 ) {
        index = mid;
        int done = 0;
        while( (index >= 0) && !done ) {
            if( Expenses[index].user_id != UserID ) {
                done = 1;
            }
            else {
                index = index - 1;
            }
        } // loop invariant: after every iteration, Expense record index 'index + 1' refers to the desired UserID 
        index = index + 1;
    }
    else {
        index = -1;
    }
    
    return index;
}

// Creates New family:
// dependancies: Get_individual_expense(UserID)
int CreateFamily(struct family Families[], struct expense Expenses[], struct user NewUser, char FamilyName[NAME_SIZE], int FamilyID) {
    int status_code = 0;
    int empty_index = FindFirstEmpty(Families);
    int j;
    int prev_index;
    if( empty_index == -1 ) {
        printf("\nRecords full. Cannot add more records.\n");
        status_code = 0;
    }
    else {
        prev_index = max(empty_index - 1, 0);
        if( FamilyID == 0 ) {
            FamilyID = Families[prev_index].family_id + 1;
            // since Families[] is sorted over family_id, this operation will always ensure a unique family_id
        }
        else {
          
            j = empty_index - 1;
            int done = 0;
            while( j >= 0 && !done ) {
                if( FamilyID < Families[j].family_id ) {
                    Families[j+1] = Families[j];
                    j = j - 1;
                }
                else {
                    done = 1;
                }
            }
            empty_index = j + 1;
        }

        Families[empty_index].family_id = FamilyID;
        strcpy(Families[empty_index].family_name, FamilyName);
        Families[empty_index].members[0] = NewUser.user_id;
        for( j = 1; j < 4; j++ ) {
            Families[empty_index].members[j] = 0;
        }
        Families[empty_index].total_family_income = NewUser.income;
        Families[empty_index].monthly_family_expense = Get_individual_expense(Expenses, NewUser.user_id, 0);
        status_code = Families[empty_index].family_id;
    }
    return status_code;
}
// functionality 3 end







// functionality 1 begin:
void strcpy(char s1[], char s2[]) {
    int i = 0;
    int done = 0;
    while( i < NAME_SIZE && !done) {
        s1[i] = s2[i];
        if( s2[i] == '\0' ) {
            done = 1;
        }
        i = i + 1;
    }
}

int strcmp(char s1[], char s2[]) {
   int i = 0;

   while( (s1[i] == s2[i]) && (s1[i] != '\0') ) {
        i = i + 1;
   }

   return (s1[i] - s2[i]);
}

int strlen( char s1[] ) {
    int i = 0;
    int len = 0;
    while( s1[i] != '\0' ) {
        len = len + 1;
        i = i + 1;
    }
    return len;
}

void strcat(char s1[], char s2[]) {
    int i = 0;
    int len1 = strlen(s1);

    while( s2[i] != '\0' ) {
        s1[i + len1] = s2[i];
        i = i + 1;
    }
}
      
void copy_user(struct user *A, struct user *B) {
    (*A).user_id = (*B).user_id;
    strcpy((*A).user_name, (*B).user_name);
    (*A).income = (*B).income;
}

// Adding a new user
// Required: Update_or_delete_individual_family_details() with appropriate arguments
// also add the functionality to check if an individual trying to join a family is not already a part of
// any other family or the current family
/*
    required FamilyID such that: 
    FamilyID = 0 if user is not part of an existing family, 
    and = required family id if wants to be part of an existing family

    and FamilyName = Name given by user if new family to be created
    and = don't care if existing family ( FamilyID != 0 ) ( to keep things consistent,
    we define FamilyName[0] = '\0' if FamilyID != 0
*/
// we can also define a an array user_family_dict st:
// user_family_dict[user_id] = users's family id
int FindUserInFamilies(struct family Families[], int UserID) {
    int i = 0;
    int j;
    int found = 0;
    while( (i < NUM_FAMILIES) && (!found) ) {
        j = 0;
        while( (j < 4) && !found) {
            if( Families[i].members[j] == UserID ) {
                found = 1;
            }
            else {
                j = j + 1;
            }
        }
        i = i + 1;
    }// loop invariant: after k'th iteration, i = k; if !found then first k-1 families do not contain UserID
    // if found then k-1'th family contains UserID

    i = i - 1;
    // now, i'th family contains UserID if found else UserID does not belong to a family

    int ans;
    // just an observation to note here, in case in future you 
    // want to return a tuple in R^n:
    // A bijection f: R^2 -> R is f(i, j) = (2^i)*(3^j)  [2, 3 or any two primes ]
    // i and j are uniquely encoded and can be retrieved from this return value outside the function
    if( found == 0 ) {
        ans = -1;
    }
    else {
        ans = i;
    }
    
    return ans;
}

            
int CalcNumUsers(struct user Users[NUM_USERS]) {
    int i = 0;
    int done = 0;
    while( (i<NUM_USERS) && !done) {
        if( Users[i].user_id == 1001 ) { // default value
            done = 1;
        }
        else {
            i = i + 1;
        }
    }

    return i;
}

// define Get_individual_expense(UserID)
int new_user_family_update(struct family Families[NUM_FAMILIES], struct expense Expenses[], struct user NewUser, int FamilyID, int family_rec) {
    int i = 0;
    int status_code = 0;
    while( (i < 4) && (!status_code)) {
        if( Families[family_rec].members[i] == 0 ) {
            Families[family_rec].members[i] = NewUser.user_id;
            Families[family_rec].total_family_income += NewUser.income;
            Families[family_rec].monthly_family_expense += Get_individual_expense(Expenses, NewUser.user_id, 0);
            status_code = 1;
        }
        else {
            i = i + 1;
        }
    }
    if( status_code == 0 ) {
        printf("\nFamily Full, cannot insert more users\n");
    }
    return status_code;
}

// ensure that the user does not already exist
// maintain a user dictionary such that: user_dict[UserID] = i such that Users[i].user_id = UserID
int AddUser(int num_records, struct user NewUser, int FamilyID, char FamilyName[]) {

    int status_code;
    if( num_records == -1 ) {
        // meaning if we don't know beforehand how many records are there
        num_records = CalcNumUsers(Users);
    }

    int record_number = -1;

    if( num_records == NUM_USERS ) {
        printf("\nRecords full");
        status_code = 0;
    }
    else if( user_dict[NewUser.user_id - 1] != -1 ) {
        printf("\nUser already in records");
        status_code = 0;
    }
    else {  
        // shifting all elements on a higher level than the current record to find it's correct position
        int j = num_records - 1; // index of last entry
        // since it is sorted according to UserId, j represents the same order
        int done = 0;
        while( j>=0 && !done) {

            if( Users[j].user_id > NewUser.user_id ) {
                
                // Copies all fields from Users[j] to Users[j+1]
                copy_user(&Users[j+1], &Users[j]);
                user_dict[Users[j].user_id - 1] = j + 1;
                j = j - 1;
            }
            else {
                done = 1;
            }
           
        }// loop invariant: after k'th iteration, j = num_records - 1 - k and 
        // the all records form j+1 to num_users-1 in original array are 
        // higher in order than NewUser and are shifted one place forward
        // making space for NewUser at j+1'th index
         
        // after the loop ends, j+1 is the correct position of NewUser
        copy_user(&Users[j+1], &NewUser);
        record_number = j+1;
        user_dict[NewUser.user_id - 1] = record_number;

        // Adding user to Family:
        int family_rec = FamilySearch(FamilyID);

        if( FamilyID == 0 ) { // user doesn't want to belong to existing family
            strcpy(FamilyName, NewUser.user_name);
            strcat(FamilyName, "'s family");
            CreateFamily(Families, Expenses, NewUser, FamilyName, FamilyID);
        }
        //(else): FamilyID != 0 means user wants to belong to a family
        else if( family_rec == -1 ) { 
            // FamilyID doesn't exist in record -> create one
            printf("\n%d doesn't exist, creating a new family with given ID", FamilyID);
            strcpy(FamilyName, NewUser.user_name);
            strcat(FamilyName, "'s family");
            CreateFamily(Families, Expenses, NewUser, FamilyName, FamilyID);
        }
        else { // if user wants to belong to an existing family, update the family record
            int update_sc = new_user_family_update(Families, Expenses, NewUser, FamilyID, family_rec);
            // if family is full, create a new family for the user:
            if( update_sc == 0 ) {
                strcpy(FamilyName, NewUser.user_name);
                strcat(FamilyName, "'s family");
                FamilyID = CreateFamily(Families, Expenses, NewUser, FamilyName, 0);
                printf("\nA new family is created with family_id: %d\n", FamilyID);
            }
        }
    }
    return record_number;
}
// functionality 1 end









// functionality 2 begin:

// compares expenses E1 and E2. outputs:
// 1 if E1 > E2, 0 if E1 = E2, -1 if E1 < E2
void copy_expense(struct expense *E1, struct expense *E2) {
    
    (*E1).expense_id = (*E2).expense_id;
    (*E1).user_id = (*E2).user_id;
    strcpy((*E1).expense_category, (*E2).expense_category);
    (*E1).expense_amount = (*E2).expense_amount;
    strcpy((*E1).date_of_expense, (*E2).date_of_expense);
}
int compare_expense(struct expense E1, struct expense E2) {
    // default value 0 (in a partial ordered relation this stands for incomparable)
    int ans = 0;
    if( E1.user_id > E2.user_id ) {
        ans = 1;
    }
    else if( E1.user_id < E2.user_id ) {
        ans = -1;
    }
    else if( E1.user_id == E2.user_id ) {
        if( E1.expense_id > E2.expense_id ) {
            ans = 1;
        }
        else if ( E1.expense_id < E2.expense_id ) {
            ans = -1;
        }
    }
    return ans;
}

// REQUIRED: add functionality to update family records accordingly 
// by defining the Update_delete_expense() function and using it
// in this function    
int CalcNumExpenses() {
    int i = 0;
    int done = 0;

    while( (i < NUM_EXPENSES) && (!done) ) {
        if( Expenses[i].expense_id == 1001 ) {
            done = 1;
        }
        else {
            i = i + 1;
        }
    }

    return i;
}

// function specially costructed for read_expense_data() function
int _AddExpense(struct expense Expenses[NUM_EXPENSES], int num_records, struct expense NewExpense) {
    int status_code = 0;
    int j;

    if( num_records == NUM_EXPENSES ) {
        status_code = 0;
    }
    else {
        // assumning first 'num_records' expenses are sorted, perform appropriate insertion operation
        j = num_records - 1; // last element
        int done = 0;
        while(j>=0 && !done ) {
            if( compare_expense(Expenses[j], NewExpense) == 1 ) {
                copy_expense(&Expenses[j+1], &Expenses[j]);
                expense_dict[Expenses[j].expense_id - 1] = j+1;
                j = j - 1;
            }
            else {
                done = 1;
            }
        }

        copy_expense(&Expenses[j+1], &NewExpense);
        expense_dict[NewExpense.expense_id - 1] = j + 1;

        status_code = 1;
    }
    
    return status_code;
}

// function for post_initialisation operations
int AddExpense(struct expense NewExpense) {
    int num_records = CalcNumExpenses();
    int j = num_records - 1;
    int insertion_index; 
    if( num_records == NUM_EXPENSES ) {
        insertion_index = -1;
    }
    else {
        // insertion into Expenses[] array
        int done = 0;
        while(j>=0 && !done ) {
            if( compare_expense(Expenses[j], NewExpense) == 1 ) {
                copy_expense(&Expenses[j+1], &Expenses[j]);
                expense_dict[Expenses[j].expense_id - 1] = j+1;
                j = j - 1;
            }
            else {
                done = 1;
            }
        }

        copy_expense(&Expenses[j+1], &NewExpense);
        insertion_index = j + 1;

        // updation of Families[]
        int family_rec = FindUserInFamilies(Families, NewExpense.expense_id);

        Families[family_rec].monthly_family_expense += NewExpense.expense_amount;

        expense_dict[NewExpense.expense_id - 1] = insertion_index;
    }
    return insertion_index;
}

// functionality 2 end




   


// reading data
int string_to_int(char s[]) {
    int i = 0;
    int ans = 0;
    while (s[i] != '\0') {
        ans = 10*ans + (int)(s[i] - 48);
        i = i + 1;
    }
    return ans;
}
float string_to_float(char s[]) {
    // pithole: saves with some extra digits after decimal point following the least significant digits
    int i = 0;
    float whole = 0;
    float fraction = 0;
    float p_10 = 10;
    int decimal_point_flag = 0;
    if( s[0] == '.' ) {
        decimal_point_flag = 1;
        i = i + 1;
    }
    while(s[i] != '\0') {
        if( decimal_point_flag == 1 ) {
            fraction = fraction + (float)(s[i] - 48)/p_10;
            p_10 = p_10*10;
        }
        else if( s[i] == '.' ) {
            decimal_point_flag = 1;
        }
        else {
            whole = 10*whole + (float)(s[i] - 48);
        }
        i = i + 1;
    }
    return (whole+fraction);
}

/*
    dependancies: 
        string_to_int
        string_to_float
        AddUser
*/
int read_user_data(struct user Users[], struct family Families[], struct expense Expenses[], int n, int user_dict[1000]) {
    int status_code;
    Initialise_users(Users);
    FILE *file_ptr = fopen("user_data.txt", "r");
    if( file_ptr == NULL ) {
        printf("\nError: file cannot be opened");
        status_code = 0;
    }
    else {
        int rec_num = 0;
        struct user NewUser;
        char UserId[10], UserName[100], Income[50], FamilyIDstr[10], FamilyName[100];
        int FamilyID;
        while( rec_num < n ) {
            fscanf(file_ptr, "%s %s %s %s", UserId, UserName, Income, FamilyIDstr);

            FamilyName[0] = '\0'; // family name will be decided in AddUser() function
            NewUser.user_id = string_to_int(UserId);
            strcpy(NewUser.user_name, UserName);
            NewUser.income = string_to_float(Income);
            FamilyID = string_to_int(FamilyIDstr);

            
            if( user_dict[NewUser.user_id] == -1 ) {
                int position = AddUser(rec_num, NewUser, FamilyID, FamilyName);
                user_dict[NewUser.user_id] = position;
            }
            else {
                printf("\nUser already exists\n");
            }

            rec_num = rec_num + 1;
        }
        status_code = 1;
    }
    
    return status_code;
}
int read_expense_data(struct expense Expenses[NUM_EXPENSES], int num_rec) {
    int status_code;
    Initialise_expenses(Expenses);
    FILE *file_ptr = fopen("expense_data.txt", "r");
    if( file_ptr == NULL ) {
        printf("\nError: file cannot be opened");
        status_code = 0;
    }
    else {
        int curr_rec = 0;
        struct expense NewExpense;
        char ExpenseID[5], UserID[5], ExpenseCategory[25], ExpenseAmount[25], DateOfExpense[20];

        while( curr_rec < num_rec ) {
            fscanf(file_ptr, "%s %s %s %s %s", ExpenseID, UserID, ExpenseCategory, ExpenseAmount, DateOfExpense);

            NewExpense.expense_id = string_to_int(ExpenseID);
            NewExpense.user_id = string_to_int(UserID);
            strcpy(NewExpense.expense_category, ExpenseCategory);
            NewExpense.expense_amount = string_to_float(ExpenseAmount);
            strcpy(NewExpense.date_of_expense, DateOfExpense);

            // copy_expense(&Expenses[curr_rec], &NewExpense);
            _AddExpense(Expenses, curr_rec, NewExpense);

            curr_rec = curr_rec + 1;
        }
        status_code = 1;
    }
    return status_code;
}

// display function
void print_Users(struct user Users[NUM_USERS], int n) {  
    int i = 0;
    if( n == -1 ) {
        n = CalcNumUsers(Users);
    }
    while( i < n ) {
        
        printf("\n\n---Record number %d---\nUser id: %d\nUsername: %s\nIncome: %f", i, Users[i].user_id, Users[i].user_name, Users[i].income); 
        i = i + 1;
    }
}
void print_Expenses(struct expense Expenses[NUM_EXPENSES], int n) {  
    int i = 0;
    if( n == -1 ) {
        n = CalcNumExpenses();
    }

    // n = number of records present
    while( i < n ) {
        
        printf("\n\n---Record number %d---\nExpenseID: %d\nUserID: %d\nExpense Category: %s\nExpense Amount: %.2f\nDate of expense: %s\n", i, Expenses[i].expense_id, Expenses[i].user_id, Expenses[i].expense_category, Expenses[i].expense_amount, Expenses[i].date_of_expense); 
        i = i + 1;
    }
}
void print_Families(struct family Families[NUM_FAMILIES], int n) {
    
    if( n == - 1) {
        // when we don't know how many records are filled
        n = FindFirstEmpty(Families);
    }
    
    int i = 0;
    int j = 0;

    while( i < n ) {
        printf("\n---Record no. %d---\nFamilyID: %d\nFamilyName: %s\nIncome: %.2f\nExpense: %.2f", i, Families[i].family_id, Families[i].family_name, Families[i].total_family_income, Families[i].monthly_family_expense);
        j = 0;
        while( j < 4 ) {
            printf("\nmember-%d ID: %d", j+1, Families[i].members[j]);
            j = j + 1;
        }
        i = i + 1;
        printf("\n");
    }
}


// functionality 4:
int delete_user(int UserID) {
    int status_code = 1;
    int user_rec = user_dict[UserID - 1];
    if( user_rec == -1 ) {
        printf("\nUser does not exist\n");
        status_code = 0;
    }
    else {
        int fam_rec = FindUserInFamilies(Families, UserID);

        int i = 0;
        int found = 0;
        while( i < 4 ) {
            if( Families[fam_rec].members[i] == UserID ) {
                found = 1;
            }
            else if( found == 1) {
                Families[fam_rec].members[i-1] = Families[fam_rec].members[i];
            }
            i = i + 1;
        } // invariant: after k iterations, i = k and first k members are operated on 
        // once found, the elements that follow are shifted to the left by one position

        if( found ) {
            Families[fam_rec].members[3] = 0; // initialising the last value to 0
        }

        Families[fam_rec].total_family_income -= Users[user_rec].income;
        Families[fam_rec].monthly_family_expense -= Get_individual_expense(Expenses, UserID, 0);

        if( Families[fam_rec].members[0] == 0 ) {
            // delete_family(int Families[fam_rec].family_id);
            // shift the family records
            fam_rec = fam_rec + 1;
            int done = 0;
            while( fam_rec < NUM_FAMILIES && !done) {
                copy_family(&Families[fam_rec - 1], &Families[fam_rec]);
                if( Families[fam_rec].family_id == 1001 ) {
                    done = 1;
                }
                fam_rec = fam_rec + 1;
            }
            
            if( !done ) {
                // intitialise last record
                Families[NUM_FAMILIES - 1].family_id = 1001;
                Families[NUM_FAMILIES - 1].family_name[0] = '\0';
                i = 0;
                while( i < 4 ) {
                    Families[NUM_FAMILIES - 1].members[i] = 0;
                    i = i + 1;
                }
                Families[NUM_FAMILIES - 1].total_family_income = 0;
                Families[NUM_FAMILIES - 1].monthly_family_expense = 0;
            }


        }

        // shifting the Expense records:
        int start = UserExpenseSearch(Expenses, UserID);
        int end = start;
        int done = 0;

        while( end < NUM_EXPENSES && !done ) {
            if( Expenses[end].user_id != UserID ){
                done = 1;
            }
            else {
                end = end + 1;
            }
        }
    

        while( end < NUM_EXPENSES ) {
            copy_expense(&Expenses[start], &Expenses[end]);
            expense_dict[Expenses[end].expense_id - 1] = start;
            start = start + 1;
            end = end + 1;
        }

        while( start < NUM_EXPENSES ) {
            ClearExpense(&Expenses[start]);
            // no need of modification over here, all expenses after the deleted expense are already considered in previous loop
            start = start + 1;
        }

        
        // shifting the user records:

        user_rec = user_rec + 1;

        while( user_rec < NUM_USERS ) {
            copy_user(&Users[user_rec - 1], &Users[user_rec]);
            user_dict[Users[user_rec].user_id - 1] = user_rec - 1;
            user_rec = user_rec + 1;
        }

        // re-initialise the last entry
        Users[NUM_USERS-1].user_id = 1001;
        Users[NUM_USERS -1].user_name[0] = '\0';
        Users[NUM_USERS -1].income = 0;

        // updating user_dict
        user_dict[UserID - 1] = -1;

    }

    return status_code;
}

int update_user(struct user NewUser) {
    int status_code = 1;
    int user_rec = user_dict[NewUser.user_id - 1];
    if( user_rec == -1 ) {
        printf("\nUser does not exist\n");
        status_code = 0;


    }
    else {

        // Update family
        int family_rec = FindUserInFamilies(Families, NewUser.user_id);
        Families[family_rec].total_family_income += NewUser.income - Users[user_rec].income;

        // update Users[]
        copy_user(&Users[user_rec], &NewUser);
    }
    
    return status_code;
}

int delete_family(int FamilyID) {
    int family_rec = FamilySearch(FamilyID);
    int status_code = 1;
    if( family_rec == -1 ) {
        status_code = 0;
        printf("\nFamilyID %d does not exist", FamilyID);
    }
    else {
        int i = 3;
        while( i >= 0 ) {
            delete_user(Families[family_rec].members[i]);
            i = i - 1;
        }
    }

    return status_code;
}

// functionality 5 begin:
int Update_delete_expense(int ExpenseID, struct expense NewExpense, int updation) {
    int status_code;

    // for family updation:
    int UserID;
    int family_rec;

    
    int exp_rec = expense_dict[ExpenseID - 1];
    if( exp_rec == -1 ) {
        printf("\nGiven expense ID does not exist. Please use the AddExpense() function to add a new expense\n");
        status_code = 0;
    }
    else {
        // deleting old expense from family:
        UserID = Expenses[exp_rec].user_id;
        family_rec = FindUserInFamilies(Families, UserID);
        Families[family_rec].monthly_family_expense -= Expenses[exp_rec].expense_amount;


        if( updation == 1) {
            copy_expense(&Expenses[exp_rec], &NewExpense);
            status_code = 1;
            // updating family with new expense:
            Families[family_rec].monthly_family_expense += NewExpense.expense_amount;
        }
        else {

            // shifting later expenses
            int done = 0;
            int j = exp_rec + 1;

            expense_dict[Expenses[exp_rec].expense_id - 1] = -1;

            while( (j < NUM_EXPENSES) && !done ) {
                if( Expenses[j].expense_id == 1001 ) {
                    done = 1;
                }
                else {
                    copy_expense(&Expenses[j-1], &Expenses[j]);
                    expense_dict[Expenses[j].expense_id - 1] = j-1;
                    j = j + 1;
                }

            }
            if( !done ) {
                // will happen if array was full
                // this deletes the last expense remaining after shifting
                ClearExpense(&Expenses[NUM_EXPENSES-1]);
                // no need to update expense_dict here
                
            }

            status_code = 1;
        }

    }
    return status_code;
}
// functionality 5 end



// functionality 6
void Get_total_expense(int FamilyID) {
    int fam_rec = FamilySearch(FamilyID);
    printf("\nfamily expense: %.2f", Families[fam_rec].monthly_family_expense);
    float diff = Families[fam_rec].total_family_income - Families[fam_rec].monthly_family_expense;

    if( diff < 0 ) {
        printf("\nExpense surpases Income by %.2f", -diff);
    }
    else {
        printf("\nExpense within limits by %.2f", diff);
    }
}

// functionality 7:
// to be rectified -> ( sorting part ) 
float Get_categorial_expense(char category[11], int FamilyID) {
    float user_contri[4];
    int position[4];
    float total = 0;
    int cat = enumerate_exp_cat(category);
    int family_rec = FamilySearch(FamilyID);
    int exp_rec;
    int i = 0, j, UserID, temp, done;
    
    while( i < 4 ) {
        position[i] = i;
        user_contri[i] = 0;
        i = i + 1;
    }
    i = 0;
    
    while( i < 4 ) {
        UserID = Families[family_rec].members[i];
        exp_rec = UserExpenseSearch(Expenses, UserID);
        // first occurrence
        while( Expenses[exp_rec].user_id == UserID ) {
            if( enumerate_exp_cat(Expenses[exp_rec].expense_category) == cat ) {
                user_contri[i] += Expenses[exp_rec].expense_amount;
                total += Expenses[exp_rec].expense_amount;
            }
            exp_rec++;
        }

        // position[x] = UserNo. of user at position x as in a sorted sequence
        // loop invariant: after above block, i'th index is assigned
        // each of the user in user_contri[0 -> i-1] have correct position in position[0 -> i-1] array (permuted bijection exists for identity mapping)
        // and position[i] = i
        j = i-1;
        done = 0;
        // at this point position[i] = i

        while( j >= 0 && !done ) {
            if( user_contri[position[j]] > user_contri[i] ) {
                // if the contribution of user are position j is higher than the contri of newly added user i, then i'th user and j'th user switch positions in position array
                position[j+1] = position[j];
                j = j - 1;
            }
            else {  
                done = 1;
            }
            position[j+1] = i;
        }
        i = i + 1;
    }
    printf("\nTotal expense for category %s is %.2f", category, total);
    printf("\nindividual contributions are:\n");
    for(i = 0; i< 4; i++ ) {
        printf("User %d spent %f\n", position[i]+1, user_contri[position[i]]);
    }

    return total;
}

// functionality 8:
/* dependancies: UserExpenseSearch()
*/
// ensure: dates are of one month and only first 10 days of the month
// returns dd as integer
int Get_highest_expense_day(int FamilyID) {
    printf("\nentered inside the function");
    int i;
    float day_wise_expense[10];

    char day_str[3];

    int day;
    int UserID, exp_rec, family_rec;
    int done;
    int max_index;
    float max_exp;


    i = 0; 
    while( i < 10 ) {
        day_wise_expense[i] = 0;
        i = i + 1;
    }

    i = 0;
    while( i < 4 ) {
        family_rec = FamilySearch(FamilyID);
        UserID = Families[family_rec].members[i];
        exp_rec = UserExpenseSearch(Expenses, UserID);

        if( exp_rec == -1 ) {
            done = 1;
        }
        else {
            done = 0;
        }

        while( (exp_rec < NUM_EXPENSES) && (!done) ) {
            if( Expenses[exp_rec].user_id == UserID ) {
                day_str[0] = Expenses[exp_rec].date_of_expense[0];
                day_str[1] = Expenses[exp_rec].date_of_expense[1];

                day = string_to_int(day_str);
                day_wise_expense[day - 1] += Expenses[exp_rec].expense_amount;
                exp_rec = exp_rec + 1;
            }
            else {
                done = 1;
            }
        }
        i = i + 1;
    }// day_wise_expense[] contains aggregate of all 4 users

    max_exp = day_wise_expense[0];
    max_index = 0;
    i = 1;
    while( i < 10 ) {
        if( max_exp < day_wise_expense[i] ) {
            max_exp = day_wise_expense[i];
            max_index = i;
        }
        i = i + 1;
    }
    day = max_index + 1;

    printf("\nhighest expense of FamilyID: %d was %f on day %d\n", FamilyID, max_exp, day);

    return max_index;
}
























