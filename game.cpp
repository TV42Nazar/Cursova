#include <iostream>
#include <vector>

#include"game.h"
#include"field.h"
#include"logic.h"
#include"constants.h"

using namespace std;
using namespace constants;

    void game::analyze(field main_field) {
        // Копіюємо поле в новий масив
        cell** temp_field = new cell*[main_field.get_rows()];
        for (int i = 0; i < main_field.get_rows(); i++) {
            temp_field[i] = new cell[main_field.get_cols()];
            for (int j = 0; j < main_field.get_cols(); j++) {
                temp_field[i][j] = main_field.get_field()[i][j];
            }
        }

        // Збираємо всі дерева в список
        vector<xy> trees;
        for (int i = 0; i < main_field.get_rows(); i++) {
            for (int j = 0; j < main_field.get_cols(); j++) {
                if (temp_field[i][j].has_tree()) {
                    trees.push_back(xy(i, j));
                }
            }
        }

        // Створюємо тимчасовий об'єкт поля з копії
        field temp_class_field(temp_field, main_field.get_rows(), main_field.get_cols());

        // Запускаємо рекурсивний розв’язувач
        if (!logic::backtrack_tents(temp_class_field, trees, 0)) {
            cout << "No valid arrangement found.\n";
            // Звільнення пам’яті
            for (int i = 0; i < main_field.get_rows(); i++) delete[] temp_field[i];
            delete[] temp_field;
            return;
        }

        // Підрахунок кількості наметів у кожному рядку і стовпці
        vector<int> row_tents(main_field.get_rows(), 0);
        vector<int> col_tents(main_field.get_cols(), 0);
        for (int i = 0; i < main_field.get_rows(); i++) {
            for (int j = 0; j < main_field.get_cols(); j++) {
                if (temp_field[i][j].has_tent()) {
                    row_tents[i]++;
                    col_tents[j]++;
                }
            }
        }

        // Вивід аналізованого поля (без наметів, лише з деревами і підказками)
        cout << "\nAnalyzed field (without tents):\n";
        for (int i = 0; i < main_field.get_rows(); i++) {
            for (int j = 0; j < main_field.get_cols(); j++) {
                if (main_field.get_field()[i][j].has_tree()) {
                    cout << CHAR_TREE << " ";
                } else {
                    cout << CHAR_EMPTY << " ";
                }
            }
            cout << "| " << row_tents[i] << "\n";
        }

        // Підказки для стовпців
        for (int j = 0; j < main_field.get_cols(); j++) cout << "--";
        cout << "\n";
        for (int j = 0; j < main_field.get_cols(); j++) cout << col_tents[j] << " ";
        cout << "\n";

        // Показати розв’язок із наметами, якщо користувач бажає
        cout << "\nShow solution with tents? (y to show): ";
        char choice;
        if(!(cin >> choice)){
            choice = (char)"y";
            
        }

        if (choice == 'y' || choice == 'Y') {
            temp_class_field.print_field();
        }
        // Звільнення пам’яті
        for (int i = 0; i < main_field.get_rows(); i++) delete[] temp_field[i];
        delete[] temp_field;
    }

    // Режим ручного розв'язання задачі користувачем
    void game::manual_solve_mode(field main_field) {
        // Копія поля для зберігання правильного розв’язку
        cell** temp_field = new cell*[main_field.get_rows()];
        for (int i = 0; i < main_field.get_rows(); i++) {
            temp_field[i] = new cell[main_field.get_cols()];
            for (int j = 0; j < main_field.get_cols(); j++) {
                temp_field[i][j] = main_field.get_field()[i][j];
            }
        }

        // Збір дерев
        vector<xy> trees;
        for (int i = 0; i < main_field.get_rows(); i++) {
            for (int j = 0; j < main_field.get_cols(); j++) {
                if (main_field.get_field()[i][j].has_tree()) {
                    trees.push_back(xy(i, j));
                }
            }
        }

        // Створюємо клас поля з копії
        field temp_class_field(temp_field, main_field.get_rows(), main_field.get_cols());

        // Автоматичне розміщення наметів для створення шаблону-розв’язку
        if (!logic::backtrack_tents(temp_class_field, trees, 0)) {
            cout << "No valid solution exists.\n";
            for (int i = 0; i < main_field.get_rows(); i++){
                delete[] temp_field[i];
            }
            delete[] temp_field;
            return;
        }

        // Підрахунок правильної кількості наметів у рядках/стовпцях
        int total = 0;
        vector<int> required_row_tents(main_field.get_rows(), 0);
        vector<int> required_col_tents(main_field.get_cols(), 0);
        for (int i = 0; i < main_field.get_rows(); i++) {
            for (int j = 0; j < main_field.get_cols(); j++) {
                if (temp_field[i][j].has_tent()) {
                    total++;
                    required_row_tents[i]++;
                    required_col_tents[j]++;
                }
            }
        }

        // Користувацьке поле, яке заповнює гравець
        cell** user_field = new cell*[main_field.get_rows()];
        for (int i = 0; i < main_field.get_rows(); i++) {
            user_field[i] = new cell[main_field.get_cols()];
            for (int j = 0; j < main_field.get_cols(); j++) {
                user_field[i][j] = main_field.get_field()[i][j];
            }
        }

        // Кількість правильних розміщень
        int correct = 0;
        while (correct < total) {
            system("cls");
            cout << "=== Tent Puzzle ===\n\n";
            // Вивід поля
            for (int i = 0; i < main_field.get_rows(); i++) {
                for (int j = 0; j < main_field.get_cols(); j++) {
                    if (main_field.get_field()[i][j].has_tree()) {
                        cout << CHAR_TREE << " ";
                    } else if (user_field[i][j].has_tent()) {
                        cout << "^ ";
                    } else {
                        cout << CHAR_EMPTY << " ";
                    }
                }
                cout << "| " << required_row_tents[i] << "\n";
            }
            for (int j = 0; j < main_field.get_cols(); j++) cout << "--";
            cout << "\n";
            for (int j = 0; j < main_field.get_cols(); j++) cout << required_col_tents[j] << " ";
            cout << "\n";

            // Введення координат
            int x, y;
            cout << "\ncoords (row col): ";
            if(!(cin >> x >> y)){
                std::cout<<"invalid input";
                continue;
            }

            // Перевірка на допустимість координат
            if (x < 0 || x >= main_field.get_rows() || y < 0 || y >= main_field.get_cols()) {
                cout << "Out of bounds!\n";
                continue;
            }

            if (user_field[x][y].has_tent()) {
                cout << "You already placed a tent there!\n";
                continue;
            }

            // Перевірка правильності розміщення
            if (temp_field[x][y].has_tent()) {
                user_field[x][y].set_tent();
                correct++;
                cout << "Correct!\n";
            } else {
                cout << "Incorrect.\n";
            }

            system("pause");
        }

        // Вивід фінального повідомлення
        system("cls");
        cout << "Congratulations! All " << total << " tents placed correctly!\n\n";

        // Звільнення пам’яті
        for (int i = 0; i < main_field.get_rows(); i++) {
            delete[] temp_field[i];
            delete[] user_field[i];
        }
        delete[] temp_field;
        delete[] user_field;
    }



    void game::main_menu() {
        field F;
        bool has_field = false;
    
        while (true) {
            cout << "\n===== MENU =====\n"
                      << "1. Load from file\n"
                      << "2. Input from keyboard\n"
                      << "3. Generate random\n"
                      << "4. Solve (auto)\n"
                      << "5. Print\n"
                      << "6. Save to file\n"
                      << "7. Analyze\n"
                      << "8. Solve manually\n"
                      << "0. Exit\n"
                      << "Choice: ";
    
            int ch;
            if (!(std::cin >> ch)) {
                cout << "Invalid input!\n";
                clear_input();
                continue;
            }
            clear_input();
    
            switch (ch) {
                case 0:
                    cout << "Bye!\n";
                    return;
    
                case 1:
                    F.load_field_from_file("input.txt");
                    has_field = true;
                    break;
    
                case 2:
                    F.read_field_from_stdin();
                    has_field = true;
                    break;
    
                case 3: {
                    int r, c, tc;
                    cout << "Rows Columns: ";
                    if (!(cin >> r >> c)) {
                        cout << "Invalid input!\n";
                        clear_input();
                        break;
                    }
    
                    cout << "Tree count(-1=default): ";
                    if (!(cin >> tc)) {
                        cout << "Invalid input!\n";
                        clear_input();
                        break;
                    }
    
                    clear_input();
                    F.cleanup_field();
                    F.set_rows(r);
                    F.set_cols(c);
                    F.generate_field(tc);
                    has_field = true;
                    break;
                }
    
                case 4:
                    if (has_field) {
                        F.cleanup_tents();
                        logic::place_tents(F);
                    } else {
                        std::cout << "No field loaded\n";
                    }
                    break;
    
                case 5:
                    if (has_field) {
                        F.print_field();
                    } else {
                        std::cout << "No field loaded\n";
                    }
                    break;
    
                case 6:
                    if (has_field) {
                        F.save_result("output.txt");
                        cout << "Saved to output.txt\n";
                    } else {
                        cout << "No field loaded\n";
                    }
                    break;
    
                case 7:
                    if (has_field) {
                        F.cleanup_tents();
                        game::analyze(F);
                    } else {
                        std::cout << "No field loaded\n";
                    }
                    break;
    
                case 8:
                    if (has_field) {
                        F.cleanup_tents();
                        game::manual_solve_mode(F);
                    } else {
                        cout << "No field loaded\n";
                    }
                    break;
    
                default:
                    cout << "Invalid choice\n";
            }
        }
    }

    void game::clear_input() {
        std::cin.clear();            // Скидає флаги помилок
        while (std::cin.get() != '\n'); // Чистить буфер до кінця рядка
    }
    
