#include "memoryentry.hpp"
#include <iostream>

memoryentry::memoryentry(int page, int frame) :
    page(page),
    frame(frame),
    dirty(false),
    secondChance(true) {
}

memoryentry::~memoryentry() {
    std::cout << "del" << std::endl;
}

int memoryentry::getPageNum() { return page; }

int memoryentry::getFrame() { return frame; }

bool memoryentry::getDirty() { return dirty; }

bool memoryentry::getSecondChance() { return secondChance; }

void memoryentry::setFrame(int nframe) { frame = nframe; }

void memoryentry::setDirty(bool flag) { dirty = flag; }

void memoryentry::setSecondChance(bool flag) { secondChance = flag; }
