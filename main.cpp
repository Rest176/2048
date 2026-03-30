#define size_of_field 4
#define high_block_chance 0.2

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <ctype.h>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

enum move_direction{
    UP, DOWN, LEFT, RIGHT
};

pair<int, int> get_move() {
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 'w' || key == 'W') return {1, 0};
            if (key == 's' || key == 'S') return {-1, 0};
            if (key == 'a' || key == 'A') return {0, -1};
            if (key == 'd' || key == 'D') return {0, 1};
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                    case 72: return {1, 0};  // Up
                    case 80: return {-1, 0}; // Down
                    case 75: return {0, -1}; // Left
                    case 77: return {0, 1};  // Right
                }
            }
        }
    }
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

bool isCharCaseInsensitive(char input_char, char target_char) {
    if(!input_char) return false;

    unsigned char uc_input = static_cast<unsigned char>(input_char);
    unsigned char uc_target = static_cast<unsigned char>(target_char);

    return tolower(uc_input) == tolower(uc_target);
}

void wait_for_symbol(char symbol){
    char input;

    do {
            input = getch();
        } while (!isCharCaseInsensitive(input, symbol));

    return;
}

int findMax(const vector<vector<unsigned short>>& field) {
    unsigned short maxVal = 0;

    for (const auto& row : field) {
        for (unsigned short element : row) {
            if (element > maxVal) {
                maxVal = element;
            }
        }
    }
    return maxVal;
}

unsigned short generate_block(vector<vector<unsigned short>>& field, unsigned count = 2) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<float> chance_dist(0.0f, 1.0f);

    unsigned plus_score = 0;

    for (int i = 0; i < count; ++i) {
        vector<pair<int, int>> empty_cells;

        for (int r = 0; r < field.size(); ++r) {
            for (int c = 0; c < field[r].size(); ++c) {
                if (field[r][c] == 0) {
                    empty_cells.push_back({r, c});
                }
            }
        }

        if (empty_cells.empty()) break;

        uniform_int_distribution<int> index_dist(0, empty_cells.size() - 1);
        pair<int, int> cell = empty_cells[index_dist(gen)];

        unsigned short block = (chance_dist(gen) < high_block_chance) ? 4 : 2;
        plus_score += block;

        field[cell.first][cell.second] = block;
    }

    return plus_score;
}

void draw_field(const vector<vector<unsigned short>>& field) {
    const int cell_width = to_string(findMax(field)).length();

    for (unsigned short i = 0; i < field.size(); ++i) {
        cout << '|';
        for (unsigned short j = 0; j < field[i].size(); ++j) {
            cout << setw(cell_width) << field[i][j] << " |";
        }
        cout << '\n';
    }
}

void draw_statistic(unsigned long long move_number, unsigned long long score){
    cout << "\n";
    cout << "Move number: " << move_number << '\n';
    cout << "Score: " << score << '\n';
}

void restart_game(vector<vector<unsigned short>>& field, unsigned long long& move_number, unsigned long long& score){
    for(unsigned short i = 0; i < size_of_field; ++i){
        for(unsigned short j = 0; j < size_of_field; ++j){
            field[i][j] = 0;
        }
    }

    move_number = 0;
    score = 0;
}

void move_block(vector<vector<unsigned short>>& field) {
    pair<int, int> direction = get_move();
    int dy = direction.first;
    int dx = direction.second;

    if (dy == 0 && dx == 0) return;

    int start = (dy == -1 || dx == 1) ? size_of_field - 1 : 0;
    int end = (dy == -1 || dx == 1) ? -1 : size_of_field;
    int step = (dy == -1 || dx == 1) ? -1 : 1;

    vector<vector<bool>> merged(size_of_field, vector<bool>(size_of_field, false));

    for (int i = (dy != 0 ? start : 0); i != (dy != 0 ? end : (int)size_of_field); i += (dy != 0 ? step : 1)) {
        for (int j = (dx != 0 ? start : 0); j != (dx != 0 ? end : (int)size_of_field); j += (dx != 0 ? step : 1)) {

            if (field[i][j] != 0) {
                int curr_y = i;
                int curr_x = j;

                while (true) {
                    int next_y = curr_y - dy;
                    int next_x = curr_x + dx;

                    if (next_y >= 0 && next_y < size_of_field && next_x >= 0 && next_x < size_of_field) {

                        if (field[next_y][next_x] == 0) {
                            field[next_y][next_x] = field[curr_y][curr_x];
                            field[curr_y][curr_x] = 0;
                            curr_y = next_y;
                            curr_x = next_x;
                        }
                        else if (field[next_y][next_x] == field[curr_y][curr_x] && !merged[next_y][next_x]) {
                            field[next_y][next_x] *= 2;
                            field[curr_y][curr_x] = 0;
                            merged[next_y][next_x] = true;
                            break;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

bool isAbleMove(const vector<vector<unsigned short>>& field){
    for(int i = 0; i < size_of_field; ++i){
        for(int j = 0; j < size_of_field; ++j){
            if(field[i][j] == field[i + 1][j] || field[i][j] == field[i - 1][j] || field[i][j] == field[i][j + 1] || field[i][j] == field[i][j - 1]){
                return true;
            }
        }
    }

    return false;
}

int main(){
    ShowConsoleCursor(false);

    vector<vector<unsigned short>> field(size_of_field, vector<unsigned short>(size_of_field));

    unsigned long long move_number = 0;
    unsigned long long score = 0;

    bool isGameOver = false;
    bool isWon = false;

    generate_block(field);
    draw_field(field);
    draw_statistic(move_number, score);

    while(!isWon && !isGameOver){
        move_block(field);

        move_number++;
        score += generate_block(field);

        system("CLS");

        draw_field(field);
        draw_statistic(move_number, score);

        if(findMax(field) >= 2048) isWon = true;
        isGameOver = !isAbleMove(field);
    }

    if(isWon)

    return 0;
}
