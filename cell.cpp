#include<iostream>
#include<vector>
#include"cell.h"
#include"field.h"
#include"constants.h"

using namespace std;
using namespace constants;

// Конструктор клітинки з координатами
cell::cell(int x, int y) : is_tree(false), is_tent(false), self(x, y) {
}

// Конструктор клітинки за замовчуванням (позиція 0,0)
cell::cell() : is_tree(false), is_tent(false), self(0, 0) {
}

// Перевіряє, чи є в клітинці дерево
bool cell::has_tree() {
    return is_tree;
}

// Перевіряє, чи є в клітинці намет
bool cell::has_tent() {
    return is_tent;
}

// Встановлює дерево в клітинку
void cell::set_tree() {
    is_tree = true;
}

// Встановлює намет в клітинку
void cell::set_tent() {
    is_tent = true;
}

// Видаляє намет із клітинки
void cell::remove_tent() {
    is_tent = false;
}

// Повертає вектор, що описує ситуацію навколо клітинки:
// якщо поряд є намет — відповідна позиція вектора стає false
vector<pair<int, bool>> cell::situation(field f) {
    xy directions[NEIGHBOR_COUNT] = {
        {self.x - 1, self.y},
        {self.x + 1, self.y},
        {self.x, self.y - 1},
        {self.x, self.y + 1},
        {self.x - 1, self.y - 1},
        {self.x - 1, self.y + 1},
        {self.x + 1, self.y - 1},
        {self.x + 1, self.y + 1}
    };

    vector<pair<int, bool>> free(NEIGHBOR_COUNT, {0, true});

    for (int i = 0; i < NEIGHBOR_COUNT; i++) {
        int nx = directions[i].x;
        int ny = directions[i].y;

        if (nx >= 0 && nx < f.get_rows() && ny >= 0 && ny < f.get_cols() &&
            f.get_field()[nx][ny].has_tent()) {
            free[i].second = false; // Поруч є намет — клітинка не підходить
        }
    }

    return free;
}