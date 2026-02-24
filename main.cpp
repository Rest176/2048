#define size_of_field 4

#include <iostream>
#include <vector>
#include <string>
//#include <windows.h>

using namespace std;

void draw_field(const vector<vector<unsigned short>>& field){
    for(unsigned short i = 0; i < size_of_field; ++i){
        cout << '|';
        for(unsigned short j = 0; j < size_of_field; ++j){
            cout << field[i][j] << '|';
        }
        cout << '\n';
    }
}

void draw_statistic(unsigned long long move_number, unsigned long long score){
    cout << "\n";
    cout << "Move number: " << move_number << '\n';
    cout << "Score: " << score << '\n';
}

int main(){
    vector<vector<unsigned short>> field(size_of_field, vector<unsigned short>(size_of_field));

    unsigned long long move_number = 0;
    unsigned long long score = 0;

    draw_field(field);
    draw_statistic(move_number, score);

    return 0;
}