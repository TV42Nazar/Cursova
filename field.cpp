#include<iostream>
#include<random>
#include<algorithm>
#include<fstream>
#include"field.h"
#include"cell.h"
#include"constants.h"

using namespace std;
using namespace constants;

// Конструктор за замовчуванням — поле порожнє
    field::field() {
        rows = 0;
        cols = 0;
        grid = nullptr;
    }

    // Конструктор з уже готовим масивом клітинок
    field::field(cell ** grid, int rows, int cols) {
        this->grid = grid;
        this->rows = rows;
        this->cols = cols;
    }

    // Геттери
    int field::get_rows() { return rows; }
    int field::get_cols() { return cols; }
    cell** field::get_field() { return grid; }

    // Сеттери
    void field::set_rows(int num) { rows = num; }
    void field::set_cols(int num) { cols = num; }

    // Вивід поля з підрахунком наметів
    void field::print_field() {
        vector<int> row_tents(rows, 0);
        vector<int> col_tents(cols, 0);

        // Підрахунок кількості наметів у кожному рядку/стовпці
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j].has_tent()) {
                    row_tents[i]++;
                    col_tents[j]++;
                }
            }
        }

        // Виведення сітки
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j].has_tree()) {
                    cout << CHAR_TREE << " ";
                } else if (grid[i][j].has_tent()) {
                    cout << "^ ";
                } else {
                    cout << CHAR_EMPTY << " ";
                }
            }
            cout << "| " << row_tents[i] << "\n";
        }

        // Розділова лінія
        for (int j = 0; j < cols; j++) {
            cout << "--";
        }
        cout << "\n";

        // Підпис знизу: кількість наметів у стовпцях
        for (int j = 0; j < cols; j++) {
            cout << col_tents[j] << " ";
        }
        cout << "\n";
    }

    void field::generate_field(int tree_count) {
        if (grid != nullptr) cleanup_field(); // уникнення витоків памʼяті

        // Виділення памʼяті
        grid = new cell*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new cell[cols];
        }

        // Визначення кількості дерев, якщо не задано
        if (tree_count == -1) {
            tree_count = (rows * cols) / DEFAULT_TREE_DIVISOR;
        }

        // Підготовка випадкових позицій
        vector<xy> positions;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                positions.push_back(xy(i, j));
            }
        }

        // Перемішування позицій і розміщення дерев
        random_device rd;
        mt19937 gen(rd());
        shuffle(positions.begin(), positions.end(), gen);

        for (int i = 0; i < min(tree_count, static_cast<int>(positions.size())); i++) {
            int x = positions[i].x;
            int y = positions[i].y;
            grid[x][y] = cell(x, y);
            grid[x][y].set_tree();
        }

        // Ініціалізація інших клітинок
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (!grid[i][j].has_tree()) {
                    grid[i][j] = cell(i, j);
                }
            }
        }
    }
    // Зчитування поля з консолі
        void field::read_field_from_stdin() {
        if (grid != nullptr) cleanup_field(); // очищення перед новим зчитуванням

        // Зчитування розмірів
        cout << "Enter number of rows: ";
        while (!(cin >> rows) || rows <= 0) {
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Invalid input. Enter a positive number for rows: ";
        }

        cout << "Enter number of columns: ";
        while (!(cin >> cols) || cols <= 0) {
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Invalid input. Enter a positive number for columns: ";
        }

        // Виділення памʼяті
        grid = new cell*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new cell[cols];
        }

        // Зчитування по рядках
        cout << "Enter the field row by row (use '" << CHAR_TREE
            << "' for tree, '" << CHAR_EMPTY
            << "' for empty):\n";

        for (int i = 0; i < rows; i++) {
            string line;
            // тут >> std::ws спочатку з’їсть усі переводи рядка/пробіли,
            // тому getline завжди чекатиме наступного повноцінного рядка
            getline(cin >> ws, line);

            for (int j = 0; j < cols; j++) {
                grid[i][j] = cell(i, j);
                if (j < static_cast<int>(line.size()) && line[j] == CHAR_TREE) {
                    grid[i][j].set_tree();
                }
            }
        }
    }

    // Завантаження поля з файлу
    void field::load_field_from_file(const string& filename) {
        if (grid != nullptr) cleanup_field(); // очищення перед завантаженням

        ifstream input(filename);
        if (!input.is_open()) {
            cout << "Error: cannot open file\n";
            return;
        }

        vector<string> lines;
        string line;
        while (getline(input, line)) {
            string clean_line;
            for (char ch : line) {
                if (ch != ' ') clean_line += ch;
            }
            if (!clean_line.empty()) {
                lines.push_back(clean_line);
            }
        }

        if (lines.empty()) {
            cout << "Error loading failed\n";
            return;
        }

        rows = static_cast<int>(lines.size());
        cols = static_cast<int>(lines[0].size());

        grid = new cell*[rows];
        for (int i = 0; i < rows; i++) {
            grid[i] = new cell[cols];
            for (int j = 0; j < cols; j++) {
                grid[i][j] = cell(i, j);
                char ch = lines[i][j];
                if (ch == CHAR_TREE) {
                    grid[i][j].set_tree();
                } else if (ch != CHAR_EMPTY && ch != CHAR_TENT) {
                    lines[i][j] = CHAR_EMPTY;
                }
            }
        }
    }

    // Збереження результату в файл
    void field::save_result(const string& filename){
        ofstream out(filename);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j].has_tree()) {
                    out << CHAR_TREE;
                } else if (grid[i][j].has_tent()) {
                    out << CHAR_TENT;
                } else {
                    out << CHAR_EMPTY;
                }
            }
            out << "\n";
        }
        out.close();
    }

    // Повне очищення памʼяті поля
    void field::cleanup_field() {
        for (int i = 0; i < rows; i++) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
        rows = 0;
        cols = 0;
    }

    // Видалення всіх наметів (без знищення дерев)
    void field::cleanup_tents() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j].has_tent()) {
                    grid[i][j].remove_tent();
                }
            }
        }
    }
