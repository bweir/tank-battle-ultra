#include "tankbattle.h"

int tb_input::c_in = 0;
char tb_input::c_scancode = 0;
char tb_input::c = 0;

bool tb_input::updated = 0;
bool tb_input::in_console = 0;

char tb_input::oldkey[KEY_MAX] = {0};

int tb_input::linecursor = 0;
int tb_input::columncursor = 0;

int tb_input::buffersize = 0;

string tb_input::outputbuffer;

