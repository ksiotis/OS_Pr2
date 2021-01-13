#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <iostream>
#include "list.hpp"
#include "hashtable.hpp"

template<typename T>
class hashtable {
private:
    int bucketNumber;
    list<T>** table;
    int count;

    unsigned int hash(int address);
public:

    hashtable(int buckets);
    ~hashtable();

    int getBucketNumber();
    int getTotalCount();
    bool search(int rec);
    void insert(T *content);

    T* getContentByKey(int page);
};

template<typename T>
unsigned int hashtable<T>::hash(int address) {
    // return the hashed value of given string
    return address % bucketNumber;
}

template<typename T>
hashtable<T>::hashtable(int buckets) :
    bucketNumber(buckets) {
    table = new list<T> *[bucketNumber];
    for (int i = 0; i < bucketNumber; i++) {
        table[i] = NULL;
    }
}

template<typename T>
hashtable<T>::~hashtable() {
    for (int i = 0; i < bucketNumber; ++i) {
        if (table[i] != NULL) {
            table[i]->emptyContent();
            delete table[i];
        }
    }
    delete[] table;
}

template<typename T>
int hashtable<T>::getBucketNumber() { return bucketNumber; }

template<typename T>
int hashtable<T>::getTotalCount() {
    int sum = 0;
    for (int i = 0; i < bucketNumber; i++) {
        if (table[i] != NULL)
            sum += table[i]->getCount();
    }
    if (sum != count) {
        std::cerr << "Sizes do not match! sum: " << sum << " count: " << count <<std::endl;
    }
    return sum;
}

template<typename T>
bool hashtable<T>::search(int rec) {
// is something equal to the key of rec inside?
    unsigned int bucket = hash(rec);
    if (table[bucket] != NULL)
        return table[bucket]->search(rec);
    else
        return false;
}

template<typename T>
void hashtable<T>::insert(T *content) {
/*attempts to insert rec into the hashtable,
  if it already exists => do nothing,
  if not => inserts rec to the list of apropriate bucket*/
    int page = content->getPageNum();
    if (!search(page)) { //if keyvalue is not in a bucket
        unsigned int bucket = hash(page);
        if (table[bucket] == NULL)
            table[bucket]= new list<T>();
        table[bucket]->insert(content);
        count++;
    }
}

template <typename T>
T* hashtable<T>::getContentByKey(int page) {
    unsigned int bucket = hash(page);
    if (table[bucket] != NULL)
        return table[bucket]->getContentByKey(page);
    else
        return NULL;
}

#endif /*HASHTABLE_HPP*/
