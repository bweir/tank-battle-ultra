#define BULLETS     (50)

#ifndef TB_BULLET
#define TB_BULLET
class tb_bullet : public tb_object {
    private:

        bool on;
        int speedx;
        int speedy;

        tb_bullet() {
            speedx = 0;
            speedy = 0;
            on = 0;
            w = 8;
            h = 8;
        }

        friend class tb_bulletHandler;
};
#endif




#ifndef TB_BULLETHANDLER
#define TB_BULLETHANDLER
class tb_bulletHandler {
    private:
        static int index;
        static int speed;
        static tb_bullet bullet[BULLETS];

        static int img;

    public:

        static void init_config() {
            index = 0;
            speed = 3;
            img = tb_display::load_image_into_stack("tankbattletiles/bullet.bmp", 8, 8);

            for (int i = 0; i < BULLETS; i++) {
                bullet[i].on = 0;
                bullet[i].x = 0;
                bullet[i].y = 0;
                bullet[i].speedx = 0;
                bullet[i].speedy = 0;
            }


        }

        static void remove() {
            img = NULL;
        }



        static void process_bullets () {
            for (int i = 0; i < BULLETS; i++) {
                if (bullet[i].on) {
                    bullet[i].x += bullet[i].speedx;
                    bullet[i].y += bullet[i].speedy;

                    bullet[i].on = tb_display::draw_sprite(img, 0, 0, bullet[i].x-XOFFSET, bullet[i].y-YOFFSET);
                    //if (!bullet[i].on) cout << "Bullet " << i << " went offscreen!\n";
                }
            }
        }



        static void spawn_bullet (int xvar, int yvar, int dirvar) {
                bullet[index].on = 1;
                bullet[index].speedx = 0;
                bullet[index].speedy = 0;

                if (dirvar == 0) {
                    bullet[index].x = xvar;
                    bullet[index].y = yvar; //- img->h/2;
                    bullet[index].speedx -= speed;

                }
                if (dirvar == 1) {
                    bullet[index].x = xvar;
                    bullet[index].y = yvar; //- img->h/2;
                    bullet[index].speedx += speed;
                }
                if (dirvar == 2) {
                    bullet[index].x = xvar; //- img->w/2;
                    bullet[index].y = yvar; //- img->w/2;
                    bullet[index].speedy -= speed;
                }
                if (dirvar == 3) {
                    bullet[index].x = xvar; //- img->w/2;
                    bullet[index].y = yvar;
                    bullet[index].speedy += speed;
                }

                index++;
                if (index >= BULLETS) index = 0;

        }

};
#endif
