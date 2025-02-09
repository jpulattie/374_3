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
//movie struct and other variables take from assignment 2
// vvvv
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
// ^^^^^^

char newDirectory[257];
int choice_control;

//year search taken from assignment 2 and modified
struct movie *year_search(int search_year)
{
    struct movie *current = head;
    int found = 0;
    char newFileName[9];
    char newFilePath[267];
    FILE *fp = NULL;

    while (current != NULL)
    {
        if (current->year == search_year && found == 0)
        {
            sprintf(newFileName, "%d.txt", search_year);
            sprintf(newFilePath, "%s/%s", newDirectory, newFileName);
            fp = fopen(newFilePath, "w");
            found++;
        }
        if (current->year == search_year && found != 0)
        {
            fprintf(fp, "%s\n", current->title);
            found++;
        }

        current = current->next;
    }
    if (fp) {fclose(fp);}
    return NULL;
};

// modified from highest_rated_yr() from assignment2
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

//create movie used from assignment 2
struct movie *create_movie(char *currLine){
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
void readFile(char *filePath){
    printf("\nNow processing the chosen file named %s\n", filePath);
    choice_control = 1;
    char *currLine = NULL;
    size_t len = 0;
    FILE *movieFile = fopen(filePath, "r");

    int rando_num;

    srand(time(NULL));
    rando_num = (rand() % 99999) + 0;

    char filenameNew[257];
    sprintf(filenameNew, "pulattij.movies.%d", rando_num);
    const char *pathname = filenameNew;
    strcpy(newDirectory, pathname);

    // printf("%s\n", pathname); // Output: file.movie.123
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

//NEW CODE FOR ASSIGNMENT 3 BELOW
int name_check(char *filename)
{
    // printf("name check for %s\n", filename);
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
    // printf("CATCH- choice: %d\n", choice);
    //  Open the current directory
    currDir = opendir(".");

    // Go through all the entries
    while ((entry = readdir(currDir)) != NULL)
    {
        // Get meta-data for the current entry
        stat(entry->d_name, &dirStat);
        // printf("\nchecking %s with size %ld\n", entry->d_name, dirStat.st_size);
        if (name_check(entry->d_name) == 0 && choice == 1)
        {
            if (dirStat.st_size > biggest_size)
            {
                biggest_size = dirStat.st_size;
                biggest_entry = entry;
                // printf("File:%s Size:%0lf\n", biggest_entry->d_name, biggest_size);
                biggestPath = biggest_entry->d_name;
                // printf("\n Biggest path");
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
        // printf("Biggest:");
        readFile(biggestPath);
    }
    if (smallest_entry != NULL)
    {
        // printf("\nsmallest");
        readFile(smallestPath);
    }

    // Close the directory
    closedir(currDir);
    return 0;
}

//read_choice modifies read_directories taken from Module Exploration: "Directories"
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
            //printf("Choice:");
            readFile(choicePath);
        }
        else if (!choice_entry)
        {
            printf("The file %s was not found. Try again\n", name);
            break;
        }
    }
    // Close the directory
    closedir(currDir);
    return 0;
}

int redirect(){
    int user_input;

    while (choice_control != 1){

    printf("\nWhich file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\n");
    printf("Enter 3 to specify the name of a file\n\nEnter a choice from 1 to 3:");
    scanf("%d", &user_input);
    // printf("%d\n", user_input);
    if (user_input > 0 && user_input < 3)
    {
        read_directories(user_input);
    }
    if (user_input == 3)
    {
        read_choice();
    }
}
    return 0;
}

int menu()
{
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
            choice_control = 0;
            redirect();
            break;
        case 2:
            break;
        default:
            // printf("case: %d\n", input);
            printf("You entered an incorrect choice. Try again.\n");
            break;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    menu();

    return EXIT_SUCCESS;
};