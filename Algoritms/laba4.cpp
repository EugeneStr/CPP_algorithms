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

struct Edge {
	int begin;
	int end;
};

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

	void printListReb() {
		cout << "Список ребер\n";
		list<list<int>> list = list_reb;
		int i = 0;
		while (list.size() > 0) {
			cout << i + 1 << " | " << (char)(list.front().front() + 65) << " -> " << (char)(list.front().back() + 65) << endl;
			list.pop_front();
			i++;
		}
	}

};

Grapth genMatSmezh(Grapth gr, int max_bind, bool oriented) {

	int M = gr.ver * (gr.ver - 1) / 2;
	vector <int> arrray_reb(M);

	for (int i = 0; i < gr.reb; i++) {
		arrray_reb[i] = 1;
	}

	random_shuffle(arrray_reb.begin(), arrray_reb.end());


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
			if (arrray_reb[k] != 0 && (gr.binds[i] == max_bind || gr.binds[j] == max_bind)) {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 0;
				k++;
			}
			else {
				gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = arrray_reb[k];
				k++;
				count++;
			}
			if (abs(gr.matrix_smezh[i][j]) == 1) {
				gr.binds[i]++;
				gr.binds[j]++;
			}
		}
	}

	while (M - count > 0) {
		for (int i = 0; i < gr.ver; i++) {
			
			if (gr.binds[i] < max_bind) {
				for (int j = i + 1; j < gr.ver; j++) {
					if (gr.matrix_smezh[i][j] == 0 && gr.binds[j] < max_bind) {
						gr.matrix_smezh[i][j] = gr.matrix_smezh[j][i] = 1;
						count++;
						gr.binds[i]++;
					}
					if (M - count <= 0) break;
				}
			}
			if (M - count < 0) break;
		}
	}

	return gr;
}

Grapth genMatInc(Grapth gr, bool oriented) {
	list<list<int>> temp = gr.list_reb;

	gr.matrix_inc = new int* [gr.ver];
	for (int i = 0; i < gr.ver; i++) {
		gr.matrix_inc[i] = new int[gr.reb];
	}

	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.reb; j++) {
			gr.matrix_inc[i][j] = 0;
		}
	}

	int i = 0;

	while (temp.size() > 0) {
		if (oriented) {
			gr.matrix_inc[temp.front().front()][i] = 1;
			gr.matrix_inc[temp.front().back()][i] = -1;

		}
		else {
			if (temp.front().front() < temp.front().back()) {
				gr.matrix_inc[temp.front().front()][i] = gr.matrix_inc[temp.front().back()][i] = 1;
			}
		}
		temp.pop_front();
		i++;
	}

	return gr;
}

Grapth genListSmezh(Grapth gr) {
	list<int> temp;
	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[j][i] == 1) {
				temp.push_back(j);
			}
		}
		gr.list_smezh.push_back(temp);
		temp.clear();
	}
	return gr;
}

Grapth genListReb(Grapth gr) {
	list<int> temp;
	for (int i = 0; i < gr.ver; i++) {
		for (int j = 0; j < gr.ver; j++) {
			if (gr.matrix_smezh[i][j] == 1) {
				temp.push_back(i);
				temp.push_back(j);
				
				gr.list_reb.push_back(temp);
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
		cout << endl << node + 1; // выводим номер вершины
	}
	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); // конечное время сортировки
	chrono::duration<double, milli> milli_diff = stop - begin; // время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "\nПуть до вершины " << req + 1 << endl;
	cout << req + 1;
	while (!Edges.empty()) {
		e = Edges.top();
		Edges.pop();
		if (e.end == req) {
			req = e.begin;
			cout << " <- " << req + 1;
		}
	}
	delete[] nodes;
}

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

	start = rand() % gr.ver;
	cout << "\n" << start;
	req = rand() % gr.ver;
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
		cout << node + 1 << endl; // выводим номер вершины
	}
	chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now(); // конечное время сортировки
	chrono::duration<double, milli> milli_diff = stop - begin; // время в наносекундах
	cout << "\nВремя (млсек): " << milli_diff.count() << endl;
	cout << "Путь до вершины " << req + 1 << endl;
	cout << req + 1;
	bool temp = 0;
	int end = req;
	while (!Edges.empty())
	{
		e = Edges.top();
		Edges.pop();
		if (e.end == req)
		{
			req = e.begin;
			cout << " <- " << req + 1;
			temp = 1;
		}
	}
	delete[] nodes;
	if (!temp) cout << "\nотсутствует";
	else BFS(gr, start, end);
	return gr;
}

Grapth grapthGeneration(int i) {
	int max_ver = 30, min_ver = 3;
	int max_reb, min_reb = 3;
	int max_bind;
	bool oriented;
	int max_entry, max_exit;
	
	//min_ver = 3 + i * 2;
	//max_ver = min_ver + 1;

	Grapth gr;

	/*cout << "Введите максимальное количество вершин: ";
	cin >> max_ver;
	cout << "Введите минимальное количество вершин: ";
	cin >> min_ver;
	cout << "Введите максимальное количество связей у вершины: ";
	cin >> max_bind;
	cout << "Ориентированный ли граф? ";
	cin >> oriented;
	cout << "Максимальное количество входов: ";
	cin >> max_entry;
	cout << "Максимальное количество выходов: ";
	cin >> max_exit;*/

	/*gr.ver = min_ver + rand() % (max_ver - min_ver);
	min_reb = 1;
	max_reb = gr.ver * (gr.ver - 1) / 2;

	gr.reb = min_reb + rand() % (max_reb - min_reb + 1);*/
	gr.ver = (max_ver) / 10 * (i + 1);
	gr.reb = min_reb + i * (min_reb + 3);
	max_bind = ceil(((double)gr.reb * 2) / (double)gr.ver) + rand() % (gr.ver + 1);
	max_entry = max_exit = max_bind / 2 + 1;
	oriented = rand() % 2;

	cout << "\nВершин: " << gr.ver << "\nРебер: " << gr.reb <<
		"\nМаксимальное количество связей: " << max_bind << 
		"\nОриентированный: " << oriented << 
		"\nМаксимальное количество входов: " << max_entry << 
		"\nМаксимальное количество выходов: "<< max_exit << endl;

	gr = genMatSmezh(gr, max_bind, oriented);
	if (oriented) genOriented(gr, max_entry, max_exit);
	gr = genListReb(gr);
	gr = genListSmezh(gr);
	gr = genMatInc(gr, oriented);

	gr.printMatrixSmezh();
	gr.printListSmezh();
	gr.printListReb();
	gr.printMatrixInc();

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
