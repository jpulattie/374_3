#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <float.h>
#include <time.h>

int main()
{
    int rando_num;
    int rand_mult;
    srand(time(NULL) * rand_mult);
    rando_num = (rand() % 99999) + 0;
    printf("random number:%d", rando_num);

    char filename[100]; // Buffer to store the final string
    int variable = 123; // Example variable

    sprintf(filename, "file.movie.%d", variable);
    printf("%s\n", filename); // Output: file.movie.123

    return 0;
}
