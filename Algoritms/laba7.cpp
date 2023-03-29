#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include <vector>
#include <fstream>

using namespace std;

/* структура для представления узлов дерева */
struct node {
	int key; // значение
	int size; // размер узла
	node* left; // левый потомок
	node* right; // правый потомок
	node(int k) { key = k; left = right = 0; size = 1; }
};

/* получение размера узла */
int getsize(node* p) { 
	if (!p) return 0;
	return p->size;
}

/* фиксирование размера */
void fixsize(node* p) {
	p->size = getsize(p->left) + getsize(p->right) + 1;
}

/* Разница между высотой правого и левого потомков */
int bfactor(node* p) {
	return getsize(p->right) - getsize(p->left);
}

/* правый поворот вокруг узла p */
node* rotateright(node* p) { 
	node* q = p->left; // левый потомок
	if (!q) return p; 
	p->left = q->right; // замена
	q->right = p;
	q->size = p->size; 
	fixsize(p); // фиксируем размер
	return q;
}

/* левый поворот вокруг узла p */
node* rotateleft(node* q) { 
	node* p = q->right; // правый потомок
	if (!p) return q;
	q->right = p->left; // замена
	p->left = q;
	p->size = q->size;
	fixsize(q); // фиксируем размер
	return p;
}

/* Балансировка */
node* balance(node* p) {
	fixsize(p); // фиксирование глубины
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

/* Добавление узла */
node* addnode(int k, node* p, bool AVL) {
	if (!p) return new node(k); // пока не найдем пустое место
	if (k <= p->key) { // Если значение меньше или равно текущему узлу
		p->left = addnode(k, p->left, AVL);  // идем влево
		return AVL ? balance(p) : rotateright(p);
	}
	else {
		p->right = addnode(k, p->right, AVL); // идем вправо
		return AVL ? balance(p) : rotateleft(p);
	}
}

/* Случайное добавление узла */
node* insert_random(node* p, int k) {
	if (!p) return new node(k); // если нет потомков
	if (rand() % (p->size + 1) == 0) // случайное добавление
		return addnode(k, p, 0); // обычная вставка
	if (p->key > k) // если меньше текущего узла
		p->left = insert_random(p->left, k); // идем влево
	else
		p->right = insert_random(p->right, k); // идем вправо
	fixsize(p); //фиксируем размер
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

/* объединение деревьев */
node* join(node* p, node* q) {
	if (!p) return q; // если пустое
	if (!q) return p;
	if (rand() % (p->size + q->size) < p->size) { // случайное объединение
		p->right = join(p->right, q); // идем вправо
		fixsize(p); // фиксируем размер
		return p;
	} else {
		q->left = join(p, q->left); // идем влево
		fixsize(q); // фиксируем размер
		return q;
	}
}

/* объединение AVL-деревьев */
node* join_AVL(node* q, node* r) {
	if (!r) return q; // если правого потомка нет 
	node* min = findmin(r); // ищем минимальное значение в правом потомке
	min->right = removemin(r); // удаляем минимальное
	min->left = q;
	return balance(min);
}

/* удаление из дерева p первого найденного узла с ключом k */
node* remove(node* p, int k, bool AVL) {
	if (!p) return p;
	if (p->key == k) { // если найден нужный узел
		node* q;
		if (AVL) q = join_AVL(p->left, p->right); // объединение деревьев
		else q = join(p->left, p->right);
		delete p; // удаление узла
		return q;
	}
	else if (k < p->key)
		p->left = remove(p->left, k, AVL); // идем влево
	else
		p->right = remove(p->right, k, AVL); // идем вправо
	if (AVL) return balance(p); // балансировка при необходимости
	else return p;
}

/* проход по дереву */
vector<int> treewalk(node* p, int level, vector<int>& a) {
	if (p) {
		if (p->left == NULL && p->right == NULL) { // если конец ветки
			a.push_back(level); // записываем глубину
		}	
		treewalk(p->right, level + 1, a); // правый потомк
		treewalk(p->left, level + 1, a); // левый потомок
	}
	return a;
}

/* вывод всех глубин веток */
void printTreeWalk(vector<int> a) {
	ofstream out1;
	out1.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_AVL_7_2.txt", ios::app);
	out1 << "\n";
	for (int i = 0; i < a.size(); i++) {
		out1 << a[i] << " ";
	}
	out1 << "\n";
	out1.close();
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

/* вывод максимальной глубины дерева */
void maxsize(vector<int> a) {
	ofstream out1;
	out1.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_AVL_7_max.txt", ios::app);
	out1 << *max_element(a.begin(), a.end());
	out1 << endl;
	out1.close();
}

/* Освобождение памяти */
void freemem(node* tree) {
	if (tree != NULL) {	// если дерево не пустое
		freemem(tree->left); // рекурсивно удаляем левую ветку
		freemem(tree->right); // рекурсивно удаляем правую ветку
		delete tree; // удаляем корень
	}
}

int main() {
	struct node* AVL_root = 0;    // AVL-дерево
	struct node* rand_root = 0; // Рандомизированное дерево
	int M = 1, cycle = 50, count = 1000; // Количество серий, циклов и операций
	int N; // Количество элементов в дереве
	vector<char> a; // Вектор элементов
	vector<int> rand_depths, AVL_depths;
	chrono::high_resolution_clock::time_point begin;
	chrono::high_resolution_clock::time_point end;
	chrono::duration<double, milli> milli_diff;

	srand(time(0));
	setlocale(LC_ALL, "Russian");

	ofstream out;
	out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_AVL_7.txt");
	ofstream out1;
	out1.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_AVL_7_2.txt");
	out1.close();
	ofstream out2;
	out2.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data_AVL_7_max.txt");
	out2.close();

	for (int ser = 0; ser < M; ser++) {
		N = pow(2, 14 + ser);
		for (int c = 0; c < cycle; c++) {
			cout << "\nСерия " << ser << " цикл " << c << endl;
			out << ser << " " << c << endl;

			for (int i = 0; i < N; i++) {
				a.push_back(1 + rand() % N); // Вставка случайных чисел
			}

			////out << "\nОбычное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			rand_root = insertBinTree(a, N, 0); // Вставка в обычное дерево
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nСбалансированное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			AVL_root = insertBinTree(a, N, 1); // Вставка в AVL дерево
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			vector<char> t;
			for (int i = 0; i < count; i++) {
				t.push_back(1 + rand() % N);
				a.push_back(t.back());
			}

			////out << "\nВставка в рандомизированное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				rand_root = addnode(t[i], rand_root, 0); // Вставка в обычное дерево
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			//out << "\nВставка в сбалансированное бинарное дерево\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				AVL_root = addnode(t[i], AVL_root, 1); // Вставка в обычное дерево
			}
			end = chrono::high_resolution_clock::now(); // конечное время
			milli_diff = end - begin; // время в микросекундах
			out << milli_diff.count() << " " << milli_diff.count() / count << endl;

			
			rand_depths = treewalk(rand_root, 0, rand_depths); // глубина каждой ветки
			AVL_depths = treewalk(AVL_root, 0, AVL_depths);
			
			maxsize(rand_depths); // максимальная глубина
			maxsize(AVL_depths);

			random_shuffle(a.begin(), a.end()); // перемешивание вектора
			vector<char> b = a;

			//out << "\nУдаление из обычного бинарного дерева\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				rand_root = remove(rand_root, (int)b.back(), 0);
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

			
			////out << "\nПоиск в обычном бинарном дереве\n";
			begin = chrono::high_resolution_clock::now(); // стартовое время
			for (int i = 0; i < count; i++) {
				reverse(rand_root, (int)a[i]);	
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

			//cout << "\nСерия " << ser << " цикл " << c << endl;
			rand_depths.clear();
			AVL_depths.clear();
			rand_depths = treewalk(rand_root, 0, rand_depths);
			AVL_depths = treewalk(AVL_root, 0, AVL_depths);
			printTreeWalk(rand_depths);
			printTreeWalk(AVL_depths);

			a.clear();
			t.clear();
			b.clear();
			freemem(rand_root); // Освобождение памяти
			freemem(AVL_root);
		}
	}
	cout << "Programm end!";
	out.close();
	return 0;
}
