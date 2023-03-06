#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <vector>

using namespace std;

const int M = 20; // Количество тестов
unsigned int count_of_recursion = 0; // Счетчик рекурсий
int height = 0;

/* Очистка файла перед записью */
void clearFile() {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_test.txt");
}

/* Выбор опорного элемента 
* @param pivot_value опорный элемент
* @param i новое начало
* return i + 1 возвращает индекс опорного элемента
*/
int partition(vector <double> v, int begin, int last) {
    double pivot_value = v[last];
    int i = begin - 1;
    double temp;
    for (int j = begin; j < last; j++) {
        if (v[j] <= pivot_value) {
            i++;
            temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
    temp = v[i + 1];
    v[i + 1] = v[last];
    v[last] = temp;
    return i + 1;
}

/* Быстрая сортировка
*
* @param pivot опорный элемент
* @param depth глубина ветки
* @param count_of_recursion количество рекурсий
* @param height максимальная глубина стека
* @param v массив
*/
void quickSort(vector <double> v, int begin, int last, int depth) {
    double pivot;
    count_of_recursion++;
    if (depth > height) height = depth;
    if (begin < last) {
        pivot = partition(v, begin, last);
        quickSort(v, begin, pivot - 1, depth + 1);
        quickSort(v, pivot + 1, last, depth + 1);
    }
}

/* Запись результата в файл */
void writer(int i, int N, double msec) {
    ofstream out;

    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_test.txt", ios::app);
    if (out.is_open())
    {
        out << i << " " << N << " " << msec << " " << count_of_recursion-1 << " " << height << endl;
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
    int begin = 0, last = v.size() - 1, depth = 0;
    height = 0;
    count_of_recursion = 0; // Обнуление счетчика
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now(); // стартовое время сортировки
    quickSort(v, begin, last, depth); // сортировка
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
    int N[] = { 1000, 2000, 4000, 8000, 16000, 32000 }; // массив различных длин массива
    vector <double> v; // массив
    clearFile(); // очистка файла

    for (int j = 0; j < size(N); j++) {

        vector <double> test_array(N[j]); // Массив из нулей
        nullSort(test_array, v.size()); // Сортировка нулевого массива

        for (int i = 0; i < M; i++) { // M тестов          

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
        test_array.clear();
    }
    cout << "Programm end!";
}
