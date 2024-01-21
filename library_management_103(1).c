#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ANSI escape codes for text colors
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"

// File path for admin credentials
#define ADMIN_CREDENTIALS_FILE "admin_credentials.txt"

// Structure to represent a book
struct Book {
    char title[50];
    char author[50];
    char genre[20];
    int book_id;
};

void initializeLibrary(struct Book **library, int *numBooks);
void addBook(struct Book **library, int *numBooks);
void displayBooks(struct Book *library, int numBooks);
void searchBooks(struct Book *library, int numBooks);
void deleteBook(struct Book **library, int *numBooks);
void saveLibrary(struct Book *library, int numBooks);
void loadLibrary(struct Book **library, int *numBooks);
void freeLibrary(struct Book **library);
void loadingAnimation();
void recommendBook(struct Book *library, int numBooks);
void sortLibrary(struct Book *library, int numBooks);
int compareTitles(const void *a, const void *b);
int compareAuthors(const void *a, const void *b);
int compareGenres(const void *a, const void *b);
void displayStatistics(struct Book *library, int numBooks);

void clearScreen();

#define MAX_BOOKS 1000
int adminLogin();
int main() {
    struct Book *library = NULL;
    int numBooks = 0;
    int choice;

    // Admin login
    if (!adminLogin()) {
        printf(ANSI_COLOR_RED "Admin login failed. Exiting the program.\n" ANSI_COLOR_RESET);
        freeLibrary(&library); // Free dynamically allocated memory
        return 1;
    }
        initializeLibrary(&library, &numBooks); // Initialize library

    do {
        clearScreen(); // Clear the console screen
        printf(ANSI_COLOR_CYAN "\nLibrary Management System\n" ANSI_COLOR_RESET);
        printf("1. " ANSI_COLOR_YELLOW "Add Book\n" ANSI_COLOR_RESET);
        printf("2. " ANSI_COLOR_YELLOW "Display Books\n" ANSI_COLOR_RESET);
        printf("3. " ANSI_COLOR_YELLOW "Search for a Book\n" ANSI_COLOR_RESET);
        printf("4. " ANSI_COLOR_YELLOW "Delete a Book\n" ANSI_COLOR_RESET);
        printf("5. " ANSI_COLOR_YELLOW "Save Library\n" ANSI_COLOR_RESET);
        printf("6. " ANSI_COLOR_YELLOW "Recommend a Book\n" ANSI_COLOR_RESET);
        printf("7. " ANSI_COLOR_YELLOW "Sort Library\n" ANSI_COLOR_RESET);
        printf("8. " ANSI_COLOR_YELLOW "Display Statistics\n" ANSI_COLOR_RESET);
        printf("9. " ANSI_COLOR_RED "Exit\n" ANSI_COLOR_RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(&library, &numBooks);
                break;
            case 2:
                displayBooks(library, numBooks);
                break;
            case 3:
                searchBooks(library, numBooks);
                break;
            case 4:
                deleteBook(&library, &numBooks);
                break;
            case 5:
                saveLibrary(library, numBooks);
                break;
            case 6:
                recommendBook(library, numBooks);
                break;
            case 7:
                sortLibrary(library, numBooks);
                break;
            case 8:
                displayStatistics(library, numBooks);
                break;
            case 9:
                printf(ANSI_COLOR_RED "Exiting the program. Goodbye!\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_RED "Invalid choice. Please enter a valid option.\n" ANSI_COLOR_RESET);
        }

        if (choice != 9) {
            loadingAnimation();
        }

        printf("Press enter to continue...");
        while (getchar() != '\n'); // Clear input buffer
        getchar(); // Wait for Enter key press

    } while (choice != 9);

    freeLibrary(&library); // Free dynamically allocated memory

    return 0;
}

int adminLogin() {

    FILE *credentialsFile = fopen(ADMIN_CREDENTIALS_FILE, "r");

    if (credentialsFile == NULL) {
        printf(ANSI_COLOR_RED "Error: Unable to open admin credentials file.\n" ANSI_COLOR_RESET);
        return 0;
    }

    char username[20];
    char password[20];

    // Read username
    if (fgets(username, sizeof(username), credentialsFile) == NULL) {
        printf(ANSI_COLOR_RED "Error: Unable to read admin username.\n" ANSI_COLOR_RESET);
        fclose(credentialsFile);
        return 0;
    }

    // Remove newline character from username
    username[strcspn(username, "\n")] = '\0';

    // Read password
    if (fgets(password, sizeof(password), credentialsFile) == NULL) {
        printf(ANSI_COLOR_RED "Error: Unable to read admin password.\n" ANSI_COLOR_RESET);
        fclose(credentialsFile);
        return 0;
    }

    // Remove newline character from password
    password[strcspn(password, "\n")] = '\0';

    fclose(credentialsFile);

    // Prompt for credentials
    char inputUsername[20];
    char inputPassword[20];

    printf("Admin Login\n");
    printf("Enter username: ");
    gets(inputUsername);

    printf("Enter password: ");
    gets(inputPassword);

    // Check credentials
    if (strcmp(inputUsername, username) == 0 && strcmp(inputPassword, password) == 0) {
        printf(ANSI_COLOR_GREEN "Admin login successful. Welcome!\n" ANSI_COLOR_RESET);
        return 1;
    } else {
        printf(ANSI_COLOR_RED "Incorrect username or password.\n" ANSI_COLOR_RESET);
        return 0;
    }
}

void clearScreen() {
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen
}

void initializeLibrary(struct Book **library, int *numBooks) {
    // Allocate memory for the library
    *library = (struct Book *)malloc(sizeof(struct Book) * MAX_BOOKS);
    if (*library == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Load existing library data from the file
    loadLibrary(library, numBooks);
}

void addBook(struct Book **library, int *numBooks) {
    if (*numBooks % MAX_BOOKS == 0) {
        *library = (struct Book *)realloc(*library, sizeof(struct Book) * (*numBooks + MAX_BOOKS));
        if (*library == NULL) {
            printf("Error: Memory reallocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter book title: ");
    char buffer[100];
    gets(buffer);
    gets((*library)[*numBooks].title);

    printf("Enter author name: ");
    gets((*library)[*numBooks].author);

    printf("Enter book genre: ");
    gets((*library)[*numBooks].genre);

    (*library)[*numBooks].book_id = *numBooks + 1;

    (*numBooks)++;

    saveLibrary(*library, *numBooks);

    printf("Book added successfully!\n");
}

void displayBooks(struct Book *library, int numBooks) {
    loadLibrary(&library, &numBooks);

    // Display books as before
    if (numBooks == 0) {
        printf("No books in the library.\n");
    } else {
        printf("Book List:\n");
        printf("%-5s %-30s %-30s %-20s\n", "ID", "Title", "Author", "Genre");

        for (int i = 0; i < numBooks; i++) {
            printf("%-5d %-30s %-30s %-20s\n", library[i].book_id, library[i].title, library[i].author, library[i].genre);
        }
    }
}

void searchBooks(struct Book *library, int numBooks) {
    char query[50];
    printf("Enter the title or author to search: ");
    scanf("%s", query);

    printf("Search Results:\n");
    printf("%-5s %-30s %-30s %-20s\n", "ID", "Title", "Author", "Genre");

    int resultsFound = 0;

    loadLibrary(&library, &numBooks);

    for (int i = 0; i < numBooks; i++) {
        if (strstr(library[i].title, query) != NULL || strstr(library[i].author, query) != NULL) {
            printf("%-5d %-30s %-30s %-20s\n", library[i].book_id, library[i].title, library[i].author, library[i].genre);
            resultsFound = 1;
        }
    }

    if (!resultsFound) {
        printf("No matching books found.\n");
    }
}

void deleteBook(struct Book **library, int *numBooks) {
    int bookID;
    printf("Enter the book ID to delete: ");
    scanf("%d", &bookID);

    int foundIndex = -1;

    loadLibrary(library, numBooks);

    for (int i = 0; i < *numBooks; i++) {
        if ((*library)[i].book_id == bookID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {

        for (int i = foundIndex; i < *numBooks - 1; i++) {
            (*library)[i] = (*library)[i + 1];
        }

        (*numBooks)--;

        printf("Book deleted successfully!\n");

        saveLibrary(*library, *numBooks);
    } else {
        printf("Book not found.\n");
    }
}

void saveLibrary(struct Book *library, int numBooks) {

    FILE *file = fopen("library_data.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numBooks; i++) {
        fprintf(file, "%s|%s|%s|\n", library[i].title, library[i].author, library[i].genre);

    }

    fclose(file);
    printf("Library data saved successfully!\n");
}

void loadLibrary(struct Book **library, int *numBooks) {

    FILE *file = fopen("library_data.txt", "r");
    if (file != NULL) {
        *numBooks = 0;

    char line[100];

            while (fgets(line, sizeof(line), file) != NULL) {

        // Remove newline character if present
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        char word[100];
        int j=0;
        char title[100], author[100], genre[100];
        for(int i=0; i<strlen(line); i++)
        {
            if(line[i]=='|')
            {

                if(strlen(title)==0)
                {
                    strcpy(title,word);
                    strcpy((*library)[*numBooks].title,title);
                }
                else if(strlen(author)==0)
                {
                    strcpy(author,word);
                    strcpy((*library)[*numBooks].author,author);
                }
                else{
                    strcpy(genre,word);
                    strcpy((*library)[*numBooks].genre,genre);
                }
                for(int k=0; k<j; k++)
                {

                    word[k]='\0';
                }
                j=0;

            }
            else
            {
                word[j]=line[i];
                j++;
            }
        }
            (*library)[*numBooks].book_id = *numBooks + 1;
            (*numBooks)++;



            }

        fclose(file);
        printf("Library data loaded successfully!\n");
    }
}
void freeLibrary(struct Book **library) {
    free(*library);
}

void loadingAnimation() {
    printf("\nLoading ");
    fflush(stdout);

    for (int i = 0; i < 10; i++) {
        printf("\b|");
        fflush(stdout);
        usleep(100000);

        printf("\b/");
        fflush(stdout);
        usleep(100000);

        printf("\b-");
        fflush(stdout);
        usleep(100000);

        printf("\b\\");
        fflush(stdout);
        usleep(100000);
    }

    printf("\b\b\b\b\b\b\b\b\b\b");
    fflush(stdout);
    printf(" Done!\n");
}

void recommendBook(struct Book *library, int numBooks) {
    char favoriteGenre[20];
    printf("Enter your favorite genre: ");
    scanf("%s", favoriteGenre);

    int recommendedBookFound = 0;

    printf("\nRecommended Book(s) in %s genre:\n", favoriteGenre);
    printf("%-5s %-30s %-30s %-20s\n", "ID", "Title", "Author", "Genre");

    for (int i = 0; i < numBooks; i++) {
        if (strcmp(library[i].genre, favoriteGenre) == 0) {
            printf("%-5d %-30s %-30s %-20s\n", library[i].book_id, library[i].title, library[i].author, library[i].genre);
            recommendedBookFound = 1;
        }
    }

    if (!recommendedBookFound) {
        printf("No books found in the specified genre.\n");
    }
}

void sortLibrary(struct Book *library, int numBooks) {
    int choice;
    printf("\nSort Library by:\n");
    printf("1. Title\n");
    printf("2. Author\n");
    printf("3. Genre\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            qsort(library, numBooks, sizeof(struct Book), compareTitles);
            printf("Library sorted by title.\n");
            break;
        case 2:
            qsort(library, numBooks, sizeof(struct Book), compareAuthors);
            printf("Library sorted by author.\n");
            break;
        case 3:
            qsort(library, numBooks, sizeof(struct Book), compareGenres);
            printf("Library sorted by genre.\n");
            break;
        default:
            printf("Invalid choice. No sorting performed.\n");
            return;
    }


    FILE *file = fopen("library_data.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < numBooks; i++) {
        fprintf(file, "%s %s %s\n", library[i].title, library[i].author, library[i].genre);
    }

    fclose(file);
    printf("Sorted library data written to file.\n");
}



int compareTitles(const void *a, const void *b) {
    return strcmp(((struct Book *)a)->title, ((struct Book *)b)->title);
}


int compareAuthors(const void *a, const void *b) {
    return strcmp(((struct Book *)a)->author, ((struct Book *)b)->author);
}

int compareGenres(const void *a, const void *b) {
    return strcmp(((struct Book *)a)->genre, ((struct Book *)b)->genre);
}

void displayStatistics(struct Book *library, int numBooks) {
    printf("\nLibrary Statistics:\n");
    printf("Total number of books: %d\n", numBooks);


    int genreCount[MAX_BOOKS] = {0};
    int maxCount = 0;
    char mostPopularGenre[20];

    for (int i = 0; i < numBooks; i++) {
        genreCount[i] = 1;


        for (int j = i + 1; j < numBooks; j++) {
            if (strcmp(library[i].genre, library[j].genre) == 0) {
                genreCount[i]++;
            }
        }

        if (genreCount[i] > maxCount) {
            maxCount = genreCount[i];
            strcpy(mostPopularGenre, library[i].genre);
        }
    }

    printf("Most popular genre: %s\n", mostPopularGenre);
}
