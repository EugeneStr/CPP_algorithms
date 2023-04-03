#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;

const int SIZE = 100000000; // максимальный размер кучи

// Класс кучи
class Heap {
public:
	int* h;         // указатель на массив кучи
	int HeapSize; // размер кучи
	
	// Конструктор
	Heap() {
		h = new int[SIZE];
		HeapSize = 0;
	}
	
	// Деструктор
	~Heap()
	{
		delete h;
	}

	// Добавление элемента
	void addelem(int n) {
		int i, parent;
		i = HeapSize; // номер элемента
		h[i] = n; // Последний элеемнт
		parent = (i - 1) / 2; // Расчет позиции родителя
		// Перестановка
		while (parent >= 0 && i > 0) {
			if (h[i] > h[parent]) {
				int temp = h[i];
				h[i] = h[parent];
				h[parent] = temp;
			}
			i = parent;
			parent = (i - 1) / 2;
		}
		HeapSize++; // Увеличение размера
	}
	
	// Вывод кучи
	void outHeap() {
		int i = 0;
		int k = 1; // Количество элементов на вывод в строке
		int levels = floor(log2(HeapSize)) + 1; // Уровень, на котором находится элемент
		while (i < HeapSize) {
			for (int j = 0; j < pow(2, levels - 1) - 1; j++) cout << setw(2) << " "; // Вывод отсупа
			while ((i < k) && (i < HeapSize)) {
				cout << setw(2) << h[i]; // Вывод элемента
				for (int j = 0; j < pow(2, levels) - 1; j++) cout << setw(2) << " "; // Вывод отступа
				i++;
			}
			cout << endl;
			levels--; // Уменьшение уровня
			k = k * 2 + 1; // Увеличение количества элементов на вывод
		}
	}

	// Упорядочивание кучи
	void heapify(int i) {
		int left, right; // Позиция левого и правого потомков
		int temp; 
		left = 2 * i + 1; 
		right = 2 * i + 2;
		if (left < HeapSize) { // Если левая позиция меньше размера кучи
			if (h[i] < h[left]) { // Если родитель меньше потомка
				temp = h[i];
				h[i] = h[left];
				h[left] = temp;
				heapify(left); // Рекурсия
			} 
		}
		if (right < HeapSize) { // Если правая позиция меньше размера кучи
			if (h[i] < h[right]) { // Если родитель меньше потомка
				temp = h[i];
				h[i] = h[right];
				h[right] = temp;
				heapify(right); // Рекурсия
			}
		}
	}		

	// Удаление максимального элемента
	int getmax() {
		int x;
		x = h[0]; // Максимальный элемент
		h[0] = h[HeapSize - 1]; // Последний элемент ставим в корень
		HeapSize--; // Уменьшаем кучу
		heapify(0); // Вызываем упорядочивание с корня
		return x;
	}


	// Поиск максимального
	int findmax() {
		return h[0]; // Максимальный всегда корень
	}

};

int main() {
	
	int k;
	srand(time(0));
	system("chcp 1251");
	system("cls");

	int test = 5; // Количество тестов
	int N, count = 1000; // Количество операций
	double all_time = 0;
	vector<double> times;
	chrono::high_resolution_clock::time_point begin;
	chrono::high_resolution_clock::time_point end;
	chrono::high_resolution_clock::time_point begin_max;
	chrono::high_resolution_clock::time_point end_max;
	chrono::duration<double, milli> milli_diff;
	chrono::duration<double, milli> milli_diff_max;

	ofstream out;
	out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data8.txt");

	for (int i = 0; i < test; i++) {
		N = pow(10, 3 + i); // Рассчитываем количество элементов
		Heap heap;
		for (int j = 0; j < N; j++) {
			heap.addelem(rand() % 100); // Заполняем кучу N элементами
		}

		cout << N << " " << heap.HeapSize << endl;
		all_time = 0; // Обнуляем время
		for (int j = 0; j < count; j++) {
			begin_max = chrono::high_resolution_clock::now();
			heap.findmax(); // Поиск максимального элемента
			end_max = chrono::high_resolution_clock::now();
			milli_diff_max = end_max - begin_max; // время в микросекундах
			times.push_back(milli_diff_max.count()); // Записываем время
			all_time += milli_diff_max.count(); // Прибавляем в общее время
		}
		out << "НайтиMax " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();

		cout << N << " " << heap.HeapSize << endl;
		all_time = 0; // Обнуляем время
		for (int j = 0; j < count; j++) {
		    begin_max = chrono::high_resolution_clock::now();
			heap.addelem(rand() % 100); // Добавление элемента
		    end_max = chrono::high_resolution_clock::now();
		    milli_diff_max = end_max - begin_max; // время в микросекундах
		    times.push_back(milli_diff_max.count()); // Записываем время
			all_time += milli_diff_max.count(); // Прибавляем в общее время
		}
		out << "Добавление " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();

		all_time = 0; // Обнуляем время
		for (int j = 0; j < count; j++) {
			begin_max = chrono::high_resolution_clock::now();
			heap.getmax(); // Удаление элемента
			end_max = chrono::high_resolution_clock::now();
			milli_diff_max = end_max - begin_max; // время в микросекундах
			times.push_back(milli_diff_max.count()); // Записываем время
			all_time += milli_diff_max.count(); // Прибавляем в общее время
		}
		end = chrono::high_resolution_clock::now();
		milli_diff = end - begin; // время в микросекундах
		out << "УдалитьMax " << all_time << " " << all_time / count << " " << *max_element(times.begin(), times.end()) << endl;
		times.clear();

		
		cout << N << " " << heap.HeapSize << endl;

	}

	return 0;
}
