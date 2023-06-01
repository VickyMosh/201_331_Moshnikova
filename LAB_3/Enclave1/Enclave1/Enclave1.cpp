#include <iostream>
#include <string>

using namespace std;

const int MAX_SIZE = 100; // ������������ ������ ��������� ������

// �����, ����������� ��������� ��������� ������
class TableStorage {
private:
    string data[MAX_SIZE]; // ������ �����, ����������� ��������� ������
    int size; // ������ ��������� ������

public:
    // ����������� �� ���������
    TableStorage() {
        size = 0;
    }

    // ���������� �������� � ��������� ������
    void addData(string value) {
        if (size >= MAX_SIZE) {
            cout << "��������� ������ ������\n";
            return;
        }

        data[size] = value;
        size++;
    }

    // ��������� �������� ������ �� �������
    string getData(int index) {
        if (index < 0 || index >= size) {
            return "";
        }

        return data[index];
    }

    // ��������� ������� ��������� ������
    int getSize() {
        return size;
    }
};

// ������� ��� ������ �������� ������ �� �������
void printData(TableStorage& storage, int index) {
    string data = storage.getData(index);

    if (data == "") {
        cout << "������� ������ � �������� " << index << " �� ������\n";
    }
    else {
        cout << "������� ������ � �������� " << index << ": " << data << "\n";
    }
}

int main() {
    TableStorage storage;

    // ���������� ��������� ������
    storage.addData("������ 1");
    storage.addData("������ 2");
    storage.addData("������ 3");
    storage.addData("������ 4");
    storage.addData("������ 5");

    // ��������� �������� ������ �� �������
    int index;
    cout << "������� ������ �������� ������: ";
    cin >> index;

    printData(storage, index);

    return 0;
}
