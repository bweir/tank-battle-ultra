#include "tankbattle.h"

int tb_display::screenw = 160, tb_display::screenh = 144;
int tb_display::depth = 0;
int tb_display::scale = 1;
int tb_display::mode = 0;
bool tb_display::created = 0;
string tb_display::title = "";
BITMAP * tb_display::buffer = NULL;

vector<imagedata> tb_display::image;

int tb_display::imagemax = 0;

bool tb_display::framerate_enabled = 0;
volatile int tb_display::ticks = 0;

volatile int tb_display::close_button_pressed = FALSE;
