#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <vector>

const int M = 5; // Количество тестов
unsigned int count_of_recursion = 0; // Счетчик рекурсий

using namespace std;

/* Очистка файла перед записью */
void clearFile() {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_h.txt");
}

/* Быстрая сортировка
*
* @param pivot опорный элемент
* @param new_begin начало сортировки
* @param new_end конец сортировки
* @param count переменная для хранения
* @param v массив
*/
void quickSort(vector <double> v, int begin, int last) {
    double pivot, count;
    int new_begin = begin, new_last = last;
    count_of_recursion++;
    pivot = v[(new_begin + new_last) / 2]; //вычисление опорного элемента
    do
    {
        while (v[new_begin] < pivot) new_begin++;
        while (v[new_last] > pivot) new_last--;
        if (new_begin <= new_last) //перестановка элементов
        {
            count = v[new_begin];
            v[new_begin] = v[new_last];
            v[new_last] = count;
            new_begin++;
            new_last--;
        }
    } while (new_begin <= new_last);
    if (begin < new_last) quickSort(v, begin, new_last); // Вызов рекурсии
    if (new_begin < last) quickSort(v, new_begin, last); // Вызов рекурсии
}

/* Запись результата в файл */
void writer(int i, int N, double msec) {
    ofstream out;

    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_h.txt", ios::app);
    if (out.is_open())
    {
        out << i << " " << N << " " << msec << " " << count_of_recursion-1 << endl;
    }
}

/* Отделение части записи */
void piece() {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data2.txt", ios::app);
    if (out.is_open())
    {
        out << "---------------------------------" << endl;
    }
}

/* Таймер
*
* @param i номер теста
* @param v массив
* @param N количество элементов в массиве
* @param begin начало сортировки
* @param last конец сортировки
*/
void timer(vector <double> v, int N, int i) {
    int begin = 0, last = v.size() - 1;
    count_of_recursion = 0; // Обнуление счетчика
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now(); // стартовое время сортировки
    quickSort(v, begin, last); // сортировка
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); // конечное время сортировки
    chrono::duration<double, nano> nano_diff = end - start; // время в наносекундах
    chrono::duration<double, micro> micro_diff = end - start; // время в микросекундах
    chrono::duration<double, milli> milli_diff = end - start; // время в миллисекундах
    chrono::duration<double> sec_diff = end - start; // время в секундах
    writer((i + 1), v.size(), milli_diff.count()); // запись в файл
}

/* Отделение части записи */
void sortingSort(vector <double> v, int N, int i) {
    ofstream out;
    timer(v, N, i);
}

/* Отделение части записи */
void nullSort(vector <double> v, int N) {
    ofstream out;
    timer(v, N, 0);
}

/* Перестановка отсортированного массива */
void antiQSort(vector <double> v, int N, int i) {
    for (int j = 2; j <= v.size(); j++) {
        int added_index = N - j;
        int middle = (added_index + (N - 1)) / 2; // корневой индекс для быстрой сортировки
        swap(v[added_index], v[middle]);
    }
    timer(v, v.size(), i);
}

/* Перестановка отсортированного массива для детермированного метода */
void determQSort(vector <double> v, int N, int i) {
    double temp_array;
    int k = 0, n = N - 1;

    for (int j = 0; j < N; j++) {
        temp_array = v[n - j];
        v[n - j] = v[(n - j) / 2];
        v[(n - j) / 2] = temp_array;
    }
    timer(v, N, i);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    mt19937 engine(time(0));
    int N[] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 }; // массив различных длин массива
    vector <double> v; // массив
    clearFile(); // очистка файла
    for (int j = 0; j < size(N); j++) {
        vector <double> test_array(N[j]); // Массив из нулей
        nullSort(test_array, v.size()); // Сортировка нулевого массива
        for (int i = 0; i < M; i++) { // M тестов
            piece();
                       
            // генерация случайных чисел от -1 до 1
            uniform_real_distribution<double> gen(-1.0, 1.0);
            for (int el = 0; el < N[j]; el++) {
                v.push_back(gen(engine));
            }
            timer(v, v.size(), i); // таймер
            sortingSort(v, v.size(), i); // Отсортированный массив
            antiQSort(v, v.size(), i); // Массив с макс колвом сравнений
            determQSort(v, v.size(), i); // Массив с макс колвом сравнений
            v.clear();
            
        }
        piece();
        test_array.clear();
    }
    cout << "Programm end!";
}
