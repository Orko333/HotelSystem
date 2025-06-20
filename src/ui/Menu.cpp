#include "ui/Menu.h"
#include "utils.h"
#include <limits>

// Оголошення функції, визначеної в іншому файлі
void clearConsole();

void Menu::run(UserRole currentUserRole) {
    bool running = true;
    while (running) {
        clearConsole();
        std::cout << "\n--- Головне меню ---\n";
        std::vector<int> displayedItems;
        int displayIndex = 1;

        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].canExecute(currentUserRole)) {
                std::cout << displayIndex << ". " << items[i].name << std::endl;
                displayedItems.push_back(i);
                displayIndex++;
            }
        }
        std::cout << "0. Вихід\n";
        std::cout << "--------------------\n";
        std::cout << "Ваш вибір: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Помилка: будь ласка, введіть номер.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера

        if (choice == 0) {
            running = false;
        } else if (choice > 0 && choice <= displayedItems.size()) {
            items[displayedItems[choice - 1]].action();
        } else {
            std::cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }
} 