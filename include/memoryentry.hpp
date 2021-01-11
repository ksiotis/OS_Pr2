#ifndef MEMORYENTRY_HPP
#define MEMORYENTRY_HPP

class memoryentry {
private:
    int page;
    int frame;
    bool dirty;
    bool secondChance;

    //char content[4096];
public:
    memoryentry(int page, int frame);
    ~memoryentry();

    int getPageNum();
    int getFrame();
    bool getDirty();
    bool getSecondChance();

    void setFrame(int frame);
    void setDirty(bool flag);
    void setSecondChance(bool flag);
};

#endif /*MEMORYENTRY_HPP*/
