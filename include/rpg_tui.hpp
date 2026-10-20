#include <ncurses.h>
#include <string>
#include <locale.h>
#include <vector>

#include "character/character.hpp"
#include "creature/monster.hpp"

// ============================================================================
// ncurses lifecycle
// ============================================================================

struct NCursesGuard {
    NCursesGuard() {}
    ~NCursesGuard() { endwin(); }
};

void init_ncurses();

// ============================================================================
// Layout
// ============================================================================

struct WindowLayout {
    WINDOW *ctx_win;    // upper-left  — context (char list, arena info, combat)
    WINDOW *stats_win;  // upper-right — player stats
    WINDOW *log_win;    // middle      — scrolling message log
    WINDOW *input_win;  // bottom      — keybind hints / text entry
    int total_h, total_w;
    int top_h, ctx_w, stats_w;
    int log_h, input_h;
};

WindowLayout create_layout();
void         destroy_layout(WindowLayout &l);

// ============================================================================
// Drawing helpers
// ============================================================================

void draw_box_title(WINDOW *win, const std::string &title);
void draw_hp_bar(WINDOW *win, int y, int x, int cur, int max_val, int width);
void draw_stats(const WindowLayout &l, Character &player);
void render_log(const WindowLayout &l, const std::vector<std::string> &log);
void append_log(const WindowLayout &l, std::vector<std::string> &log, const std::string &msg);
void set_input_hint(const WindowLayout &l, const std::string &hint);
std::string prompt_input(const WindowLayout &l, const std::string &prompt);

// ============================================================================
// Screens
// ============================================================================
Character character_select(std::vector<std::string> &characters);
int combat_screen(Character &player, Monster &opponent);
void main_screen(Character &player);
