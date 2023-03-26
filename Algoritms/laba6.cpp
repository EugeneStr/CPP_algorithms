#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

/* Структура узла */
struct node {
	int key; //значение элемента
	unsigned char height; // уровень потомка
	node* left; // левый потомок
	node* right; // правый потомок
	node(int k) { key = k; left = right = 0; height = 1; } // начальная инциализация
};

/* Глубина узла */
unsigned char height(node* p) { 
	return p ? p->height : 0;
}

/* Разница между высотой правого и левого потомков */
int bfactor(node* p) {
	return height(p->right) - height(p->left);
}

/* Фиксирование глубины узла */
void fixheight(node* p) {
	unsigned char hl = height(p->left); // Глубина левого потомка
	unsigned char hr = height(p->right); // Глубина правого потомка
	p->height = (hl > hr ? hl : hr) + 1; // Определение глубины потомка
}

/* Правый поворот вокруг узла */
node* rotateright(node* p) { 
	node* q = p->left; // левый потомок
	p->left = q->right; 
	q->right = p; 
	fixheight(p); // фиксирование глубины
	fixheight(q);
	return q;
}

/* Левый поворот вокруг узла */
node* rotateleft(node* q) {
	node* p = q->right; // правый потомок
	q->right = p->left;
	p->left = q;
	fixheight(q); // фиксирование глубины
	fixheight(p);
	return p;
}

/* Балансировка */
node* balance(node* p) {
	fixheight(p); // фиксирование глубины
	if (bfactor(p) == 2) { // если по правый потомок глубже левого
		if (bfactor(p->right) < 0) // если у правого потомка левый потомок глубже
			p->right = rotateright(p->right); // правый поворот вокруг правого потомка
		return rotateleft(p); // левый поворот вокруг узла
	}
	if (bfactor(p) == -2) { // если по левый потомок глубже правого
		if (bfactor(p->left) > 0)  // если у левого потомка правый потомок глубже
			p->left = rotateleft(p->left); // левый поворот вокруг левого потомка
		return rotateright(p); // правый поворот вокруг узла
	}
	return p;
}

/* Поиск минимального узла */
node* findmin(node* p) {
	return p->left ? findmin(p->left) : p;
}

/* Удаление минимального узла */
node* removemin(node* p) {
	if (p->left == 0) // Если нет левого потомка
		return p->right;
	p->left = removemin(p->left); // рекурсия
	return balance(p); // балансировка
}

/* Удаление узла
* 
* @param p узел
* @param value удаляемое значение
* @param AVL сбаласированное ли дерево
* @return возвращает дерево с удаленным узлом
*/
node* remove(node* p, int value, bool AVL) {
	if (!p) return 0; 
	if (value < p->key) // если значение меньше текущего узла
		p->left = remove(p->left, value, AVL); // идем в левого потомка
	else if (value > p->key) // если значение больше текущего узла
		p->right = remove(p->right, value, AVL); // идем в правого потомка
	else { // если нашли нужный узел
		node* q = p->left; // левый потомок
		node* r = p->right; // правый потомок
		delete p; // удаляем текущий узел
		if (!r) return q; // если правого потомка нет 
		node* min = findmin(r); // ищем минимальное значение в правом потомке
		min->right = removemin(r); // удаляем минимальное
		min->left = q; 
		return balance(min);
	}
	if (AVL) return balance(p); // балансировка при необходимости
	else return p;
}

/* Поиск в дереве */
void reverse(node* p, int k) {
	//if (p->key == k) cout << "Найден узел со значением " << k << endl;
	//else 
	if (p->key != k) { //если не нашли нужный узел
		if (k < p->key) reverse(p->left, k);		//Рекурсивная функция для левого поддерева
		else reverse(p->right, k);					//Рекурсивная функция для правого поддерева
	}
}

/* Добавление узла */
node* addnode(int k, node* p, bool AVL) {
	if (!p) return new node(k); // пока не найдем пустое место
	if (k <= p->key) // Если значение меньше или равно текущему узлу
		p->left = addnode(k, p->left, AVL);  // идем влево
	else 
		p->right = addnode(k, p->right, AVL); // идем вправо
	if (AVL) return balance(p); // балансировка при необходимости
	else return p;
}

/* Освобождение памяти */
void freemem(node* tree) {
	if (tree != NULL) {	// если дерево не пустое
		freemem(tree->left); // рекурсивно удаляем левую ветку
		freemem(tree->right); // рекурсивно удаляем правую ветку
		delete tree; // удаляем корень
	}
}

/* Вывод дерева */
void print_Tree(node* p, int level) {
	if (p) {
		print_Tree(p->right, level + 1); // правый потомк
		for (int i = 0; i < level; i++) cout << "   ";
		cout << p->key << endl; // текущий узел
		print_Tree(p->left, level + 1); // левый потомок
	}
}

/*Вставка в дерево */
node* insertBinTree(vector<char> a, int N, bool AVL) {
	struct node* root = 0; // Дерево

	for (int i = 0; i < N; i++) {
		root = addnode((int)a[i], root, AVL); // размещаем введенный узел на дереве
	}
	//print_Tree(root, AVL);
	return root;
}

// Тестирование работы
int main() {
	struct node* AVL_root = 0;    // AVL-дерево
	struct node* root = 0; // Просто дерево
	int M = 10, cycle = 20, count = 1000; // Количество серий, циклов и операций
	int N; // Количество элементов в дереве
	vector<char> a; // Вектор элементов
	chrono::high_resolution_clock::time_point begin;
	chrono::high_resolution_clock::time_point end;
	chrono::duration<double, milli> milli_diff;

	srand(time(0));
	setlocale(LC_ALL, "Russian");

	ofstream out;
	out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data6.txt");

	for (int ser = 0; ser < M; ser++) {
		N = pow(2, 10 + ser);
		for (int c = 0; c < cycle; c++) {
			//out << "\nСерия " << ser << " цикл " << c << endl;
			out << ser << " " << c << endl;
			if (2 * c < cycle) {
				for (int i = 0; i < N; i++) {
					a.push_back(1 + rand() % N); // Вставка случайных чисел
				}
			}
			else {
				for (int i = 0; i < N; i++) {
					a.push_back(i + 1); // Вставка последовательности
				}
			}

			//out << "\nОбычное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			root = insertBinTree(a, N, 0); // Вставка в обычное дерево
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nСбалансированное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			AVL_root = insertBinTree(a, N, 1); // Вставка в AVL дерево
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			random_shuffle(a.begin(), a.end()); // перемешивание вектора

			//out << "\nПоиск в обычном бинарном дереве\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				reverse(root, (int)a[i]);
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nПоиск в сбалансированном бинарном дереве\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				reverse(AVL_root, (int)a[i]);
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nПоиск в массиве\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				int k = (int)a[rand() % a.size()];
				for (int i = 0; i < N; i++) {
					if (k == (int)a[i]) break;
				}
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			vector<char> b = a;

			//out << "\nУдаление из обычного бинарного дерева\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				root = remove(root, (int)b.back(), 0);
				b.pop_back();
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nУдаление из сбалансированого бинарного дерева\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				AVL_root = remove(AVL_root, (int)a.back(), 1);
				a.pop_back();
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			a.clear();
			b.clear();
			freemem(root); // Освобождение памяти
			freemem(AVL_root);
		}
	}
	cout << "Programm end!";
	out.close();
	return 0;
}
