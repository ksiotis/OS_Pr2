#ifndef HASHENTRY_HPP
#define HASHENTRY_HPP

class hashentry {
private:
    int page;
    int frame;
    bool loaded;
public:
    hashentry(int page, int frame);
    ~hashentry();

    int getPageNum();
    int getFrame();
    bool getLoaded();

    void setFrame(int frame);
    void setLoaded(bool flag);
};

#endif /*HASHENTRY_HPP*/
