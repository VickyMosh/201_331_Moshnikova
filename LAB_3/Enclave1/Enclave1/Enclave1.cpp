#include <iostream>
#include <string>

using namespace std;

const int MAX_SIZE = 100; // Максимальный размер хранилища данных

// Класс, реализующий табличное хранилище данных
class TableStorage {
private:
    string data[MAX_SIZE]; // Массив строк, имитирующий хранилище данных
    int size; // Размер хранилища данных

public:
    // Конструктор по умолчанию
    TableStorage() {
        size = 0;
    }

    // Добавление элемента в хранилище данных
    void addData(string value) {
        if (size >= MAX_SIZE) {
            cout << "Хранилище данных полное\n";
            return;
        }

        data[size] = value;
        size++;
    }

    // Получение элемента данных по индексу
    string getData(int index) {
        if (index < 0 || index >= size) {
            return "";
        }

        return data[index];
    }

    // Получение размера хранилища данных
    int getSize() {
        return size;
    }
};

// Функция для вывода элемента данных по индексу
void printData(TableStorage& storage, int index) {
    string data = storage.getData(index);

    if (data == "") {
        cout << "Элемент данных с индексом " << index << " не найден\n";
    }
    else {
        cout << "Элемент данных с индексом " << index << ": " << data << "\n";
    }
}

int main() {
    TableStorage storage;

    // Заполнение хранилища данных
    storage.addData("Данные 1");
    storage.addData("Данные 2");
    storage.addData("Данные 3");
    storage.addData("Данные 4");
    storage.addData("Данные 5");

    // Получение элемента данных по индексу
    int index;
    cout << "Введите индекс элемента данных: ";
    cin >> index;

    printData(storage, index);

    return 0;
}
