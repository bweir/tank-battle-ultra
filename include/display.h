/*! \class tb_display
    \brief Display/graphics class.

    This class handles all drawing operations to the screen, and also screen configuration.
    It places drawing commands into a queue to allow all drawing operations to be executed
    at once.
*/

#define BUFFER      (tb_display::getbuffer())
#define SCREENW     (tb_display::getscreen_w())
#define SCREENH     (tb_display::getscreen_h())

#define BLACK           (makecol(0,0,0))
#define DEEPGRAY        (makecol(20,20,20))
#define DARKGRAY        (makecol(40,40,40))
#define MIDGRAY         (makecol(60,60,60))
#define GRAY            (makecol(80,80,80))
#define LIGHTGRAY       (makecol(150,150,150))
#define WHITE           (makecol(255,255,255))

#define LIGHTBLUE       (makecol(150,150,255))
#define BLUE            (makecol(0,0,255))
#define DARKBLUE        (makecol(0,0,150))

#define DEEPGREEN       (makecol(0,80,0))
#define DARKGREEN       (makecol(0,150,0))
#define GREEN           (makecol(0,255,0))
#define LIGHTGREEN      (makecol(150,255,150))

#define DARKRED         (makecol(150,0,0))
#define RED             (makecol(255,0,0))
#define LIGHTRED        (makecol(255,150,150))

#define PURPLE          (makecol(255,255,0))



#ifndef TB_DISPLAY
#define TB_DISPLAY
class imagedata {
    private:
        int w;
        int h;
        int maxframex;
        int maxframey;

        BITMAP * image;
        string name;

        imagedata() {
            w = 0;
            h = 0;

            maxframex = 0;
            maxframey = 0;

            image = NULL;
            name = "";
        }

    public:



    friend class tb_display;
};


class tb_display {
    private:
        static int screenw, screenh;
        static int depth;
        static int scale;
        static int mode;
        static bool created;
        static string title;

        static bool framerate_enabled;

        static BITMAP * buffer;



        static vector<imagedata> image;
        static int imagemax;



        static volatile int ticks;
        static void ticker(void) {
            ticks++;
        } END_OF_FUNCTION(ticker);


        static volatile int close_button_pressed;
        static void close_button_handler (void) {
            close_button_pressed = TRUE;
        } END_OF_FUNCTION(close_button_handler);


    public:

        static BITMAP * getbuffer(void) {return buffer;}

        static int getscreen_w(void) {return screenw;}
        static int getscreen_h(void) {return screenh;}

        static int window_closed(void) {return close_button_pressed;}


        static void enable_framecap (int framerate) {
            LOCK_FUNCTION(ticker);
            LOCK_VARIABLE(ticks);

            if (!framerate_enabled) {
                install_int_ex(ticker, BPS_TO_TIMER(framerate));
            }
            framerate_enabled = 1;
        }


        static void disable_framecap() {
            if (framerate_enabled) {
                remove_int(ticker);
            }
            framerate_enabled = 0;
        }




        //! \brief Initializes or resizes the screen, depending on whether a screen already exists.
        /*! This version of the function extracts the display configuration data from the open
            config file, so be sure to set the config file with set_config_file().
        */
        static void set_display_from_config () {

            cout << "TB_DISPLAY: ";

            if (created) {
                cout << "Resizing screen...";
                remove_display();
            } else {
                cout << "Initializing screen...";
            }

            screenw = get_config_int("display", "screenw", 160);
            screenh = get_config_int("display", "screenh", 144);
            scale = get_config_int("display", "scale_multiplier", 1);
            mode = get_config_int("display", "mode", GFX_AUTODETECT_WINDOWED);
            title = get_config_string(NULL, "title", "default title");




            imagemax = get_config_int("display","imagemax",100);

            image.clear();
            image.reserve(imagemax);



            depth = desktop_color_depth();
            if (depth != NULL) {
                set_color_depth(depth);
            } else {
                allegro_message( "Unable to set color depth." );
            }
            cout << depth << "-bit, ";


            int ret = set_gfx_mode( mode, screenw*scale, screenh*scale, 0, 0);
            if (ret != NULL) {
                allegro_message( allegro_error );
            }
            set_display_switch_mode(SWITCH_BACKGROUND);
            set_window_title(title.c_str());
            cout << "mode " << mode << ", ";

            cout << "buffered" << "\n";
            buffer = create_bitmap(screenw, screenh);



            LOCK_FUNCTION(close_button_handler);
            set_close_button_callback(close_button_handler);

            created = 1;
        }









        //! \brief Destroys the current screen if it exists.
        /*! Removes the display initialized by set_display() and set_display_from_config().
            This is called when the display is resized, so it is not necessary to call
            it by the application for the most part.

        */
        static void remove_display () {

            cout << "TB_DISPLAY: Removing display\n";

            screenw = 160;
            screenh = 144;
            scale = 1;
            depth = 0;
            if (buffer != NULL) destroy_bitmap(buffer);
            buffer = NULL;
            mode = 0;
            created = 0;
            title.clear();

            set_close_button_callback(NULL);
        }



        //! \brief Load a bitmap image into the current screen format.
/*
        static BITMAP * load_image_bmp (const char * imagefile) {
            BITMAP * file = load_bitmap(imagefile, NULL);
            if (file == NULL) {
                string filename = "Failed to load";
                filename += imagefile;
                filename += ".\nCheck filename and directory are correct.";
                allegro_message(filename.c_str());
            }
            return file;
        }

*/

        static int load_image_into_stack (const char * imagefile, int wvar, int hvar) {
            imagedata tempdata;
            tempdata.name = imagefile;
            tempdata.image = load_bitmap(imagefile, NULL);

            if (tempdata.image == NULL) {
                string filename = "Failed to load";
                filename += imagefile;
                filename += ".\nCheck filename and directory are correct.";
                allegro_message(filename.c_str());

            } else {


                if (wvar > 0) {
                    tempdata.w = wvar;
                    tempdata.maxframex = (tempdata.image->w) / tempdata.w;
                } else {
                    tempdata.w = tempdata.image->w;
                    tempdata.maxframex = 0;
                }

                if (hvar > 0) {
                    tempdata.h = hvar;
                    tempdata.maxframey = (tempdata.image->w) / tempdata.h;
                } else {
                    tempdata.h = tempdata.image->h;
                    tempdata.maxframey = 0;
                }

                image.push_back(tempdata);

            }


            int imgindex = image.size()-1;
            cout << "TB_DISPLAY: Loaded image: " << imgindex << ", " << image[imgindex].name << "\n";
            return imgindex;
        }




        /*! This function will return a 1 if the sprite is drawn, otherwise it will return a 0
            to indicate the sprite is off the screen and will not be drawn.
        */
        static int draw_sprite (int imgindex, int framex, int framey, int destx, int desty) {
            if (framex < 0) framex = 0;
            else if (framex > image[imgindex].maxframex) framex = image[imgindex].maxframex;

            if (framey < 0) framey = 0;
            else if (framey > image[imgindex].maxframey) framey = image[imgindex].maxframey;


            if (image[imgindex].image != NULL) {
                if (destx+image[imgindex].w < 0 || destx > screenw || desty+image[imgindex].h < 0 || desty > screenh) {
                    return 0;
                } else {
                    masked_blit(image[imgindex].image, buffer, framex*image[imgindex].w, framey*image[imgindex].h, destx, desty, image[imgindex].w, image[imgindex].h);
                    return 1;
                }
            }
        }





        /*! This function blits with no error checking.  Really just a wrapper for the allegro function.*/
        static void draw_tile (int imgindex, int tilenum, int destx, int desty) {
            int tilex = tilenum%image[imgindex].maxframex;
 //           if (tilex > image[imgindex].maxframex) tilex = image[imgindex].maxframex;

            int tiley = tilenum/image[imgindex].maxframex;
   //         if (tiley > image[imgindex].maxframey) tiley = image[imgindex].maxframey;

            blit(image[imgindex].image, buffer, tilex*image[imgindex].w, tiley*image[imgindex].h, destx, desty, image[imgindex].w, image[imgindex].h);
        }






        //! Clear the screen to the color specified by the argument.
        static void clear_screen(int color) {
           rectfill(buffer, 0, 0, buffer->w, buffer->h, color);
        }




        /*! This function outputs the video buffer to the screen, then waits to maintain the frame rate
            if frame rate regulation is enabled to prevent the game from running too fast.
        */
        static void refresh() {
            vsync();
            acquire_screen();
            stretch_blit(buffer, screen, 0, 0, buffer->w, buffer->h, 0, 0, SCREEN_W, SCREEN_H);
            release_screen();

            if (framerate_enabled) {
                while (ticks<1) {
                    rest(1);
                }
                ticks=0;
            }
        }

};
#endif
