#ifndef TB_OBJECT
#define TB_OBJECT
class tb_object {
    protected:
        int x;
        int y;
        int w;
        int h;

        int framex;
        int framey;

        int img;




    public:

        void set_x(int xvar) {x = xvar;}
        void set_y(int yvar) {y = yvar;}
        void set_w(int wvar) {w = wvar;}
        void set_h(int hvar) {h = hvar;}

        int X() {return x;}
        int Y() {return y;}
        int W() {return w;}
        int H() {return h;}

        tb_object() {
            x = 0;
            y = 0;
            w = 0;
            h = 0;

            img = 0;
            framex = 0;
            framey = 0;
        }

        ~tb_object() {
            x = 0;
            y = 0;
            w = 0;
            h = 0;
        }


        void load_img (char * imgname, int wvar, int hvar) {
            img = tb_display::load_image_into_stack (imgname, wvar, hvar);
        }

        void draw() {
            tb_display::draw_sprite(img, framex, framey, x-XOFFSET, y-YOFFSET);
        }


};
#endif
