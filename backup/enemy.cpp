#ifndef ENEMY_H
#define ENEMY_H
//Object class
class enemy
{
    public:
       void x(int n){pos.x=n;}
       void y(int n){pos.y=n;}
       void spawn(int x, int y, int n);
       int xpos(){return pos.x;}
       int ypos(){return pos.y;}
       int xcam(){return pos.x+UNIT/2;}
       int ycam(){return pos.y+UNIT/2;}
       int Arc(){return dir;}
       int speed;
       int type;
       double distance;
       int sx;
       int sy;
       bool flag;
    private:
       position pos;
       int dir;
       int sprite;
       bool status;

};

void enemy::spawn(int x, int y, int n)
{

    pos.x = x;
    pos.y = y;
    type = n;
}

void enemy::spawn(int x, int y, int n)
{

    pos.x = x;
    pos.y = y;
    type = n;
}
#endif
