#ifndef PLAYER_H
#define PLAYER_H
#include "structure.cpp"
#include <iostream>
using namespace std;
class basicmovable{
    public:
       basicmovable();
       int a;
       int b;
       int isMove;
       int state;
       void setx(int n){pos.x=n;}
       void sety(int n){pos.y=n;}
       void x(int n){pos.x+=n;}
       void y(int n){pos.y+=n;}
       int xpos(){return pos.x;}
       int xcam(){return pos.x+UNIT/2;}
       int ycam(){return pos.y+UNIT/2;}
       int ypos(){return pos.y;}
    private:
        position pos;
};

basicmovable::basicmovable()
{
    pos.x = TILE*7 + 1;
    pos.y = TILE*6 + 1;
    a = 90;
    b = 0;
    state = 0;
}


#endif
