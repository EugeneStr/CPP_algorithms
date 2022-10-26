/*
Вариант 1
1. Создайте классы Шестиугольник, Прямоугольник, Круг, которые хранят свое описание и положение на сцене. 
	Пусть каждый из них хранит необходимое количество переменных для описания фигуры на сцене.

2. Создайте класс Фигура, который будет хранить указатель на пустоту, за которым будет скрываться фигура, а так же ее тип.

3. Создайте класс сцены с параметрами , которы будет хранить в себе массив фигур.

4. Заполните сцену случайными фигурами в случайном положении так, в количестве введенном пользователем, чтобы они
	не пересекались друг с другом. (Если новую фигуру добавить не получится в течении 100 раз, остановить заполнение
	и считать, что больше фигур поместить не получается, вывести предупреждение в консоль)

5. Создайте метод, который подсчитает суммарную площадь фигур на сцене.

6. (Усложненный) Смоделируйте постоянное движение фигур по сцене в течении 10 минут, с шагом 0.01 секунду.
	Скорости определяются случайно и не меняются. Направление движения определяется случайно в начале и меняется
	в зависимости от столкновения. Столкновения фигур со стенкой и друг другом оценивать по описанной вокруг фигуры
	окружности.
*/

#include <iostream>
#include <math.h>
#include <iomanip>
#define PI 3.14159265 // Число ПИ

using namespace std;

/*
 * Класс с координатами фигуры
 *
 * @param x координата x
 * @param y координата y
 */
class Position {
public:
	double x;
    double y;

    /*
    * Генерация случайной координаты
    * 
    * @param a нижняя граница
    * @param b верхняя граница
    * @return возвращает рандомную координату
    */
    double randomCoordinate(double a, double b) {
        return rand()* (b - a) / RAND_MAX + a;
    }
};

/*
 * Класс с фигурами
 *
 * @param figure пустой указатель на фигуру
 * @param outer_radius радиус описанной окружности
 * @param type_of_figure тип фигуры
 * @param way направление движения
 */
class Figure {
public:
	void* figure;
    double outer_radius;
	int type_of_figure;
    int way;

    /*
    * Изменение направления движения
    *
    * @param way направление движения
    */
    void changeWay() {
        if (way < 4) way += 4;
        else way -= 4;
    }
};

/*
 * Класс сцены
 *
 * @param N количество фигур
 * @param width ширина сцены
 * @param height высота сцены
 * @param figures_array массив фигур
 */
class Scene {
public:
	int N;
	int width = 50, height = 50;
	Figure* figures_array;

    /*
    * Конструктор сцены
    * 
    * Запрашивает у пользователя N
    * Создает динамический массив
    */
	Scene () {
		cout << "Введите количество фигур: ";
		cin >> this->N;
		figures_array = new Figure[N];
	}
};

/*
 * Класс шестиугольника
 *
 * @param side_hexagon сторона шестиугольника
 */
class Hexagon : public Position {
public:
	int side_hexagon;

    /*
    * Генерация стороны шестиугольника
    *
    * @param side_hexagon сторона шестиугольника
    */
    void hexagonSide() {
        side_hexagon = 1 + rand() % 10;
    }

    /*
    * Вывод характеристик шестиугольника
    *
    * @param side_hexagon сторона шестиугольника
    */
    void hexagonCout() {
        cout << "(a = " << side_hexagon << ", R = ";
    }

    /*
    * Площадь шестиугольника
    *
    * @param a сторона шестиугольника
    * @return возвращает площадь шестиугольника
    */
    double hexagonArea(int a) {
        return 3 * sqrt(3) * a / 2;
    }
};

/*
 * Класс прямоугольника
 *
 * @param side1_rectangle сторона прямоугольника
 * @param side2_rectangle сторона прямоугольника
 */
class Rectangle : public Position {
public:
	int side1_rectangle;
	int side2_rectangle;

    /*
    * Генерация сторон прямоугольника
    *
    * @param side1_rectangle сторона прямоугольника  
    * @param side2_rectangle сторона прямоугольника
    */
    void rectangleSides() {
        side1_rectangle = 1 + rand() % 10;
        side2_rectangle = 1 + rand() % 10;
    }

    /*
    * Вывод характеристик прямоугольника
    *
    * @param side1_rectangle сторона прямоугольника
    * @param side2_rectangle сторона прямоугольника
    */
    void rectangleCout() {
        cout << "(a = " << side1_rectangle << ", b = " << side2_rectangle << ", R = ";
    }

    /*
    * Площадь прямоугольника
    *
    * @param a сторона прямоугольника
    * @param b сторона прямоугольника
    * @return возвращает площадь прямоугольника
    */
    double rectangleArea(int a, int b) {
        return a * b;
    }
};

/*
 * Класс круга
 *
 * @param radius_circle радиус круга
 */
class Circle : public Position {
public:
	int radius_circle;

    /*
    * Генерация радиуса круга
    *
    * @param radius_circle радиус круга
    */
    void circleRadius() {
        radius_circle = 1 + rand() % 10;
    }

    /*
    * Вывод характеристик круга
    *
    * @param radius_circle радиус круга
    */
    void circleCout() {
        cout << "(r = " << radius_circle << ", R = ";
    }

    /*
    * Площадь круга
    *
    * @param r радиус круга
    * @return возвращает площадь круга
    */
    double circleArea(int r) {
        return r*r*PI;
    }
};

double rectangleRadius(int a, int b); // Радиус описанной окружности прямоугольника 
int generationHex(Scene scene, Hexagon* hex, int i); // Генерация координат шестиугольника
int generationRect(Scene scene, Rectangle* rect, int i); // Генерация координат прямоугольника
int generationCirc(Scene scene, Circle* circ, int i); // Генерация координат круга
double findingTempX(Scene scene, int j); // Нахождение координаты x заданной фигуры
double findingTempY(Scene scene, int j); // Нахождение координаты y заданной фигуры
bool coordinateTest(Scene scene, double temp_x, double temp_y, double x, double y, int j, int i); // Проверка на соприкосновение с фигурой
bool wallTest(Scene scene, int i); // Проверка координаты на соприкосновение со стенками
void moving(Scene scene, int time_of_moving); // Движение фигур
void moveFigure(int way, Scene scene, int i); // Передвижение одной фигуры
void growthX(Scene scene, int j, double h); // Передвижение по x
void growthY(Scene scene, int j, double h); // Передвижение по y
void inputCoordinate(double coord); // Вывод координат в консоль
void selectMove(Scene scene); // Запрос у пользователя о моделировании движения

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian"); // Руссификатор
    Scene scene;
    string figures[] = { "шестиугольник", "прямоугольник", "круг" }; // массив названий для вывода
    int type = 0, k, check = 0;
    double s = 0;

    // Создание фигур
    for (int i = 0; i < scene.N; i++) {
        k = 0;
        type = rand() % 3; // Генерация типа фигуры
        cout << figures[type] << " ";
        // шестиугольник
        if (type == 0) {
            Hexagon* hex = new Hexagon;
            hex->hexagonSide(); // сторона шестиугольника
            scene.figures_array[i].outer_radius = hex->side_hexagon; // радиус описанной окружности
            check = generationHex(scene, hex, i); // попытка расположить фигуру на сцене
            if (check > 99) break; // проверка количества итераций
            scene.figures_array[i].figure = static_cast<Hexagon*>(hex); // запись характеристик в массив
            s += hex->hexagonArea(hex->side_hexagon); // подсчет суммарной площади
            hex->hexagonCout(); // вывод информации о фигуре
        }
        // прямоугольник
        else if (type == 1) {
            Rectangle* rect = new Rectangle;
            rect->rectangleSides(); // стороны прямоугольника
            // радиус описанной окружности
            scene.figures_array[i].outer_radius = rectangleRadius(rect->side1_rectangle, rect->side2_rectangle);
            check = generationRect(scene, rect, i); // попытка расположить фигуру на сцене
            if (check > 99) break; // проверка количества итераций
            scene.figures_array[i].figure = static_cast<Rectangle*>(rect); // запись характеристик в массив
            s += rect->rectangleArea(rect->side1_rectangle, rect->side2_rectangle); // подсчет суммарной площади
            rect->rectangleCout(); // вывод информации о фигуре
        }
        // круг
        else {
            Circle* circ = new Circle;
            circ->circleRadius(); // радиус круга
            scene.figures_array[i].outer_radius = circ->radius_circle; // радиус описанной окружности
            check = generationCirc(scene, circ, i); // попытка расположить фигуру на сцене
            if (check > 99) break; // проверка количества итераций
            scene.figures_array[i].figure = static_cast<Circle*>(circ); // запись характеристик в массив
            s += circ->circleArea(circ->radius_circle); // подсчет суммарной площади
            circ->circleCout(); // вывод информации о фигуре
        }

        // вывод радиуса
        cout << fixed << setprecision(2) << scene.figures_array[i].outer_radius << ")\n";
    }

    cout << "\nСуммарная площадь: " << s; // вывод суммарной площади
    selectMove(scene); // запрос пользователю
    delete scene.figures_array; // очищение динамического массива
}

/*
* Радиус описанной окружности прямоугольника
* 
* @param a сторона прямоугольника
* @param b сторона прямоугольника
* @return возвращает радиус описанной окружности прямоугольника
*/
double rectangleRadius(int a, int b) {
    return sqrt(a * a + b * b) / 2;
}

/*
* Генерация координат шестиугольника
* 
* @param a нижняя граница допутимого расстояния до соприкосновения
* @param b верхняя граница допутимого расстояния до соприкосновения правой стенкой
* @param c верхняя граница допутимого расстояния до соприкосновения нижней стенкой
* @param k итерации
* @param j номер фигур, расставленных до проверяемой фигуры
* @param temp_x координата x прошлой фигуры
* @param temp_y координата y прошлой фигуры
* @param x координата x текущей фигуры
* @param y координата y текущей фигуры
* @return возвращает количество итераций
*/
int generationHex(Scene scene, Hexagon* hex, int i) {
    double a = scene.figures_array[i].outer_radius;
    double b = scene.width - a;
    double c = scene.height - a;
    int k = 0;
    int j = 0;
    double temp_x, temp_y;
    double x = hex->randomCoordinate(a,b); // генерация x
    double y = hex->randomCoordinate(a,c); // генерация y
    // цикл итераций
    while (k < 100 && j != i) { 
        // проверка созданных координат
        for (j = 0; j < i; j++) {
            temp_x = findingTempX(scene, j); // вытягивание x прошлой фигуры
            temp_y = findingTempY(scene, j); // вытягивание y прошлой фигуры
            if (coordinateTest(scene, temp_x, temp_y, x, y, j, i)) { // проверка на соприкосновение с фигурами
                k++;
                break;
            }
        }
        // генерация новых координат
        x = hex->randomCoordinate(a, b);
        y = hex->randomCoordinate(a, c);
    }
    if (k > 99) // проверка количества итераций
        cout << "не получилось вставить";
    else {
        // сохранение координат
        hex->x = x;
        hex->y = y;
    }
    return k;
}

/*
* Генерация координат прямоугольника
*
* @param a нижняя граница допутимого расстояния до соприкосновения
* @param b верхняя граница допутимого расстояния до соприкосновения правой стенкой
* @param c верхняя граница допутимого расстояния до соприкосновения нижней стенкой
* @param k итерации
* @param j номер фигур, расставленных до проверяемой фигуры
* @param temp_x координата x прошлой фигуры
* @param temp_y координата y прошлой фигуры
* @param x координата x текущей фигуры
* @param y координата y текущей фигуры
* @return возвращает количество итераций
*/
int generationRect(Scene scene, Rectangle* rect, int i) {
    double a = scene.figures_array[i].outer_radius;
    double b = scene.width - a;
    double c = scene.height - a;
    int k = 0;
    int j = 0;
    double temp_x, temp_y;
    double x = rect->randomCoordinate(a, b); // генерация x
    double y = rect->randomCoordinate(a, c); // генерация y
    // цикл итераций
    while (k < 100 && j != i) {
        // проверка созданных координат
        for (j = 0; j < i; j++) {
            temp_x = findingTempX(scene, j); // вытягивание x прошлой фигуры
            temp_y = findingTempY(scene, j); // вытягивание y прошлой фигуры
            if (coordinateTest(scene, temp_x, temp_y, x, y, j, i)) { // проверка на соприкосновение с фигурами
                k++;
                break;
            }
        }
        // генерация новых координат
        x = rect->randomCoordinate(a, b);
        y = rect->randomCoordinate(a, c);
    }
    if (k > 99) // проверка количества итераций
        cout << "не получилось вставить";
    else {
        // сохранение координат
        rect->x = x;
        rect->y = y;
    }
    return k;
}

/*
* Генерация координат круга
*
* @param a нижняя граница допутимого расстояния до соприкосновения
* @param b верхняя граница допутимого расстояния до соприкосновения правой стенкой
* @param c верхняя граница допутимого расстояния до соприкосновения нижней стенкой
* @param k итерации
* @param j номер фигур, расставленных до проверяемой фигуры
* @param temp_x координата x прошлой фигуры
* @param temp_y координата y прошлой фигуры
* @param x координата x текущей фигуры
* @param y координата y текущей фигуры
* @return возвращает количество итераций
*/
int generationCirc(Scene scene, Circle* circ, int i) {
    double a = scene.figures_array[i].outer_radius;
    double b = scene.width - a;
    double c = scene.height - a;
    int k = 0;
    int j = 0;
    double temp_x, temp_y;
    double x = circ->randomCoordinate(a, b); // генерация x
    double y = circ->randomCoordinate(a, c); // генерация y
    // цикл итераций
    while (k < 100 && j != i) {
        // проверка созданных координат
        for (j = 0; j < i; j++) {
            temp_x = findingTempX(scene, j); // вытягивание x прошлой фигуры
            temp_y = findingTempY(scene, j); // вытягивание y прошлой фигуры
            if (coordinateTest(scene, temp_x, temp_y, x, y, j, i)) { // проверка на соприкосновение с фигурами
                k++;
                break;
            }
        }
        // генерация новых координат
        x = circ->randomCoordinate(a, b);
        y = circ->randomCoordinate(a, c);
    }
    if (k > 99) // проверка количества итераций
        cout << "не получилось вставить";
    else {
        // сохранение координат
        circ->x = x;
        circ->y = y;
    }
    return k;
}

/*
* Нахождение координаты x заданной фигуры
* 
* @param temp_x координата x
* @return возвращает координату x
*/
double findingTempX(Scene scene, int j) {
    double temp_x;
    switch (scene.figures_array[j].type_of_figure)
    {
    case 0: // шестиугольник
        temp_x = static_cast<Hexagon*>(scene.figures_array[j].figure)->x;
        break;
    case 1: // прямоугольник
        temp_x = static_cast<Rectangle*>(scene.figures_array[j].figure)->x;
        break;
    default: // круг
        temp_x = static_cast<Circle*>(scene.figures_array[j].figure)->x;
        break;
    }
    return temp_x;
}

/*
* Нахождение координаты y заданной фигуры
*
* @param temp_y координата y
* @return возвращает координату y
*/
double findingTempY(Scene scene, int j) {
    double temp_y;
    switch (scene.figures_array[j].type_of_figure)
    {
    case 0: // шестиугольник
        temp_y = static_cast<Hexagon*>(scene.figures_array[j].figure)->y;
        break;
    case 1: // прямоугольник
        temp_y = static_cast<Rectangle*>(scene.figures_array[j].figure)->y;
        break;
    default: // круг
        temp_y = static_cast<Circle*>(scene.figures_array[j].figure)->y;
        break;
    }
    return temp_y;
}

/*
* Проверка на соприкосновение с фигурой
* 
* @return возвращает результат проверки на соприкосновение фигур
*/
bool coordinateTest(Scene scene, double temp_x, double temp_y, double x, double y, int j, int i) {
    return sqrt((x - temp_x) * (x - temp_x) + (y - temp_y) * (y - temp_y))
        < scene.figures_array[j].outer_radius + scene.figures_array[i].outer_radius;
}

/*
* Проверка координаты на соприкосновение со стенками
*
* @param r радиус описанной окружности фигуры
* @param x координата x текущей фигуры
* @param y координата y текущей фигуры
* @return возвращает результат проверки на соприкосновение со стенами
*/
bool wallTest(Scene scene, int i) {
    double x = 0, y = 0, r = 0;
    r = scene.figures_array[i].outer_radius;
    x = findingTempX(scene, i);
    y = findingTempY(scene, i);
    return ((r > x) || (r > y) || (r > scene.width - x) || (r > scene.height - y));
}

/*
* Движение фигур
* 
* @param x координата x текущей фигуры
* @param y координата y текущей фигуры
* @param temp_x координата temp_x другой фигуры
* @param temp_y координата temp_y другой фигуры
* @param i текущая фигура
* @param j другая фигура
*/
void moving(Scene scene, int time_of_moving) {
    double x = 0, y = 0, temp_x = 0, temp_y = 0;

    // Вывод шапки таблицы
    cout << "|   Time   ";
    for (int i = 0; i < scene.N; i++) {
        scene.figures_array[i].way = rand() % 8;
        cout << "| Way |   X   |   Y   ";
    }
    cout << "|\n===========";
    for (int i = 0; i < scene.N; i++) {
        cout << "======================";
    }
    cout << "=\n";
    // Цикл передвижения
    for (int min = 0; min < time_of_moving; min++) {
        for (int sec = 0; sec < 60; sec++) {
            for (int msec = 0; msec < 100; msec++) {
                printf("| %02d:%02d:%02d | ", min, sec, msec); // вывод времени
                // движение каждой фигуры
                for (int i = 0; i < scene.N; i++) {
                    printf(" %d  | ", scene.figures_array[i].way); // вывод направления
                    moveFigure(scene.figures_array[i].way, scene, i); // передвижение фигуры
                    // проверка на стену
                    if (wallTest(scene, i)) {
                        scene.figures_array[i].changeWay();
                    }
                    x = findingTempX(scene, i); // координата x текущей фигуры
                    y = findingTempY(scene, i); // координата y текущей фигуры
                    for (int j = 0; j < i; j++) {
                        temp_x = findingTempX(scene, j); // координата x другой фигуры
                        temp_y = findingTempY(scene, j); // координата y другой фигуры
                        // проверка на столкновение
                        if (coordinateTest(scene, temp_x, temp_y, x, y, j, i)) {
                            scene.figures_array[i].changeWay();
                            scene.figures_array[j].changeWay();
                            break;
                        }
                    }
                    
                }
                cout << endl;
            }
        }
    }
}

/*
* Передвижение одной фигуры
* 
* @param h скорость передвижения
* @param way направление движения
*/
void moveFigure(int way, Scene scene, int i) {
    double h = 0.01;
    switch (way)
    {
    case 0: // 🡩
        growthX(scene, i, 0);
        growthY(scene, i, -h);
        break;
    case 1: // 🡭
        growthX(scene, i, h);
        growthY(scene, i, -h);
        break;
    case 2: // 🡪
        growthX(scene, i, h);
        growthY(scene, i, 0);
        break;
    case 3: // 🡮
        growthX(scene, i, h);
        growthY(scene, i, h);
        break;
    case 4: // 🡫
        growthX(scene, i, 0);
        growthY(scene, i, h);
        break;
    case 5: // 🡯
        growthX(scene, i, -h);
        growthY(scene, i, h);
        break;
    case 6: // 🡨
        growthX(scene, i, -h);
        growthY(scene, i, 0);
        break;
    default: // 🡬
        growthX(scene, i, -h);
        growthY(scene, i, -h);
        break;
    }
}

/*
* Передвижение по x
* 
* @param h скорость движения
* @param j текущая фигура
*/
void growthX(Scene scene, int j, double h) {
    switch (scene.figures_array[j].type_of_figure)
    {
    case 0: // шестиугольник
        static_cast<Hexagon*>(scene.figures_array[j].figure)->x += h;
        inputCoordinate(static_cast<Hexagon*>(scene.figures_array[j].figure)->x);
        break;
    case 1: // прямоугольник
        static_cast<Rectangle*>(scene.figures_array[j].figure)->x += h;
        inputCoordinate(static_cast<Rectangle*>(scene.figures_array[j].figure)->x);
        break;
    default: // круг
        static_cast<Circle*>(scene.figures_array[j].figure)->x += h;
        inputCoordinate(static_cast<Circle*>(scene.figures_array[j].figure)->x);
        break;
    }
}

/*
* Передвижение по y
*
* @param h скорость движения
* @param j текущая фигура
*/
void growthY(Scene scene, int j, double h) {
    switch (scene.figures_array[j].type_of_figure)
    {
    case 0: // шестиугольник
        static_cast<Hexagon*>(scene.figures_array[j].figure)->y += h;
        inputCoordinate(static_cast<Hexagon*>(scene.figures_array[j].figure)->y);
        break;
    case 1: // прямоугольник
        static_cast<Rectangle*>(scene.figures_array[j].figure)->y += h;
        inputCoordinate(static_cast<Rectangle*>(scene.figures_array[j].figure)->y);
        break;
    default: // круг
        static_cast<Circle*>(scene.figures_array[j].figure)->y += h;
        inputCoordinate(static_cast<Circle*>(scene.figures_array[j].figure)->y);
        break;
    }
}

/*
* Вывод координат в консоль
* 
* @param coord координата
*/
void inputCoordinate(double coord) {
    cout << setw(5) << fixed << setprecision(2) << coord << " | ";
}

/*
* Запрос у пользователя о моделировании движения
* 
* @param key ключ запроса
* @param time_of_moving время движения фигур
*/
void selectMove(Scene scene) {
    int key = 0, time_of_moving = 0;
    cout << "\nСмоделировать движение?\n[0] - нет\n[1] - да\n";
    cin >> key;
    switch (key)
    {
    case 0:
        break;
    case 1:
        cout << "Введите количество минут: ";
        cin >> time_of_moving;
        moving(scene, time_of_moving);
        break;
    default:
        cout << "Ошибка ввода";
        break;
    }
}
