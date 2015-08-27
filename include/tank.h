#ifndef TB_TANK
#define TB_TANK
class tb_tank : public tb_object {
    public:
        int speed;
        int dir;
        bool walking;
        bool shooting;
        int shot;
        int sound;


        tb_tank() {
            speed = 5;
            dir = 0;
            walking = 0;
            shooting = 0;
            shot = 0;
            sound = tb_audio::open_sound("sound/chicken.wav");
        }


        void process_input() {

            walking = 0;
            if (key[KEY_LEFT])  {dir = 0;   x-=speed;   walking=1;}
            if (key[KEY_RIGHT]) {dir = 1;   x+=speed;   walking=1;}
            if (key[KEY_UP])    {dir = 2;   y-=speed;   walking=1;}
            if (key[KEY_DOWN])  {dir = 3;   y+=speed;   walking=1;}

            if (key[KEY_S]) {
                if (!shot) {
                  //  shot = 1;
                    shooting = 1;

                    tb_audio::play_sound(sound);

                    if (dir == 0) {
                        tb_bulletHandler::spawn_bullet (x, y + h/2, dir);
                    }
                    if (dir == 1) {
                        tb_bulletHandler::spawn_bullet (x + w, y + h/2, dir);
                    }
                    if (dir == 2) {
                        tb_bulletHandler::spawn_bullet (x + w/2, y, dir);
                    }
                    if (dir == 3) {
                        tb_bulletHandler::spawn_bullet (x + w/2, y + h, dir);
                    }

                }
            } else {
                shot = 0;
                shooting = 0;
            }


            if (walking) {
                if (framex == 1) {
                    framex = 0;
                } else {
                    framex = 1;
                }
            }
            framey = dir;


        }


};
#endif




#ifndef TB_TANKHANDLER
#define TB_TANKHANDLER
class tb_tankHandler {
    static vector<tb_object> tank;


};
#endif
