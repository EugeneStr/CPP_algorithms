#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>
#include <stack>
#include <queue>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <math.h>

using namespace std;

//Максимальное значение веса = 20
#define INF 21
const int tests = 10; // Количество тестов
ofstream out;

/* Класс графа
*
* @param ver Количество вершин
* @param reb Количество ребер
* @param oriented Ориентированность
* @param matrix_smezh Матрица смежности
* @param binds Количество связанных вершин
*/
class Grapth {
public:
	int ver;
	int reb;
	bool oriented;
	int** matrix_smezh;
	int* binds;

	/* Вывод матрицы смежности в файл*/
	void printMatrixSmezh() {
		out << "Матрица смежности\n";
		out << setw(5) << "|";
		for (int i = 0; i < ver; i++) {
			out << setw(3) << (char)(i + 65) << " ";
		}
		out << endl;
		for (int i = 0; i < ver; i++) {
			out << setw(3) << (char)(i + 65) << " |";
			for (int j = 0; j < ver; j++) {
				if (matrix_smezh[i][j] == INF)
					out << "INF" << " ";
				else
					out << setw(3) << matrix_smezh[i][j] << " ";
			}
			out << endl;
		}
	}
};

/*Генератор значения веса*/
int genWeight() {
	return 1 + rand() % 20;
}

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
Grapth genMatSmezh(Grapth gr, int min_bind) {

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
			gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = arrray_reb[k];
			k++;
			count++; // Записываем успешно поставленное ребро
			// Прибавляем к количеству связанных вершин
			if (abs(gr.matrix_smezh[i][j]) == 1) {
				gr.binds[i]++;
				gr.binds[j]++;
			}
		}
	}
	// Заполнение недостающих связей
	int temp = 0;
	for (int i = 0; i < gr.ver; i++) {
		if (gr.binds[i] < min_bind) { // Если связей не хватает
			for (int j = 0; j < gr.ver; j++) {
				if (gr.matrix_smezh[i][j] == 0 && i != j) {
					gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 1;
					gr.binds[i]++;
					gr.binds[j]++;
					temp++;
					i--;
					break;
				}
			}
		}
	}

	// Удаление лишних связей
	while (temp != 0) {
		for (int i = 0; i < gr.ver; i++) {
			if (gr.binds[i] > min_bind && temp > 0) { // Если связей много
				for (int j = 0; j < gr.ver; j++) {
					if (gr.binds[j] > min_bind && i != j) {
						gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 0;
						gr.binds[i]--;
						gr.binds[j]--;
						temp--;
						break;
					}
				}
			}
		}
	}

	return gr;
}

// Ориентированность
void genOriented(Grapth gr) {

	int a[] = { -1, 1 }; // Направление
	for (int i = 0; i < gr.ver; i++) {
		for (int j = i + 1; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == 1) {
				gr.matrix_smezh[i][j] = a[rand() % 2];
				gr.matrix_smezh[j][i] = -gr.matrix_smezh[i][j];
			}
		}
	}

	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == -1) gr.matrix_smezh[i][j] = 0;
		}
	}
}

/*Расстановка весов*/
void weights(Grapth gr, bool oriented) {
	if (oriented) { // Ориентированный
		for (int i = 0; i < gr.ver; i++) {
			for (int j = 0; j < gr.ver; j++) {
				if (gr.matrix_smezh[i][j] == 1) {
					gr.matrix_smezh[i][j] = genWeight(); // Вес
				}
				else if (i != j) {
					gr.matrix_smezh[i][j] = INF; // Нет пути
				}
			}
		}
	}
	else { // Неориентированный
		for (int i = 0; i < gr.ver; i++) {
			for (int j = i + 1; j < gr.ver; j++) {
				if (gr.matrix_smezh[i][j] == 1) {
					gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = genWeight(); // Вес
				}
				else {
					gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = INF; // Нет пути
				}
			}
		}
	}
}

/*Удаление динамических массивов*/
void deleteDynamicArray(Grapth gr) {
	for (int i = 0; i < gr.ver; i++) {
		delete[] gr.matrix_smezh[i];
	}
	delete[] gr.matrix_smezh;

	delete[] gr.binds;
}

/*Соединение вершин*/
void connected(int* nodes, Grapth gr) {
	int not_bind_1 = 0, with_bind_1 = 0, not_bind_2 = 0, with_bind_2 = 0;

	for (int i = 0; i < gr.ver; i++) {
		if (nodes[i] == 0) not_bind_1 = i; // Если не связанная часть
		else with_bind_1 = i; // Связанная часть
	}
	
	for (int i = 0; i < gr.ver; i++) {
		if (gr.matrix_smezh[not_bind_1][i] == 1) not_bind_2 = i; // Связанная вершина
		if (gr.matrix_smezh[with_bind_1][i] == 1) with_bind_2 = i; // Связанная вершина
	}

	if (gr.oriented) { // Ориентированный
		gr.matrix_smezh[with_bind_1][with_bind_2] = 0; // Удаляем связь
		gr.matrix_smezh[with_bind_1][not_bind_1] = genWeight(); // Генерируем новую связь
		gr.matrix_smezh[not_bind_1][not_bind_2] = 0; // Удаляем связь
		gr.matrix_smezh[not_bind_2][with_bind_2] = genWeight(); // Генерируем новую связь
	}
	else { // Неориентированный
		gr.matrix_smezh[with_bind_1][with_bind_2] = gr.matrix_smezh[with_bind_2][with_bind_1] = 0; // Удаляем связь
		gr.matrix_smezh[with_bind_1][not_bind_1] = gr.matrix_smezh[not_bind_1][with_bind_1] = genWeight(); // Генерируем новую связь
		gr.matrix_smezh[not_bind_1][not_bind_2] = gr.matrix_smezh[not_bind_2][not_bind_1] = 0; // Удаляем связь
		gr.matrix_smezh[not_bind_2][with_bind_2] = gr.matrix_smezh[with_bind_2][not_bind_2] = genWeight(); // Генерируем новую связь
	}
}

/*Поиск в глубину
*
* @param gr Граф
* @param Stack Стек для посещения
* @param nodes Список посещенных вершин
* @param node Вершина
*/
bool DFS(Grapth gr) {
	//cout << "\nПоиск в глубину (Проверка на существование пути)\n";
	stack<int> Stack;
	int* nodes = new int[gr.ver]; // вершины графа
	for (int i = 0; i < gr.ver; i++) // исходно все вершины равны 0
		nodes[i] = 0;

	Stack.push(0); // помещаем в очередь первую вершину

	while (!Stack.empty())
	{ // пока стек не пуст
		int node = Stack.top(); // извлекаем вершину
		Stack.pop();
		if (nodes[node] == 2) continue;
		nodes[node] = 2; // отмечаем ее как посещенную
		for (int j = gr.ver - 1; j >= 0; j--)
		{ // проверяем для нее все смежные вершины
			if (gr.matrix_smezh[node][j] != 0 && nodes[j] != 2)
			{ // если вершина смежная и не обнаружена
				Stack.push(j); // добавляем ее в cтек
				nodes[j] = 1; // отмечаем вершину как обнаруженную
			}
		}
	}

	bool check = 1; // Проверка связности
	for (int i = 0; i < gr.ver; i++) {
		if (nodes[i] == 0) {
			//cout << "Входит\n";
			check = 0;
			connected(nodes, gr);
			break;
		}		
	}
	delete[] nodes;
	return check;
}

/*Генератор графа
*
* @param i Номер теста
* @param max_ver, min_ver Максимальное/минимальное количество вершин
* @param max_reb, min_reb Максимальное/минимальное количество ребер
* @param max_bind Минимальное количество связей у вершины
* @param oriented Ориентированность
* @param gr Граф
* @return gr Граф
*/
Grapth grapthGeneration(int i) {
	int ver[] = { 10, 20, 50, 100 };
	int min_bind[] = { 3, 4, 10, 20 };
	int max_reb, min_reb;
	bool oriented;

	i /= tests;

	Grapth gr;

	gr.ver = ver[i]; // Вершины
	cout << "Вершин: " << gr.ver << endl;
	min_reb = ceil((double)gr.ver / 2) * min_bind[i]; // Минимум ребер
	max_reb = gr.ver * (gr.ver - 1) / 2; // Максимум ребер
	gr.reb = min_reb + rand() % (max_reb - min_reb + 1); // Ребра
	oriented = rand() % 2; // Ориентированность

	out << "\nВершин: " << gr.ver << "\nРебер: " << gr.reb <<
		"\nМинимальное количество связей: " << min_bind[i] <<
		"\nОриентированный: " << oriented << endl;

	gr = genMatSmezh(gr, min_bind[i]); // Матрица смежности
	if (oriented) genOriented(gr); // Ориентированность
	weights(gr, oriented); // Расстановка весов

	return gr;
}

/*Алгоритм Флойда-Уоршелла*/
void Floyd_Warshall(Grapth gr) {
	for (int k = 0; k < gr.ver; k++) {	//Пробегаемся по всем вершинам и ищем более короткий путь через вершину k
		for (int i = 0; i < gr.ver; i++) {
			for (int j = 0; j < gr.ver; j++) {
				gr.matrix_smezh[i][j] = min(gr.matrix_smezh[i][j], gr.matrix_smezh[i][k] + gr.matrix_smezh[k][j]);	//Новое значение ребра равно минимальному между старым ребром и суммой ребер 
			}
		}
	}
}

void main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_5.txt");

	int M = 4 * tests;
	Grapth* gr = new Grapth[M];
	for (int i = 0; i < M; i++) {
		out << "\nТест №" << (i % tests) + 1 << endl;
		cout << "\nТест №" << (i % tests) + 1 << endl;
		gr[i] = grapthGeneration(i); // Генерируем граф
		while(!DFS(gr[i])); // Проверка связности
		gr[i].printMatrixSmezh(); // Вывод матрицы

		chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now(); // стартовое время сортировки
		Floyd_Warshall(gr[i]); // Флойд-Уоршелл
		chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now(); // стартовое время сортировки
		chrono::duration<double, milli> milli_diff = end - begin; // время в микросекундах
		cout << milli_diff.count() << endl;

		gr[i].printMatrixSmezh(); // Вывод матрицы
		deleteDynamicArray(gr[i]); // Очищение массивов
	}
	delete[] gr;
	out.close();
}
