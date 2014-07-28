#ifndef WEAPON_H
#define WEAPON_H
class guninfo
{         
    public:
        //assessor
        bool isauto(){return automatic;}
        int max_frame(){return maximum_frame;}
        int frame_cd(){return frame_cdtime;}
        int frame_shoot(){return shooting_frame;}
        int damage_n(){return normal_damage;}
        int damage_h(){return head_damage;}
        int r_min(){return recoil_min;}
        int r_max(){return recoil_max;}
        int r_incre(){return recoil_incre;}
        int recoil(){return recoil_shift;}
        //bool clip_done();
    
        //modifier
        void setup(bool a, int t_f,int f_cd, int f_s,int g_d,int h_d,int r_min,int r_max,int r_i,int r_s);
        
        
    private:
        //
        bool automatic;
        int maximum_frame;
        int frame_cdtime;
        int shooting_frame;
        int normal_damage;
        int head_damage;
        int recoil_shift;
        int recoil_min;
        int recoil_incre;
        int recoil_max;
};

void guninfo::setup(bool a, int t_f,int f_cd, int f_s,int g_d,int h_d,int r_min,int r_max,int r_i,int r_s)
{
     automatic = a;
     maximum_frame = t_f;
     frame_cdtime = f_cd;
     shooting_frame = f_s;
     normal_damage = g_d;
     head_damage = h_d;
     recoil_min = r_min;
     recoil_incre = r_i;
     recoil_max = r_max;
     recoil_shift = r_s;
     
}

#endif

