#ifndef STRUCTURE_H
#define STRUCTURE_H
struct position
{
    int x;
    int y;
    int i;
    int j;
};

struct Sprite
{
    int x;
    int y;
    int type;
    int bitmap;
    int distance;
};

template <class type>
struct NODE{
    type val;
    NODE *next;
    NODE *prev;
};
#endif
