#ifndef TB_INPUT
#define TB_INPUT




class tb_keypad {



};



class tb_input {
    private:
        static int c_in;
        static char c_scancode;
        static char c;

        static bool updated;

        static char oldkey[KEY_MAX];

        static string outputbuffer;

        static int linecursor;
        static int columncursor;

        static bool in_console;

        static int buffersize;

    public:


        static void init_input() {
            bool limitbuffer = get_config_int("input","limitbuffer",0);
            if (limitbuffer) {
                buffersize = get_config_int("input","buffersize",50);
            } else {
                buffersize = SCREENW/8;
            }

            outputbuffer.clear();
            outputbuffer.reserve(buffersize);
        }

        static void process_input() {
            updated = 0;
            while (keypressed()) {
                updated = 1;
                c_in = readkey();
               // cout << c_in << "\t" << (char) c_in << "\n";

                c_scancode = (char) (c_in >> 8);
                c = (char) (c_in & 0xFF);

                if (c >= ' ' && c <= '~') {
                    if (in_console) {
                        if (outputbuffer.length() < buffersize) {
                            outputbuffer.append(1,c);
                            columncursor++;

                        }
                    }
                }
                else if (c_scancode == KEY_ENTER) {

                    if (in_console) {
                        outputbuffer.clear();
                        columncursor = 0;
                        in_console = 0;
                    } else {
                        in_console = 1;
                    }


                } else if (c_scancode == KEY_BACKSPACE) {
                    if (in_console) {
                        int cursor = outputbuffer.length()-1;

                        if (outputbuffer.length() > 0) {
                            if (outputbuffer[cursor] == '\n') {
                                if (linecursor > 0)
                                    linecursor--;
                                else
                                    linecursor = 0;

                            } else {
                                outputbuffer.erase (outputbuffer.end()-1);
                                columncursor--;
                            }

                        }
                    }

                }
            }
        }

        static void display_console() {
            if (in_console) {
                rectfill(BUFFER, 0, 0, SCREENW, 16, BLACK);
                textprintf_ex(BUFFER, font, 0, 0, WHITE, BLACK, "%s", outputbuffer.c_str());
            }
        }

        static int is_in_console() {
            return in_console;
        }

};
#endif
