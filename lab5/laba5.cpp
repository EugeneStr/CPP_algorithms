#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>

using namespace std;

/* Класс магов */
class Wizard {
public:
	double health = 100; // Здоровье
	int mana = 1000; // Мана
	int wizard_distance; // Дистанция мага
	int* spells; // Его возможные заклинания
	string otchet; // Сохраняющийся отчет
	int status_lasting[4] = { 0,0,0,0 }; // Длительность статусов
	bool type_of_status[4] = {false,false, false, false}; // Статусы

	int M = 0; // Количество заклинаний у мага

	Wizard() {
		wizard_distance = 1 + rand() % 51; // Генерация дистанции
		M = 1 + rand() % 10;
		spells = new int[M];
		randomSpell(); // Генерация типов заклинаний
	}

	/* Вывод характеристик мага
	* 
	* @param health Здоровье
	* @param mana Мана
	* @param M Количество заклинаний
	* @param spells Виды заклинаний
	* @param type_of_status Виды статусов
	*/
	void printWizardCharacteristics(bool* type_of_status) {
		string type_spells[4] = { "Дальнее", "Ближнее", "Хил", "Проклятие" };
		string statuses[4] = { "Нокаут ", "Ярость ", "Слепота ", "Бонусхил " };
		cout << "\nhealth: " << health <<
			"\nmana: " << mana << "\nspells: ";
		for (int i = 0; i < M; i++) {
			if (spells[i] > 0)
			 cout << type_spells[spells[i] - 1] << " ";
		}
		
		for (int i = 0; i < 4; i++) {
			if (type_of_status[i]) cout << "\nstatus: " + statuses[i] + " ";
		}
	}

	/* 2 рандомных заклинания
	*
	* @param spell_1, spell_2 Виды заклинаний
	*/
	void randomSpell() {
		int spell_1 = 1 + rand() % 4;
		int spell_2 = 1 + rand() % 4;
		while (spell_2 == spell_1) {
			spell_2 = 1 + rand() % 4;
		}
		for (int i = 0; i < M; i++) {
			spells[i] = (rand() % 2 == 0) ? spell_1 : spell_2;
		}
	}

	/* Случайные значения характеристик
	*
	* @return возвращает характеристику
	*/
	int randomCharacteristic(int a, int b) {
		return a + rand() % (b - a + 1);
	}

	/* Проверяет попал ли маг во врага
	*
	* @return возвращает результат удара
	*/
	bool successDamage(int character_1) {
		return wizard_distance > character_1;
	}

	/* Проверяет сработал ли статус
	*
	* @param type_of_status тип статуса
	* @param chance шанс выпадения статуса
	* @param status_lasting количество раундов, на которых действует статус
	*/
	void Success(double chance, int i, int rounds) {
		type_of_status[i] = (((rand() % 100) / 100.0) < chance);
		status_lasting[i] = rounds;
	}

	/* Количество заклинаний
	*
	* @return возвращает количество заклинаний у мага
	*/
	int countOfArray() {
		return M;
	}

	/* Смена статуса */
	void changeStatus() {
		for (int i = 0; i < 4; i++) {
			if (status_lasting[i] == 0) type_of_status[i] = false;
			else status_lasting[i]--;
		}
	};
};

/* Класс заклинаний */
class Spell {
private:
	int mana_price; // Цена
	int difficult; // Сложность
	int time; // Время каста
	int distance; // Дистанция действия
	int lasting_rounds; // Длительность на раундах
	int mass; // Массовость
	int damage; // Дамаг

public:
	Spell(int distance, int damage) : distance(distance), damage(damage) {}

	/* Вывод характеристик */
	void printCharacteristics() {
		cout << "mana_price: " << mana_price <<
			"\ncomplexity: " << difficult <<
			"\nlasting: " << time <<
			"\nlasting_rounds: " << lasting_rounds <<
			"\nmass: " << mass << endl;
	}

	/* Ввод характеристик */
	void setCharacteristics() {
		mana_price = 1 + rand() % 100;
		difficult = 1 + rand() % 10;
		time = 1 + rand() % 3600;
		lasting_rounds = 1 + rand() % 10;
		mass = 1 + rand() % 3;
	}

	/* Возвращение дамага */
	int getDamage() {
		return damage;
	}

	/* Возвращение дистанции */
	int getDistance() {
		return distance;
	}

	/* Возвращение длительности */
	int getLastingRounds() {
		return lasting_rounds;
	}

	/* Изменение длительности */
	void setLastingRounds() {
		lasting_rounds = 1;
	}

	/* Возвращение цены */
	int getManaPrice() {
		return mana_price;
	}
};

/* Класс дальнобойного заклинания */
class LongRange : public Spell {
public:
	double chance = 0.6; // шанс статуса

	LongRange(int distance, int damage) :Spell(distance, damage) {}

	/* Вывод характеристик */
	void printLongRangeCharacteristics() {
		cout << "\n-----LongRang-----\n";
		printCharacteristics();
		cout << "distance: " << getDistance() <<
			"\ndamage: " << getDamage() << endl;
	}
};

/* Класс заклинания ближнего боя */
class ShortRange : public Spell {
public:
	double chance1 = 0.7, chance2 = 0.3; // шанс статуса

	ShortRange(int distance, int damage) :Spell(distance, damage) {}

	/* Вывод характеристик */
	void printShortRangeCharacteristics() {
		cout << "\n-----ShortRang-----\n";
		printCharacteristics();
		cout << "distance: " << getDistance() <<
			"\ndamage: " << getDamage() << endl;
	}
};

/* Класс хила */
class Heal : public Spell {
public:
	double chance = 0.5; // шанс статуса

	Heal(int distance, int damage) :Spell(distance, damage) {}

	/* Вывод характеристик */
	void printHealCharacteristics() {
		cout << "\n-----Heal-----\n";
		printCharacteristics();
		cout << "distance: " << getDistance() <<
			"\ndamage: " << getDamage() << endl;
	}
};

/* Класс проклятия */
class Curse : public Spell {
public:
	Curse(int distance, int damage) :Spell(distance, damage) {}

	/* Вывод характеристик */
	void printCurseCharacteristics() {
		cout << "\n-----Curse-----\n";
		printCharacteristics();
		cout << "distance: " << getDistance() <<
			"\ndamage: " << getDamage() << endl;
	}
};

/* Класс боя */
class Fight {
public:
	Wizard** array_of_wizards = new Wizard * [2]; // Массив команд магов
	int N; // Количество магов в каждой команде
	int countRounds; // Количество раундов

	Fight() {
		getN(); // Получение количества магов
		array_of_wizards[0] = new Wizard[N]; // Выделение памяти под каждую команду
		array_of_wizards[1] = new Wizard[N];
		getCountRounds(); // Получение количества раундов
	}

	/* Расчет дамага
	* 
	* @param all_damage дамаг с учетом всех статусов
	* @return возращает суммарный дамаг
	*/
	double apply(bool* status, int damage) {
		double all_damage = damage;
		if (status[1]) all_damage *= 2;
		if (status[2]) all_damage /= 2;
		if (status[3]) all_damage *= 2;
		return all_damage;
	}

	/* Ввод количества магов в командах */
	void getN() {
		cout << "Введите количество магов: ";
		cin >> N;
	}

	/* Ввод количества раундов */
	void getCountRounds() {
		cout << "Введите количество раундов: ";
		cin >> countRounds;
	}

	/* Проверка здоровья мага */
	void endHealth(int i, int j) {
		if (array_of_wizards[i][j].health <= 0) {
			array_of_wizards[i][j].type_of_status[0] = 1;
			array_of_wizards[i][j].health = 0;
		}
	}

	/* Выбор заклинания для каста
	* 
	* @param spell заклинание
	* @param arrayLenght количество возможных заклинаний
	* @return возвращает вид заклинания
	*/
	int castSpell(int round, int i, int j) {
		int spell, arrayLenght;
		arrayLenght = array_of_wizards[i][j].countOfArray(); // количество заклинаний
		if (arrayLenght < round) spell = -1; // Если заклинаний не осталось
		else spell = array_of_wizards[i][j].spells[round] - 1;
		return spell;
	}

	/* Проверка на нокаут
	*
	* @return возвращает актуальность статуса
	*/
	bool knockoutStatus(int i, int j, int round) {
		if (array_of_wizards[i][j].type_of_status[0]) { // Если маг в нокауте
			cout << "\nПропуск хода\n";
			array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик
		}
		return array_of_wizards[i][j].type_of_status[0];
	}

	/* Каст заклинания 
	* 
	* @param character_1, character_2 дистанция и дамаг
	* @param dam общий дамаг
	* @param enemy враг
	* @param sp заклинание
	*/
	void Spell(int round, int i) {
		int character_1, character_2, dam, enemy, sp;
		for (int j = 0; j < N; j++) { // порядковый номер мага
			printHeader(round + 1, i + 1, j + 1); // Вывод шапки раунда
			array_of_wizards[i][j].changeStatus(); // Смена статусов
			endHealth(i, j); // Проверка здоровья мага
			dam = 0; 
			sp = castSpell(round, i, j); // Выбор заклинания
			if (knockoutStatus(i, j, round)) { // Проверка на нокаут
				otchet(round, i, j); // Запись в отчет
				continue;
			}
			// Дальнобойное заклинание
			if (sp == 0) {
				character_1 = array_of_wizards[i][j].randomCharacteristic(50, 100); // Дистанция
				character_2 = array_of_wizards[i][j].randomCharacteristic(5, 10); // Дамаг
				if (array_of_wizards[i][j].successDamage(character_1)) character_2 = 0; // Проверка на попадание
				LongRange long_range(character_1, character_2);
				long_range.setCharacteristics(); // Ввод характеристик
				if (array_of_wizards[i][j].mana < long_range.getManaPrice()) { // Проверка на количество маны
					array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); 
					cout << "\nНет маны\n";
					continue;
				}
				long_range.setLastingRounds(); // Изменение длительности статуса 
				enemy = rand() % N; // Выбор врага
				array_of_wizards[1 - i][enemy].Success(long_range.chance, 0, long_range.getLastingRounds()); // Проверка на получение статуса
				otchet(round, i, j); // Запись в отчет
				dam = apply(array_of_wizards[i][j].type_of_status, long_range.getDamage()); // Подсчет дамага
				array_of_wizards[1 - i][enemy].health -= dam; // Дамаг
				array_of_wizards[i][j].mana -= long_range.getManaPrice(); // Затрата маны
				long_range.printLongRangeCharacteristics(); // Вывод характеристик заклинания
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик мага
			}
			// Заклинание ближнего боя
			else if (sp == 1) {
				character_1 = array_of_wizards[i][j].randomCharacteristic(1, 10); // Дистанция
				character_2 = array_of_wizards[i][j].randomCharacteristic(5, 10); // Дамаг
				if (array_of_wizards[i][j].successDamage(character_1)) character_2 = 0; // Проверка на попадание
				ShortRange short_range(character_1, character_2); 
				short_range.setCharacteristics(); // Ввод характеристик
				if (array_of_wizards[i][j].mana < short_range.getManaPrice()) { // Проверка на количество маны
					array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status);
					cout << "\nНет маны\n";
					continue;
				}
				enemy = rand() % N; // Выбор врага
				array_of_wizards[i][j].Success(short_range.chance1, 1, short_range.getLastingRounds()); // Проверка на получение статуса
				array_of_wizards[i][j].Success(short_range.chance2, 2, short_range.getLastingRounds());
				otchet(round, i, j); // Запись в отчет
				dam = apply(array_of_wizards[i][j].type_of_status, short_range.getDamage()); // Подсчет дамага
				array_of_wizards[1 - i][enemy].health -= dam; // Дамаг
				array_of_wizards[i][j].mana -= short_range.getManaPrice(); // Затрата маны
				short_range.printShortRangeCharacteristics(); // Вывод характеристик заклинания
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик мага
			}
			// Заклинание хила
			else if (sp == 2) {
				character_1 = array_of_wizards[i][j].randomCharacteristic(50, 100); // Дистанция
				character_2 = array_of_wizards[i][j].randomCharacteristic(5, 10); // Дамаг
				if (array_of_wizards[i][j].successDamage(character_1)) character_2 = 0; // Проверка на попадание
				Heal heal(character_1, character_2);
				heal.setCharacteristics(); // Ввод характеристик
				if (array_of_wizards[i][j].mana < heal.getManaPrice()) { // Проверка на количество маны
					array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status);
					cout << "\nНет маны\n";
					continue;
				}
				heal.setLastingRounds(); // Изменение длительности статуса 
				array_of_wizards[i][j].Success(heal.chance, 3, heal.getLastingRounds()); // Проверка на получение статуса
				otchet(round, i, j); // Запись в отчет
				dam = apply(array_of_wizards[i][j].type_of_status, heal.getDamage()); // Подсчет хила
				array_of_wizards[i][rand() % N].health += dam; // Хил
				array_of_wizards[i][j].mana -= heal.getManaPrice(); // Затрата маны
				if (array_of_wizards[i][j].health >= 100) array_of_wizards[i][j].health = 100; // Проверка на макимальное здоровье
				heal.printHealCharacteristics(); // Вывод характеристик заклинания
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик мага
			}
			// Заклинание проклятия
			else if (sp == 3) {
				character_1 = array_of_wizards[i][j].randomCharacteristic(50, 100); // Дистанция
				character_2 = array_of_wizards[i][j].randomCharacteristic(5, 10); // Дамаг
				if (array_of_wizards[i][j].successDamage(character_1)) character_2 = 0; // Проверка на попадание
				Curse curse(character_1, character_2);
				curse.setCharacteristics(); // Ввод характеристик
				if (array_of_wizards[i][j].mana < curse.getManaPrice()) { // Проверка на количество маны
					array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status);
					cout << "\nНет маны\n";
					continue;
				}
				array_of_wizards[i][j].Success(0, 0, curse.getLastingRounds());
				otchet(round, i, j); // Запись в отчет
				dam = curse.getDamage(); // Подсчет хила
				enemy = rand() % N; // Выбор врага
				array_of_wizards[1 - i][enemy].health -= dam; // Дамаг
				array_of_wizards[i][j].mana -= curse.getManaPrice(); // Затрата маны
				curse.printCurseCharacteristics();; // Вывод характеристик заклинания
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик мага
			}
			else {
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status); // Вывод характеристик мага
				cout << "\nЗакончились заклинания\n";
				array_of_wizards[i][j].spells[round] = -1;
				otchet(round, i, j); // Запись в отчет
				continue;

			}
		}
	}

	/* Вывод команды и мага */
	void printTeams() {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < N; j++) {
				cout << "\n------Команда " << i + 1 << "------";
				cout << "\n---Маг " << j + 1 << "---";
				array_of_wizards[i][j].printWizardCharacteristics(array_of_wizards[i][j].type_of_status);
			}
		}
	}

	/* Запись  в отчет */
	void otchet(int round, int team, int wizard) {
		array_of_wizards[team][wizard].otchet += "\n---\nRound: " + to_string(round + 1) + "\nTeam: " + to_string(team + 1)
			+ "\nWizard: " + to_string(wizard + 1) + "\nSpell: " + spellsName(round);
		for (int i = 0; i < 4; i++) {
			array_of_wizards[team][wizard].otchet += statusName(team, wizard, i);
		}
	}

	/* Вывод отчета */
	void printOtchet() {
		int key;
		cout << "\n\n[0] - Вывести отчет\n[1] - Закрыть программу\n";
		cin >> key;
		switch (key)
		{
		case 0:
			cout << "\n\n---------ОТЧЕТ---------";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < N; j++) {
					cout << array_of_wizards[i][j].otchet << endl;
				}
			}
			break;
		case 1:
			exit(0);
			break;
		default:
			cout << "Неверный ключ";
			printOtchet();
			break;
		}
		
	}

	/* Очищение массивов */
	void deleteArrays() {
		for (int i = 0; i < 2; i++) {
			delete[] array_of_wizards[i]->spells;
		}
		delete[] array_of_wizards;
	}

	/* Вывод шапки раундов */
	void printHeader(int round, int team, int wizard) {
		cout << "\n\n---------Раунд " << round << "---------";
		cout << "\n------Команда " << team << "------";
		cout << "\n---Маг " << wizard << "---";
	}

	/* Названия заклинаний */
	string spellsName(int i) {
		string spells[4] = { "Дальнобойное", "Ближнее", "Хил", "Проклятие" };
		return spells[i];
	}

	/* Названия статусов */
	string statusName(int team, int wizard, int i) {
		string statuses[4] = { "Нокаут ", "Ярость ", "Слепота ", "Бонус хил " };
		if (array_of_wizards[team][wizard].type_of_status[i]) return  "\nStatus: " + statuses[i];
		else return "";
	}
};

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Russian"); // Руссификатор
	Fight fight;
	fight.printTeams(); // Вывод команд
	for (int round = 0; round < fight.countRounds; round++) {
		for (int i = 0; i < 2; i++) {
			fight.Spell(round, i); // Раунды
		}
	}
	fight.printOtchet(); // Вывод отчета
	fight.deleteArrays(); // Очищение массивов
	return 0;
}
