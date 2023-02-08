#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

const int M = 20; // Количество тестов


using namespace std;

/* Очистка файла перед записью */
void clearFile() {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data1.txt");
}

/* Сортировка перемешиванием
* 
* @param begin начало массива
* @param end конец массива
* @param swapped проверка необходимости перестановки
* @param v массив
* @param N количество элементов в массиве
*/
void shakerSort(double* v, int N) {
    int begin = -1; // сдвиг на 1 влево
    int end = N - 1; // сдвиг на 1 влево
    bool swapped = 1; 
    while (swapped) {
        swapped = false;
        begin++;
        for (int i = begin; i < end; i++) { // сортировка с начала
            if (v[i] > v[i + 1]) { // сравнение двух элементов
                swap(v[i], v[i + 1]); // перестановка
                swapped = true;
            }
        }
        if (!swapped) break; // отсортирован ли массив
        swapped = false;
        end--;
        for (int i = end - 1; i >= begin; i--) { // сортировка с конца
            if (v[i] > v[i + 1]) { // сравнение двух элементов
                swap(v[i], v[i + 1]); // перестановка
                swapped = true;
            }
        }
    }
}

/* Запись результата в файл */
void writer(int i, int N, double msec) {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data1.txt", ios::app);
    if (out.is_open())
    {
        out << "Test №" << i << " N = " << N << " Time: " << msec << endl;
    }
}

/* Отделение части записи */
void piece() {
    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data1.txt", ios::app);
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
*/
void timer(double* v, int N, int i) {
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now(); // стартовое время сортировки
    shakerSort(v, N); // сортировка
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); // конечное время сортировки
    chrono::duration<double, nano> nano_diff = end - start; // время в наносекундах
    chrono::duration<double, micro> micro_diff = end - start; // время в микросекундах
    chrono::duration<double, milli> milli_diff = end - start; // время в миллисекундах
    chrono::duration<double> sec_diff = end - start; // время в секундах
    writer((i + 1), N, milli_diff.count()); // запись в файл
}

int main()
{
    int N[] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 }; // массив различных длин массива
    double* v; // массив
    clearFile(); // очистка файла
    for (int j = 0; j < size(N); j++) {
        v = new double[N[j]]; // выделение памяти
        for (int i = 0; i < M; i++) {
            // генерация случайных чисел от -1 до 1
            mt19937 engine(time(0));
            uniform_real_distribution<double> gen(-1.0, 1.0);
            for (int el = 0; el < N[j]; el++) {
                v[el] = gen(engine);
            }
            timer(v, N[j], i); // таймер
            /*cout << "\n----- TEST #" << i + 1 << " ----- N = " << N[j] << " -----\n\n";
            cout << "Time: " << nano_diff.count() << " nano sec." << endl;
            cout << "Time: " << micro_diff.count() << " micro sec." << endl;
            cout << "Time: " << milli_diff.count() << " milli sec." << endl;
            cout << "Time: " << sec_diff.count() << " sec." << endl;*/
        }
        piece();
        delete[] v; // очистка памяти
    }
    cout << "Programm end!";
}
