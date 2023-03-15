#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>
#include <stack>
#include <queue>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;

/* Структура для ребра*/
struct Edge {
	int begin; // Начало ребра
	int end; // Конец ребра
};

/* Класс графа
* 
* @param ver Количество вершин
* @param reb Количество ребер
* @param oriented Ориентированность
* @param matrix_smezh Матрица смежности
* @param matrix_inc Матрица инцидентности
* @param list_smezh Список смежности
* @param list_reb Список ребер
* @param binds Количество связанных вершин
*/
class Grapth {
public:
	int ver;
	int reb;
	bool oriented;
	int** matrix_smezh;
	int** matrix_inc;
	list <list<int>> list_smezh;
	list <list<int>> list_reb;
	int* binds;

	/* Вывод матрицы смежности*/
	void printMatrixSmezh() {
		cout << "Матрица смежности\n";
		cout << setw(4) << "|";
		for (int i = 0; i < ver; i++) {
			cout << setw(2) << (char)(i + 65) << " ";
		}
		cout << endl;
		for (int i = 0; i < ver; i++) {
			cout << setw(2) << (char)(i + 65) << " |";
			for (int j = 0; j < ver; j++) {
				cout << setw(2) << matrix_smezh[i][j] << " ";
			}
			cout << endl;
		}
	}

	/* Вывод матрицы инцидентности*/
	void printMatrixInc() {
		cout << "Матрица инцендентности\n";
		cout << setw(4) << "|";
		for (int i = 0; i < reb; i++) {
			cout << setw(2) << i + 1 << " ";
		}
		cout << endl;
		for (int i = 0; i < ver; i++) {
			cout << setw(2) << (char)(i + 65) << " |";
			for (int j = 0; j < reb; j++) {
				cout << setw(2) << matrix_inc[i][j] << " ";
			}
			cout << endl;
		}
	}

	/* Вывод списка смежности*/
	void printListSmezh() {
		list<list<int>> list = list_smezh;
		cout << "Список смежности\n";
		int i = 0;
		while (list.size() > 0) {
			cout << (char)(i + 65) << " | ";
			if (!list.front().empty()) {
				while (list.front().size() > 0) {
					cout << (char)(list.front().front() + 65) << " ";
					list.front().pop_front();
				}
			}
			else {
				cout << "-";
			}
			i++;
			list.pop_front();
			cout << endl;
		}
	}

	/* Вывод списка ребер*/
	void printListReb() {
		cout << "Список ребер\n";
		list<list<int>> list = list_reb;
		int i = 0;
		while (list.size() > 0) {
			cout << i + 1 << " | " << (char)(list.front().front() + 65) << 
				" -> " << (char)(list.front().back() + 65) << endl;
			list.pop_front();
			i++;
		}
	}

};

/* Генерация матрицы смежности
* 
* @param gr Граф
* @param max_bind Максимальное количество связей у вершины
* @param oriented Ориентированность
* @param M Максимально возможное количество ребер
* @param array_reb Вектор ребер
* @param count Количество расставленных ребер
* 
* @return Граф
*/
Grapth genMatSmezh(Grapth gr, int max_bind, bool oriented) {

	int M = gr.ver * (gr.ver - 1) / 2; // Максимальное количество возможных ребер
	vector <int> arrray_reb(M); // Вектор ребер
	for (int i = 0; i < gr.reb; i++) { // Заполняем первые элементы
		arrray_reb[i] = 1;
	}
	random_shuffle(arrray_reb.begin(), arrray_reb.end()); // Перемешиваем вектор

	// Динамически выделяем память под массивы
	gr.matrix_smezh = new int* [gr.ver]; 
	gr.binds = new int[gr.ver];
	for (int i = 0; i < gr.ver; i++) {
		gr.matrix_smezh[i] = new int[gr.ver];
		gr.binds[i] = 0;
	}

	int k = 0, count = 0;
	for (int i = 0; i < gr.ver; i++) {
		gr.matrix_smezh[i][i] = 0;
		for (int j = i + 1; j < gr.ver; j++) {
			// Если есть ребро, а места на этой вершине уже нет 
			if (arrray_reb[k] != 0 && (gr.binds[i] == max_bind || gr.binds[j] == max_bind)) {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 0;
				k++;
			}
			else {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = arrray_reb[k];
				k++;
				count++; // Записываем успешно поставленное ребро
			}
			// Прибавляем к количеству связанных вершин
			if (abs(gr.matrix_smezh[i][j]) == 1) {
				gr.binds[i]++;
				gr.binds[j]++;
			}
		}
	}

	// Если есть не проставленные ребра
	while (M - count > 0) {
		for (int i = 0; i < gr.ver; i++) {
			// Если у вершины есть свободное место
			if (gr.binds[i] < max_bind) {
				for (int j = i + 1; j < gr.ver; j++) {
					if (gr.matrix_smezh[i][j] == 0 && gr.binds[j] < max_bind) {
						gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 1;
						count++; // Записываем успешно поставленное ребро
						gr.binds[i]++; // Прибавляем к количеству связанных вершин
					}
					if (M - count <= 0) break;
				}
			}
			if (M - count < 0) break;
		}
	}

	return gr;
}

/* Генерация матрицы инцидентности
* 
* @param gr Граф
* @param oriented Ориентированность
* @param temp Список ребер
* 
* @return gr Граф
*/
Grapth genMatInc(Grapth gr, bool oriented) {
	list<list<int>> temp = gr.list_reb; // Приравниваем к промежутному списку список ребер
	// Выделяем динамически память
	gr.matrix_inc = new int* [gr.ver];
	for (int i = 0; i < gr.ver; i++) {
		gr.matrix_inc[i] = new int[gr.reb];
	}
	// Заполняем матрицу 0
	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.reb; j++) {
			gr.matrix_inc[i][j] = 0;
		}
	}

	int i = 0;
	// Пока список не пустой
	while (temp.size() > 0) {
		if (oriented) { // Если ориентированный
			gr.matrix_inc[temp.front().front()][i] = 1; // Выход ребра
			gr.matrix_inc[temp.front().back()][i] = -1; // Вход ребра

		}
		else {
			if (temp.front().front() < temp.front().back()) { // Для исключения повторений ребер
				gr.matrix_inc[temp.front().front()][i] = gr.matrix_inc[temp.front().back()][i] = 1;
			}
		}
		temp.pop_front(); // Убираем записанное ребро
		i++;
	}

	return gr;
}

/* Генерация списка смежности
* 
* @param gr Граф
* @param temp Список для записи
* @return gr Граф
*/
Grapth genListSmezh(Grapth gr) {
	list<int> temp;
	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[j][i] == 1) { // Если есть связь
				temp.push_back(j); // Добавляем связь к этой вершине
			}
		}
		gr.list_smezh.push_back(temp); // Записываем список связей для i-ой вершины
		temp.clear();
	}
	return gr;
}

/* Генерация списка ребер
* 
* @param gr Граф
* @param temp Список для записи
* @return gr Граф
*/
Grapth genListReb(Grapth gr) {
	list<int> temp;
	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == 1) { // Если есть связь
				temp.push_back(i); // Записываем начало
				temp.push_back(j); // Записываем конец
				
				gr.list_reb.push_back(temp); // Добавляем ребро
				temp.clear();
			}
		}
	}
	return gr;
}

int countOfEntryExit(int** array,int i ,int N, int e) {
	int count = 0;
	for (int j = 0; j < N; j++) {
		if (array[i][j] == e) count++;
	}
	return count;
}

void genOriented(Grapth gr, int max_entry, int max_exit) {

	int a[] = { -1, 1 };
	for (int i = 0; i < gr.ver; i++) {
		for (int j = i + 1; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == 1) {
				gr.matrix_smezh[i][j] = a[rand() % 2];
				gr.matrix_smezh[j][i] = -gr.matrix_smezh[i][j];
			}
		}
	}

	for (int i = 0; i < gr.ver; i++) {
		if (countOfEntryExit(gr.matrix_smezh, i, gr.ver, -1) > max_entry) {
			for (int j = 0; j < gr.ver; j++) {
				if (gr.matrix_smezh[i][j] == -1 && countOfEntryExit(gr.matrix_smezh, j, gr.ver, -1) < max_entry) {
					swap(gr.matrix_smezh[i][j], gr.matrix_smezh[j][i]);
					i--;
					break;
				}

			}
		}
		if (countOfEntryExit(gr.matrix_smezh, i, gr.ver, 1) > max_exit) {
			for (int j = 0; j < gr.ver; j++) {
				if (gr.matrix_smezh[i][j] == 1 && countOfEntryExit(gr.matrix_smezh, j, gr.ver, 1) < max_exit) {
					swap(gr.matrix_smezh[i][j], gr.matrix_smezh[j][i]);
					i--;
					break;
				}
			}
		}
	}

	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == -1) gr.matrix_smezh[i][j] = 0;
		}
	}
}

void deleteDynamicArray(Grapth gr) {
	for (int i = 0; i < gr.ver; i++) {
		delete[] gr.matrix_smezh[i];
	}
	delete[] gr.matrix_smezh;
	
	delete[] gr.binds;
	
	for (int i = 0; i < gr.ver; i++) {
		delete[] gr.matrix_inc[i];
	}
	delete[] gr.matrix_inc;
}

/*Поиск в ширину
* 
* @param gr Граф
* @param start Начальная вершина
* @param req Конечная вершина
* @param Queue Очередь для посещения
* @param Edges Стек ребер
* @param e Ребро
* @param nodes Список посещенных вершин 
* @param node Вершина
* @param begin Начало отсчета времени
* @param stop Окончание отсчета времени
* @param milli_diff Время
*/
void BFS(Grapth gr, int start, int req) {
	cout << "\n\nПоиск в ширину (Нахождение кратчайшего пути)\n";
	chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); // стартовое время сортировки
	queue<int> Queue;
	stack<Edge> Edges;
	Edge e;

	int* nodes = new int[gr.ver]; // вершины графа
	for (int i = 0; i < gr.ver; i++) // исходно все вершины равны 0
		nodes[i] = 0;

	Queue.push(start); // помещаем в очередь первую вершину
	while (!Queue.empty())
	{
		int node = Queue.front(); // извлекаем вершину
		Queue.pop();
		nodes[node] = 2; // отмечаем ее как посещенную

		for (int j = 0; j < gr.ver; j++)
		{
			if (gr.matrix_smezh[node][j] == 1 && nodes[j] == 0)
			{ // если вершина смежная и не обнаружена
				Queue.push(j); // добавляем ее в очередь
				nodes[j] = 1; // отмечаем вершину как обнаруженную
				e.begin = node; e.end = j;
				Edges.push(e);
				if (node == req) break;
			}
		}
		//cout << endl << node + 1; // выводим номер вершины
	}
	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); // конечное время сортировки
	chrono::duration<double, milli> milli_diff = stop - begin; // время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "\nПуть до вершины " << req + 1 << endl;
	cout << req + 1;
	while (!Edges.empty()) { // Пока есть вершины
		e = Edges.top(); 
		Edges.pop();
		if (e.end == req) {
			req = e.begin; // Посещенная вершина
			cout << " <- " << req + 1;
		}
	}
	delete[] nodes;
}

/*Поиск в глубину
* 
* @param gr Граф
* @param start Начальная вершина
* @param req Конечная вершина
* @param Stack Стек для посещения
* @param Edges Стек ребер
* @param e Ребро
* @param nodes Список посещенных вершин 
* @param node Вершина
* @param begin Начало отсчета времени
* @param stop Окончание отсчета времени
* @param milli_diff Время
* @return gr Граф
*/
Grapth DFS(Grapth gr) {
	cout << "\nПоиск в глубину (Проверка на существование пути)\n";
	chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); // стартовое время сортировки
	int req, start;
	Edge e;
	stack<int> Stack;
	stack<Edge> Edges;
	int* nodes = new int[gr.ver]; // вершины графа
	for (int i = 0; i < gr.ver; i++) // исходно все вершины равны 0
		nodes[i] = 0;

	start = rand() % gr.ver; // Выбираем стартовую вершину
	cout << "\n" << start; 
	req = rand() % gr.ver; // Выбираем конечную вершину
	cout << "\n" << req << endl;
	
	Stack.push(start); // помещаем в очередь первую вершину

	while (!Stack.empty())
	{ // пока стек не пуст
		int node = Stack.top(); // извлекаем вершину
		Stack.pop();
		
		if (nodes[node] == 2) continue;
		nodes[node] = 2; // отмечаем ее как посещенную
		
		for (int j = gr.ver - 1; j >= 0; j--)
		{ // проверяем для нее все смежные вершины
			if (gr.matrix_smezh[node][j] == 1 && nodes[j] != 2)
			{ // если вершина смежная и не обнаружена
				Stack.push(j); // добавляем ее в cтек
				nodes[j] = 1; // отмечаем вершину как обнаруженную
				e.begin = node; e.end = j;
				Edges.push(e);
				if (node == req) break;
			}
		}
		//cout << node + 1 << endl; // выводим номер вершины
	}
	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); // конечное время сортировки
	chrono::duration<double, milli> milli_diff = stop - begin; // время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "Путь до вершины " << req + 1 << endl;
	cout << req + 1;
	bool temp = 0; // Существует ли путь
	int end = req;
	while (!Edges.empty())
	{
		e = Edges.top();
		Edges.pop();
		if (e.end == req)
		{
			req = e.begin;
			cout << " <- " << req + 1; // Выписываем посещенную вершину
			temp = 1;
		}
	}
	delete[] nodes;
	if (!temp) cout << "\nотсутствует";
	else BFS(gr, start, end); // Вызываем поиск в ширину
	return gr;
}

/*Генератор графа
* 
* @param i Номер теста
* @param max_ver, min_ver Максимальное/минимальное количество вершин
* @param max_reb, min_reb Максимальное/минимальное количество ребер
* @param max_bind Максимальное количество связей у вершины
* @param oriented Ориентированность
* @param max_entry, max_exit Максимальное количество входов/выходов
* @param gr Граф
* @return gr Граф
*/
Grapth grapthGeneration(int i) {
	int max_ver = 100, min_ver = 10;
	int max_reb, min_reb = 10;
	int max_bind;
	bool oriented;
	int max_entry, max_exit;

	Grapth gr;

	gr.ver = (max_ver) / 10 * (i + 1); // Вершины
	gr.reb = min_reb + (i * min_reb * 2); // Ребра
	max_bind = ceil(((double)gr.reb * 2) / (double)gr.ver) + rand() % (gr.ver + 1); // Количество связей
	max_entry = max_exit = max_bind / 2 + 1; // Входы/выходы
	oriented = rand() % 2; // Ориентированность
	cout << "\nВершин: " << gr.ver << "\nРебер: " << gr.reb <<
		"\nМаксимальное количество связей: " << max_bind << 
		"\nОриентированный: " << oriented << 
		"\nМаксимальное количество входов: " << max_entry << 
		"\nМаксимальное количество выходов: "<< max_exit << endl;

	gr = genMatSmezh(gr, max_bind, oriented); // Матрица смежности
	if (oriented) genOriented(gr, max_entry, max_exit); // Ориентированность
	gr = genListReb(gr); // Список ребер
	gr = genListSmezh(gr); // Список смежности
	gr = genMatInc(gr, oriented); //Матрица инцидентности

	gr.printMatrixSmezh(); // Вывод матрицы смежности
	gr.printListSmezh(); // Вывод списка смежности
	gr.printListReb(); // Вывод списка ребер
	gr.printMatrixInc(); // Вывод матрицы инцидентности

	return gr;
}

void main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	int M = 10;
	Grapth* gr = new Grapth[M]; 
	for (int i = 0; i < M; i++) {
		cout << "\nТест №" << i + 1 << endl;
		gr[i] = grapthGeneration(i);
		gr[i] = DFS(gr[i]);
		deleteDynamicArray(gr[i]);
	}
	delete[] gr;
}
