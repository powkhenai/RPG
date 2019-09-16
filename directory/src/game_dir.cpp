#include "directory/game_dir.hpp"

std::vector<std::string> read_directory(const std::string& name)
{
    std::vector<std::string> item_list;
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL)
    {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type != DT_DIR)
        {
            item_list.push_back(dp->d_name);
        }
    }
    closedir(dirp);
    return item_list;
}
