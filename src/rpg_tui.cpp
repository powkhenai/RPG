#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <locale.h>

#include "character/character.hpp"
#include "creature/monster.hpp"
#include "location/combat_location.hpp"
#include "directory/game_dir.hpp"
#include "item/item.hpp"
#include "rpg_tui.hpp"

namespace fs = std::filesystem;

// ============================================================================
// ncurses lifecycle
// ============================================================================

void init_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    // Color pairs: 1=green, 2=yellow, 3=red, 4=cyan, 5=default (white-on-black)
    init_pair(1, COLOR_GREEN,  COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED,    COLOR_BLACK);
    init_pair(4, COLOR_CYAN,   COLOR_BLACK);
    init_pair(5, COLOR_WHITE,  COLOR_BLACK);

    // Fill stdscr background so uncovered areas are consistently black.
    bkgd(COLOR_PAIR(5));
    // Establish physical screen baseline so subwindow wrefresh calls render immediately.
    refresh();
}

// ============================================================================
// Layout
// ============================================================================

WindowLayout create_layout() {
    WindowLayout l;
    getmaxyx(stdscr, l.total_h, l.total_w);
    l.input_h = 3;
    l.stats_w = std::max(26, l.total_w / 3);
    l.ctx_w   = l.total_w - l.stats_w;
    l.log_h   = std::max(6, l.total_h / 3);
    l.top_h   = std::max(6, l.total_h - l.log_h - l.input_h);

    l.ctx_win   = newwin(l.top_h,   l.ctx_w,   0,                       0);
    l.stats_win = newwin(l.top_h,   l.stats_w, 0,                       l.ctx_w);
    l.log_win   = newwin(l.log_h,   l.total_w, l.top_h,                 0);
    l.input_win = newwin(l.input_h, l.total_w, l.top_h + l.log_h,      0);

    // Set background color for all cells (not just cells with text).
    wbkgd(l.ctx_win,   COLOR_PAIR(5));
    wbkgd(l.stats_win, COLOR_PAIR(5));
    wbkgd(l.log_win,   COLOR_PAIR(5));
    wbkgd(l.input_win, COLOR_PAIR(5));

    return l;
}

void destroy_layout(WindowLayout &l) {
    delwin(l.ctx_win);
    delwin(l.stats_win);
    delwin(l.log_win);
    delwin(l.input_win);
    l.ctx_win = l.stats_win = l.log_win = l.input_win = nullptr;
}

// ============================================================================
// Drawing helpers
// ============================================================================

void draw_box_title(WINDOW *win, const std::string &title) {
    int h, w;
    getmaxyx(win, h, w);
    (void)h;
    box(win, 0, 0);
    int tx = (w - (int)title.length()) / 2;
    if (tx < 1) tx = 1;
    mvwaddch(win,  0, tx - 1, ' ');
    mvwaddstr(win, 0, tx,     title.c_str());
    mvwaddch(win,  0, tx + (int)title.length(), ' ');
}

void draw_hp_bar(WINDOW *win, int y, int x, int cur, int max_val, int width) {
    if (max_val <= 0) max_val = 1;
    float pct = static_cast<float>(cur) / max_val;
    if (pct < 0) pct = 0;
    if (pct > 1) pct = 1;

    int filled = static_cast<int>(width * pct);
    int empty  = width - filled;
    int pair   = (pct >= 0.5f) ? 1 : (pct >= 0.25f) ? 2 : 3;

    wattron(win, COLOR_PAIR(pair));
    for (int i = 0; i < filled; i++) mvwaddch(win, y, x + i, '#');
    wattroff(win, COLOR_PAIR(pair));
    for (int i = 0; i < empty;  i++) mvwaddch(win, y, x + filled + i, '-');
}

void draw_stats(const WindowLayout &l, Character &player) {
    werase(l.stats_win);
    draw_box_title(l.stats_win, "Stats");

    int max_hp = player.get_str() * player.get_fort();
    int bar_w  = std::max(4, l.stats_w - 10);

    int y = 1;
    mvwprintw(l.stats_win, y++, 2, "%-*s", l.stats_w - 4, player.get_name().c_str());
    mvwprintw(l.stats_win, y++, 2, "EXP: %d", player.get_exp());
    mvwprintw(l.stats_win, y,   2, "HP:  ");
    draw_hp_bar(l.stats_win, y, 7, player.get_hp(), max_hp, bar_w);
    y++;
    mvwprintw(l.stats_win, y++, 2, "     %d/%d", player.get_hp(), max_hp);
    y++;
    mvwprintw(l.stats_win, y++, 2, "STR:%-3d  AGI:%-3d", player.get_str(),   player.get_agi());
    mvwprintw(l.stats_win, y++, 2, "INT:%-3d  CHA:%-3d", player.get_intel(), player.get_cha());
    mvwprintw(l.stats_win, y++, 2, "FRT:%-3d  WIS:%-3d", player.get_fort(),  player.get_wis());
    wrefresh(l.stats_win);
}

void render_log(const WindowLayout &l, const std::vector<std::string> &log) {
    werase(l.log_win);
    draw_box_title(l.log_win, "Log");
    int inner_h = l.log_h - 2;
    int inner_w = l.total_w - 4;
    size_t start = (log.size() > (size_t)inner_h) ? log.size() - inner_h : 0;
    for (size_t i = start; i < log.size(); i++) {
        std::string line = log[i];
        if ((int)line.size() > inner_w) line = line.substr(0, inner_w);
        mvwprintw(l.log_win, 1 + (int)(i - start), 2, "%s", line.c_str());
    }
    wrefresh(l.log_win);
}

void append_log(const WindowLayout &l, std::vector<std::string> &log, const std::string &msg) {
    log.push_back(msg);
    render_log(l, log);
}

void set_input_hint(const WindowLayout &l, const std::string &hint) {
    werase(l.input_win);
    box(l.input_win, 0, 0);
    wattron(l.input_win, COLOR_PAIR(4));
    mvwprintw(l.input_win, 1, 2, "%s", hint.c_str());
    wattroff(l.input_win, COLOR_PAIR(4));
    wrefresh(l.input_win);
}

// Prompt for a single line of text inside the input subwindow.
std::string prompt_input(const WindowLayout &l, const std::string &prompt) {
    werase(l.input_win);
    box(l.input_win, 0, 0);
    wattron(l.input_win, COLOR_PAIR(4));
    mvwprintw(l.input_win, 1, 2, "%s", prompt.c_str());
    wattroff(l.input_win, COLOR_PAIR(4));
    wrefresh(l.input_win);

    echo();
    curs_set(1);
    char buf[64] = {};
    mvwgetnstr(l.input_win, 1, 2 + (int)prompt.size(), buf, 63);
    noecho();
    curs_set(0);
    return std::string(buf);
}

// ============================================================================
// Utility
// ============================================================================

void ensure_rpg_dir() {
    if (!fs::exists(".rpg")) fs::create_directory(".rpg");
    if (!fs::exists(".rpg/creatures")) fs::create_directory(".rpg/creatures");
}

// ============================================================================
// Screen: Character Select
// ============================================================================

Character character_select(std::vector<std::string> &characters) {

    WindowLayout l = create_layout();
    std::vector<std::string> log = {
        "Welcome to DrakkonQuest!",
        "Select a character or create a new one."
    };
    render_log(l, log);
    set_input_hint(l, "[" "↑" "↓" "] Select  [Enter] Load  [n] New  [q] Quit");
    int selected = 0;

    while (true) {
        // Context pane: character list
        werase(l.ctx_win);
        draw_box_title(l.ctx_win, "Characters");
        for (size_t i = 0; i < characters.size() && i < (size_t)(l.top_h - 2); i++) {
            const char *mark = ((int)i == selected) ? "> " : "  ";
            if ((int)i == selected) wattron(l.ctx_win, A_BOLD);
            mvwprintw(l.ctx_win, 1 + (int)i, 1, "%s%s", mark, characters[i].c_str());
            if ((int)i == selected) wattroff(l.ctx_win, A_BOLD);
        }
        int new_y = 1 + (int)characters.size();
        if (new_y < l.top_h - 1) {
            const char *mark = (selected == (int)characters.size()) ? "> " : "  ";
            if (selected == (int)characters.size()) wattron(l.ctx_win, A_BOLD);
            mvwprintw(l.ctx_win, new_y, 1, "%s[New Character]", mark);
            if (selected == (int)characters.size()) wattroff(l.ctx_win, A_BOLD);
        }
        wrefresh(l.ctx_win);

        // Stats pane: preview selected character
        if (selected < (int)characters.size()) {
            try {
                Character preview("Tmp", "Chr");
                preview.load(".rpg/" + characters[selected]);
                draw_stats(l, preview);
            } catch (...) {
                werase(l.stats_win);
                draw_box_title(l.stats_win, "Stats");
                mvwprintw(l.stats_win, 1, 2, "Failed to load");
                wrefresh(l.stats_win);
            }
        } else {
            werase(l.stats_win);
            draw_box_title(l.stats_win, "Stats");
            mvwprintw(l.stats_win, 2, 2, "New character");
            wrefresh(l.stats_win);
        }

        int key = getch();
        switch (key) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < (int)characters.size()) selected++;
                break;
            case '\n': {
                if (selected < (int)characters.size()) {
                    Character loaded("Tmp", "Chr");
                    loaded.load(".rpg/" + characters[selected]);
                    destroy_layout(l);
                    return loaded;
                }
                // fall through to create new character
                [[fallthrough]];
            }
            case 'n':
            case 'N': {
                std::string first = prompt_input(l, "First name: ");
                std::string last  = prompt_input(l, "Last name:  ");
                Character newch(first, last);
                newch.save();
                destroy_layout(l);
                return newch;
            }
            case 'q':
            case 'Q':
                destroy_layout(l);
                endwin();
                exit(0);
        }
    }
}

// ============================================================================
// Screen: Combat
// ============================================================================

int combat_screen(Character &player, Monster &opponent) {
    WindowLayout l = create_layout();
    std::vector<std::string> log = {"Combat started!"};
    int result = 10;  // in-progress
    int enemy_max_hp = opponent.get_hp();

    while (result == 10) {
        // Context pane: side-by-side combatant info
        werase(l.ctx_win);
        draw_box_title(l.ctx_win, "Combat");

        int half   = (l.ctx_w - 4) / 2;
        int bar_w  = std::max(4, half - 8);
        int player_max_hp = player.get_str() * player.get_fort();

        wattron(l.ctx_win, COLOR_PAIR(4) | A_BOLD);
        mvwprintw(l.ctx_win, 1, 2,          "%-*s", half, player.get_name().c_str());
        mvwprintw(l.ctx_win, 1, 2 + half,   opponent.get_species().c_str());
        wattroff(l.ctx_win,  COLOR_PAIR(4) | A_BOLD);

        mvwprintw(l.ctx_win, 2, 2,         "HP:");
        draw_hp_bar(l.ctx_win, 2, 6, player.get_hp(), player_max_hp, bar_w);
        mvwprintw(l.ctx_win, 3, 2,         "%d/%d", player.get_hp(), player_max_hp);

        mvwprintw(l.ctx_win, 2, 2 + half,  "HP:");
        draw_hp_bar(l.ctx_win, 2, 2 + half + 4, opponent.get_hp(), enemy_max_hp, bar_w);
        mvwprintw(l.ctx_win, 3, 2 + half,  "%d", opponent.get_hp());

        wrefresh(l.ctx_win);

        draw_stats(l, player);
        render_log(l, log);
        set_input_hint(l, "[a] Attack  [e] Escape");

        int key = getch();
        if (key == 'a' || key == 'A') {
            int dmg = player.attack(opponent);
            opponent.wound(dmg);
            append_log(l, log, player.get_name() + " attacks for " + std::to_string(dmg) + " damage!");

            if (opponent.get_hp() <= 0) {
                append_log(l, log, "Enemy defeated!");
                result = 1;
            } else {
                int odm = opponent.attack(player);
                player.wound(odm);
                append_log(l, log, "Enemy attacks for " + std::to_string(odm) + " damage!");
                if (player.get_hp() <= 0) {
                    append_log(l, log, "You were defeated!");
                    result = 0;
                }
            }
        } else if (key == 'e' || key == 'E') {
            append_log(l, log, "Escaped from combat!");
            result = -1;
        }
    }

    // Show outcome in context pane
    werase(l.ctx_win);
    draw_box_title(l.ctx_win, "Combat");
    const char *msg   = (result == 1) ? "Victory!" : (result == 0) ? "Defeat!" : "Escaped!";
    int         color = (result == 1) ? 1           : (result == 0) ? 3          : 2;
    wattron(l.ctx_win,  COLOR_PAIR(color) | A_BOLD);
    mvwprintw(l.ctx_win, l.top_h / 2, (l.ctx_w - (int)strlen(msg)) / 2, "%s", msg);
    wattroff(l.ctx_win,  COLOR_PAIR(color) | A_BOLD);
    wrefresh(l.ctx_win);
    sleep(2);

    destroy_layout(l);
    return result;
}

// ============================================================================
// Screen: Main Game Loop
// ============================================================================

void main_screen(Character &player) {
    Combat_Location arena;

    WindowLayout l = create_layout();
    std::vector<std::string> log = {"Welcome to the Arena!"};
    bool done = false;

    while (!done) {
        // Context pane: arena title and action list
        werase(l.ctx_win);
        draw_box_title(l.ctx_win, "The Arena");
        int y = 2;
        wattron(l.ctx_win, COLOR_PAIR(4));
        mvwprintw(l.ctx_win, y++, 3, "[f] Fight");
        mvwprintw(l.ctx_win, y++, 3, "[h] Heal  (-10 EXP)");
        mvwprintw(l.ctx_win, y++, 3, "[g] Grow  (+80 EXP)");
        mvwprintw(l.ctx_win, y++, 3, "[Q] Quit & Save");
        wattroff(l.ctx_win, COLOR_PAIR(4));
        wrefresh(l.ctx_win);

        draw_stats(l, player);
        render_log(l, log);
        set_input_hint(l, "[f] Fight  [h] Heal  [g] Grow  [Q] Quit");

        int key = getch();
        switch (key) {
            case 'f':
            case 'F': {
                Monster enemy;
                enemy.random_spawn();
                destroy_layout(l);
                int res = combat_screen(player, enemy);
                l = create_layout();
                if (res == 1) {
                    player.award_exp(5);
                    append_log(l, log, "Victory! +5 EXP");
                } else if (res == 0) {
                    append_log(l, log, "You died! Game over.");
                    done = true;
                } else {
                    append_log(l, log, "Escaped from combat.");
                }
                break;
            }
            case 'h':
            case 'H': {
                if (player.consume_exp(10)) {
                    player.heal_direct();
                    append_log(l, log, "Healed to full HP! (-10 EXP)");
                } else {
                    append_log(l, log, "Not enough EXP to heal!");
                }
                break;
            }
            case 'g':
            case 'G': {
                player.award_exp(80);
                append_log(l, log, "Power increased! (+80 EXP)");
                break;
            }
            case 'Q': {
                player.save();
                append_log(l, log, "Game saved. Farewell!");
                done = true;
                break;
            }
        }
    }

    destroy_layout(l);
}

// ============================================================================
// Main
// ============================================================================
/*
int main() {
    NCursesGuard guard;
    init_ncurses();

    ensure_rpg_dir();
    Character player = character_select();
    main_game(player);

    return 0;
}
*/
