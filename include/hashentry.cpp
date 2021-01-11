#include "hashentry.hpp"

hashentry::hashentry(int page, int frame) :
    page(page),
    frame(frame),
    loaded(false) {
}

hashentry::~hashentry() {}

int hashentry::getPageNum() { return page; }

int hashentry::getFrame() { return frame; }

bool hashentry::getLoaded() { return loaded; }

void hashentry::setFrame(int nframe) { frame = nframe; }

void hashentry::setLoaded(bool flag) { loaded = flag; }
