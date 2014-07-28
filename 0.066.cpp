//Constants
int SCREEN_X = 800;
int SCREEN_Y = 600;
#define DISPLAY_X 800
#define DISPLAY_Y 600
#define MAX_E 40
int size_of_object = 0;
int ACTUAL_X = 800;
int ACTUAL_Y = 600;
int SPEED = 5;
int FOV  = 160;
int TILE = 64;
int UNIT = 32;
#define PI 3.1415926535898
#define RED makecol(255, 0, 0)
#define PURPLE makecol(255, 0, 255)
#define GREEN makecol(0, 255, 0)
#define BLUE makecol(0, 128, 255)
#define BLACK makecol(0, 0, 0)
#define WHITE makecol(255, 255, 255)
//allegro archives
#include <allegro.h>
//std libraries
//C++
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
//C
#include <stdio.h>
#include <math.h>
//custom made classes
#include "structure.cpp"
#include "weapon.cpp"
#include "player.cpp"
#include "terrain.cpp"
#include "Object.cpp"
//#include "enemy.cpp"

int light_level = 3;
int flash_light = 3;

//angle variables
static const int ANGLE60 = SCREEN_X;
static const int ANGLE30 = (ANGLE60/2);
static const int ANGLE15 = (ANGLE30/2);
static const int ANGLE90 = (ANGLE30*3);
static const int ANGLE45 = ANGLE90/2;
static const int ANGLE180 = (ANGLE90*2);
static const int ANGLE270 = (ANGLE90*3);
static const int ANGLE360 = (ANGLE60*6);
static const int ANGLE0 = 0;
static const int ANGLE5 = (ANGLE30/6);
static const int ANGLE10 = (ANGLE5*2);

using namespace std;
//Timers
volatile long speed_counter = 0; //A long integer which will store the value of the
								 //speed counter.
void increment_speed_counter() //A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one. :)
}END_OF_FUNCTION(increment_speed_counter);
//End of Timers
/***************************************************************************************************/

//Functions
bool collide(int x1, int y1,int w1,int h1,int x2, int y2, int w2,int h2);
void collide_detection(int xpos, int ypos, vector<terrain> tile,vector<object> enemies, basicmovable &player,int &xmove, int &ymove, int index);
void Load_Map(int stage, vector<terrain> &load, vector<object> &temp);
void Sort_Sprite(vector<object> &temp, vector<object> &sprite, vector<object> &enemies);
int Round(double num);
bool checkDot(int x,int y, vector<terrain> &tile,int xmove,int ymove);
int determine_d(int d);
float arcToRad(float arcAngle);
float ARCTAN(float x,float y);
float arcToAngle(float angle);
int AngletoArc(float arc);
/**************************************************************************************************************/
//Current Global Variables (Will be Removed)
int incre;
//------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]){
    
    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ACTUAL_X, ACTUAL_Y, 0, 0);
    LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(30));
    COLOR_MAP global_light_table;
    set_trans_blender(0, 0, 0, 0);
    srand(time(0));
    //Bitmaps
    
    BITMAP *buffer = create_bitmap(SCREEN_X, SCREEN_Y);
    if(buffer == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Error: Could not create buffer!");
		exit(EXIT_FAILURE);
	}
    BITMAP *p1 = load_bitmap("BITMAP/Player.bmp", NULL);
    BITMAP *cross = load_bitmap("BITMAP/cross2.bmp", NULL);
    BITMAP *Brick = load_bitmap("BITMAP/Brick6.bmp", NULL);
    BITMAP *Brick2 = load_bitmap("BITMAP/Brick7.bmp", NULL);
    BITMAP *Ground = load_bitmap("BITMAP/grass.bmp", NULL);
    BITMAP *floor = load_bitmap("BITMAP/floor.bmp", NULL);
    BITMAP *floor_tile = load_bitmap("BITMAP/Floortile.bmp", NULL);
    BITMAP *ceiling = load_bitmap("BITMAP/ceil.bmp",NULL);
    BITMAP *cursor = load_bitmap("BITMAP/mouse.bmp",NULL);
    BITMAP *hand = load_bitmap("BITMAP/hand.bmp",NULL);
    BITMAP *armor = load_bitmap("BITMAP/armor.bmp",NULL);
    //TILES ==========================================================
    BITMAP *all_Tile = load_bitmap("BITMAP/tileset.bmp",NULL);
    BITMAP *tile_map[114];
    for(int i = 0; i <19;i++)
    {
        for(int j = 0;j<6;j++)
        {
            tile_map[i*6+j] = create_bitmap(TILE,TILE);
            blit(all_Tile,tile_map[i*6+j],j*64,i*64,0,0,64,64);

        }
    }
    //Objects ===========================================================
    BITMAP *Item[10] = {0};
    Item[0] = load_bitmap("BITMAP/armor.bmp", NULL);
    Item[1] = load_bitmap("BITMAP/tablechairs.bmp", NULL);
    Item[2] = load_bitmap("BITMAP/plantgreen.bmp", NULL);
    
    //Enemy Sprites ====================================================
    BITMAP *ENEMY[10][14] = {0};
    BITMAP *ENEMY_TEMP[10] = {0};
    ENEMY_TEMP[0] = load_bitmap("BITMAP/guard.bmp", NULL);
    ENEMY_TEMP[1] = load_bitmap("BITMAP/guard2.bmp", NULL);
    for(int i = 0; i < ENEMY_TEMP[0]->w/64;i++)
    {
        ENEMY[0][i] = create_bitmap(64,64);
        blit(ENEMY_TEMP[0],ENEMY[0][i],i*64,0,0,0,64,64);
    }
    for(int i = 0; i < ENEMY_TEMP[1]->w/64;i++)
    {
        ENEMY[1][i] = create_bitmap(64,64);
        blit(ENEMY_TEMP[1],ENEMY[1][i],i*64,0,0,0,64,64);
    }
    //===================================================================
    BITMAP *s_temp = 0;
    BITMAP *s_temp_2 = 0;
    BITMAP *wall_temp = create_bitmap(64,64);
    
    //Weapon Sprites ===========================================
    BITMAP *weapon_temp[10];
    BITMAP *weapon[10][5];
    weapon_temp[0] = load_bitmap("BITMAP/Knife.bmp", NULL);
    weapon_temp[1] = load_bitmap("BITMAP/Pisto.bmp", NULL);
    weapon_temp[2] = load_bitmap("BITMAP/Submachine.bmp", NULL);
    guninfo gundata[5];
    for(int j = 0;j<3;j++)
    {
        if(j == 0)gundata[j].setup(false,4,2,3,2,3,0,0,0,0); // knife
        if(j == 1)gundata[j].setup(false,4,3,2,3,5,SCREEN_X/35,SCREEN_X/16,SCREEN_X/36,3); //pisto
        if(j == 2)gundata[j].setup(true,2,1,2,2,4,SCREEN_X/32,SCREEN_X/12,SCREEN_X/38,5); //submachine
        for(int i = 0; i < 5;i++)
        {
            weapon[j][i] = create_bitmap(64,64);
            blit(weapon_temp[j],weapon[j][i],i*64,0,0,0,64,64);
        }
    }
    BITMAP *cross_h;
    BITMAP *cross_v;
    cross_h = create_bitmap(5,3);
    cross_v = create_bitmap(3,5);
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0;j < 5;j++)
        {
            if(i == 1)
            {
                putpixel(cross_h,j,i,WHITE);
                putpixel(cross_v,i,j,WHITE);
            }
            else
            {
                putpixel(cross_h,j,i,BLACK);
                putpixel(cross_v,i,j,BLACK);
            }
        }
    }
    //Variable Section ---------------------------------------
    const float theta = SCREEN_X/FOV;
    const float screen_distance = (SCREEN_Y/2)/tan((FOV/2)*PI/180);
    const float XY_ratio = float(SCREEN_X)/SCREEN_Y;
    // trigonometric tables
    float *fSinTable;
    float *fISinTable;
    float *fCosTable;
    float *fICosTable;
    float *fTanTable;
    float *fITanTable;
    float *fFishTable;
    float *fXStepTable;
    float *fYStepTable;
    float *fFloorTable;
    float **fFloorTable2;
    float ***fFloorSinTable;
    float ***fFloorCosTable;
    float *fFloorLightTable;
    float radian;
    //Calculations
    fFloorTable2 = new float*[ANGLE360+1];
   // fFloorSinTable = new float**[ANGLE360+1];
    //fFloorCosTable = new float**[ANGLE360+1];
    for(long i = 0;i<ANGLE360+1;i++)
    {
        fFloorTable2[i] = new float[SCREEN_Y];
     //   fFloorSinTable[i] = new float*[SCREEN_Y];
     //   fFloorCosTable[i] = new float*[SCREEN_Y];
    }

    fSinTable = new float[ANGLE360+1];
    fISinTable = new float[ANGLE360+1];
    fCosTable = new float[ANGLE360+1];
    fICosTable = new float[ANGLE360+1];
    fTanTable = new float[ANGLE360+1];
    fITanTable = new float[ANGLE360+1];
    fFishTable = new float[ANGLE60+1];
    fXStepTable = new float[ANGLE360+1];
    fYStepTable = new float[ANGLE360+1];
    fFloorTable = new float[ANGLE360+1];
    for (int i = 0; i <= ANGLE360;i++)
    {

      // get the radian value (the last addition is to avoid division by 0, try removing
          // that and you'll see a hole in the wall when a ray is at 0, 90, 180, or 270 degree)
      radian = arcToRad(i) + 0.001;
      fSinTable[i] = sin(radian);
      fCosTable[i] = cos(radian);
      fTanTable[i] = tan(radian);
      
      fICosTable[i] = (1.0/(fCosTable[i]));
      fISinTable[i] = (1.0/(fSinTable[i]));
      fITanTable[i] = (1.0/fTanTable[i]);

        //  you can see that the distance between xi is the same
        //  if we know the angle
        //  _____|_/next xi______________
        //       |
        //  ____/|next xi_________   slope = tan = height / dist between xi's
        //     / |
        //  __/__|_________  dist between xi = height/tan where height=tile size
        // old xi|
        //                  distance between xi = x_step[view_angle];
        //
        //
        // facine left
      // facing up
      if (i>=ANGLE0 && i<ANGLE180)
      {
        fXStepTable[i] = (float)(TILE/fTanTable[i]);
      }
      // facing down
      else
      {
        fXStepTable[i] = (float)(-TILE/fTanTable[i]);
      }

      // FACING LEFT
      if (i > ANGLE90 && i < ANGLE270)
      {
        fYStepTable[i] = (float)(TILE*fTanTable[i]);
      }
      // FACING Right
      else
      {
        fYStepTable[i] = (float)(-TILE*fTanTable[i]);
      }

      fFloorTable[i] = (screen_distance*(TILE/2*XY_ratio))/fCosTable[i];
      for(int j = 0;j<SCREEN_Y;j++)
      {
          fFloorTable2[i][j] = fFloorTable[i]/j;
       /*   fFloorSinTable[i][j] = new float[ANGLE360+1];
          fFloorCosTable[i][j] = new float[ANGLE360+1];
          for (int k = 0; k <= ANGLE360;k++)
          {
              fFloorSinTable[i][j][k] = fFloorTable2[i][j] * fSinTable[k];
              fFloorCosTable[i][j][k] = fFloorTable2[i][j] * fCosTable[k];
          }
     */
      }
    }


    //Lighting
    bool toggle_l = false;
    int pixel;
    int light_decrement;
    int red, green, blue;
    int we = 0;
    int frame_counter = 0;
    int FOV_speed = 1;
    int middle_angle;
    int speed = SPEED;
    bool full_screen = false;
    int height = 0;
    int keyhold = FALSE;
    //player
    basicmovable player;
    bool movement;
    //2d
    int x_scroll = 0;
    int y_scroll = 0;
    //mouse commands
    int horizontal_scroll_speed = 5;
    int vertical_scroll_speed = 2;
    //3d wall casting
    int PlayerArc = ANGLE90;
    int castArc;
    float camera_angle;
    int camera_y,camera_x;
    //3d sprites
    //efficiency constants
    int player_xcam;
    int player_ycam;
    int player_xpos;
    int player_ypos;
    int player_i[2];
    int player_j[2];
    int tile_hit;
    float sprite_cos;
    //vector<int> sprite_distance;
    int w_w_ratio;
    int draw_start;
    int draw_end;
    int sprite_x;
    int sprite_y;
    int sprite_proj_h;
    int sprite_proj_w;
    int sprite_proj_x;
    int sprite_proj_y;
    int sprite_arc;
    float sprite_size;
    float sprite_angle;
    float Player_Angle;
    float Item_Angle;
    float First_Angle;
    float Second_Angle;
    //===============
    
    int PlayerMove;
    position ray;
    float vx;
    float vy;
    float hx;
    float hy;
    int di;
    int dj;
    int show3D = TRUE;
    int keyNotZ = FALSE;
    int sizeof_tile;
    float x_i;
    float y_i;
    double horizontal_d;
    double vertical_d;
    double displacement;
    const float projr = XY_ratio*TILE*screen_distance;
    int h_ray;
    int Middle = (SCREEN_Y/2);
    int texture;
    //Sprites
    int Z_Index[SCREEN_X];
    vector<Sprite> sprites;
    //floor tile
    int tile_hit_h;
    int tile_hit_v;
    int tile_texture;

    double constant;
    double constantcos;
    double constantsin;
    double floor_actual_d;
    int floor_angle;
    int floor_hit_x;
    int floor_hit_y;
    int floor_hit_i;
    int floor_hit_j;
    int floor_texture[2];
    int floor_pixel = makecol(255,255,255);
    double b_m_table[SCREEN_Y];
    for(int i = 1;i<SCREEN_Y;i++)
    {
        b_m_table[i] = i;
    }
     //Objects
    vector<object> Item_set(1);
    vector<object> Display_Sprite(1);
    //ENEMIES
    vector<object> enemies(MAX_E);
    int spawn_enemy_counter = 0;
    if(MAX_E !=0)
    {
        for(int z = 0;z<MAX_E/2;z++) enemies[z].spawn(64+z*(UNIT+1),128,0,z);
        for(int z = MAX_E/2;z<MAX_E;z++) enemies[z].spawn(64+(z-MAX_E/2)*(UNIT+1),256,1,z);
    }
   /* for(int x = 0; x<enemies.size();x++)
    {
        // translate position to viewer space
  	    if(enemies[x].enemynum == x) cout << x << "true" << endl;

    }*/
    //terrain
    vector<terrain> tile;
    Load_Map(1,tile,Item_set);

    //Collision
	int xspeed = 0;
	int yspeed = 0;
	//setting the size of tile
	sizeof_tile = tile.size();
	//counter
    int ticker_1 = 0;
    //Shooting
    int current_gun = 0;
    bool Shooting = false;
    bool Shot = false;
    int enemy_take_damage = 0;
    int dot_x = SCREEN_X/2;
    int dot_y = SCREEN_Y/2;
    bool mouse_hold = false;
    int gun_damage = 0;
    int current_recoil = 0;

    while(!key[KEY_ESC])
    {
		while(speed_counter > 0)
        {
        	xspeed = 0;
        	yspeed = 0;
            player.isMove = FALSE;
    		if(key[KEY_Z] && keyNotZ == FALSE){
                keyNotZ = TRUE;
                if(show3D == TRUE)
                    show3D = FALSE;
                else if(show3D == FALSE)
                    show3D = TRUE;
    		}
    		else if(!key[KEY_Z])
    		{
                keyNotZ = FALSE;
            }
            if(abs(mouse_x - ACTUAL_X/2) !=0 || abs(mouse_y - ACTUAL_Y/2) !=0)
            {
                PlayerArc -= (mouse_x - ACTUAL_X/2)/horizontal_scroll_speed;
                Middle -= (mouse_y - ACTUAL_Y/2)/vertical_scroll_speed;
                position_mouse(ACTUAL_X/2,ACTUAL_Y/2);
            }
            
    		if(PlayerArc < 0)
                PlayerArc += ANGLE360;
    		if(PlayerArc > ANGLE360)
                PlayerArc -= ANGLE360;

           if(Middle < 1)
               Middle = 1;
           else if(Middle > SCREEN_Y)
               Middle = SCREEN_Y;
               
            if(mouse_b & 1 && Shooting == false && (gundata[current_gun].isauto() == true || mouse_hold == false))
            {
                Shooting = true;
                mouse_hold = true;
                enemy_take_damage = -1;
            }
            if(ticker_1%2==0 && current_recoil > gundata[current_gun].r_min())current_recoil--;
            
            if(!(mouse_b & 1))
            {
                mouse_hold = false;
            }
            //moving ========================================================
            yspeed = 0;
            xspeed = 0;
            movement = false;

    		if(key[KEY_W]){
                movement = true;
                PlayerMove = PlayerArc;
            }
    		else if(key[KEY_S]){
                PlayerMove =PlayerArc - ANGLE180;
                movement = true;
    		}
    		else if(key[KEY_D]){
                PlayerMove = PlayerArc - ANGLE90;
                movement = true;
    		}
    		else if(key[KEY_A]){
                PlayerMove = PlayerArc + ANGLE90;
                movement = true;
    		}    		
    		
    		if(key[KEY_W] && key[KEY_A])PlayerMove = PlayerArc + ANGLE45;
    		if(key[KEY_W] && key[KEY_D])PlayerMove = PlayerArc - ANGLE45;
    		if(key[KEY_S] && key[KEY_A])PlayerMove =PlayerArc - ANGLE180 - ANGLE45;
    		if(key[KEY_S] && key[KEY_D])PlayerMove =PlayerArc - ANGLE180 + ANGLE45;
    		if(movement == true)
    		{
        		if(PlayerMove < 0)
                    PlayerMove += ANGLE360;
        		if(PlayerMove > ANGLE360)
                    PlayerMove -= ANGLE360;
                xspeed = Round((double)speed * fCosTable[PlayerMove]);
                yspeed = -Round((double)speed * fSinTable[PlayerMove]);
            }
            collide_detection(player.xpos(),player.ypos(), tile, enemies,player,xspeed, yspeed, -1);
            player.x(xspeed);
            player.y(yspeed);
            player_xcam = player.xcam();
            player_ycam = player.ycam();
            player_xpos = player.xpos();
            player_ypos = player.ypos();
            player_i[0] = (player_xcam/TILE)*TILE;
            player_j[0] = (player_ycam/TILE)*TILE;
            player_i[1] = player_i[0] +TILE;
            player_j[1] = player_j[0] +TILE;
            //===============================================================
            for(int x = 0; x<enemies.size();x++)
            {
                if(!enemies[x].isHurt()  && !enemies[x].isDead() && enemies[x].distance > 5)
                {
                    xspeed = -Round((double)enemies[x].speed * fCosTable[enemies[x].dir]);
                    yspeed = -Round((double)enemies[x].speed * fSinTable[enemies[x].dir]);
                    collide_detection(enemies[x].xpos(),enemies[x].ypos(), tile,enemies,player, xspeed, yspeed, x);
              	    enemies[x].x(enemies[x].xpos()+xspeed);
              	    enemies[x].y(enemies[x].ypos()+yspeed);
                    if(xspeed == 0 && yspeed == 0) enemies[x].FreezeFrame();
                    else enemies[x].nextFrame();
                }
                else enemies[x].FreezeFrame();
            }

            if(xspeed != 0 || yspeed != 0)
            {
                player.isMove = TRUE;
            }


            if(key[KEY_Q]  && keyhold && Shooting == false)
            {
                current_gun--;
                keyhold = FALSE;
                if(current_gun<0)current_gun = 2;
                if(current_recoil < gundata[current_gun].r_min())current_recoil=gundata[current_gun].r_min();
            }
            if(key[KEY_E] && keyhold && Shooting == false)
            {
                current_gun++;
                keyhold = FALSE;
                if(current_gun>2)current_gun = 0;
                if(current_recoil < gundata[current_gun].r_min())current_recoil=gundata[current_gun].r_min();
            }
            if(key[KEY_F]  && keyhold)
            {
                keyhold = FALSE;
                if(toggle_l)
                    toggle_l = false;
                else
                    toggle_l = true;
            }
            if(key[KEY_T] && keyhold)
            {
              /* light_level++;
                if(light_level > 5) light_level = 1;
                keyhold = FALSE;
                system("cls");
                cout << "Light Level:" << light_level << endl;
                cout << "Narrow Level:" << flash_light << endl;
              */
                for(int z = 0;z<MAX_E/2;z++) enemies[z].spawn(64+z*35+1,128,0,z);
                for(int z = MAX_E/2;z<MAX_E;z++) enemies[z].spawn(64+(z-MAX_E/2)*35,256,1,z);
            }
            if(key[KEY_G]&& keyhold)
            {
                flash_light++;
                if(flash_light > 5) flash_light = 1;
                keyhold = FALSE;
                system("cls");
                cout << "Light Level:" << light_level << endl;
                cout << "Narrow Level:" << flash_light << endl;
            }
            if(!key[KEY_G] && !key[KEY_T] && !key[KEY_F] && !key[KEY_Q] && !key[KEY_E]) keyhold = TRUE;
    		if(key[KEY_O])
    		{
                ACTUAL_X = 1200;
                ACTUAL_X = 900;
                set_gfx_mode(GFX_AUTODETECT_WINDOWED, ACTUAL_X, ACTUAL_Y, 0, 0);
                horizontal_scroll_speed = 5;
                vertical_scroll_speed = 3;
                full_screen = false;
            }
            else if(key[KEY_P])
            {
                ACTUAL_X = 800;
                ACTUAL_Y = 600;
                set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, ACTUAL_X, ACTUAL_Y,0,0);
                horizontal_scroll_speed = 5;
                vertical_scroll_speed = 5;
                full_screen = true;
            }


            x_scroll = player.xpos() - SCREEN_X/2;
            y_scroll = player.ypos() - SCREEN_Y/2;
            //counters---------------------------
            if(Shooting == true)ticker_1++;
            else ticker_1 = 0;
            if(ticker_1 > gundata[current_gun].frame_cd() && Shooting == true)
            {
                player.state++;
                if(player.state == gundata[current_gun].frame_shoot())
                {
                    Shot = true;
                    if(current_gun != 0)
                    {
                        dot_x = SCREEN_X/2 + rand()%(current_recoil*2)-current_recoil;
                        dot_y = SCREEN_Y/2 + rand()%(current_recoil*2)-current_recoil;
                        current_recoil+=gundata[current_gun].r_incre();
                        if(current_recoil > gundata[current_gun].r_max())current_recoil = gundata[current_gun].r_max();
                        light_level++;
                        flash_light++;
                        Middle+=gundata[current_gun].recoil()+current_recoil/4;
                    }
                }
                if(player.state == gundata[current_gun].frame_shoot()+1)
                {
                    if(current_gun != 0)
                    {
                        light_level--;
                        flash_light--;
                        Middle-=gundata[current_gun].recoil()+current_recoil/4;
                    }
                }
                if(player.state>gundata[current_gun].max_frame())
                {
                    player.state=0;
                    Shooting = false;
                    
                }
                ticker_1 = 0;                
            }
            speed_counter --;
        }
        castArc = PlayerArc;
        castArc+= ANGLE30;
        if (castArc > ANGLE360)
        {
          castArc-=ANGLE360;
        }
        //3D display---------------------------------------
        for(int a = 0; a < SCREEN_X; a++){
            //Horizontal Intersection-----------------------------------------------------------------------------
            if(castArc > ANGLE0 && castArc < ANGLE180){ //Facing Up
                vy = player_j[0];
                y_i = -TILE;
                vx = player.xcam() + (player.ycam()-vy)*fITanTable[castArc];
                vy--;
            }
            else//Facing Down
            {
                vy = player_j[1];
                y_i = TILE;
                vx = player_xcam + (player_ycam-vy)*fITanTable[castArc];
            }
            x_i = fXStepTable[castArc];
            di = (int)vx/TILE;
            dj = (int)vy/TILE;
            tile_hit = dj*incre+di;
            while((tile_hit > 0 && tile_hit<sizeof_tile) && tile[tile_hit].type <= 0 )
            {
                vx += x_i;
                vy += y_i;
                di = (int)vx/(TILE);
                dj = (int)vy/TILE;
                tile_hit = dj*incre+di;
            }
            tile_hit_h = tile_hit;
            horizontal_d = (vx-player.xcam())*(vx-player.xcam())+(vy-player.ycam())*(vy-player.ycam());


            //Vertical Intersection----------------------------------------------------------------------------------------
            if (castArc > ANGLE90 && castArc < ANGLE270){ //Facing Left
                hx = player_i[0];
                x_i = -TILE;
                hy = player.ycam() + (player.xcam()-hx)*fTanTable[castArc];
                hx--;
            }
            else //Facing Right
            {
                hx = player_i[1];
                x_i = TILE;
                hy = player.ycam() + (player.xcam()-hx)*fTanTable[castArc];
            }
            y_i = fYStepTable[castArc];
            di = (int)hx/TILE;
            dj = (int)hy/TILE;
            tile_hit = dj*incre+di;
            while((tile_hit > 0 && tile_hit<sizeof_tile) && tile[tile_hit].type <= 0 )
            {
                hx += x_i;
                hy += y_i;
                di = (int)hx/TILE;
                dj = (int)(hy)/TILE;
                tile_hit = dj*incre+di;
            }
            tile_hit_v = tile_hit;
            vertical_d = (hx-player.xcam())*(hx-player.xcam())+(hy-player.ycam())*(hy-player.ycam());
            //final distance
            if(vertical_d < horizontal_d)
            {
                displacement = sqrt(vertical_d);
                tile_texture = tile[tile_hit_v].type*2-2;
                
             //   if(show3D == FALSE)
             //       line(buffer,(int) player.xcam()-x_scroll,(int) player.ycam()-y_scroll, (int)hx-x_scroll, (int)hy -y_scroll, GREEN);
            }
            else
            {
                displacement = sqrt(horizontal_d);
                tile_texture =  tile[tile_hit_h].type*2-1;
                
             //   if(show3D == FALSE)
            //        line(buffer,(int) player.xcam()-x_scroll,(int) player.ycam()-y_scroll, (int)vx-x_scroll, (int)vy -y_scroll, BLUE);
                    
            }
            floor_angle = castArc-PlayerArc;
            if(floor_angle < 0) floor_angle+=ANGLE360;
            //Sprites
            Z_Index[a] = (int)displacement;
            light_decrement = determine_d((int)displacement)+abs(SCREEN_X/2-a)/flash_light;
            displacement = displacement * fCosTable[floor_angle];
  
            //displacement = displacement * cos(floor_angle*PI/180);
            //Determine the height
            h_ray = int(projr/displacement);

            camera_y = Middle - abs(h_ray)/2;
            ///Display  ----------------------------
            if(light_decrement < 255 && toggle_l == true){
               if(vertical_d < horizontal_d)
                    texture = abs(int(hy)) % TILE;
                else
                    texture = abs(int(vx)) % TILE;
                    
                draw_lit_sprite(wall_temp, tile_map[tile_texture], 0, 0, light_decrement);
                stretch_blit(wall_temp, buffer,texture,0, texture+1,TILE, a, camera_y, 1, h_ray);
            }
            else if(toggle_l == false)
            {
               if(vertical_d < horizontal_d)
                    texture = abs(int(hy)) % TILE;
                else
                    texture = abs(int(vx)) % TILE;
                stretch_blit(tile_map[tile_texture], buffer,texture,0, texture+1,TILE, a, camera_y, 1, h_ray);
            }
            //Floor Casting -----------------------------------------------------------
            int i;
            
            constantcos = fCosTable[castArc];
            constantsin = fSinTable[castArc];
            if(Middle > SCREEN_Y/2)
            {
                i = 2*camera_y+h_ray-1;
                for(int b = 0; b < camera_y; b++)
                {
                    floor_actual_d = fFloorTable2[floor_angle][abs(Middle-b)];
                    floor_texture[0] = (int)(player_xcam+floor_actual_d*constantcos)%TILE;
                    floor_texture[1] = (int)(player_ycam-floor_actual_d*constantsin)%TILE;
                    floor_hit_i = (int)(player_xcam+floor_actual_d*constantcos)/64;
                    floor_hit_j = (int)(player_ycam-floor_actual_d*constantsin)/64;
                    if(floor_hit_j*incre+floor_hit_i < 0) cout << floor_hit_j<<":"<<floor_hit_i;
                    pixel = getpixel(tile_map[tile[floor_hit_j*incre+floor_hit_i].floortype],floor_texture[0], floor_texture[1]);
                    if(toggle_l == true)
                    {
                        light_decrement = (int)floor_actual_d/light_level+abs(SCREEN_X/2-a)/flash_light;
                        if(light_decrement < 255)
                        {
                            red = getr(pixel) - light_decrement;
                            green = getg(pixel) - light_decrement;
                            blue = getb(pixel) - light_decrement;
                            if(red < 0) red = 0;
                            if(green < 0) green = 0;
                            if(blue < 0) blue = 0;
                            pixel = makecol(red,green,blue);
                        }
                        else pixel = BLACK;
                    }
                    if(pixel != BLACK){
                        putpixel(buffer, a,i, pixel);
                        putpixel(buffer, a,b, pixel);
                    }
                    //getpixel(ceiling,floor_texture[0], floor_texture[1])
                    
                    i--;
                }
            }
            else
            {
                i = 1;
                for(int b = (camera_y+h_ray); b < SCREEN_Y; b++)
                {
                    floor_actual_d = fFloorTable2[floor_angle][abs(Middle-b)];
                    floor_hit_x = abs((int)(player_xcam+floor_actual_d*constantcos));
                    floor_hit_y = abs((int)(player_ycam-floor_actual_d*constantsin));
                    floor_texture[0] = floor_hit_x%TILE;
                    floor_texture[1] = floor_hit_y%TILE;
                    floor_hit_i = floor_hit_x/64;
                    floor_hit_j = floor_hit_y/64;
                    pixel = getpixel(tile_map[tile[floor_hit_j*incre+floor_hit_i].floortype],floor_texture[0], floor_texture[1]);
                    if(toggle_l == true)
                    {
                        light_decrement = (int)floor_actual_d/light_level+abs(SCREEN_X/2-a)/flash_light;
                        if(light_decrement < 255)
                        {
                            red = getr(pixel) - light_decrement;
                            green = getg(pixel) - light_decrement;
                            blue = getb(pixel) - light_decrement;
                            if(red < 0) red = 0;
                            if(green < 0) green = 0;
                            if(blue < 0) blue = 0;
                            pixel = makecol(red,green,blue);
                        }
                        else pixel = BLACK;
                    }
                    if(pixel != BLACK){
                        putpixel(buffer, a,b, pixel);
                        putpixel(buffer, a,camera_y-i, pixel);
                    }
                    i++;
                }
            }
            castArc--;
            if (castArc < 0)
                castArc+=ANGLE360;
        }
        Player_Angle = arcToAngle(PlayerArc);
        if(Player_Angle<0)Player_Angle+=360;
        
        //This is for calculations
        for(int x = 0; x<size_of_object;x++)
        {
            // translate position to viewer space
      	    Item_set[x].sx = Item_set[x].xcam() - player.xcam();
        	Item_set[x].sy = Item_set[x].ycam() - player.ycam();
        	// distance to sprite
        	Item_set[x].distance = sqrt(Item_set[x].sx*Item_set[x].sx + Item_set[x].sy*Item_set[x].sy);
        }
        for(int x = 0; x<enemies.size();x++)
        {
            // translate position to viewer space
      	    enemies[x].sx = enemies[x].xcam() - player.xcam();
        	enemies[x].sy = enemies[x].ycam() - player.ycam();
        	// distance to sprite
        	enemies[x].distance = sqrt(enemies[x].sx*enemies[x].sx + enemies[x].sy*enemies[x].sy);
        	enemies[x].dir = AngletoArc(ARCTAN(enemies[x].sx,enemies[x].sy));
        
        }
        Sort_Sprite(Item_set,Display_Sprite,enemies);
        for(int x = 0; x<enemies.size();x++)enemies[x].enemynum = x;
        //This is for drawing
    	for (int x = 0;x<Display_Sprite.size();x++) {
            if(Display_Sprite[x].distance > 20)
            {
        		// sprite angle relative to viewing angle
        		Item_Angle = ARCTAN(Display_Sprite[x].sx,-Display_Sprite[x].sy);
                sprite_angle =  Player_Angle-Item_Angle;
                sprite_cos = cos(sprite_angle*PI/180);
                if(sprite_angle > 180) sprite_angle -= 360;
                else if(sprite_angle < -180) sprite_angle +=360;
                
                if(Display_Sprite[x].movable == false)
                {
            		// size of the sprite
            		sprite_size = screen_distance / (sprite_cos*Display_Sprite[x].distance);
                    sprite_proj_w = int(sprite_size * Item[Display_Sprite[x].type]->w*XY_ratio);
                    sprite_proj_h = int(sprite_size* Item[Display_Sprite[x].type]->h*XY_ratio);
            		// x-position on screen
            		sprite_proj_x = (int)(SCREEN_X/2+tan(sprite_angle*PI/180)*screen_distance*XY_ratio-sprite_proj_w/2);
                    sprite_proj_y = (int)(Middle-sprite_proj_h/2);
                    draw_start = sprite_proj_x;
                    draw_end = sprite_proj_x + sprite_proj_w;

                    if(draw_start < SCREEN_X && draw_end >0){
                        if(draw_start < 0) draw_start = 0;
                        if(draw_end >= SCREEN_X)draw_end = SCREEN_X-1;
                        while(Z_Index[draw_start] < Display_Sprite[x].distance)
                        {
                           draw_start++;
                           if(draw_start < SCREEN_X-1 && draw_start < draw_end)
                               draw_start++;
                           else
                               break;
                        }
                        draw_end = draw_start;
                        while(Z_Index[draw_end] > Display_Sprite[x].distance)
                        {
                           draw_end++;
                           if(draw_start < SCREEN_X-1 && draw_end-sprite_proj_x <= sprite_proj_w)
                               draw_end++;
                           else
                           {
                               draw_end--;
                               break;
                           }
                        }
                        
                        if(draw_start < draw_end)
                        {
                            light_decrement = determine_d((int)Display_Sprite[x].distance)+abs(AngletoArc(sprite_angle))/flash_light;
                            s_temp = create_bitmap(sprite_proj_w,sprite_proj_h);
                            stretch_blit(Item[Display_Sprite[x].type], s_temp,0,0, Item[Display_Sprite[x].type]->w,Item[Display_Sprite[x].type]->h, 0, 0, sprite_proj_w,sprite_proj_h);

                            if(Shot == true)
                            {
                                if(draw_start <= dot_x && draw_end > dot_x && sprite_proj_y <= dot_y && (sprite_proj_y+sprite_proj_h)>dot_y)
                                {
                                    floor_pixel = getpixel(s_temp,dot_x-sprite_proj_x,dot_y-sprite_proj_y);
                                    if( floor_pixel!= PURPLE)
                                    {
                                        enemy_take_damage = -1;
                                    }

                                }
                            }

                            if(toggle_l == true && light_decrement > 0 &&  light_decrement < 255){
                                draw_lit_sprite(s_temp, s_temp, 0, 0,light_decrement);
                                masked_blit(s_temp, buffer,draw_start-sprite_proj_x,0, draw_start,sprite_proj_y,(draw_end-draw_start),sprite_proj_h);
                            }
                            else if(toggle_l == false || light_decrement == 0)
                                masked_blit(s_temp, buffer,draw_start-sprite_proj_x,0, draw_start,sprite_proj_y,(draw_end-draw_start),sprite_proj_h);
                            destroy_bitmap(s_temp);
                        }
                    }
                }
                else if(sprite_cos>0.40 && (Display_Sprite[x].distance > 40 || Display_Sprite[x].death_animation || !Display_Sprite[x].isDead()))//drawing enemies
                {
            		//size of the sprite
            		sprite_size = screen_distance / (sprite_cos*Display_Sprite[x].distance);//*
                    sprite_proj_w = int(sprite_size * ENEMY[Display_Sprite[x].type][0]->w*XY_ratio);
                    sprite_proj_h = int(sprite_size* ENEMY[Display_Sprite[x].type][0]->h*XY_ratio);
            		// x-position on screen
            		sprite_proj_x = (int)(SCREEN_X/2+tan(sprite_angle*PI/180)*screen_distance*XY_ratio-sprite_proj_w/2);
                    sprite_proj_y = (int)(Middle-sprite_proj_h/2);
                    draw_start = sprite_proj_x;
                    draw_end = sprite_proj_x + sprite_proj_w;

                    if(draw_start < SCREEN_X && draw_end >0){
                        if(draw_start < 0) draw_start = 0;
                        if(draw_end >= SCREEN_X)draw_end = SCREEN_X-1;
                        while(Z_Index[draw_start] < Display_Sprite[x].distance)
                        {
                           draw_start++;
                           if(draw_start < SCREEN_X-1 && draw_start < draw_end)
                               draw_start++;
                           else
                               break;
                        }
                        draw_end = draw_start;
                        while(Z_Index[draw_end] > Display_Sprite[x].distance)
                        {
                           draw_end++;
                           if(draw_start < SCREEN_X-1 && draw_end-sprite_proj_x <= sprite_proj_w)
                               draw_end++;
                           else
                           {
                               draw_end--;
                               break;
                           }
                        }
                        if(draw_start < draw_end)
                        {
                            light_decrement = determine_d((int)Display_Sprite[x].distance)+abs(AngletoArc(sprite_angle))/flash_light;
                            s_temp = create_bitmap(sprite_proj_w,sprite_proj_h);
                            if(sprite_proj_w > 2000) {cout << sprite_cos << endl;cout << Display_Sprite[x].distance << endl;}
                            stretch_blit(ENEMY[Display_Sprite[x].type][Display_Sprite[x].frame()], s_temp,0,0, ENEMY[Display_Sprite[x].type][0]->w,ENEMY[Display_Sprite[x].type][0]->h, 0, 0, sprite_proj_w,sprite_proj_h);

                            if(!Display_Sprite[x].isDead() && Shot == true)
                            {
                                if(current_gun !=0 &&  draw_start <= dot_x && draw_end > dot_x && sprite_proj_y <= dot_y && (sprite_proj_y+sprite_proj_h)>dot_y)
                                {
                                    floor_pixel = getpixel(s_temp,dot_x-sprite_proj_x,dot_y-sprite_proj_y);
                                    if( floor_pixel!= PURPLE)
                                    {
                                        enemy_take_damage = Display_Sprite[x].enemynum;
                                        if(!Display_Sprite[x].isDead())
                                        {
                                          if(dot_y-sprite_proj_y<28*sprite_proj_h/ENEMY[Display_Sprite[x].type][0]->h)gun_damage = gundata[current_gun].damage_h();
                                          else gun_damage = gundata[current_gun].damage_n();
                                        }
                                    }

                                }
                                else
                                {
                                    if(Display_Sprite[x].distance < 70 && abs(sprite_angle) < 25)
                                    {
                                        enemy_take_damage = Display_Sprite[x].enemynum;
                                        if(abs(sprite_angle) < 10)gun_damage = gundata[current_gun].damage_h();
                                        else  gun_damage = gundata[current_gun].damage_n();
                                    }
                                }
                            }
                            if(toggle_l == true && light_decrement > 0 &&  light_decrement < 255){
                                draw_lit_sprite(s_temp, s_temp, 0, 0,light_decrement);
                                masked_blit(s_temp, buffer,draw_start-sprite_proj_x,0, draw_start,sprite_proj_y,(draw_end-draw_start),sprite_proj_h);
                            }
                            else if(toggle_l == false || light_decrement == 0)
                                masked_blit(s_temp, buffer,draw_start-sprite_proj_x,0, draw_start,sprite_proj_y,(draw_end-draw_start),sprite_proj_h);
                            destroy_bitmap(s_temp);
                        }
                    }
                }
            }
    	}
        if(Shot == true && enemy_take_damage != -1)
        {   
            enemies[enemy_take_damage].hit(gun_damage);
        }
        Shot = false;
        masked_stretch_blit(weapon[current_gun][player.state], buffer,0,0, 64,64, SCREEN_X/2-SCREEN_X/4, SCREEN_Y-SCREEN_X/2, SCREEN_X/2, SCREEN_X/2);
       // draw_sprite(buffer, cursor, mouse_x,mouse_y);
        //draw_sprite(buffer, weapon[0][0], 0,SCREEN_Y-hand->w);
        putpixel(buffer,dot_x,dot_y,WHITE);
        if(current_gun != 0)
        {
            draw_sprite(buffer, cross_h, SCREEN_X/2-cross_h->w-current_recoil,SCREEN_Y/2-cross_h->h/2-1);
            draw_sprite(buffer, cross_h, SCREEN_X/2+current_recoil,SCREEN_Y/2-cross_h->h/2-1);
            draw_sprite(buffer, cross_v, SCREEN_X/2-cross_v->w/2,SCREEN_Y/2-cross_v->h-current_recoil);
            draw_sprite(buffer, cross_v, SCREEN_X/2-cross_v->w/2,SCREEN_Y/2+current_recoil);
        }
        if(show3D == FALSE)
        {
            for(int i = 0; i < tile.size(); i++)
            {
                if(tile[i].type > 0)
                    masked_stretch_blit(Brick, buffer,0,0, Brick->w,Brick->h, tile[i].xpos() - x_scroll, tile[i].ypos() - y_scroll, TILE, TILE);
                else if(tile[i].type == 0)
                    masked_stretch_blit(floor_tile, buffer,0,0, Ground->w,Ground->h, tile[i].xpos() - x_scroll, tile[i].ypos() - y_scroll, TILE, TILE);
                else
                    masked_stretch_blit(Ground, buffer,0,0, Ground->w,Ground->h, tile[i].xpos() - x_scroll, tile[i].ypos() - y_scroll, TILE, TILE);
            }
            for(int x = 0; x<enemies.size();x++)
            {
                if(!enemies[x].isDead())
                    masked_stretch_blit(ceiling, buffer,0,0, 32,32,enemies[x].xpos() - x_scroll ,enemies[x].ypos() - y_scroll, UNIT,UNIT);
            }
            masked_stretch_blit(p1, buffer,0,0, 32,32,player.xpos() - x_scroll ,player.ypos() - y_scroll, UNIT,UNIT);
        }
        if(full_screen == false)
            stretch_blit(buffer, screen,0,0, SCREEN_X,SCREEN_Y, 0, 0, ACTUAL_X,ACTUAL_Y);
        else
            stretch_blit(buffer, screen,0,0, SCREEN_X,SCREEN_Y, 0, 0, 800,600);
        clear_bitmap(buffer);
	}			
	destroy_bitmap(buffer);
    return(0);
}END_OF_MAIN();


void Load_Map(int stage, vector<terrain> &load, vector<object> &temp)
{
    char sFile[10];
    char type;
    bool floor = false;
    int tile,i = 0,j = 0;
    size_of_object = 0;
    load.resize(1);
    sprintf(sFile,"DATA/MAP/map%d.txt",stage); //loads the map base on the level
    ifstream fptr(sFile);

    if (fptr == NULL){ //Error checking
        set_gfx_mode(GFX_TEXT,0,0,0,0);
        allegro_message("Could not Open Level!");
        exit(EXIT_FAILURE);
    }
    else{
         //read from file
        fptr >> incre;
        while(!fptr.eof())
        {
            floor = false;
            fptr >> type;
            if(type >= 'a' && type <='z')
            {
                size_of_object++;
                temp.resize(size_of_object);
                temp[size_of_object-1].initialize(i,j,type-'a');
                type = '0'-1;
            }
            if(type == '+')
            {
                floor = true;
                type = '0';
            }
            tile = type - '0';
            
            load.resize(i*incre+j+1);
            load[i*incre+j].initialize(i,j,tile);
            load[i*incre+j].floortype = ((i+j*incre)*2)%110;
            
            j++;
            if(j >= incre)
            {
                i++;
                j = 0;
            }
        }

    }
}
END_OF_FUNCTION(Load_Map)


void collide_detection(int xpos, int ypos, vector<terrain> tile,vector<object> enemies, basicmovable &player,int &xmove, int &ymove, int index)
{
    int xmove_temp,ymove_temp;
    int origin_i, origin_d;
    int min = FALSE;
    int case1 = 0;
    bool flag;
    int newx;
    int newy;
    int di;
    int dj;
    int di2;
    int dj2;
    //tile[dj*incre+di].type
    if(abs(xmove) > 0)
    {
        newy = ypos;
        dj = newy/TILE;
        dj2 = (newy+UNIT)/TILE;
        
        newx = (xpos + xmove);
        di = newx/TILE;
        di2 = (newx+UNIT)/TILE;
        origin_i = xpos/TILE;
        
        if(tile[dj*incre+di].type!=0 ||tile[dj2*incre+di].type!=0 ||tile[dj*incre+di2].type!=0||tile[dj2*incre+di2].type!=0)
        {
            if(xmove > 0)xmove = (origin_i+1)*TILE-xpos-UNIT-1;
            else xmove = origin_i*TILE-xpos;
        }
    }
    
    if(abs(ymove) > 0)
    {
        newx = xpos;
        di = newx/TILE;
        di2 = (newx+UNIT)/TILE;
        
        newy = (ypos + ymove);
        origin_d = ypos/TILE;
        dj = newy/TILE;
        dj2 = (newy+UNIT)/TILE;

        if(tile[dj*incre+di].type!=0 ||tile[dj2*incre+di].type!=0 ||tile[dj*incre+di2].type!=0||tile[dj2*incre+di2].type!=0)
        {
            if(ymove > 0)ymove = (origin_d+1)*TILE-ypos-UNIT-1;
            else ymove = origin_d*TILE-ypos;
        }
    }
    
    if(xmove != 0 || ymove!=0)
    {
        for(int i =  0 ; i < enemies.size(); i++)
        {
            if(!(enemies[i].isDead()) || enemies[i].death_animation)
            {
                if(index != i)
            	{
                    while(xmove != 0 && collide(xpos+xmove, ypos,UNIT,UNIT,enemies[i].xpos(), enemies[i].ypos(),UNIT,UNIT))
                    {
                        if(xmove > 0) xmove--;
                        else xmove++;
                    }
                    while(ymove != 0 && collide(xpos, ypos+ymove,UNIT,UNIT,enemies[i].xpos(), enemies[i].ypos(),UNIT,UNIT))
                    {
                        if(ymove > 0) ymove--;
                        else ymove++;
                    }
                }
            }
        }
    }
    if(index != -1 && (xmove != 0 || ymove!=0))
    {
        while(abs(xmove) > 0 && collide(xpos+xmove, ypos,UNIT,UNIT,player.xpos(), player.ypos(),UNIT,UNIT))
        {
            if(xmove > 0) xmove--;
            else xmove++;
        }
        while(abs(ymove) > 0 && collide(xpos, ypos+ymove,UNIT,UNIT,player.xpos(), player.ypos(),UNIT,UNIT))
        {
            if(ymove > 0) ymove--;
            else ymove++;
        }
    }
}

//---------------------------------------------------------------------------------------
int determine_d(int d)
{
    int increment = abs(2*d/light_level);
    if(increment < 50)
    {
        return 0;
    }
    else if(increment < 255)
    {
        return increment;
    }
    else
        return 255;
}

void Sort_Sprite(vector<object> &temp1, vector<object> &sprite, vector<object> &temp2)
{
    object hold;
    int max;
    int i1 = 0,i2 = 0;
    sprite.resize(temp2.size()+temp1.size());
    for(int i = 0;i<temp1.size()-1;i++)
    {
        max = i;
        for(int j = i;j<temp1.size();j++)
        {
            if(temp1[max].distance < temp1[j].distance)
                max = j;
        }
        hold = temp1[i];
        temp1[i] = temp1[max];
        temp1[max] = hold;
    }
    for(int i = 0;i<temp2.size()-1;i++)
    {
        max = i;
        for(int j = i;j<temp2.size();j++)
        {
            if(temp2[max].distance < temp2[j].distance)
                max = j;
        }
        hold = temp2[i];
        temp2[i] = temp2[max];
        temp2[max] = hold;
    }

    for(int i = 0; i < sprite.size(); i++)
    {
        if((i2 < temp2.size()) && (i1 < temp1.size())) //case 1: both array are not finished
        {
            if(temp1[i1].distance < temp2[i2].distance){ //comparsion will be made
                sprite[i] = temp2[i2];
                i2++;
            }
            else{
                sprite[i] = temp1[i1];
                i1++;
            }
        }
        else //case 2: one of the array is finished
        {
            if(i1 >= temp1.size()){  //if the first array is finished
                sprite[i] = temp2[i2];
                i2++;
            }
            else{ //if the second array is finished
                sprite[i] = temp1[i1];
                i1++;
            }
        }
    }

}
bool collide(int x1, int y1,int w1,int h1,int x2,int y2,int w2,int h2)
{
    //the code is taken from allegro tutorial and made into a function
    int image1_bb_left = x1;
    int image1_bb_top = y1;
    int image1_bb_right = (image1_bb_left + w1);
    int image1_bb_bottom = (image1_bb_top + h1);
    int image2_bb_left = x2;
    int image2_bb_top = y2;
    int image2_bb_right = (image2_bb_left + w2);
    int image2_bb_bottom = (image2_bb_top + h2);
    if(image1_bb_bottom < image2_bb_top)
    {
        return false;
    }
    else if(image1_bb_top > image2_bb_bottom)
    {
        return false;

    }
    else if(image1_bb_right < image2_bb_left)
    {
        return false;
    }
    else if(image1_bb_left > image2_bb_right)
    {
        return false;
    }
    return true; //determines if it collides or not
}
END_OF_FUNCTION(collide)

int Round(double num)
{
    int floornum = int(num);
    if(num > 0)
    {
        num -= floornum;
        if(num > 0.5)
            return (floornum+1);
        return(floornum);
    }
    else
    {
        num += floornum;
        if(num < -0.5)
            return (floornum-1);
        return(floornum);
    }
}


float arcToRad(float arcAngle)
{
    return ((arcAngle*PI)/ANGLE180);
}

float arcToAngle(float angle)
{
   // cout <<   << endl;
    return (60*angle/SCREEN_X);
}

int AngletoArc(float arc)
{
   // cout <<   << endl;
    return int(arc*SCREEN_X/60);
}

float ARCTAN(float x,float y)
{
    bool mode;
    float incre;
    if(x == 0)
    {
        if(y > 0)
            return 90;
        else
            return 270;
    }
    if(y == 0)
    {
        if(x > 0)
            return 0;
        else
            return 180;
    }
    //determine quad
    if(x > 0)
    {
       if(y > 0)
       {
           incre = 0;
           mode = true;
       }
       else
       {
           incre = 360;
           mode = false;
       }
    }
    else
    {
       incre = 180;
       if(y > 0)
       {
           
           mode = false;
       }
       else
       {
           incre = 180;
           mode = true;
       }
        
    }
    if(mode)
        return atan(abs(y/x))*180/PI+incre;
    return incre - atan(abs(y/x))*180/PI;
}
