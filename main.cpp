#include <iostream>
#include <cstring>
#include <stdio.h>

#include "./include/hashentry.hpp"
#include "./include/memoryentry.hpp"
#include "./include/list.hpp"
#include "./include/hashtable.hpp"

#define PAGESIZE 4096 

//~~~~~~~~global counters~~~~~~~~~~
int secondChanceVictim = 0;

int readFromMemory = 0;
int writeToMemory = 0;
int readFromDisc = 0;
int writeToDisc = 0;
int removeFromMemory = 0;
int simpleLoad = 0;

int findvictim(list<memoryentry> &mymemory, const char *algorithm);

int main(int argc, char **argv) {
    //TODO ~~~~~~~~~~~~~~~~~~~ handle input arguments ~~~~~~~~~~~~~~~~~~~~~~~~
    
    if (argc < 4) {
        std::cerr << "Not enough arguments!" << std::endl;
        std::cerr << "Correct usage: ./emulation LRU/SC maxFrames q <maxq>" << std::endl;
        return -1;
    }
    if (argc > 5) {
        std::cerr << "Too many arguments!" << std::endl;
        std::cerr << "Correct usage: ./emulation LRU/SC maxFrames q <maxq>" << std::endl;
        return -1;
    }

    const char *algorithm = argv[1];
    int maxFrames = atoi(argv[2]);
    int q = atoi(argv[3]);
    if (strcmp(algorithm, "LRU") != 0 && strcmp(algorithm, "SC") != 0) {
        std::cerr << "Bad arguments! algorithm should be 'LRU' or 'SC'" << std::endl;
        return -2;
    }
    if (q < 1) {
        std::cerr << "Bad arguments! maxFrames must be a positive integer" << std::endl;
        return -2;
    }
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

    hashtable<hashentry> index(16);
    list<memoryentry> mymemory;

    char current = 0;
    char finished = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //TODO ~~~~~~~~~~~~~~~~~~~ basic function ~~~~~~~~~~~~~~~~~~~~~~~~

    while (1) {
        for (int i = 0; i < q; i++) {
            if ((read = getline(&line, &len, fptr[current])) == -1) { //if reached EOF
                finished++;                                         //note it and switch to the other file
                current = (current == 0 ? 1 : 0);
                break;
            }
            //isolate number and R/W
            long number = strtol(line, NULL, 16);
            char flag = line[strlen(line)-2];

            int pagenum = number / PAGESIZE;
            int offset = number % PAGESIZE;

            hashentry *newhashentry = index.getContentByKey(pagenum); //if not inside hashtable add it
            if (newhashentry == NULL) { 
                index.insert(pagenum, mymemory.getCount());
                newhashentry = index.getContentByKey(pagenum);
            }
            if (!newhashentry->getLoaded()) { //if not loaded, load it

                if (mymemory.getCount() < maxFrames) {//if memory is not full, simply load it
                    mymemory.insert(pagenum, mymemory.getCount() - 1);
                    newhashentry->setFrame(mymemory.getCount() - 1);

                    simpleLoad++;
                }
                else { //if needed to throw away another one
                    int victimpage = findvictim(mymemory, algorithm); //find which one
                    memoryentry *victim = mymemory.getContentByKey(victimpage);
                    if (victim->getDirty()) //if it is changed, write it
                        writeToDisc++;
                    int frame = victim->getFrame(); //keep which frame it was on
                    mymemory.remove(victimpage); //remove it
                    index.getContentByKey(victimpage)->setLoaded(false); //mark it as unloaded

                    mymemory.insert(pagenum, frame); //insert new one
                    newhashentry->setFrame(frame); //update frame in index

                    removeFromMemory++;
                }
                newhashentry->setLoaded(true);
                readFromDisc++;
            }

            memoryentry *currentEntry = mymemory.getContentByKey(pagenum);
            currentEntry->setSecondChance(true);
            if (algorithm == "LRU") {
                memoryentry temp = currentEntry;
            }

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

int findvictim(list<memoryentry> &mymemory, const char *algorithm) {

}
