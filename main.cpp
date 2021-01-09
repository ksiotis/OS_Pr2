#include <iostream>
#include <cstring>
#include <stdio.h>

#define PAGESIZE 4096

int main(int argc, char **argv) {
    //TODO ~~~~~~~~~~~~~~~~~~~ handle input arguments ~~~~~~~~~~~~~~~~~~~~~~~~
    
    // if (argc < 4 {
    //     std::cerr << "Not enough arguments! Correct usage:" << std::endl;
    //     std::cerr << "./emulation <q>" << std::endl;
    //     return -1
    // })


    int q = atoi(argv[argc-1]);
    if (q < 1) {
        std::cerr << "Bad arguments! q must be a positive integer" << std::endl;
        return -2;
    }

    FILE *fptr[2];
    if ((fptr[0] = fopen("gcc.trace", "r")) == NULL) {
        printf("Error! opening file 1");
        return -3;
    }
    if ((fptr[1] = fopen("bzip.trace", "r")) == NULL) {
        printf("Error! opening file 2");
        return -3;
    }

    //TODO ~~~~~~~~~~~~~~~~~~~ initialization of data ~~~~~~~~~~~~~~~~~~~~~~~~

    char current = 0;
    char finished = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //TODO ~~~~~~~~~~~~~~~~~~~ basic function ~~~~~~~~~~~~~~~~~~~~~~~~

    while (1) {
        for (int i = 0; i < q; i++) {
            if (read = getline(&line, &len, fptr[current]) == -1) { //if reached EOF
                finished++;                                         //note it and switch to the other file
                current = (current == 0 ? 1 : 0);
                break;
            }
            //isolate number and R/W
            long number = strtol(line, NULL, 16);
            char flag = line[strlen(line)-2];

            //TODO basic functions
            int pagenum = number / PAGESIZE;
            int offset = number % PAGESIZE;

            
            // if (flag == 'R')
            // else if (flag == 'W')
            // else
        }

        if (finished == 2) { //if finished reading both files end program
            //TODO end
            
            break;
        }
        else if (!finished) { //if the other file is not finished, switch to that
            current = (current == 0 ? 1 : 0);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~ delete allocated memory ~~~~~~~~~~~~~~~~~~~~~~~~

    free(line);
    fclose(fptr[0]);
    fclose(fptr[1]);
    return 0;
}