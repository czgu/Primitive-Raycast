#ifndef OBJECT_H
#define OBJECT_H
//Object class
class object
{
    public:
       //Modifier
       void x(int n){pos.x=n;}
       void y(int n){pos.y=n;}
       void initialize(int i, int j, int n);
       void spawn(int x, int y, int n,int s);
       void nextFrame();
       void FreezeFrame();
       void hit(int damage);

       //Accesor
       int frame();
       int xpos(){return pos.x;}
       int ypos(){return pos.y;}
       int xcam(){if(movable)return pos.x+UNIT/2;return pos.x+TILE/2;}
       int ycam(){if(movable)return pos.y+UNIT/2;return pos.y+TILE/2;}
       int Arc(){return dir;}
       bool isDead(){return hp < 1;}
       bool isHurt(){return hurt>0;}
       //Public Variables
       int speed;
       int type;
       double distance;
       int sx;
       int sy;
       bool flag;
       bool movable;
       int dir;
       int enemynum;
       int hp;
       bool death_animation;
    private:
       position pos;
       int counter;
       bool no_move;
       
       int death_animation_counter;
       int hurt;
       

};

void object::hit(int damage)
{
     if(isDead())return;
     hp-=damage;
     hurt=5;
     if(isDead())
     {
         death_animation = true;
         death_animation_counter = 0;
         counter = 0;
     }
}

int object::frame()
{
    if(hp < 1 && death_animation)
    {
        return (death_animation_counter/5+5);
    }
    if(hp < 1) return 10;
    if(hurt>0) return 9;
    if(no_move)  return 0;
    else
    {
        return (counter/10+1);
    }
}

void object::nextFrame()
{
    counter++;
    no_move = false;
    if(counter>=40)counter = 0;
}

void object::FreezeFrame()
{
     no_move = true;
     if(hurt>0)hurt--;
     if(death_animation)death_animation_counter++;
     if(death_animation_counter>=20)death_animation = false;
}

void object::initialize(int i, int j, int n)
{
    pos.i = i;
    pos.j = j;
    pos.x = pos.j * TILE;
    pos.y = pos.i * TILE;
    type = n;
    movable = false;
    speed = 0;
    hp = 1;
}

void object::spawn(int x, int y, int n,int s)
{
    movable = true;
    no_move = false;
    speed = 1;
    enemynum = s;
    pos.x = x;
    pos.y = y;
    type = n;
    hp = 1;
    death_animation = false;
    death_animation_counter = 0;
}
#endif

