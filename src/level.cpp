#include "tankbattle.h"

tb_level * tb_levelHandler::levelpack = NULL;
string tb_levelHandler::levelpackname = "";
int tb_levelHandler::levelcount = 0;
int tb_levelHandler::currentlevel = 0;
bool tb_levelHandler::loaded = 0;

int tb_levelHandler::tilemap = -1;
short int tb_levelHandler::tilesize = 0;

int tb_levelHandler::xoffset = 0;
int tb_levelHandler::yoffset = 0;
