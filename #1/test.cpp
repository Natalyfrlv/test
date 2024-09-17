#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <stdexcept> 

// Структура, представляющая студента с id, именем и возрастом
struct Student {
    int id;
    std::string name;
    int age;

    // Конструктор с проверкой корректности данных
    Student(int id, const std::string& name, int age)
        : id(id), name(name), age(age) {
        if (id <= 0 || age <= 0) {
            throw std::invalid_argument("ID and age must be positive."); // Проверка на положительные id и возраст
        }
    }
};

// Класс для управления базой данных студентов
class StudentDatabase {
private:
    // Используем std::map для хранения студентов по их ID
    std::map<int, std::shared_ptr<Student>> students;

public:
    // Добавление нового студента в базу данных
    bool addStudent(int id, const std::string& name, int age) {
        // Проверка на отрицательные или нулевые id и возраст
        if (id <= 0 || age <= 0 || name.empty()) { // Имя не должно быть пустым
            std::cerr << "ID, age, and name must be positive, and name must not be empty." << std::endl;
            return false; // Возвращаем false, если входные данные неверны
        }

        // Проверка, существует ли уже студент с таким ID
        if (students.find(id) != students.end()) {
            std::cerr << "A student with this ID already exists!" << std::endl;
            return false; // Возвращаем false, если студент уже существует
        }

        // Если проверки пройдены, добавляем студента
        students[id] = std::make_shared<Student>(id, name, age);
        std::cout << "Student added: " << name << std::endl;
        return true; // Успешное добавление студента
    }

    // Удаление студента по ID
    bool removeStudent(int id) {
        // Проверка на неотрицательный ID
        if (id <= 0) {
            std::cerr << "Invalid ID. ID must be positive." << std::endl;
            return false; // ID должен быть положительным
        }

        // Проверка, существует ли студент с таким ID
        if (students.find(id) == students.end()) {
            std::cerr << "Student with this ID not found!" << std::endl;
            return false; // Студент не найден
        }

        // Удаляем студента, если найден
        students.erase(id);
        std::cout << "Student removed: " << id << std::endl;
        return true; // Успешное удаление
    }

    // Получение информации о студенте по ID
    void getStudentInfo(int id) const {
        // Проверка на валидный ID
        if (id <= 0) {
            std::cerr << "Invalid ID. ID must be positive." << std::endl;
            return; // Завершаем, если ID неверный
        }

        auto it = students.find(id); // Ищем студента по ID
        if (it == students.end()) { // Если студент не найден
            std::cerr << "Student with this ID not found!" << std::endl;
            return; // Завершаем без вывода информации
        }

        // Если студент найден, выводим его данные
        auto student = it->second;
        std::cout << "Student information - ID: " << student->id << ", Name: " << student->name
            << ", Age: " << student->age << std::endl;
    }

    // Вывод всех студентов из базы данных
    void printAllStudents() const {
        if (students.empty()) { // Если база данных пуста
            std::cout << "No students in the database." << std::endl;
            return; // Завершаем, если нет студентов
        }

        // Вывод всех студентов
        std::cout << "All students in the database:" << std::endl;
        for (const auto& pair : students) {
            auto student = pair.second;
            std::cout << "ID: " << student->id << ", Name: " << student->name
                << ", Age: " << student->age << std::endl;
        }
    }
};

int main() {
    // Создаем базу данных студентов
    StudentDatabase db;

    try {
        // Добавляем студентов
        db.addStudent(1, "Nataly Frolova", 20);
        db.addStudent(2, "Alex Smith", 22);

        // Запрашиваем ввод данных для нового студента
        int new_id, new_age;
        std::string new_name;

        std::cout << "Enter new student ID: ";
        std::cin >> new_id;

        // Проверяем, корректен ли ввод
        if (std::cin.fail() || new_id <= 0) {
            std::cerr << "Invalid ID. Please enter a positive integer." << std::endl;
            return 1; // Завершаем, если ID некорректен
        }

        std::cout << "Enter new student's name: ";
        std::cin.ignore();
        std::getline(std::cin, new_name);

        if (new_name.empty()) { // Проверяем, что имя не пустое
            std::cerr << "Name cannot be empty. Please enter a valid name." << std::endl;
            return 1; // Завершаем, если имя пустое
        }

        std::cout << "Enter new student's age: ";
        std::cin >> new_age;

        if (std::cin.fail() || new_age <= 0) { // Проверяем, что возраст положительный
            std::cerr << "Invalid age. Please enter a positive integer." << std::endl;
            return 1; // Завершаем, если возраст некорректен
        }

        // Добавляем нового студента в базу данных
        db.addStudent(new_id, new_name, new_age);

        // Получаем информацию о студенте по ID
        int search_id;
        std::cout << "Enter student ID to get information about: ";
        std::cin >> search_id;

        db.getStudentInfo(search_id); // Выводим информацию

        // Удаляем студента по ID
        int remove_id;
        std::cout << "Enter student ID to remove: ";
        std::cin >> remove_id;

        db.removeStudent(remove_id); // Удаляем студента

        // Выводим всех студентов в базе данных
        db.printAllStudents();

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl; // Обрабатываем исключения
    }

    return 0;
}