/// --------------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------------
/// Project: Tank Battle Online
/// Creator: Brett Weir
/// Date: 5/24/2010
/// --------------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------------
#include "tankbattle.h"



int main (void) {

    allegro_init();
    install_keyboard();
    install_timer();


    set_config_file("game.cfg");
    string gametitle = get_config_string(NULL,"title","default title");
    string levelpackname = get_config_string(NULL,"levelpack","default");
    cout << "Game:\t" << gametitle << endl;
    cout << "Levelpack:\t" << levelpackname << endl;

    tb_display::enable_framecap(30);
    tb_display::set_display_from_config ();



    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    tb_audio::init_config();
    int song = tb_audio::open_song("sound/DouglasAdamsSong1.mid");
//    tb_audio::play_song(song);



    tb_levelHandler::open(levelpackname.c_str());
    tb_levelHandler::set_level(1);

    tb_bulletHandler::init_config();


    tb_tank tank;
    tank.load_img("tankbattletiles/extremetankn.bmp", 16, 16);

    tb_input::init_input();









    bool running = 1;
    while (running) {
        tb_display::clear_screen(BLACK);

        tb_input::process_input();

        if (key[KEY_ESC] || tb_display::window_closed()) running = 0;

        if (!tb_input::is_in_console()) {
            tank.process_input();
            // select level
            for (int i = 0; i < 4; i++)
                if (key[KEY_1+i]) {tb_levelHandler::set_level(i);}


        }


        tb_levelHandler::set_offset(tank.X() - SCREENW/2 + tank.W()/2, tank.Y() - SCREENH/2 + tank.H()/2);
        tb_levelHandler::draw();

        tank.draw();


        tb_bulletHandler::process_bullets();

        tb_input::display_console();

        tb_display::refresh();
    }

    cout << "Quitting...\n";

    //tb_bulletHandler::remove();

    allegro_exit();
    return 0;
}
END_OF_MAIN()
