//Constants
#define SCREEN_X 800
#define SCREEN_Y 600
#define ACTUAL_X 1200
#define ACTUAL_Y 900
int SPEED = 5;
int FOV  = 50;
int TILE = 64;
int UNIT = 32;
#define PI 3.1415926535898
#define RED makecol(255, 0, 0)
#define PURPLE makecol(255, 0, 255)
#define GREEN makecol(0, 255, 0)
#define BLUE makecol(0, 128, 255)
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
#include "player.cpp"
#include "terrain.cpp"
#include "Object.cpp"


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
bool collide(int x1, int y1,int x2, int y2);
void collide_detection(int xpos, int ypos, vector<terrain> tile, int &xmove, int &ymove);
void Load_Map(int stage, vector<terrain> &load, vector<object> &temp);
int Round(double num);
bool checkDot(int x,int y, vector<terrain> &tile,int xmove,int ymove);
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
    
    //Bitmaps
    BITMAP *buffer = create_bitmap(SCREEN_X, SCREEN_Y);
    if(buffer == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Error: Could not create buffer!");
		exit(EXIT_FAILURE);
	}
    BITMAP *p1 = load_bitmap("BITMAP/Player.bmp", NULL);
    BITMAP *Brick = load_bitmap("BITMAP/Brick6.bmp", NULL);
    BITMAP *Brick2 = load_bitmap("BITMAP/Brick7.bmp", NULL);
    BITMAP *Ground = load_bitmap("BITMAP/grass.bmp", NULL);
    BITMAP *floor = load_bitmap("BITMAP/floor.bmp", NULL);
    BITMAP *floor_tile = load_bitmap("BITMAP/Floortile.bmp", NULL);
    BITMAP *ceiling = load_bitmap("BITMAP/ceil.bmp",NULL);
    BITMAP *cursor = load_bitmap("BITMAP/mouse.bmp",NULL);
    BITMAP *Item[10] = {0};
    Item[0] = load_bitmap("BITMAP/armor.bmp", NULL);
    
    //variables
    int frame_counter = 0;
    int FOV_speed = 1;
    int middle_angle;
    int speed = SPEED;
    bool full_screen = false;
    int aspect = 0;
    int keyhold = FALSE;
    //player
    basicmovable player;
    //2d
    int x_scroll = 0;
    int y_scroll = 0;
    //mouse commands
    int horizontal_scroll_speed = 20;
    int vertical_scroll_speed = 2;
    //3d wall casting
    float angle = 90;
    float camera_angle;
    int camera_y,camera_x;
    const float theta = SCREEN_X/FOV;
    const float screen_distance = (SCREEN_Y/2)/tan((FOV/2)*PI/180);
    const float XY_ratio = float(SCREEN_X)/SCREEN_Y;
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
    const float projr = TILE*screen_distance;
    int h_ray;
    int Middle = (SCREEN_Y/2);
    int texture;
    //Sprites
    int Z_Index[SCREEN_X];
    vector<Sprite> sprites;
    //floor tile
    double constant;
    double constantcos;
    double constantsin;
    double floor_actual_d;
    double floor_angle;
    int floor_texture[2];
    int floor_pixel = makecol(255,255,255);
    double b_m_table[SCREEN_Y];
    for(int i = 1;i<SCREEN_Y;i++)
    {
        b_m_table[i] = i;
    }
     //Objects
    vector<object> Item_set(1);
    //terrain
    vector<terrain> tile;
    Load_Map(1,tile,Item_set);
    Item_set[0].initialize(600,400,0);

    //Collision
	int xspeed = 0;
	int yspeed = 0;
	//setting the size of tile
	sizeof_tile = tile.size();

    while(!key[KEY_ESC])
    {
		while(speed_counter > 0)
        {
        //    cout << player.xpos()<< endl;
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
    		if(angle < 0)
                angle = 360;
    		if(angle > 360)
                angle = 0;
            if(abs(mouse_x - SCREEN_X/2) !=0 || abs(mouse_y - SCREEN_Y/2) !=0)
            {
                angle -= (mouse_x - SCREEN_X/2)/horizontal_scroll_speed;
                Middle -= (mouse_y - SCREEN_Y/2)/vertical_scroll_speed;
                position_mouse(SCREEN_X/2,SCREEN_Y/2);
            }
    		if(key[KEY_R] && aspect<400){
                aspect++;
                keyhold = FALSE;
    		}
    		if(key[KEY_F] && aspect>-400){
                aspect--;
    		}
    		cout << aspect << endl;
           if(Middle < 0)
               Middle = 0;
           else if(Middle > SCREEN_Y)
               Middle = SCREEN_Y;
           /* if(key[KEY_LSHIFT])
            {
                Middle = SCREEN_Y/2-50;
                speed = 3;
            }
            if(!key[KEY_LSHIFT])
            {
                Middle = SCREEN_Y/2;
                speed = 5;
            }
          */
    		if(key[KEY_D]){
                xspeed = Round((double)speed/2 * sin(angle*PI/180));
                yspeed = Round((double)speed/2 * cos(angle*PI/180));
    		} 
    		else if(key[KEY_A]){
                xspeed = -Round((double)speed/2 * sin(angle*PI/180));
                yspeed = -Round((double)speed/2 * cos(angle*PI/180));
    		}
    		else if(key[KEY_W]){
                xspeed = Round((double)speed * cos(angle*PI/180));
                yspeed = -Round((double)speed * sin(angle*PI/180));
            }
    		else if(key[KEY_S]){
                xspeed = -Round((double)speed * cos(angle*PI/180));
                yspeed = Round((double)speed * sin(angle*PI/180));
    		}
    		
    		if(key[KEY_O])
    		{
                set_gfx_mode(GFX_AUTODETECT_WINDOWED, ACTUAL_X, ACTUAL_Y, 0, 0);
                horizontal_scroll_speed = 20;
                vertical_scroll_speed = 3;
                full_screen = false;
            }
            else if(key[KEY_P])
            {
                set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREEN_X, SCREEN_Y, 0, 0);
                horizontal_scroll_speed = 30;
                vertical_scroll_speed = 5;
                full_screen = true;
            }
                
            collide_detection(player.xpos(),player.ypos(), tile, xspeed, yspeed);
            player.x(xspeed);
            player.y(yspeed);
            if(xspeed != 0 || yspeed != 0)
                player.isMove = TRUE;
            x_scroll = player.xpos() - SCREEN_X/2;
            y_scroll = player.ypos() - SCREEN_Y/2;
            //counters---------------------------
            //Dizzy simulator
            //frame_counter++;
            if(frame_counter > 0)
            {
                middle_angle = (int)angle + FOV/2;
                frame_counter = 0;
                if(FOV > 120)
                {
                    FOV_speed = -5;
                }
                else if(FOV < 5)
                {
                    FOV_speed = 5;
                }
                FOV+= FOV_speed;

              // angle = middle_angle - FOV/2;
            }

            speed_counter --;
        }

        if(show3D == FALSE)
        {
            for(int i = 0; i < tile.size(); i++)
            {
                if(tile[i].type != 0)
                    masked_stretch_blit(Brick, buffer,0,0, Brick->w,Brick->h, tile[i].xpos() - x_scroll, tile[i].ypos() - y_scroll, TILE, TILE);
                else
                    masked_stretch_blit(floor_tile, buffer,0,0, Ground->w,Ground->h, tile[i].xpos() - x_scroll, tile[i].ypos() - y_scroll, TILE, TILE);
            }
                masked_stretch_blit(p1, buffer,0,0, 32,32,player.xpos() - x_scroll ,player.ypos() - y_scroll, UNIT,UNIT);
        }

        
        //3D display---------------------------------------
        for(int a = 0; a < SCREEN_X; a++){
            camera_angle = angle+FOV/2 - a/theta;
     		if(camera_angle < 0)
                camera_angle += 360;
    		else if(camera_angle >= 360)
                camera_angle -= 360;

            //Horizontal Intersection-----------------------------------------------------------------------------
            if(camera_angle > 0 && camera_angle < 180){ //Facing Up
                vy = (player.ycam()/TILE) * (TILE);
                y_i = -TILE;
                vx = player.xcam() + (player.ycam()-vy)/tan(camera_angle*PI/180);
                vy--;
            }
            else//Facing Down
            {
                vy = (player.ycam()/TILE) * (TILE) + TILE;
                y_i = TILE;
                vx = player.xcam() + (player.ycam()-vy)/tan(camera_angle*PI/180);
            }
            x_i = -y_i/tan(camera_angle*PI/180);
            di = (int)vx/TILE;
            dj = (int)vy/TILE;

            while((dj*incre+di > 0 && dj*incre+di<sizeof_tile) && tile[dj*incre+di].type == 0 )
            {
                vx += x_i;
                vy += y_i;
                di = (int)vx/(TILE);
                dj = (int)vy/TILE;
            }
            horizontal_d = (vx-player.xcam())*(vx-player.xcam())+(vy-player.ycam())*(vy-player.ycam());

            //Vertical Intersection----------------------------------------------------------------------------------------
            if (camera_angle >90 && camera_angle<270){ //Facing Left
                hx = (player.xcam()/TILE) * (TILE);
                x_i = -TILE;
                hy = player.ycam() + (player.xcam()-hx) * tan(camera_angle*PI/180);
                hx--;
            }
            else //Facing Right
            {
                hx = (player.xcam()/TILE) * (TILE) + TILE;
                x_i = TILE;
                hy = player.ycam() + (player.xcam()-hx) * tan(camera_angle*PI/180);
            }
            y_i = -x_i * tan(camera_angle*PI/180);
            
                
            di = (int)hx/TILE;
            dj = (int)hy/TILE;

            while((dj*incre+di > 0 && dj*incre+di<sizeof_tile) && tile[dj*incre+di].type == 0 )
            {
                hx += x_i;
                hy += y_i;
                di = (int)hx/TILE;
                dj = (int)(hy)/TILE;
            }
            vertical_d = (hx-player.xcam())*(hx-player.xcam())+(hy-player.ycam())*(hy-player.ycam());
            //final distance
            if(vertical_d < horizontal_d)
            {
                displacement = sqrt(vertical_d);
                if(show3D == FALSE)
                    line(buffer,(int) player.xcam()-x_scroll,(int) player.ycam()-y_scroll, (int)hx-x_scroll, (int)hy -y_scroll, GREEN);
            }
            else
            {
                displacement = sqrt(horizontal_d);
                if(show3D == FALSE)
                    line(buffer,(int) player.xcam()-x_scroll,(int) player.ycam()-y_scroll, (int)vx-x_scroll, (int)vy -y_scroll, BLUE);
            }
            floor_angle = camera_angle-angle;
            displacement = displacement * cos(floor_angle*PI/180);

            //Determine the height
            h_ray = Round(XY_ratio*projr/displacement);

            camera_y = Middle - abs(h_ray)/2 + h_ray*aspect/20;
            ///Display  ----------------------------
            if(show3D == TRUE){
               if(vertical_d < horizontal_d)
                {
                    texture = abs(int(hy)) % TILE;
                    stretch_blit(Brick, buffer,texture,0, texture+1,TILE, a, camera_y, 1, h_ray);
                }
                else
                {
                    texture = abs(int(vx)) % TILE;
                    stretch_blit(Brick2, buffer,texture,0, texture+1,TILE, a, camera_y, 1, h_ray);
                }
            }
            if(Round(vertical_d) == Round(horizontal_d) && key[KEY_X])
            {
     
                if(show3D == FALSE)
                {
                    line(buffer,(int) player.xcam()-x_scroll,(int) player.ycam()-y_scroll, (int)hx-x_scroll, (int)hy -y_scroll, RED);

                }
                else
                    line(buffer,camera_x,camera_y, a, camera_y+h_ray,RED);
            }
            //Floor Casting -----------------------------------------------------------
           int i;
            constant = (screen_distance*(42+5.1*abs(aspect)))/cos(floor_angle*PI/180);
            constantcos = cos(camera_angle*PI/180);
            constantsin = sin(camera_angle*PI/180);
            if(show3D == TRUE && (Middle + h_ray*aspect/20) > SCREEN_Y/2)
            {
                i = 2*camera_y+h_ray-1;
                for(int b = 0; b < camera_y; b++)
                {
                    floor_actual_d =  constant/(Middle-b);
                    floor_texture[0] = abs((int)(player.xcam()+floor_actual_d*constantcos))%TILE;
                    floor_texture[1] = abs((int)(player.ycam()-floor_actual_d*constantsin))%TILE;
                    putpixel(buffer, a,b, getpixel(floor_tile,floor_texture[0], floor_texture[1]));
                    putpixel(buffer, a,i, getpixel(floor_tile,floor_texture[0], floor_texture[1]));
                    i--;
                }
            }
            else if(show3D == TRUE)
            {
                i = 1;
                for(int b = (camera_y+h_ray); b < SCREEN_Y; b++)
                {
                    floor_actual_d =  constant/(b-Middle);
                    floor_texture[0] = abs((int)(player.xcam()+floor_actual_d*constantcos))%TILE;
                    floor_texture[1] = abs((int)(player.ycam()-floor_actual_d*constantsin))%TILE;
                    putpixel(buffer, a,b, getpixel(floor_tile,floor_texture[0], floor_texture[1]));
                    putpixel(buffer, a,camera_y-i, getpixel(floor_tile,floor_texture[0], floor_texture[1]));
                    i++;
                }
            }
            //Sprites
            Z_Index[a] = (int)displacement;
        }
       // draw_sprite(buffer, cursor, mouse_x,mouse_y);
        if(full_screen == false)
            stretch_blit(buffer, screen,0,0, SCREEN_X,SCREEN_Y, 0, 0, ACTUAL_X,ACTUAL_Y);
        else
            blit(buffer, screen, 0,0,0,0,SCREEN_X,SCREEN_Y);
        clear_bitmap(buffer);
	}			
	destroy_bitmap(buffer);
    return(0);
}END_OF_MAIN();


void Load_Map(int stage, vector<terrain> &load, vector<object> &temp)
{
    char sFile[10];
    int type,i = 0,j = 0;
    int size_of_object = 0;
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
            fptr >> type;
            load.resize(i*incre+j+1);
          /*  if(type == 'b')
            {
                size_of_object++;
                type = 0;
                temp.resize(size_of_object);
                temp[size_of_object-1].initialize(i,j,type);
            }
            */
            load[i*incre+j].initialize(i,j,type);
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


void collide_detection(int xpos, int ypos, vector<terrain> tile, int &xmove, int &ymove)
{
    int originx = xmove;
    int originy = ymove;
    int min = FALSE;
    int case1 = 0;
    for(int i =  0 ; i < tile.size(); i++)
    {
        case1 = 0;
    	while(tile[i].type != 0 && collide(xpos+xmove, ypos+ymove,tile[i].xpos(), tile[i].ypos()))
    	{
            if(case1 == 0){
        		if(xmove > 0)
                    xmove--;
                else if(xmove < 0)
                    xmove++;
                else{
                    xmove = originx;
                    case1++;
                }
            }
            else if(case1 == 1)
            {
        		if(ymove > 0)
                    ymove--;
                else if(ymove < 0)
                    ymove++;
                else{
                    break;
                }
            }
        }
    }
    if(xmove != 0 || ymove != 0)
    {
        for(int i =  tile.size() -1 ; i >=0 ; i--)
        {
            case1 = 0;
        	while(tile[i].type != 0 && collide(xpos+xmove, ypos+ymove,tile[i].xpos(), tile[i].ypos()))
        	{
                if(case1 == 0){
            		if(xmove > 0)
                        xmove--;
                    else if(xmove < 0)
                        xmove++;
                    else{
                        xmove = originx;
                        case1++;
                    }
                }
                else if(case1 == 1)
                {
            		if(ymove > 0)
                        ymove--;
                    else if(ymove < 0)
                        ymove++;
                    else{
                        break;
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------
bool checkDot(int x,int y, vector<terrain> &tile,int xmove,int ymove)
{

}

bool collide(int x1, int y1, int x2, int y2)
{
    return false;
    //the code is taken from allegro tutorial and made into a function
    int image1_bb_left = x1;
    int image1_bb_top = y1;
    int image1_bb_right = (image1_bb_left + UNIT);
    int image1_bb_bottom = (image1_bb_top + UNIT);
    int image2_bb_left = x2;
    int image2_bb_top = y2;
    int image2_bb_right = (image2_bb_left + TILE);
    int image2_bb_bottom = (image2_bb_top + TILE);
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

