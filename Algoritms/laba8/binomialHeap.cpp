#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <random>

using namespace std;

const int SIZE = 1000;

// Структура элемента кучи
struct Node {
    int data; // Значение
    Node* left; // Левый потомок
    Node* right; // Правый потомок

    // Конструктор
    Node(int Data) {
        data = Data;
        left = 0;
        right = 0;
    }
};

// Класс биномиальной кучи
class binomialHeap {
public:
    Node* mas[SIZE]; // Массив элементов
    int size; // Размер

    // Конструктор
    binomialHeap() {
        memset(mas, 0, sizeof(mas));
        size = 0;
    }

    binomialHeap(const binomialHeap& bq) {
        memset(mas, 0, sizeof(mas));
        size = bq.size;
        for (int i = 0; i < SIZE; i++) {
            if (bq.mas[i])
            {
                Node* deepCopy = new Node(*bq.mas[i]);
                mas[i] = deepCopy;
            }
        }
    }

    // Деструктор
    ~binomialHeap() {
        for (int i = 0; i < SIZE; ++i)
        {
            if (mas[i])
                delete mas[i];
        }
    }

    // Очищение кучи
    void clear() {
        memset(mas, 0, sizeof(mas));
        size = 0;
    }

    // Проверка на пустоту
    bool empty() {
        return size == 0;
    }

    // Объединение двух куч одного размера
    Node* join(Node* f, Node* s) {
        if (f->data > s->data) { // Если первый больше второго
            s->right = f->left;
            f->left = s;
            return f;
        } 
        else {                   // Если второй больше первого
            f->right = s->left;
            s->left = f;
            return s;
        }
    }
    
    // Формула для сложения двоичных чисел
    int num(int c, int b, int a) {
        return 4 * c + 2 * b + a;
    }

    // Объединение двух куч разного размера
    void joinBQ(Node* a[], Node* b[]) {
        Node* c = 0; // Новая куча
        for (int i = 0; i < SIZE; i++) {
            switch (num(c != 0, b[i] != 0, a[i] != 0)) { // Расчет по формуле
            case 2:
                a[i] = b[i]; // Замена а
                break;
            case 3:
                c = join(a[i], b[i]); // Объединение а и b
                a[i] = 0; 
                break;
            case 4:
                a[i] = c; // Замена а
                c = 0;
                break;
            case 5:
                c = join(a[i], c); // Объединение а и с
                a[i] = 0;
                break;
            case 6:
            case 7:
                c = join(b[i], c); // Объединение b и с
                break;
            }
        }
    }

    // Объединение двух куч разного размера
    void joinBQ(binomialHeap& bq) {
        joinBQ(mas, bq.mas); // Вызов функции объединения
        size += bq.size; // Прибавление размера
        bq.clear(); // Очистка старой кучи
    }

    // Добавление элемента
    void add(int val) {
        Node* newNode = new Node(val);
        Node* curNode = newNode; // Новый элемент
        for (int i = 0; i < SIZE; i++) {
            if (mas[i] == NULL) { // Если есть место
                mas[i] = curNode; // Вставляем новый элемент
                break;
            }
            else {
                curNode = join(curNode, mas[i]); // Объединяем наш элемент с текущим в куче
                mas[i] = NULL;
            }
        }
        size++; // Увеличиваем размер
    }

    // Удаление максимального элемента
    int getMax() {
        int res = 0; // Значение
        int resPos = 0; // Позиция
        for (int i = 0; i < SIZE; i++) {
            // Находим максимальный по значению
            if (mas[i] && mas[i]->data > res) { 
                res = mas[i]->data; // Запоминаем значение и позицию
                resPos = i;
            }
        }
        
        Node** tmp = new Node * [SIZE]; // Временная куча
        memset(tmp, 0, sizeof(tmp) * SIZE);
        Node* cur = 0;
        if (!mas[resPos])
            cur = 0;
        else
            cur = mas[resPos]->left; // Левый потомок максимального

        for (int i = resPos - 1; i >= 0; i--) { // Все кто левее максимального добавляются во вреенную кучу
            tmp[i] = new Node(*cur);
            cur = cur->right;
            tmp[i]->right = 0;
        }
        delete mas[resPos]; // Удаляем максимальный
        mas[resPos] = 0;

        joinBQ(mas, tmp); // Объединяем оставшуюся кучу и временную
        delete[] tmp; // Удаляем временную кучу
        size--; // Уменьшаем размер
        return res;
    }


    // Перегрузка оператора ввода для биномиальной кучи
    friend std::ostream& operator<<(ostream& cout, binomialHeap bq) { 
        int k = 1, i = 0, v = 0;
        int levels = floor(log2(bq.size)) + 1; // Уровень, на котором находится элемент
        while (!bq.empty()) { 
            for (int j = 0; j < pow(2, levels - 1) - 1; j++) cout << setw(2) << " "; // Вывод отступа
            while ((i < k) && !bq.empty()) {
                v = bq.getMax(); // Берем максимальный
                cout << setw(2) << v; // Вывод элемента
                for (int j = 0; j < pow(2, levels) - 1; j++) cout << setw(2) << " "; // Вывод отступа
                i++;
            }
            cout << endl;
            levels--; // Уменьшаем уровень
            k = k * 2 + 1;
        }
        return cout << endl;
    }
};



// Нахождение максимального элемента
int findMax(binomialHeap bq) {
    int res = 0;
    for (int i = 0; i < SIZE; i++) {
        if (bq.mas[i] && bq.mas[i]->data > res) {
            res = bq.mas[i]->data; // Запоминаем занчение
        }
    }
    return res;
}




int main(int argc, char* argv[]) {
    binomialHeap bq;
    srand(time(0));
    system("chcp 1251");
    system("cls");

    int test = 5; // Количество тестов
    int N, count = 1000; // количество операций
    double all_time = 0;
    vector<double> times;
    chrono::high_resolution_clock::time_point begin;
    chrono::high_resolution_clock::time_point end;
    chrono::high_resolution_clock::time_point begin_max;
    chrono::high_resolution_clock::time_point end_max;
    chrono::duration<double, milli> milli_diff;
    chrono::duration<double, milli> milli_diff_max;

    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data8_2.txt", ios::app);

    for (int i = 0; i < test; i++) {
        N = pow(10, 3 + i);
        for (int j = 0; j < N; j++) {
            bq.add(rand() % 100); // Заполняем кучу N элементами
        }
        cout << "Начало " << N << " " << bq.size << endl;
        
        all_time = 0; // Обнуляем время
        for (int j = 0; j < count; j++) {
            begin_max = chrono::high_resolution_clock::now();
            findMax(bq); // Находим максимальный элемент
            end_max = chrono::high_resolution_clock::now();
            milli_diff_max = end_max - begin_max; // время в микросекундах
            times.push_back(milli_diff_max.count()); // Записываем время
            all_time += milli_diff_max.count(); // Прибавляем время
        }
        end = chrono::high_resolution_clock::now();
        milli_diff = end - begin; // время в микросекундах
        out << "НайтиMax " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
        times.clear();
        cout << "Нашли Max " << N << " " << bq.size << endl;


        all_time = 0; // Обнуляем время
        for (int j = 0; j < count; j++) {
            begin_max = chrono::high_resolution_clock::now();
            bq.getMax();
            end_max = chrono::high_resolution_clock::now();
            milli_diff_max = end_max - begin_max; // время в микросекундах
            times.push_back(milli_diff_max.count()); // Записываем время
            all_time += milli_diff_max.count(); // Прибавляем время
        }
        end = chrono::high_resolution_clock::now();
        milli_diff = end - begin; // время в микросекундах
        out << "УдалитьMax " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
        times.clear();
        cout << "Удалили Max " << N << " " << bq.size << endl;

        all_time = 0; // Обнуляем время
        for (int j = 0; j < count; j++) {
            begin_max = chrono::high_resolution_clock::now();
            bq.add(rand() % 100);
            end_max = chrono::high_resolution_clock::now();
            milli_diff_max = end_max - begin_max; // время в микросекундах
            times.push_back(milli_diff_max.count()); // Записываем время
            all_time += milli_diff_max.count(); // Прибавляем время
        }
        end = chrono::high_resolution_clock::now();
        milli_diff = end - begin; // время в микросекундах
        out << "Добавление " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
        times.clear();
        cout << "Добавили " << N << " " << bq.size << endl;

        
        bq.clear();
        cout << "Очистили " << N << " " << bq.size << endl;
    }
    return 0;
}
