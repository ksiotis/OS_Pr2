#include <iostream>
#include <cstring>
#include <stdio.h>

#include "./include/hashentry.hpp"
#include "./include/memoryentry.hpp"
#include "./include/list.hpp"
#include "./include/hashtable.hpp"

#define PAGESIZE 4096 

// template<typename T>
// bool actualcount(list<T> &mylist) {
//     int count = 0;
//     listNode<T> *current = mylist.getStart();
//     while (current != NULL) {
//         count++;

//         current = current->getNext();
//     }
//     return (count == mylist.getCount());
// }

//~~~~~~~~global counters~~~~~~~~~~
int secondChanceVictim = 0;

int readFromMemory = 0;
int writeToMemory = 0;
int readFromDisc = 0;
int writeToDisc = 0;
int removeFromMemory = 0;
int simpleLoad = 0;

int findvictim(list<memoryentry> &mymemory, const char *algorithm, int maxframes);

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
    int maxlines = -1;

    if (argc == 5) {
        maxlines = atoi(argv[4]);
        if (maxlines < 1) {
            std::cerr << "Bad arguments! maxLines must be a positive integer" << std::endl;
            return -2;
        }
    }

    if (strcmp(algorithm, "LRU") != 0 && strcmp(algorithm, "SC") != 0) {
        std::cerr << "Bad arguments! algorithm should be 'LRU' or 'SC'" << std::endl;
        return -2;
    }
    if (maxFrames < 1) {
        std::cerr << "Bad arguments! maxFrames must be a positive integer" << std::endl;
        return -2;
    }
    if (q < 1) {
        std::cerr << "Bad arguments! q must be a positive integer" << std::endl;
        return -2;
    }

    //TODO ~~~~~~~~~~~~~~~~~~~ initialization of data ~~~~~~~~~~~~~~~~~~~~~~~~

    
    FILE *fptr[2];
    if ((fptr[0] = fopen("gcc.trace", "r")) == NULL) {
        printf("Error! opening file 1");
        return -3;
    }
    if ((fptr[1] = fopen("bzip.trace", "r")) == NULL) {
        printf("Error! opening file 2");
        return -3;
    }

    hashtable<hashentry> index(16);
    list<memoryentry> memorycontainer;
    list<memoryentry> mymemory;

    unsigned char current = 0;
    char finished = 0;

    int linesread[2] = {0, 0};
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
            // int offset = number % PAGESIZE;

            hashentry *newhashentry = index.getContentByKey(pagenum); //if not inside hashtable add it
            if (newhashentry == NULL) { 

                index.insert(new hashentry(pagenum, mymemory.getCount()));
                newhashentry = index.getContentByKey(pagenum);
            }
            if (!newhashentry->getLoaded()) { //if not loaded, load it

                if (mymemory.getCount() < maxFrames) {//if memory is not full, simply load it
                    memoryentry *newmem = new memoryentry(pagenum, mymemory.getCount());
                    memorycontainer.insert(newmem);
                    mymemory.insert(newmem);
                    simpleLoad++;
                }
                else { //if needed to throw away another one
                    int victimpage = findvictim(mymemory, algorithm, maxFrames); //find which one
                    memoryentry *victim = mymemory.getContentByKey(victimpage);
                    if (victim->getDirty()) //if it is changed, write it
                        writeToDisc++;
                    int frame = victim->getFrame(); //keep which frame it was on
                    mymemory.remove(victimpage); //remove it
                    index.getContentByKey(victimpage)->setLoaded(false); //mark it as unloaded

                    memoryentry *newmem = new memoryentry(pagenum, frame);
                    memorycontainer.insert(newmem);
                    mymemory.insert(newmem); //insert new one
                    removeFromMemory++;
                }
                newhashentry->setLoaded(true);
                readFromDisc++;
            }

            memoryentry *currentEntry = mymemory.getContentByKey(pagenum);
            currentEntry->setSecondChance(true);
            if (strcmp(algorithm, "LRU") == 0) { //if LRU get current memory entry to the back of the list and reset its second chance
                memoryentry *tempmem = mymemory.getContentByKey(pagenum);
                mymemory.remove(pagenum);
                mymemory.insert(tempmem); //resets second chance to default true
            }

            if (flag == 'R') {
                readFromMemory++;
            }
            else if (flag == 'W') {
                mymemory.getContentByKey(pagenum)->setDirty(true);
                writeToMemory++;
            }
            else {
                std::cerr << "Invalid flag in line: " << line << std::endl;
            }


            linesread[current]++;
            if (linesread[current] >= maxlines && maxlines > 0) {
                finished++;     //finished and switch to the other file
                current = (current == 0 ? 1 : 0);
                break;
            }
        }

        if (finished == 2) { //if finished reading both files end program
            break;
        }
        else if (!finished) { //if the other file is not finished, switch to that
            current = (current == 0 ? 1 : 0);
        }
    }
    std::cout << "readFromMemory = " << readFromMemory << std::endl;
    std::cout << "writeToMemory = " << writeToMemory << std::endl;
    std::cout << "readFromDisc = " << readFromDisc << std::endl;
    std::cout << "writeToDisc = " << writeToDisc << std::endl;
    std::cout << "removeFromMemory = " << removeFromMemory << std::endl;
    std::cout << "simpleLoad = " << simpleLoad << std::endl;

    // ~~~~~~~~~~~~~~~~~~~ delete allocated memory ~~~~~~~~~~~~~~~~~~~~~~~~

    memorycontainer.emptyContent();

    free(line);
    fclose(fptr[0]);
    fclose(fptr[1]);
    return 0;
}

memoryentry *getContentByFrame(list<memoryentry> &mymemory, int frame) {
    listNode<memoryentry> *current = mymemory.getStart();
    while (current != NULL) {
        if (current->getContent()->getFrame() == frame)
            return current->getContent();

        current = current->getNext();
    }
    return NULL;
}

int findvictim(list<memoryentry> &mymemory, const char *algorithm, int maxframes) {
    if (strcmp(algorithm, "LRU") == 0) {
        if (mymemory.getStart() != NULL)
            return mymemory.getStart()->getContent()->getPageNum();
        else
            return -1;
    }
    else if (strcmp(algorithm, "SC") == 0) {
        memoryentry *possiblevictim = getContentByFrame(mymemory, secondChanceVictim);
        while (possiblevictim->getSecondChance()) { //while they have a second chance
            possiblevictim->setSecondChance(false);

            secondChanceVictim = (secondChanceVictim + 1) % maxframes; //move to the next frame
            possiblevictim = getContentByFrame(mymemory, secondChanceVictim);
        }
        secondChanceVictim = (secondChanceVictim + 1) % maxframes;
        return possiblevictim->getPageNum();
    }
    else {
        std::cerr << "Invalid algorithm!" << std::endl;
        return -1;
    }
}
