#ifndef TERRAIN_H
#define TERRAIN_H
//Terrain class
#include <cmath>
class terrain
{
    public:
       terrain();
       void x(int n){pos.x=n;}
       void y(int n){pos.y=n;}
       void initialize(int i, int j, int tileset);
       int xpos(){return pos.x;}
       int ypos(){return pos.y;}
       bool within(int,int);
       bool corner(int,int);
       int type;
       int floortype;
    private:
       position pos;
};

terrain::terrain()
{
    pos.x = 0;
    pos.y = 0;
}

void terrain::initialize(int i, int j, int tileset)
{
    pos.i = i;
    pos.j = j;
    type = tileset;
    pos.x = pos.j * TILE;
    pos.y = pos.i * TILE;
    floortype = 0;
}


#endif
