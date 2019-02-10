#ifndef GAME_DIR_H_
#define GAME_DIR_H_

#include <dirent.h>
#include <string>
#include <vector>

std::vector<std::string> read_directory(const std::string& name);

#endif
