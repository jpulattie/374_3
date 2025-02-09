#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct movie{
        char *title;
        int year;
        char *languages;
        double rating;
        struct movie *next;
    };

int movie_count = -1;
struct movie *head = NULL;
struct movie *winner = NULL;

struct movie *highest_rated ( int year) {
    struct movie *current = head;
    double rating = 0;
    //printf("here");
    while(current != NULL){
        if(rating <= current->rating && current->year == year) {
            if(winner != NULL){ 
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
    if ( winner != NULL && winner->year == year){
    printf("%d %.1f %s\n", winner->year, winner->rating, winner->title);
    return winner;}
};

void highest_rated_yr () {
    int i = 1899;
    //highest_rated(2017);
    while(i < 2025){
        highest_rated(i);
        i++;
    }
}

struct movie *year_search ( int search_year) {
    struct movie *current = head;
    int found = 0;
    //printf("search by year: %d\n", search_year);
    while (current != NULL){
        //printf("Current Title: %s", current->title);
        if (current->year == search_year) {
                printf("%s \n", current->title);
                found = 1;
            }
        current = current->next;
        }
    if (found == 0) {
        printf("No data about movies released in the year %d\n", search_year);
    }
    //printf("exiting search\n");
    return NULL;
};

struct movie *language_search ( char *search_language) {
    struct movie *current = head;
    int found_lang = 0;
    while (current != NULL){
        //printf("Current Title: %s", current->title);
        char *languages_copy = calloc(strlen(current->languages)+1, sizeof(char));
        char *token_ptr;
        char *token;
        strcpy(languages_copy, current->languages);
        token = strtok_r(languages_copy, "[;]", &token_ptr);
        while (token != NULL) {
            //printf("current language: %s", token);
            if (strcmp(token, search_language) == 0) {
                printf("%d %s\n", current->year, current->title);
                found_lang = 1;
            }
            token = strtok_r(NULL, "[;]", &token_ptr);
        }
        current = current->next;
        free(languages_copy);
    }
    if (found_lang == 0) {
        printf("No data about movies released in %s\n", search_language);
    }
    //printf("exiting search");
    return NULL;
};

struct movie *create_movie(char *currLine) {
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

    //printf("Checking\n");
    //printf("Title: %s\n", newMovie->title);
    //printf("Year: %d\n", newMovie->year);
    //printf("Languages: %s\n", newMovie->languages);
    //printf("Rating: %.2f\n", newMovie->rating);
    return newMovie;
}

void readFile(char* filePath) {
    char *currLine = NULL;
    size_t len = 0;

    FILE *movieFile = fopen(filePath, "r");

    while(getline(&currLine, &len, movieFile) != -1) {
        //printf("creating struct for %s", currLine);
        // make a copy of currLine to pass into create_movie
        create_movie(currLine);
 
    }
    free(currLine);
    fclose(movieFile);
    printf("\n Processed file %s and parsed data for %d movies\n", filePath, movie_count);
};

int menu () {
    int year_input;
    char language_input[21];
    int input =0;
    while (input != 4){
    //printf("input value: %d\n", input);

    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
    printf("\nEnter a choice from 1 to 4:");
    scanf("%d", &input);

    
    switch (input) {
        case 1:
            //printf("case: %d\n", input);
            printf("Enter the year for which you want to see movies:");
            scanf("%d", &year_input);
            year_search(year_input);
            break;
        case 2:
            highest_rated_yr();
            break;
        case 3:
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", language_input);
            language_search(language_input);
            break;
        case 4:
            //printf("case: %d\n", input);
            return input = 4;
        default:
            //printf("case: %d\n", input);
            printf("You entered an incorrect choice. Try again.\n");
            break;

    }}
    
}

int main( int argc, char **argv) {
    if (argc < 2) {
        printf("Please provide a file to process");
        printf("For Example: ./example example.csv\n");
        return EXIT_FAILURE;
    }
    readFile(argv[1]);
    //highest_rated(2008);
    menu();

    return EXIT_SUCCESS;
};