#include <string>
#include <vector>
#include "tui.hpp"
#include "utils.h"

GameWindow::GameWindow() {
    create_layout();
}

GameWindow::~GameWindow() {
    destroy_layout();
    endwin();
}
// ============================================================================
// GameWindow - Layout
// ============================================================================
void GameWindow::create_layout() {
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

    getmaxyx(stdscr, total_h, total_w);
    input_h = 3;
    stats_w = std::max(26, total_w / 3);
    ctx_w   = total_w - stats_w;
    log_h   = std::max(6, total_h / 3);
    top_h   = std::max(6, total_h - log_h - input_h);

    ctx_win   = newwin(top_h,   ctx_w,   0,                       0);
    stats_win = newwin(top_h,   stats_w, 0,                       ctx_w);
    log_win   = newwin(log_h,   total_w, top_h,                 0);
    input_win = newwin(input_h, total_w, top_h + log_h,      0);

    // Set background color for all cells (not just cells with text).
    wbkgd(ctx_win,   COLOR_PAIR(5));
    wbkgd(stats_win, COLOR_PAIR(5));
    wbkgd(log_win,   COLOR_PAIR(5));
    wbkgd(input_win, COLOR_PAIR(5));
}

void GameWindow::destroy_layout() {
    delwin(ctx_win);
    delwin(stats_win);
    delwin(log_win);
    delwin(input_win);
    ctx_win = stats_win = log_win = input_win = nullptr;
}

// ============================================================================
// GameWindow - Drawing helpers
// ============================================================================

void GameWindow::draw_box_title(WINDOW *win, const std::string &title) {
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

void GameWindow::draw_hp_bar(WINDOW *win, int y, int x, int cur, int max_val, int width) {
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

void GameWindow::draw_stats(Character &player) {
    werase(stats_win);
    draw_box_title(stats_win, "Stats");

    int max_hp = player.get_str() * player.get_fort();
    int bar_w  = std::max(4, stats_w - 10);

    int y = 1;
    mvwprintw(stats_win, y++, 2, "%-*s", stats_w - 4, player.get_name().c_str());
    mvwprintw(stats_win, y++, 2, "EXP: %d", player.get_exp());
    mvwprintw(stats_win, y,   2, "HP:  ");
    draw_hp_bar(stats_win, y, 7, player.get_hp(), max_hp, bar_w);
    y++;
    mvwprintw(stats_win, y++, 2, "     %d/%d", player.get_hp(), max_hp);
    y++;
    mvwprintw(stats_win, y++, 2, "STR:%-3d  AGI:%-3d", player.get_str(),   player.get_agi());
    mvwprintw(stats_win, y++, 2, "INT:%-3d  CHA:%-3d", player.get_intel(), player.get_cha());
    mvwprintw(stats_win, y++, 2, "FRT:%-3d  WIS:%-3d", player.get_fort(),  player.get_wis());
    wrefresh(stats_win);
}

void GameWindow::draw_ctx(const std::string title, const std::vector<std::string> &lines) {
    werase(ctx_win);
    draw_box_title(ctx_win, title);
    int y = 1;
    int line_length = ctx_w - 4;
    for (const std::string &line : lines) {
        mvwprintw(ctx_win, y++, 2, "%s", line.c_str());
    }
    wrefresh(ctx_win);
}

void GameWindow::render_log() {
    werase(log_win);
    draw_box_title(log_win, "Log");
    int inner_h = log_h - 2;
    int inner_w = total_w - 4;
    size_t start = (log_data.size() > (size_t)inner_h) ? log_data.size() - inner_h : 0;
    for (size_t i = start; i < log_data.size(); i++) {
        std::string line = log_data[i];
        if ((int)line.size() > inner_w) line = line.substr(0, inner_w);
        mvwprintw(log_win, 1 + (int)(i - start), 2, "%s", line.c_str());
    }
    wrefresh(log_win);
}

void GameWindow::append_log(const std::string &msg) {
    log_data.push_back(msg);
    render_log();
}

void GameWindow::log(const std::string &msg) {
    append_log(msg);
    render_log();
}

void GameWindow::set_input_hint(const std::string &hint) {
    werase(input_win);
    box(input_win, 0, 0);
    wattron(input_win, COLOR_PAIR(4));
    mvwprintw(input_win, 1, 2, "%s", hint.c_str());
    wattroff(input_win, COLOR_PAIR(4));
    wrefresh(input_win);
}

// Prompt for a single line of text inside the input subwindow.
std::string GameWindow::prompt_input(const std::string &prompt) {
    werase(input_win);
    box(input_win, 0, 0);
    wattron(input_win, COLOR_PAIR(4));
    mvwprintw(input_win, 1, 2, "%s", prompt.c_str());
    wattroff(input_win, COLOR_PAIR(4));
    wrefresh(input_win);

    echo();
    curs_set(1);
    char buf[64] = {};
    mvwgetnstr(input_win, 1, 2 + (int)prompt.size(), buf, 63);
    noecho();
    curs_set(0);
    return std::string(buf);
}

std::vector<std::map<std::string, std::string>> GameWindow::render_scene(Character &player, Location &location) {
    GameWindow::draw_ctx(location.get_name(), split(location.get_description(), '\n'));
    std::string action_hint = "";
    for (const auto &action : location.get_actions()) {
        if (!action.name.empty()) {
            action_hint += "[" + std::string(1, action.name[0]) + "] " + action.name + "  ";
        }
    }
    GameWindow::set_input_hint(action_hint + "[Q] Quit");
    GameWindow::draw_stats(player);
    int key = getch();
    for (const auto &action : location.get_actions()) {
        if (!action.name.empty() && (key == action.name[0] || key == std::tolower(action.name[0]))) {
            GameWindow::append_log({action.description});
            return action.results;
        }
    }
    if (key == 'q' || key == 'Q') {
        GameWindow::append_log({"Quitting..."});
    }
    return { { {"type", "quit"} } };
}

std::vector<std::map<std::string, std::string>> GameWindow::render_combat(Character &player, Monster &opponent) {
    werase(ctx_win);
    draw_box_title(ctx_win, "Combat");

    int half   = (ctx_w - 4) / 2;
    int bar_w  = std::max(4, half - 8);

    wattron(ctx_win, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(ctx_win, 1, 2,          "%-*s", half, opponent.get_species().c_str());
    wattroff(ctx_win,  COLOR_PAIR(4) | A_BOLD);

    mvwprintw(ctx_win, 2, 2,         "HP:");
    draw_hp_bar(ctx_win, 2, 6, opponent.get_hp(), opponent.get_max_hp(), bar_w);
    mvwprintw(ctx_win, 3, 2,         "%d/%d", opponent.get_hp(), opponent.get_max_hp());

    wrefresh(ctx_win);
    draw_stats(player);
    render_log();
    std::string action_hint = "";
    for (const auto &action : player.get_actions()) {
        if (!action.name.empty()) {
            action_hint += "[" + std::string(1, action.name[0]) + "] " + action.name + "  ";
        }
    }
    set_input_hint(action_hint + "[E] Escape");
    int key = getch();

    for (const auto &action : player.get_actions()) {
        if (!action.name.empty() && (key == action.name[0] || key == std::tolower(action.name[0]))) {
            append_log({"Selected combat action: " + action.name});
            return action.results;
        }
    }
    if (key == 'e' || key == 'E') {
        append_log({"Escaping..."});
        return { { {"type", "escape"} } };
    }
    return { { {"type", "none"} } };
}
