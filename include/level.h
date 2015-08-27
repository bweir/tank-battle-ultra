/*! \class tb_level
    \brief Level controller class.

    This class is responsibility for loading and managing all levels
    available to the current game.  It loads level tiles, entity data,
    graphics, audio, etc. all through calls to the various Allegro
    wrapper classes.
*/

#define XOFFSET     (tb_levelHandler::getoffset_x())
#define YOFFSET     (tb_levelHandler::getoffset_y())

#ifndef TB_LEVEL
#define TB_LEVEL
class tb_level {
    private:

        bool loaded;
        short int * level;
        short int * wall;
        int w, h;
        int upperx, uppery;
        string name;
        string filename;

        tb_level () {
            level = NULL;
            wall = NULL;
            w = 0;
            h = 0;
            loaded = 0;
        }

        const char * get_name () {return name.c_str();}

        bool is_loaded () {return loaded;}



        void open (const char * dirvar, const char * namevar) {

            if (loaded != 0) close();

            name = namevar;
            filename = dirvar;
            filename += namevar;
            filename += ".txt";

            ifstream levelfile;
            cout << "\tOpening level";
            levelfile.open(filename.c_str());

            if (levelfile.good()) {
                cout << ", file is good";

                // Load level size
                levelfile >> w >> h;


                // Load tile data
                level = new short int [w*h];
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++) {
                        levelfile >> level[y*w + x];
                    }
                }

                // Load collision map
                wall = new short int [w*h];
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++) {
                        levelfile >> wall[y*w + x];
                    }
                }

                levelfile.close();
                loaded = 1;
                cout << ", loaded " << filename << "\n";

            } else {
                string errormsg = "Failed to load ";
                errormsg += name;
                errormsg += "\nPlease check filename.";
                allegro_message(errormsg.c_str());
                cout << "\tFailed to load!" << errormsg.c_str() << "Please check filename.\n";

                loaded = 0;
            }

        }

        void close () {
            cout << "Deleting level... ";

            delete[] level;
            level = NULL;

            delete[] wall;
            wall = NULL;

            name.clear();

            w = 0;
            h = 0;
            loaded = 0;

            cout << "DONE\n";
        }

        ~tb_level() {
            close();
        }

        friend class tb_levelHandler;

};
#endif



#ifndef TB_LEVELHANDLER
#define TB_LEVELHANDLER
class tb_levelHandler {
    private:
        // level data
        static tb_level * levelpack;
        static string levelpackname;
        static int levelcount;
        static int currentlevel;
        static bool loaded;

        static int xoffset, yoffset;

        // tilemap data
        static short int tilesize;
        static int tilemap;

    public:

        static int getoffset_x() {return xoffset;}
        static int getoffset_y() {return yoffset;}


        static void open (const char * packnamevar) {

            if (!loaded) {
                cout << "Loading level pack...\n";
                levelpackname = packnamevar;

                string leveldirname = levelpackname;
                leveldirname += '/';

                // load tilemap
                string levelfilename = leveldirname;
                levelfilename += levelpackname;
                levelfilename += ".bmp";
//                tilemap = tb_display::load_image_bmp(levelfilename.c_str());
                tilemap = tb_display::load_image_into_stack(levelfilename.c_str(), 8, 8);


                // load main level file
                levelfilename = leveldirname;
                levelfilename += levelpackname;
                levelfilename += ".txt";
                ifstream levelfile;
                levelfile.open(levelfilename.c_str());

                levelfile >> tilesize >> levelcount;
                cout << "\nTilesize:" << tilesize << "\n";
                cout << "Level count:" << levelcount << "\n";



                levelpack = new tb_level[levelcount];

                // load levels
               string tempname;
                for (int i = 0; i < levelcount; i++) {
                    if (levelfile.good())
                        levelfile >> tempname;
                    else
                        cout << "\tLevel data corrupted!\n";

                    levelpack[i].open(leveldirname.c_str(), tempname.c_str());

                    levelpack[i].upperx = levelpack[i].w*tilesize - SCREENW;
                    levelpack[i].uppery = levelpack[i].h*tilesize - SCREENH;
                }


                levelfile.close();
                loaded = 1;

                cout << "Finished loading level pack\n";
            } else {
                cout << "A level pack is already open!\n";
            }
        }

        static void close () {
            delete[] levelpack;
            levelpack = NULL;
            levelcount = 0;
            currentlevel = 0;
            loaded = 0;

        //    if (tilemap != NULL) destroy_bitmap(tilemap);
            tilemap = NULL;
            tilesize = 0;
            xoffset = 0;
            yoffset = 0;
        }


        static void set_offset(int xoffsetvar, int yoffsetvar) {
            xoffset = xoffsetvar;
            yoffset = yoffsetvar;

            if (xoffset > levelpack[currentlevel].upperx) xoffset = levelpack[currentlevel].upperx;
            else if (xoffset < 0) xoffset = 0;

            if (yoffset > levelpack[currentlevel].uppery) yoffset = levelpack[currentlevel].uppery;
            else if (yoffset < 0) yoffset = 0;
        }



        static void set_tilemap(BITMAP * tilemapvar, short int tilesizevar) {
            tilesize = tilesizevar;

            for (int i = 0; i < levelcount; i++) {
                levelpack[i].upperx = levelpack[i].w*tilesize - SCREENW;
                levelpack[i].uppery = levelpack[i].h*tilesize - SCREENH;
            }

        }


        static void draw () {
            BITMAP * buffer = tb_display::getbuffer();

            int destx1 = (xoffset)/tilesize, destx2 = (xoffset + SCREENW)/tilesize + 1;
            int desty1 = (yoffset)/tilesize, desty2 = (yoffset + SCREENH)/tilesize + 1;

            if (destx1 < 0) destx1 = 0;
            if (desty1 < 0) desty1 = 0;
            if (destx2 > levelpack[currentlevel].w) destx2 = levelpack[currentlevel].w;
            if (desty2 > levelpack[currentlevel].h) desty2 = levelpack[currentlevel].h;


            int tilenum = 0;
            for (int y = desty1; y < desty2; y++) {
                for (int x = destx1; x < destx2; x++) {


                    tilenum = levelpack[currentlevel].level[y*levelpack[currentlevel].w + x];
                    int tempx = (x*tilesize-xoffset), tempy = (y*tilesize-yoffset);

                    //draw_tile (int imgindex, int tilenum, int destx, int desty)

                //    blit(tilemap, buffer, (tilenum%mapw)*tilesize, (tilenum/mapw)*tilesize, tempx, tempy, tilesize, tilesize);
                    tb_display::draw_tile (tilemap, tilenum, tempx, tempy);

                    tilenum = levelpack[currentlevel].wall[y*levelpack[currentlevel].w + x];
                 //   if (tilenum == 1) rect(buffer, tempx, tempy, tempx+tilesize-1, tempy+tilesize-1, WHITE);

                }
            }
        }




        static void set_level(int levelchoice) {
            if (levelchoice < 0) levelchoice = 0;
            else if (levelchoice >= levelcount) levelchoice = levelcount-1;

            if (currentlevel != levelchoice) {
                currentlevel = levelchoice;
                cout << "Setting level: " << levelpack[currentlevel].get_name() << "\n";
            }
        }

};
#endif

