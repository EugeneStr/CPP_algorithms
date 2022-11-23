#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

/* Класс отчета */
class Otchet {
public:
	int done = 0; // Обработанон посетителей
	int not_done = 0; // Не обработано посетителей
	int time_of_visitor[3]; // Время на каждый вид посетителя
	int** window_of_visitor = new int* [3]; // Количество каждого вида у каждого окна
	int** queue_of_visitor = new int* [3]; // Количество каджого вида на оба типа очереди

	/* Вывод отчета */
	void printOtchet(int M) {
		cout << "\n\nСТАТИСТИКА";
		cout << "\n\nОбработано: " << done << "\nНе обработано: " << not_done;
		for (int i = 0; i < 3; i++) {
			cout << "\nОбщее время на посетителей типа " << i << " " << time_of_visitor[i];
		}
		for (int i = 0; i < M; i++) {
			cout << "\nОкно " << i + 1;
			for (int j = 0; j < 3; j++) {
				cout << "\nКоличество посетителей типа " << j << " " << window_of_visitor[j][i];
			}
		}
		for (int i = 0; i < 2; i++) {
			cout << "\nТип очереди " << i + 1;
			for (int j = 0; j < 3; j++) {
				cout << "\nКоличество посетителей типа " << j << " " << queue_of_visitor[j][i];
			}
		}
	}

};

/* Класс посетителя */
class Visitor {
public:
	int type; // тип посетителя
	int time; // время ообработки
	bool type_of_queue; // тип очереди
	bool in_window = false; // в окне ли посетитель
	int childChance = 20; // шанс через электронную очередь ребенка
	int adultChance = 70; // шанс через электронную очередь взрослого
	int olderChance = 5; // шанс через электронную очередь старика

	/* Конструктор посетителя */
	Visitor() {
		type = randomCharacteristic(0, 2); // тип посетителя
		genTime(type); // время посетителя
		genTypeOfQueue(type); // вид очереди
	}

	/* Вывод характеристик посетителя */
	void printCharacteristics() {
		cout << "\nType: " << type << " Time: " << time << " Type of queue: " << type_of_queue;
	}

	/* Генерация времени */
	void genTime(int type) {
		switch (type) {
		case 0: // ребенок
			time = randomCharacteristic(5, 10);
			break;
		case 1: // взрослый
			time = randomCharacteristic(2, 5);
			break;
		default: // старик
			time = randomCharacteristic(7, 9);
			break;
		}
	}

	/* Генерация типа очереди */
	void genTypeOfQueue(int type) {
		switch (type) {
		case 0: // ребенок
			type_of_queue = checkChance(childChance);
			break;
		case 1: // взрослый
			type_of_queue = checkChance(adultChance);
			break;
		default: // старик
			type_of_queue = checkChance(olderChance);
			break;
		}
	}

	/* Случайная характеристика */
	int randomCharacteristic(int a, int b) {
		return a + rand() % (b - a + 1);
	}

	/* Проверка шанса */
	bool checkChance(int chance) {
		return ((rand() % 100) < chance);
	}

};

/* Класс окна */
class Window {
public:
	bool free = true; // занятость окна
	Visitor visitor; // посетитель в окне

	/* Процесс обработки посетителя */
	void Processing() {
		visitor.time--; // Уменьшение времени
		if (visitor.time == 0) { // Посетитель ушел
			free = true;
		}
	}

	/* Заполнение окна */
	bool filling(Visitor &vis) {
		if (!vis.in_window) { // Если посетитель еще в очереди
			vis.in_window = true; // Заходит в окно
			visitor = vis;
			free = false; // Окно занято
			return 1;
		}
		else return 0;
	}
};

/* Класс учреждения */
class Institution {
public:
	int N; // Количество посетителей
	int M; // Количество окон
	vector <int> queue; // Очередь
	vector <Visitor> visitor; // Вектор посетителей
	Window* window; // Массив окон
	Otchet otchet; // Отчет

	/* Конструктор учреждения */
	Institution() {
		N = randomCharacteristic(3, 10); // Количество посетителей изначально
		cout << "Введите количество окон:\n";
		cin >> M; // Количество окон
		dynamicArray(); // Динамические массивы
		startGenerationQueue(); // Генерация очереди
	}

	/* Случайная характеристика */
	int randomCharacteristic(int a, int b) {
		return a + rand() % (b - a + 1);
	}

	/* Создание динамических массивов */
	void dynamicArray() {
		window = new Window[M]; // Массив окон
		for (int i = 0; i < 3; i++) {
			otchet.window_of_visitor[i] = new int[M]; // Массив посетителей в отчет
			otchet.queue_of_visitor[i] = new int[2];
		}
		nullMean(); // Нулевые значение массива
	}

	/* Нулевые значения массива */
	void nullMean() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < M; j++) {
				otchet.window_of_visitor[i][j] = 0;
			}
			for (int j = 0; j < 2; j++) {
				otchet.queue_of_visitor[i][j] = 0;
			}
		}
	}

	/* Стартовая генерация очереди */
	void startGenerationQueue() {
		for (int i = 0; i < N; i++) {
			Visitor new_visitor;
			visitor.push_back(new_visitor);
			queue.push_back(visitor[i].type);
			visitor[i].printCharacteristics(); // Вывод характеристик посетителей
		}
		working(); // Начало работы
	}

	/* Проверка шанса */
	bool checkChance(int chance) {
		return ((rand() % 100) < chance);
	}

	/* Работа учреждения */
	void working() {
		int hour, minute;
		for (hour = 9; hour < 19; hour++) { // Часы
			for (minute = 0; minute < 60; minute++) { // Минуты
				printf("\n\nTIME\t%02d:%02d\n", hour, minute); // вывод времени
				for (int i = 0; i < M; i++) { // Окна
					foundVisitor(); // Нахождение следующего посетителя
					if (window[i].free && hour != 13) { // Если окно свободно и сейчас не обед
						windowFilling(i); // Заполнение окна
					}
					else {
						window[i].Processing(); // Процесс обработки
						addTimeInOtchet(i); // Добавление в отчет
					}
					printWindowInfo(i); // Информация об окне
				}
				//if (minute % 5 == 0) {
					if (checkChance(5)) addVisitorQueue(0); // Появление ребенка
					if (checkChance(40)) addVisitorQueue(1); // Появление взрослого
					if (checkChance(30)) addVisitorQueue(2); // Появление старика
				//}
				for (int i = 0; i < N; i++) {
					visitor[i].printCharacteristics(); // Вывод характеристик посетителей
				}
			}
		}
		otchet.not_done = N; // Запись в отчет
		minute = 0;
		while (checkFreeWindow()) { // Обработка последних посетителей
			printf("\n\nTIME\t%02d:%02d", hour, minute); // вывод времени
			for (int i = 0; i < M; i++) {
				if (!window[i].free)
					window[i].Processing(); // Процесс обработки
					addTimeInOtchet(i); // Запись в отчет
				printWindowInfo(i); // Вывод информации об окне
			}
			minute++; // Добавление минут
		}
		otchet.printOtchet(M); // Вывод отчета
	}

	/* Добавление посетителя в очередь */
	void addQueueInOtchet(int i) {
		int e_queue = 0; // Живая очередь
		if (window[i].visitor.type_of_queue)
			e_queue = 1; // Электронная очередь
		switch (window[i].visitor.type) {
		case 0: // Ребенок
			otchet.queue_of_visitor[0][e_queue]++;
			break;
		case 1: // Взрослый
			otchet.queue_of_visitor[1][e_queue]++;
			break;
		default: // Старик
			otchet.queue_of_visitor[2][e_queue]++;
			break;
		}
	}

	/* Добавление информации об окне в отчет */
	void addWindowInOtchet(int i) {
		switch (window[i].visitor.type) {
			case 0: // Ребенок
				otchet.window_of_visitor[0][i]++;
				break;
			case 1: // Взрослый
				otchet.window_of_visitor[1][i]++;
				break;
			default: // Старик
				otchet.window_of_visitor[2][i]++;
				break;
			}
	}

	/* Добавление времени в отчет */
	void addTimeInOtchet(int i) {
		switch (window[i].visitor.type) {
		case 0: // Ребенок
			otchet.time_of_visitor[0] ++;
			break;
		case 1: // Взрослый
			otchet.time_of_visitor[1] ++;
			break;
		default: // Старик
			otchet.time_of_visitor[2] ++;
			break;
		}
	}

	/* Заполнение окна */
	void windowFilling(int i) {
		for (int j = 0; j < N; j++) {
			if (window[i].filling(visitor[j])) {
				visitor.erase(visitor.begin()); // Удаление посетителя из очереди
				queue.erase(queue.begin());
				N--; // Уменьшение очереди
				addQueueInOtchet(i); // Добавление очереди в отчет
				addWindowInOtchet(i); // Добавление информации об окне в отчет
				otchet.done++; // Добавление в отчет
				break;
			}
		}
	}

	/* Вывод информации об окне */
	void printWindowInfo(int i) {
		cout << "\nНомер окна: " << i + 1;
		if (!window[i].free)
			cout << "\nПосетитель:\nТип: " << window[i].visitor.type << " Time: " << window[i].visitor.time;
		else
			cout << " Окно свободно";
	}

	/* Проверка окна на занятость */
	bool checkFreeWindow() {
		for (int i = 0; i < M; i++) {
			if (!window[i].free) return 1;
		}
		return 0;
	}

	/* Добавление посетителя в очередь */
	void addVisitorQueue(int type) {
		Visitor new_visitor; // Новый посетитель
		new_visitor.genTime(type); // Время
		new_visitor.genTypeOfQueue(type); // Тип очереди
		new_visitor.type = type; // Тип посетителя
		new_visitor.in_window = false;
		visitor.push_back(new_visitor); // Добавление посетителя в конец очереди
		queue.push_back(type);
		N++; // Увеличение очереди
	}

	/* Нахождение следующего посетителя */ 
	void foundVisitor() {
		for (int i = 0; i < N; i++) {
			if (visitor[i].type_of_queue) {
				reposition(i); // Перестановка очереди
				break;
			}
		}
	}

	/* Перестановка очереди */
	void reposition(int number) {
		visitor.insert(visitor.begin(), visitor[number]); // Перестановка посетителя в начало очереди
		queue.insert(queue.begin(), queue[number]);
		visitor.erase(visitor.begin() + number + 1); // Удаление посетителя с прошлого места
		queue.erase(queue.begin() + number + 1);
	}

	/* Удаление посетителя из очереди */

	/* Вывод очереди */
	void printQueue() {
		cout << "\n";
		for (int i = 0; i < N; i++) {
			cout << queue[i] << " ";
		}
	}

	/* Удаление динамических массивов */
	void deleteArray() {
		delete[] window;
		for (int i = 0; i < 3; i++) {
			delete[] otchet.window_of_visitor[i];
		}
		for (int i = 0; i < 3; i++) {
			delete[] otchet.queue_of_visitor[i];
		}
	}
};



int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Russian"); // Руссификатор
	Institution institution;
	institution.deleteArray();
	return 0;
}
