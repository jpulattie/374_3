#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <float.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

DIR *currDir;

struct movie
{
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
};

int movie_count = -1;
struct movie *head = NULL;
struct movie *winner = NULL;
char newDirectory[257];


struct movie *highest_rated(int year)
{
    struct movie *current = head;
    double rating = 0;
    // printf("here");
    while (current != NULL)
    {
        if (rating <= current->rating && current->year == year)
        {
            if (winner != NULL)
            {
                free(winner->title);
                free(winner->languages);
            }
            winner = malloc(sizeof(struct movie));
            winner->title = calloc(strlen(current->title) + 1, sizeof(char));
            strcpy(winner->title, current->title);
            winner->year = current->year;
            winner->languages = calloc(strlen(current->languages) + 1, sizeof(char));
            strcpy(winner->languages, current->languages);
            winner->rating = current->rating;
            rating = current->rating;
        }
        current = current->next;
    }
    if (winner != NULL && winner->year == year)
    {
        printf("%d %.1f %s\n", winner->year, winner->rating, winner->title);
        return winner;
    }
};

struct movie *year_search(int search_year){
    struct movie *current = head;
    int found = 0;
    char newFileName[9];
    char newFilePath[267];
    FILE *fp = NULL;


    // printf("search by year: %d\n", search_year);
    while (current != NULL)
    {
        // printf("Current Title: %s", current->title);
        if (current->year == search_year && found == 0)
        {
            
            //printf("create new txt file for %d\n", current->year);
            
            sprintf(newFileName, "%d.txt", search_year);
            //printf("New file named %s", newFileName);
            
            sprintf(newFilePath, "%s/%s", newDirectory, newFileName);
            //printf("\nNEW FILE PATH: %s\n", newFilePath);
            fp = fopen(newFilePath, "w");
            found ++;
        }
        if (current->year == search_year && found != 0)
        {
            //fp = fdopen(newFile, "w");
            fprintf(fp, "%s\n",current->title);
            //printf("%s \n", current->title);
            found ++;
        } 

        current = current->next;
            
    }
    if (fp) {
        fclose(fp);
    }


    return NULL;
};

//modified from highest_rated_yr() from assignment2
void by_yr()
{
    int i = 1899;
    // highest_rated(2017);
    while (i < 2025)
    {
        year_search(i);
        i++;
    }
}

struct movie *language_search(char *search_language)
{
    struct movie *current = head;
    int found_lang = 0;
    while (current != NULL)
    {
        // printf("Current Title: %s", current->title);
        char *languages_copy = calloc(strlen(current->languages) + 1, sizeof(char));
        char *token_ptr;
        char *token;
        strcpy(languages_copy, current->languages);
        token = strtok_r(languages_copy, "[;]", &token_ptr);
        while (token != NULL)
        {
            // printf("current language: %s", token);
            if (strcmp(token, search_language) == 0)
            {
                printf("%d %s\n", current->year, current->title);
                found_lang = 1;
            }
            token = strtok_r(NULL, "[;]", &token_ptr);
        }
        current = current->next;
        free(languages_copy);
    }
    if (found_lang == 0)
    {
        printf("No data about movies released in %s\n", search_language);
    }
    // printf("exiting search");
    return NULL;
};

struct movie *create_movie(char *currLine)
{
    char *tokPtr;
    char *token;
    char *tokPtr2;
    struct movie *newMovie = malloc(sizeof(struct movie));

    token = strtok_r(currLine, ",", &tokPtr);
    newMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(newMovie->title, token);

    token = strtok_r(NULL, ",", &tokPtr);
    newMovie->year = atoi(token);

    token = strtok_r(NULL, ",", &tokPtr);
    newMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(newMovie->languages, token);

    token = strtok_r(NULL, ",", &tokPtr);
    newMovie->rating = atof(token);

    newMovie->next = head;
    head = newMovie;

    movie_count++;

    // printf("Checking\n");
    // printf("Title: %s\n", newMovie->title);
    // printf("Year: %d\n", newMovie->year);
    // printf("Languages: %s\n", newMovie->languages);
    // printf("Rating: %.2f\n", newMovie->rating);
    return newMovie;
}
// modified readFile from assignment 2
void readFile(char *filePath)
{
    printf("\nNow processing the chosen file named %s\n", filePath);

    char *currLine = NULL;
    size_t len = 0;
    FILE *movieFile = fopen(filePath, "r");

    int rando_num;
    int rand_mult;
    srand(time(NULL) * rand_mult);
    rando_num = (rand() % 99999) + 0;

    char filenameNew[257]; 
    sprintf(filenameNew, "pulattij.movie.%d", rando_num);
    const char *pathname = filenameNew;
    strcpy(newDirectory, pathname);

    //printf("%s\n", pathname); // Output: file.movie.123
    mode_t mode = 0750;
    mkdir(pathname, mode);

    while (getline(&currLine, &len, movieFile) != -1)
    {
        // printf("creating struct for %s", currLine);
        //  make a copy of currLine to pass into create_movie
        create_movie(currLine);
    }
    free(currLine);
    fclose(movieFile);
    by_yr();
    printf("Created directory with name %s\n", newDirectory);
};
// UPDATE CODE BELOW

int name_check(char *filename)
{
    //printf("name check for %s\n", filename);
    if (!filename)
    {
        return 1;
    }
    const char *extension = strrchr(filename, '.');
    if (!extension)
    {
        return 1;
    }
    if ((strcmp(extension, ".csv") == 0) && (strncmp(filename, "movies_", 7) == 0))
    {
        //printf("GOOD NAME: %s\n", filename);
        return 0;
    }
    else
    {
        return 1;
    }
}

// read_directories function taken from Module Exploration: "Directories" and modified to display size
int read_directories(int choice)
{
    struct dirent *entry;
    struct stat dirStat;
    double biggest_size = 0;
    double smallest_size = DBL_MAX;
    struct dirent *biggest_entry = NULL;
    struct dirent *smallest_entry = NULL;
    char *biggestPath = NULL;
    char *smallestPath = NULL;
    //printf("CATCH- choice: %d\n", choice);
    // Open the current directory
    currDir = opendir(".");

    // Go through all the entries
    while ((entry = readdir(currDir)) != NULL)
    {
        // Get meta-data for the current entry
        stat(entry->d_name, &dirStat);
        //printf("\nchecking %s with size %ld\n", entry->d_name, dirStat.st_size);
        if (name_check(entry->d_name) == 0 && choice == 1)
        {
            if (dirStat.st_size > biggest_size)
            {
                biggest_size = dirStat.st_size;
                biggest_entry = entry;
                //printf("File:%s Size:%0lf\n", biggest_entry->d_name, biggest_size);
                biggestPath = biggest_entry->d_name;
                //printf("\n Biggest path");
            }
        }
        if (name_check(entry->d_name) == 0 && choice == 2)
        {
            if (dirStat.st_size < smallest_size)
            {
                smallest_size = dirStat.st_size;
                smallest_entry = entry;
                // printf("File:%s Size:%0lf\n", smallest_entry->d_name, smallest_size);
                smallestPath = smallest_entry->d_name;

            }
        }
    }
    if (biggest_entry != NULL)
    {
        //printf("Biggest:");
        readFile(biggestPath);
    }
    if (smallest_entry != NULL)
    {
        //printf("\nsmallest");
        readFile(smallestPath);
    }

    // Close the directory
    closedir(currDir);
    return 0;
}

int read_choice()
{
    struct dirent *entry;
    struct stat dirStat;
    struct dirent *choice_entry = NULL;
    char *choicePath = NULL;
    char name[257];
    int found = 1;

    while (found != 0)
    {
        printf("Enter the complete file name:");
        scanf("%s", name);
        // Open the current directory
        currDir = opendir(".");

        // Go through all the entries
        while ((entry = readdir(currDir)) != NULL)
        {
            // Get meta-data for the current entry
            // printf("\nchecking %s with size %ld\n", entry->d_name, dirStat.st_size);
            if (strcmp(entry->d_name, name) == 0)
            {

                choice_entry = entry;
                // printf("File:%s Size:%0lf\n", biggest_entry->d_name, biggest_size);
                choicePath = choice_entry->d_name;
            }
        }

        if (choice_entry)
        {
            found = 0;
            printf("Choice:");
            readFile(choicePath);
        }
        else if (!choice_entry)
        {
            printf("The file %s was not found. Try again\n", name);
        }
    }
    // Close the directory
    closedir(currDir);
    return 0;
}

int menu()
{
    int user_input;
    char name_input[256];
    int size;
    char language_input[21];
    int input = 0;
    while (input != 2)
    {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("\nEnter a choice 1 or 2:");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            // printf("case: %d\n", input);
            printf("\nWhich file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\n");
            printf("Enter 3 to specify the name of a file\n\nEnter a choice from 1 to 3:");
            scanf("%d", &user_input);
            //printf("%d\n", user_input);
            if (user_input > 0 && user_input < 3)
            {
                read_directories(user_input);
            }
            if (user_input == 3)
            {
                read_choice();
            }
            break;
        case 2:
            break;
        default:
            // printf("case: %d\n", input);
            printf("You entered an incorrect choice. Try again.\n");
            break;
        }
    }
}

// UPDATE CODE ABOVE
int main(int argc, char **argv)
{
    menu();

    return EXIT_SUCCESS;
};