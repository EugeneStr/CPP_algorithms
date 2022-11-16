/*
Вариант 4
Вам необходимо реализовать пошаговый бой персонажей на клеточном поле квадратной формы. При этом:
Существует квадратное поле N на N.
По его краям располагаются две группы бойцов.
Боец имеют кол - во здоровья, броня(1 - 20), бонусный урон(1 - 8) и бонус инициативы, дальность хода.
Бойцы занимают одну клетку на поле.
Существует несколько типов бойцов,
лучники обладают дистанционной атакой на большое расстояние(10 клеток),
маги обладают атакой по площади(радиус 1 клетку от точки удара) на среднее расстояние(5 клеток),
войны обладают большим запасом прочности и атакуют на ближнем расстоянии(1 клетка).
Во время боя порядок хода определяется инициативой бойцов(Инициатива существ определяется в диапазоне от 1 до 20 перед началом боя).
Боец атакует всегда ближайшего к нему противника до тех пор, пока противник не будет иметь 0 жизней.
Если вокруг противника нет места для атаки, атакуется следующий по расстоянию противник.
Для атаки бойцу ближнего боя требуется подойти к противнику.
За один ход существо может либо 1 раз атаковать врага, либо выполнить перемещение к врагу.
Урон нанесенный бойцу вычисляется как случайное значение в диапазоне от 1 до 20 + бонусный урон - броня противника(в отрицательную сторону уйти не может)
Требуется:
1. Реализовать абстрактный класс бойца со всеми необходимыми параметрами и методами.
2. Реализовать конкретные реализации бойцов.
3. (На половину баллов) Продемонстрировать взаимодействия бойцов друг с другом в рамках боя 1 на 1.
4. (Усложненный вариант) Реализовать моделирование сражения двух групп из M бойцов, имеющих сбалансированный состав по следующему соотношению:
	50 % ближний бой, 30 % дальний бой, 20 % маги, на поле N на N клеток.Бой вести до полной победы одной группы над другой.
*/

#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>

#define underline "\033[4m"
#define underlineoff "\033[0m"

using namespace std;


/*Класс бойцов*/
class Fighter {
public:
	int type; // тип бойца
	int team; // команда
	int number; // номер в команде
	int x; // расположение
	int y;
	double health = 10; // Здоровье
	int armor; // Броня
	int bonus_armor; // Бону брони
	int damage; // Дамаг
	int initiative; // Инициатива
	int step; // Количество шагов

	/*Заполнение характеристик*/
	void setCharacteristics(int i,int j) {
		team = i;
		number = j;
		health = 10; // Здоровье
		armor = 1 + rand() % 20;
		damage = Damage() + BonusDamage();
		initiative = 1 + rand() % 20;
		step = 1 + rand() % 5;
	}

	/*Рандом бонуса брони*/
	void randomBonusArmor() {
		armor += 10 + rand() % 15;
	}

	/*Вывод характеристик*/
	void printCharacteristics() {
		if (health < 0) health = 0;
		cout << setw(6) << health << "|"
			<< setw(5) << armor << "|"
			<< setw(6) << damage << "|"
			<< setw(10) << initiative << "|"
			<< setw(4) << step << "|";
	}

	/*Случайные координаты*/
	void randomCoordinate(int N, int side) {
		x = rand() % N;
		y = side;
	}

	/*Количество шагов*/
	void randomSteps(Fighter**array_of_fighter, int team, int number) {
		array_of_fighter[team][number].step = 1 + rand() % 5;
	}

	/*Случайный дамаг*/
	int Damage() {
		return 1 + rand() % 20;
	}

	/*Случайный бонусный дамаг*/
	int BonusDamage() {
		return 1 + rand() % 8;
	}

	/*Виртуальный метод атаки*/
	virtual void methodAttack() {
		int attack = 0;
	};
};

/*Класс лучника*/
class Archer : public Fighter {
public:
	int attack = 10; // Дальность атаки

	/*Метод атаки*/
	void methodAttack(Fighter** array_of_fighter, int team, int enemy_number, int number) {
		int dam = 0;
		// Учет брони врага
		array_of_fighter[1 - team][enemy_number].armor -= array_of_fighter[team][number].damage;
		if (array_of_fighter[1 - team][enemy_number].armor < 0) {
			dam = -array_of_fighter[1 - team][enemy_number].armor;
			array_of_fighter[1 - team][enemy_number].armor = 0;
		}
		// Урон по врагу
		if (array_of_fighter[1 - team][enemy_number].armor == 0) dam = array_of_fighter[team][number].damage;
		array_of_fighter[1 - team][enemy_number].health -= dam;
	}
};

/*Класс мага*/
class Wizard : public Fighter {
public:
	int attack = 5; // Дальность атаки
	int radiusAttack = 1; // Радиус атаки

	/*Метод атаки*/
	void methodAttack(Fighter**array_of_fighter, int team, int enemy_number, int number) {
		int dam = 0;
		// Учет брони врага
		array_of_fighter[1 - team][enemy_number].armor -= array_of_fighter[team][number].damage;
		if (array_of_fighter[1 - team][enemy_number].armor < 0) {
			dam = -array_of_fighter[1 - team][enemy_number].armor;
			array_of_fighter[1 - team][enemy_number].armor = 0;
		}
		// Урон по врагу
		if (array_of_fighter[1 - team][enemy_number].armor == 0) dam = array_of_fighter[team][number].damage;
		array_of_fighter[1 - team][enemy_number].health -= dam;
	}
};
 /*Класс воина*/
class Warrior : public Fighter {
public:
	int attack = 1; // Дальность атаки
	int bonus_armor = 0; // Бонус атаки

	Warrior() {
		bonus_armor = 10 + rand() % 15;
		armor += bonus_armor;
	}

	/*Метод атаки*/
	void methodAttack(Fighter** array_of_fighter, int team, int enemy_number, int number) {
		int dam = 0;
		// Учет брони врага
		array_of_fighter[1 - team][enemy_number].armor -= array_of_fighter[team][number].damage;
		if (array_of_fighter[1 - team][enemy_number].armor < 0) {
			dam = -array_of_fighter[1 - team][enemy_number].armor;
			array_of_fighter[1 - team][enemy_number].armor = 0;
		}
		// Урон по врагу
		if (array_of_fighter[1 - team][enemy_number].armor == 0) dam = array_of_fighter[team][number].damage;
		array_of_fighter[1 - team][enemy_number].health -= dam;
	}
};

/*Класс битвы*/
class Fight {
public:
	Fighter** array_of_fighter = new Fighter * [2]; // Массив команд бойцов
	int** fighter_type = new int * [2]; // Типы бойцов
	Fighter* all_fighter_array; // Массив всех бойцов
	bool** pole_array; // Массив поля
	int N = 0; // Размер поля
	int M = 0; // Количество бойцов

	Fight() {
		getM(); // Количество бойцов
		getN(); // Размер поля
		createDinamicArray(); // Динамические массивы
		setTypeFighters(); // Типы бойцов
	}

	/* Размер поля */
	void getN() {
		cout << "Введите размер поля: ";
		cin >> N;
		if (N < M) N = M;
	}

	/* Количество бойцов */
	void getM() {
		cout << "Введите количество бойцов: ";
		cin >> M;
	}

	/*Динамические массивы*/
	void createDinamicArray() {
		pole_array = new bool* [N];
		for (int i = 0; i < N; i++) {
			pole_array[i] = new bool[N];
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				pole_array[i][j] = false;
			}
		}
		array_of_fighter[0] = new Fighter[M]; // Выделение памяти под каждую команду
		array_of_fighter[1] = new Fighter[M];
		fighter_type[0] = new int[M];
		fighter_type[1] = new int[M];
		all_fighter_array = new Fighter[M * 2];
	}

	/*Расстановка типов бойцов*/
	void setTypeFighters() {
		int warrior_count = int(M / 2); // Воины
		int wizard_count = int(M / 5); // Маги
		int archer_count = M - warrior_count - wizard_count; // Лучники
		setTypeArray(0, warrior_count, 0);
		setTypeArray(warrior_count, archer_count + warrior_count, 1);
		setTypeArray(archer_count + warrior_count, warrior_count + wizard_count + archer_count,2);
	}

	/*Тип бойца*/
	void setTypeArray(int start, int count, int type) {
		for (int team = 0; team < 2; team++) {
			for (int i = start; i < count; i++) {
				fighter_type[team][i] = type;
			}
		}
	}

	/*Вывод команд*/
	void printTeams() {
		string type_of_fighter[3] = { "Воин", "Лучник", "Маг" };
		for (int i = 0; i < 2; i++) {
			cout << "\n\n                     Команда " << i + 1 << "                      ";
			cout << "\n       |  Type |Health|Armor|Damage|Initiative|Step|";
			for (int j = 0; j < M; j++) {
				array_of_fighter[i][j].setCharacteristics(i + 1, j + 1);
				cout << "\nБоец " << setw(2) << array_of_fighter[i][j].number << "|";
				cout << setw(7) << type_of_fighter[fighter_type[i][j]] << "|";
				if (fighter_type[i][j] == 0) addBonusArmor(i, j);
				array_of_fighter[i][j].printCharacteristics();
			}
		}
	}

	/*Бонус брони*/
	void addBonusArmor(int team, int number) {
		array_of_fighter[team][number].randomBonusArmor();
	}

	/*Учет инициативы бойцов*/
	void setInitiativeArray() {
		// Запись в общий массив
		for (int team = 0; team < 2; team++) {
			for (int i = 0; i < M; i++) {
				all_fighter_array[i + team * M] = array_of_fighter[team][i];
			}
		}
		// Сортировка
		bubbleSort();
		// Вывод по порядку
		string type_of_fighter[3] = { "Воин", "Лучник", "Маг" };
		cout << "\n\n" << underline << "                     Порядок ходов                          " << underlineoff;
		cout << "\n" << underline << "       | Team |  Type  |Health|Armor|Damage|Initiative|Step|" << underlineoff;
		for (int i = 0; i < M * 2; i++) {
			cout << "\n" << underline << "Боец " << setw(2) << all_fighter_array[i].number << "| ";
			cout << setw(5) << all_fighter_array[i].team << "|";
			cout << setw(8) << type_of_fighter[fighter_type[all_fighter_array[i].team - 1][all_fighter_array[i].number -1]] << "|";
			all_fighter_array[i].printCharacteristics();
			cout << underlineoff;
		}
	}
	
	/*Сортировка по инициативе*/
	void bubbleSort()
	{
		int count = M * 2; // Количество бойцов
		while (count--)
		{
			bool swapped = false;
			for (int i = 0; i < count; i++)
			{
				if (all_fighter_array[i].initiative < all_fighter_array[i + 1].initiative)
				{
					swap(all_fighter_array[i], all_fighter_array[i + 1]);
					swapped = true;
				}
			}
			if (swapped == false) break;
		}
	}

	/*Карта*/
	void Map() {
		setPosition(0); // Расстановка команды 1
		setPosition(1); // Расстановка команды 2
		printPole(); // Вывод поля
	}

	/*Расстановка команд*/
	void setPosition(int team) {
		int side = 0;
		if (team == 1) side = N - 1;
		for (int i = 0; i < M; i++) {
			do {
				array_of_fighter[team][i].randomCoordinate(N, side);
			} while (checkPosition(array_of_fighter[team][i].x, array_of_fighter[team][i].y));
			polePositionChange(array_of_fighter[team][i].x, side);
		}
	}

	/*Расположение бойца на поле*/
	void polePositionChange(int x, int side) {
		pole_array[x][side] = true;
	}

	/*Проверка на доступность позиции*/
	bool checkPosition(int x, int y) {
		return pole_array[x][y];
	}

	/*Вывод поля*/
	void printPole() {
		cout << "\n\n_";
		for (int i = 0; i < N; i++) {
			cout << "__";
		}
		for (int i = 0; i < N; i++) {
			cout << endl << underline << "|";
			for (int j = 0; j < N; j++) {
				if (pole_array[i][j])
					printTypeFighter(i, j);
				else
					cout << " |";
			}
			cout << underlineoff;
		}
		cout << underlineoff;
	}

	/*Вывод типа бойца*/
	void printTypeFighter(int i, int j) {
		int team_of_fighter = 0;
		for (int team = 0; team < 2; team++) {
			for (int number = 0; number < M; number++) {
				if (array_of_fighter[team][number].x == i && array_of_fighter[team][number].y == j) {
					team_of_fighter = team;
				}
			}
		}
		for (int number = 0; number < M; number++) {
			if (array_of_fighter[team_of_fighter][number].x == i) {
				cout << fighter_type[team_of_fighter][number] + 1 << "|";
				break;
			}
		}
	}

	/*Бой*/
	void battle() {
		int attack, type, enemy_number = 0;
		string type_of_fighter[3] = { "Воин", "Лучник", "Маг" };
		while (checkHealth()) { // Пока обе команды живы
			for (int i = 0; i < M * 2; i++) {

				// Проверка на количество здоровья у бойца
				if (deleteDeadman(all_fighter_array[i].team - 1, all_fighter_array[i].number - 1)) {
					all_fighter_array[i].health = 0;
					continue;
				}

				type = fighter_type[all_fighter_array[i].team - 1][all_fighter_array[i].number - 1];
				enemy_number = searchEnemy(all_fighter_array[i].team - 1, all_fighter_array[i].number - 1);

				// Проверка на количество здоровья у врага
				if (enemyHealth(all_fighter_array[i].team - 1, enemy_number) <= 0) {
					continue;
				}

				// Проверка на жизни у команд
				if (!checkHealth()) {
					break;
				}

				// Вывод бойца
				cout << "\n\n" << type_of_fighter[type] << " " << all_fighter_array[i].team <<
					"\nx: " << array_of_fighter[all_fighter_array[i].team - 1][all_fighter_array[i].number - 1].x <<
					" y: " << array_of_fighter[all_fighter_array[i].team - 1][all_fighter_array[i].number - 1].y;
				// Вывод врага
				cout << "\nПротив";
				cout << "\n" << type_of_fighter[fighter_type[all_fighter_array[i].team - 1][enemy_number]] << " " << 3 - all_fighter_array[i].team <<
					"\nx: " << array_of_fighter[2 - all_fighter_array[i].team][enemy_number].x <<
					" y: " << array_of_fighter[2 - all_fighter_array[i].team][enemy_number].y;
				// Дальность атаки
				attack = getAttack(type);
				// Проверка дистанции
				checkDistance(all_fighter_array[i].team - 1, all_fighter_array[i].number - 1, enemy_number, attack, type);
				// Проверка на количество здоровья у врага
				if (deleteDeadman(2 - all_fighter_array[i].team, enemy_number)) {
					all_fighter_array[i].health = 0;
					
				}
				// Вывод поля
				printPole();
				// Вывод характеристик
				setInitiativeArray();
			}
		}
	}

	/*Радиус атаки мага
	* 
	* @param n2, m2, n1, m1 края поля 3х3
	*/
	void radiusDamage(Wizard &wizard, int x, int y, int number) {
		int n2 = x + wizard.radiusAttack + 1, m2 = y + wizard.radiusAttack + 1, n1 = x - wizard.radiusAttack, m1 = y - wizard.radiusAttack;
		if (n2 > N) n2 = N; if (m2 > N) m2 = N;
		if (n1 < 0) n1 = 0; if (m1 < 0) m1 = 0;
		// Поле 3х3 от координаты удара
		for (int i = n1; i < n2; i++) {
			for (int j = m1; j < m2; j++) {
				if (pole_array[i][j]) {
					for (int team = 0; team < 2; team++) {
						for (int enemy_number = 0; enemy_number < M; enemy_number++) {
							if (array_of_fighter[team][enemy_number].x == i && array_of_fighter[team][enemy_number].y == j) {
								wizard.methodAttack(array_of_fighter, team, enemy_number, number); // урон
								enemyHealth(team, enemy_number);
								// Проверка здоровья врага
								if (array_of_fighter[team][enemy_number].health <= 0) {
									pole_array[array_of_fighter[team][enemy_number].x][array_of_fighter[team][enemy_number].y] = false;
									array_of_fighter[team][enemy_number].health = 0;
								}
							}
						}
					}
				}
			}
		}
	}

	/*Получение дальности атаки*/
	int getAttack(int type) {
		int attack = 0;
		// Воин
		if (type == 0) {
			Warrior warrior;
			attack = warrior.attack;
		}
		// Лучник
		else if (type == 1) {
			Archer archer;
			attack = archer.attack;
		}
		// Маг
		else {
			Wizard wizard;
			attack = wizard.attack;
		}
		return attack;
	}

	/*Удаление погибших*/
	bool deleteDeadman(int team, int number) {
		if (array_of_fighter[team][number].health <= 0) {
			pole_array[array_of_fighter[team][number].x][array_of_fighter[team][number].y] = false;
			array_of_fighter[team][number].health = 0;
			return 1;
		}
		else return 0;
	}

	/*Проверка дистанции*/
	void checkDistance(int team, int number, int enemy_number, int attack, int type) {
		int to_x = groxthX(team, number, enemy_number); // Направление движения
		int to_y = groxthY(team, number, enemy_number);
		int new_x = array_of_fighter[team][number].x; // Новая позиция
		int new_y = array_of_fighter[team][number].y;
		int old_x = array_of_fighter[team][number].x; // Старая позиция
		int old_y = array_of_fighter[team][number].y;
		int count = 0; // Количество сделанных шагов
		//int distance = searchDistance(team, number, enemy_number);
		while (searchDistance(team, number, enemy_number) >= attack && array_of_fighter[team][number].step > 0) {
			// Если еще не дошел по у
			if (abs(array_of_fighter[1 - team][enemy_number].y - array_of_fighter[team][number].y) >= 1) {
				if (checkPosition(new_x, new_y + to_y)) { // Если позиция занята
					break;
				}
				new_y += to_y; // шаг
				array_of_fighter[team][number].step--;
				count++;
			}
			else break;
		}
		while (searchDistance(team, number, enemy_number) >= attack && array_of_fighter[team][number].step > 0) {
			// Если еще не дошел по х
			if (abs(array_of_fighter[1 - team][enemy_number].x - array_of_fighter[team][number].x) >= 1) {
				if (checkPosition(new_x + to_x, new_y)) { // Если позиция занята
					break;
				}
				new_x += to_x; // шаг
				array_of_fighter[team][number].step--;
				count++;
			}
			else break;			
		}
		if (count > 0) { // Если были шаги
			array_of_fighter[team][number].x = new_x; // Перезапись позиции
			array_of_fighter[team][number].y = new_y;
			polePositionChange(new_x, new_y);
			pole_array[old_x][old_y] = false; // Удаление старой позиции
		}
		else {
			kindsAttack(team, enemy_number, number, type); // Атака
		}
	}

	/*Направление по х*/
	int groxthX(int team, int number, int enemy_number) {
		if (array_of_fighter[team][number].x < array_of_fighter[1 - team][enemy_number].x)
			return 1;
		else if (array_of_fighter[team][number].x == array_of_fighter[1 - team][enemy_number].x)
			return 0;
		else 
			return -1;
	}

	/*Направление по у*/
	int groxthY(int team, int number, int enemy_number) {
		if (array_of_fighter[team][number].y < array_of_fighter[1 - team][enemy_number].y)
			return 1;
		else if (array_of_fighter[team][number].y == array_of_fighter[1 - team][enemy_number].y)
			return 0;
		else
			return -1;
	}

	/*Атака*/
	void kindsAttack(int team, int enemy_number, int number, int type) {
		// Воин
		if (type == 0) {
			Warrior warrior;
			warrior.randomSteps(array_of_fighter, 1 - team, number);
			warrior.methodAttack(array_of_fighter, team, enemy_number, number); // Атака врага
		}
		// Лучгик
		else if (type == 1) {
			Archer archer;
			archer.randomSteps(array_of_fighter, 1 - team, number);
			archer.methodAttack(array_of_fighter, team, enemy_number, number); // Атака врага
		}
		// Маг
		else {
			Wizard wizard;
			wizard.randomSteps(array_of_fighter, 1 - team, number);
			radiusDamage(wizard, array_of_fighter[team][enemy_number].x, array_of_fighter[team][enemy_number].y, number); // Атака врага с учетом радиуса
		}
	}

	/*Проверка здоровья команд
	* 
	* @return проверяет существует ли команды , у которй не осталось бойцов
	*/
	bool checkHealth() {
		bool result[2] = { false, false };
		for (int team = 0; team < 2; team++) {
			for (int i = 0; i < M; i++) {
				if (array_of_fighter[team][i].health > 0) {
					result[team] = true;
					break;
				}
			}
		}
		return (result[0] && result[1]);
	}

	/*Поиск ближнего врага
	* 
	* @param distance дистанция
	* @param min_distance минимальная дистанция
	* @param enemy_number номер врага
	* @return возвращает номер врага
	*/
	int searchEnemy(int team, int number) {
		int distance = 0, min_distance = 0, enemy_number = 0;
		// Любой живой враг
		for (int i = 0; i < M; i++) {
			if (enemyHealth(team, i) > 0) {
				enemy_number = i;
				distance = searchDistance(team, number, enemy_number);
				min_distance = distance;
				break;
			}
		}
		// Ближайший живой враг 
		for (int i = 0; i < M; i++) {
			distance = searchDistance(team, number, i);
			if (enemyHealth(team, i) > 0) {
				if (distance < min_distance) {
					min_distance = distance;
					enemy_number = i;
				}
			}
		}
		return enemy_number;
	}

	/*Поиск дистанции до врага*/
	int searchDistance(int team, int number, int enemy_number) {
		return abs(array_of_fighter[team][number].x - array_of_fighter[1 - team][enemy_number].x)
			+ abs(array_of_fighter[team][number].y - array_of_fighter[1 - team][enemy_number].y) - 1;
	}

	/*Здоровье врага*/
	int enemyHealth(int team, int enemy_number) {
		return array_of_fighter[1 - team][enemy_number].health;
	}

	/*Вывод команды победителей*/
	void printWinner() {
		for (int team = 0; team < 2; team++) {
			for (int number = 0; number < M; number++) {
				if (array_of_fighter[team][number].health > 0) {
					cout << "\n\nПобедила команда №" << team + 1 << "\n\n";
					break;
				}
			}
		}
	}

	/*Начало игры*/
	void startGame() {
		battle(); // Битва
		printPole(); // Вывод поля
		printWinner(); // Вывод победителей
	}

	/*Выбор пользователя*/
	void selector() {
		int key;
		cout << "\nВведите ключ:\n[0] - Начать бой\n[1] - Закрыть программу\n";
		cin >> key;
		switch (key)
		{
		// Начать игру
		case 0:
			startGame();
			break;
		// Закрыть программу
		case 1:
			deleteArrays();
			exit(0);
			break;
		default:
			cout << "\nНеверно введен ключ";
			selector();
			break;
		}
	}

	/*Удаление динамических массивов*/
	void deleteArrays() {
		for (int i = 0; i < N; i++) {
			delete[] pole_array[i];
		}
		delete[] pole_array;
		for (int i = 0; i < 2; i++) {
			delete[] array_of_fighter[i];
			delete[] fighter_type[i];
		}
		delete[] all_fighter_array;
	}
};

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Russian"); // Руссификатор
	Fight fight;
	cout << underline;
	fight.printTeams(); // Вывод команд
	cout << underlineoff; 
	fight.setInitiativeArray(); // Расстановка инициатив
	fight.Map(); // Карта
	fight.selector(); // Выбор пользователя
	fight.deleteArrays(); // Очищение динамических массивов
	return 0;
}
