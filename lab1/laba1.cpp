#include <iostream>
#include <vector>
#include <set>

#define N 10000 //Количество символов в строке

using namespace std;

string createLine(); //Функция для генерации строки

int main()
{
    string line = createLine();

    // Перевод строки в вектор символов
    vector<char> vector_string(line.begin(), line.end());

    // Записываем в вектор только уникальные символы
    set<char> unique_vector(vector_string.begin(), vector_string.end()); 

    // Выписываем количество одинаковых символов в строке
    for (char letter : unique_vector)
        cout << letter << " - " << count(vector_string.begin(), vector_string.end(), letter) << endl; 

    return 0;
}

string createLine() {
    srand(time(0));
    string str = "";

    // Заполнение строки случайными символами
    for (int i = 0; i < N; i++) {
        str += rand() % 26 + 0x61; // Создаем строку из случайных символов
    }
    return str;
}