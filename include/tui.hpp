#include <ncursesw/ncurses.h>
#include <string>
#include <locale.h>
#include <vector>

#include "character/character.hpp"
#include "creature/monster.hpp"
#include "location/location.hpp"
#include "item/item.hpp"

// OOO interface for TUI version of the game, using ncurses. This is a work in progress and may be incomplete or buggy.

// Main window class
struct WindowLayout {
};

class GameWindow {
    public:
        GameWindow();
        ~GameWindow();
        void log(const std::string &msg);
        std::vector<std::map<std::string, std::string>> render_scene(Character &player, Location &location);
        std::vector<std::map<std::string, std::string>> render_combat(Character &player, Monster &opponent);
        template<typename T>
        int list_select(const std::string &title, const std::vector<T> &options){
            int selected = 0;
            draw_ctx(title, {});    
            Character preview = Character("Preview", "Character");
            //draw_stats(Character("Preview", "Character"));
            set_input_hint("[" "↑" "↓" "] Select  [Enter] Load  [n] New  [q] Quit");
            while (true) {
                werase(ctx_win);
                draw_box_title(ctx_win, title);
                for (size_t i = 0; i < options.size() && i < (size_t)(top_h - 2); i++) {
                    const char *mark = ((int)i == selected) ? "> " : "  ";
                    if ((int)i == selected) wattron(ctx_win, A_BOLD);
                    mvwprintw(ctx_win, 1 + (int)i, 1, "%s%s", mark, options[i].c_str());
                    if ((int)i == selected) wattroff(ctx_win, A_BOLD);
                }
                wrefresh(ctx_win);

                // Character preview
                preview.load(".rpg/creatures/characters/" + options[selected]);
                draw_stats(preview);

                int key = getch();
                switch (key) {
                    case KEY_UP:
                        if (selected > 0) selected--;
                        break;
                    case KEY_DOWN:
                        if (selected < (int)options.size() - 1) selected++;
                        break;
                   case '\n':
                        return selected;
                }
            }
        }
    private:
        int total_h, total_w;
        int input_h, stats_w, ctx_w, log_h, top_h;
        WINDOW *ctx_win;
        WINDOW *stats_win;
        WINDOW *log_win;
        WINDOW *input_win;
        std::vector<std::string> log_data;
        void create_layout();
        void destroy_layout();
        void draw_box_title(WINDOW *win, const std::string &title);
        void draw_hp_bar(WINDOW *win, int y, int x, int cur, int max_val, int width);
        void draw_stats(Character &player);
        void draw_ctx(const std::string title, const std::vector<std::string> &lines);
        void render_log();
        void append_log(const std::string &msg);
        void set_input_hint(const std::string &hint);
        std::string prompt_input(const std::string &prompt);
};
