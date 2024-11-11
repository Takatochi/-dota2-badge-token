#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <cctype>
#include <algorithm>

// Функція для перевірки, чи є рядок числом
bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// Структура для зберігання інформації про героя
struct Hero {
    std::string name;
    std::unordered_map<std::string, int> properties;
    int difficultyScore = 0;
    int matches = 0;
};

// Функція для завантаження героїв з CSV файлу
std::vector<Hero> loadHeroes(const std::string& filename) {
    std::vector<Hero> heroes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return heroes;
    }

    std::string line;
    std::vector<std::string> headers;

    // Зчитування заголовків
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            headers.push_back(cell);
        }
    }

    // Зчитування даних про кожного героя
    while (std::getline(file, line)) {
        Hero hero;
        std::stringstream ss(line);
        std::string cell;
        size_t column = 0;

        while (std::getline(ss, cell, ',')) {
            if (column == 0) {
                hero.name = cell; // Перше значення - ім'я героя
            }
            else if (column < headers.size()) {
                if (headers[column] == "DifficultyScore") {
                    hero.difficultyScore = isNumber(cell) ? std::stoi(cell) : 0;
                }
                else if (headers[column] == "Matches") {
                    hero.matches = isNumber(cell) ? std::stoi(cell) : 0;
                }
                else {
                    hero.properties[headers[column]] = isNumber(cell) ? std::stoi(cell) : 0;
                }
            }
            ++column;
        }
        heroes.push_back(hero);
    }

    file.close();
    return heroes;
}

// Функція для пошуку героїв за властивостями та виведення таблиці
void displayHeroTableByProperties(const std::vector<Hero>& heroes, const std::vector<std::string>& properties) {
    int totalMatches = 0;
    int totalDifficultyScore = 0;
    std::unordered_map<std::string, int> totalProperties;
    std::cout << std::endl;
  
    // Виведення героїв, які відповідають принаймні одній з властивостей
    for (const auto& hero : heroes) {
        bool allPropertiesMatch = true;

        // Перевірка, чи герой має всі три властивості зі значенням, відмінним від 0
        for (const auto& property : properties) {
            if (hero.properties.find(property) == hero.properties.end() || hero.properties.at(property) == 0) {
                allPropertiesMatch = false;
                break;
            }
        }
        
        if (allPropertiesMatch) {
            // Виведення заголовку для героїв, які мають всі задані властивості
            std::cout << std::endl;
            std::cout << "Hero " << hero.name << " has all the specified properties:" << std::endl;
            std::cout << std::setw(15) << "Hero" << " " << std::setw(15) << "DifficultyScore";
            for (const auto& property : properties) {
                std::cout << std::setw(10) << property;
            }
            std::cout << std::endl;
            std::cout << std::setw(15) << hero.name
                << std::setw(15) << hero.difficultyScore;

            for (const auto& property : properties) {
                std::cout << std::setw(10) << hero.properties.at(property);
                totalProperties[property] += hero.properties.at(property);
            }
          
            
            std::cout << std::endl;
            std::cout << "------------------------------------" << std::endl;
            
        }
    }
    std::cout << std::endl;
     std::cout << std::setw(15) << "Hero" <<" "<< std::setw(15) << "DifficultyScore";
    for (const auto& property : properties) {
        std::cout << std::setw(10) << property;
    }
    std::cout << std::endl;
    for (const auto& hero : heroes) {
    int matchingProperties = 0;
    for (const auto& property : properties) {
        if (hero.properties.find(property) != hero.properties.end() && hero.properties.at(property) != 0) {
            ++matchingProperties;
        }
    }

    // Якщо герой має хоча б одну відповідну властивість
    if (matchingProperties > 0) {
        std::cout << std::setw(15) << hero.name
            << std::setw(15) << hero.difficultyScore;

        // Додаємо значення властивостей до таблиці
        for (const auto& property : properties) {
            if (hero.properties.at(property) != 0) { // Перевірка, чи значення не 0
                std::cout << std::setw(10) << hero.properties.at(property);
                totalProperties[property] += hero.properties.at(property);
            }
            else {
                std::cout << std::setw(10) << "-"; // Вивід "-" для нульових значень
            }
        }

        std::cout << std::endl;

        totalMatches += hero.matches;
        totalDifficultyScore += hero.difficultyScore;
    }
}
    // Виведення рядка Total
    std::cout << std::setw(15) << "Total"
        << std::setw(10) << totalMatches
        << std::setw(15) << totalDifficultyScore;

    for (const auto& property : properties) {
        std::cout << std::setw(10) << totalProperties[property];
    }

    std::cout << std::endl;
}
// Функція для пошуку героїв за властивостями та виведення таблиці
//void displayHeroTableByProperties(const std::vector<Hero>& heroes, const std::vector<std::string>& properties) {
//    int totalMatches = 0;
//    int totalDifficultyScore = 0;
//    std::unordered_map<std::string, int> totalProperties;
//
//    // Виведення заголовку таблиці
//    std::cout << std::setw(15) << "Hero" << std::setw(10) << "Matches" << std::setw(15) << "DifficultyScore";
//    for (const auto& property : properties) {
//        std::cout << std::setw(10) << property;
//    }
//    std::cout << std::endl;
//
//    // Виведення героїв, які відповідають принаймні одній з властивостей
//    for (const auto& hero : heroes) {
//        int matchingProperties = 0;
//        for (const auto& property : properties) {
//            if (hero.properties.find(property) != hero.properties.end() && hero.properties.at(property) != 0) {
//                ++matchingProperties;
//            }
//        }
//
//        // Якщо герой має хоча б одну відповідну властивість
//        if (matchingProperties > 0) {
//            std::cout << std::setw(15) << hero.name
//                << std::setw(10) << hero.matches
//                << std::setw(15) << hero.difficultyScore;
//
//            // Додаємо значення властивостей до таблиці
//            for (const auto& property : properties) {
//                if (hero.properties.at(property) != 0) { // Перевірка, чи значення не 0
//                    std::cout << std::setw(10) << hero.properties.at(property);
//                    totalProperties[property] += hero.properties.at(property);
//                }
//                else {
//                    std::cout << std::setw(10) << "-"; // Вивід "-" для нульових значень
//                }
//            }
//
//            std::cout << std::endl;
//
//            totalMatches += hero.matches;
//            totalDifficultyScore += hero.difficultyScore;
//        }
//    }
//
//    // Виведення рядка Total
//    std::cout << std::setw(15) << "Total"
//        << std::setw(10) << totalMatches
//        << std::setw(15) << totalDifficultyScore;
//
//    for (const auto& property : properties) {
//        std::cout << std::setw(10) << totalProperties[property];
//    }
//
//    std::cout << std::endl;
//}
int main() {
    // Завантаження даних з файлу
    std::vector<Hero> heroes = loadHeroes("heroes.csv");

    // Запит на властивості
    std::vector<std::string> properties(3);
    std::cout << "Enter three properties to search for a hero:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "Property " << (i + 1) << ": ";
        std::cin >> properties[i];
    }

    // Виведення таблиці за заданими властивостями
    displayHeroTableByProperties(heroes, properties);

    return 0;
}
