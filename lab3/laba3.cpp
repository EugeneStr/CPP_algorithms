/*Вариант 3
Требуется написать программу, в которой :
Будет реализована структуры Квадрат, Прямоугольник, Треугольник, Круг.
Каждая структура будет содержать только те поля которые нужны для однозначного ее описания.
Создан массив хранящий N фигур(использовать массив указателей void*), и массив N типов фигур(типы описать в перечислении).
Заполнить или сгенерировать массивы фигурами и типами созданных фигур.
Написать функции которые :
1. Подсчитают суммарную площадь фигур.
2. Подсчитать количество всех типов фигур имеющихся в массиве.
3. Вывести все фигуры с площадью больше указанной пользователем.
4. Найти радиус окружности в которую можно вписать все сгенерированные фигуры.*/

int getN(); // Ввод количества фигур
double Area(void* all_figure, int type_of_figure); // Подсчет площади фигуры
double squareArea(int a); // Площадь квадрата
double rectangleArea(int a, int b); // Площадь прямоугольника
double triangleArea(int a, int b, int alpha); // Площадь треуголника
double circleArea(int r); // Площадь круга
void countOfFigures(int* type_of_figure, int N); // Подсчет количества фигур
void bigArea(void* all_figure, int type_of_figure, double S); // Поиск большей площади
double outerRing(void* all_figure, int type_of_figure);  // Поиск описанной окружности
double squareRadius(int a); // Описанная окружность квадрата
double rectangleRadius(int a, int b); // Описанная окружность прямоугольника
double triangleRadius(int a, int b, int alpha); // Описанная окружность треугольника

#define PI 3.14159265 // Число ПИ

#include <iostream>
#include <math.h>
#include <vector>
#include <set>

using namespace std;

/* Структура квадрата
* @param side_square сторона квадрата.
*/
struct Square {
    int side_square;
};

/* Структура прямоугольника
* @param side1_rectangle первая сторона прямоугольника.
* @param side2_rectangle вторая сторона прямоугольника.
*/
struct Rectangle {
    int side1_rectangle;
    int side2_rectangle;
};

/* Структура треугольника
* @param side1_triangle первая сторона треугольника.
* @param side1_triangle вторая сторона треугольника.
* @param angle угол между сторонами треугольника.
*/
struct Triangle {
    int side1_triangle;
    int side2_triangle;
    int angle;
};

/* Структура круга
* @param radius_circle радиус круга.
*/
struct Circle {
    int radius_circle;
};

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian"); // Руссификатор
    string figures[] = { "квадрат", "прямоугольник", "треугольник", "круг" }; // виды фигур
    int N = 0, parametr_1 = 0, parametr_2 = 0, parametr_3 = 0;

    N = getN(); // Ввод количества фигур
    int* type_of_figure = new int[N]; // Массив типов фигур
    void** all_figure = new void* [N]; // Массив указателей на структуры

    // Создание фигур
    for (int i = 0; i < N; i++) {
        type_of_figure[i] = rand() % 4; // Генерация типа фигуры
        cout << figures[type_of_figure[i]] << " ";
        // квадрат
        if (type_of_figure[i] == 0) { 
            parametr_1 = 1 + rand() % 10; // сторона квадрата
            Square* sq = new Square{ parametr_1 }; 
            all_figure[i] = static_cast<void*>(sq);
            cout << "(a = " << static_cast<Square*>(all_figure[i])->side_square << ")";
        }
        // прямоуголник
        else if (type_of_figure[i] == 1) { 
            parametr_1 = 1 + rand() % 10; // сторона прямоугольника
            parametr_2 = 1 + rand() % 10; // сторона прямоугольника
            Rectangle* rect = new Rectangle{ parametr_1, parametr_2 };
            all_figure[i] = static_cast<void*>(rect);
            cout << "(a = " << static_cast<Rectangle*>(all_figure[i])->side1_rectangle << ", b = "
                << static_cast<Rectangle*>(all_figure[i])->side2_rectangle << ")";
        }
        // треуголник
        else if (type_of_figure[i] == 2) { 
            parametr_1 = 1 + rand() % 10; // сторона треугольника
            parametr_2 = 1 + rand() % 10; // сторона треугольника
            parametr_3 = 1 + rand() % 179; // угол треугольника
            Triangle* tri = new Triangle{ parametr_1, parametr_2, parametr_3 };
            all_figure[i] = static_cast<void*>(tri);
            cout  << "(a = " << static_cast<Triangle*>(all_figure[i])->side1_triangle << ", b = "
                << static_cast<Triangle*>(all_figure[i])->side2_triangle << ", alpha = "
                << static_cast<Triangle*>(all_figure[i])->angle << ")";
        }
        // круг
        else { 
            parametr_1 = 1 + rand() % 10;  // радиус круга
            Circle* circ = new Circle{ parametr_1 };
            all_figure[i] = static_cast<void*>(circ);
            cout << "(r = " << static_cast<Circle*>(all_figure[i])->radius_circle << ")";
        }
        cout << endl;
    }

    cout << "\n------- Задание 1 -------\n";

    double s = 0;
    for (int i = 0; i < N; i++) {
        s += Area(all_figure[i], type_of_figure[i]); // подсчет суммарной площади
    }
    cout << "Суммарная площадь всех фигур: " << s << endl;

    cout << "\n------- Задание 2 -------\nКоличество каждой фигуры:\n";

    countOfFigures(type_of_figure,N);

    cout << "\n------- Задание 3 -------\nВведите площадь для сравнения - ";
    double S = 0;
    cin >> S;

    cout << "\nФигуры, площадь которых больше заданной:\n";
    for (int i = 0; i < N; i++) {
        bigArea(all_figure[i], type_of_figure[i], S); // Вывод фигур с площадью большей S
    }

    double count = 0, R = 0;

    for (int i = 0; i < N; i++) {
        count = outerRing(all_figure[i], type_of_figure[i]); // радиус описанной окружности фигуры
        if (count > R) R = count; // Нахождение большего радиуса
    }
    cout << "\n------- Задание 4 -------\nРадиус описанной окружность - " << R;
    cout << endl;

    // очистка динамических массивов
    delete[] type_of_figure;
    delete[] all_figure;
}

/* Ввод количества фигур
* 
* @param N количество фигур.
* @return возвращает количество фигур.
*/
int getN() {
    int N = 0;
    cout << "Количество фигур - ";
    cin >> N;
    cout << "\n------- Созданные фигуры -------\n";
    return N;
}

/* Подсчет площади фигуры
* 
* @param all_figure массив с указателями на структуры.
* @param type_of_figure тип фигуры.
* @return возвращает площадь фигуры.
*/
double Area(void* all_figure, int type_of_figure) {
    switch (type_of_figure) {
    case 0: // квадрат
        return squareArea(static_cast<Square*>(all_figure)->side_square);
        break;
    case 1: // прямоугольник
        return rectangleArea(static_cast<Rectangle*>(all_figure)->side1_rectangle,
            static_cast<Rectangle*>(all_figure)->side2_rectangle);
        break;
    case 2: // треугольник
        return triangleArea(static_cast<Triangle*>(all_figure)->side1_triangle,
            static_cast<Triangle*>(all_figure)->side2_triangle,
            static_cast<Triangle*>(all_figure)->angle);
        break;
    default: // круг
        return circleArea(static_cast<Circle*>(all_figure)->radius_circle);
        break;
    }
}

/* Подсчет площади фигуры
* 
* @param N количество фигур.
* @param type_of_figure тип фигуры.
*/
void countOfFigures(int* type_of_figure, int N) {
    string figures[] = { "квадрат", "прямоугольник", "треугольник", "круг" };
    vector<int> vector_figure(type_of_figure, type_of_figure + N); // вектор из всех созданных фигур
    set<int> unique_vector(vector_figure.begin(), vector_figure.end()); // вектор уникальных фигур
    // вывод количества одинаковых фигур
    for (int figure : unique_vector)
        cout << figures[figure] << " - " << count(vector_figure.begin(), vector_figure.end(), figure) << endl;
}

/* Поиск большей площади фигуры
* 
* @param S площадь для сравнения.
* @param all_figure массив с указателями на структуры.
* @param type_of_figure тип фигуры.
*/
void bigArea(void* all_figure, int type_of_figure, double S) {
    double s = 0;
    string figures[] = { "квадрат", "прямоугольник", "треугольник", "круг" };
    s = Area(all_figure, type_of_figure); // площадь фигуры
    if (s > S) // сравнение площадей
        cout << figures[type_of_figure] << " с площадью " << s << endl;
}

/* Поиск описанной окружности
* 
* @param all_figure массив с указателями на структуры.
* @param type_of_figure тип фигуры.
* @return возвращает радиус описанной окружности.
*/
double outerRing(void* all_figure, int type_of_figure) {
    switch (type_of_figure) {
    case 0: // квадрат
        return squareRadius(static_cast<Square*>(all_figure)->side_square);
        break;
    case 1: // прямоугольник
        return rectangleRadius(static_cast<Rectangle*>(all_figure)->side1_rectangle,
            static_cast<Rectangle*>(all_figure)->side2_rectangle);
        break;
    case 2: // треугольник
        return triangleRadius(static_cast<Triangle*>(all_figure)->side1_triangle,
            static_cast<Triangle*>(all_figure)->side2_triangle,
            static_cast<Triangle*>(all_figure)->angle);
        break;
    default: // круг
        return static_cast<Circle*>(all_figure)->radius_circle;
        break;
    }
}

/* Площадь квадрата
* 
* @param a сторона квадрата.
* @return возвращает площадь квадрата.
*/
double squareArea(int a) {
    return a * a;
}

/* Площадь прямоуголника
* 
* @param a сторона прямоугольника.
* @param b сторона прямоугольника.
* @return возвращает площадь прямоуголника.
*/
double rectangleArea(int a, int b) {
    return a * b;
}

/* Площадь треугольника
* 
* @param a сторона треугольника.
* @param b сторона треугольника.
* @param alpha угол между сторонами.
* @return возвращает площадь треугольника.
*/
double triangleArea(int a, int b, int alpha) {
    return a * b * sin(alpha) * 0.5;
}

/* Площадь круга
* 
* @param r радиус круга.
* @return возвращает площадь круга.
*/
double circleArea(int r) {
    return PI * r * r;
}

/* Радиус описанной окружности квадрата
* 
* @param a сторона квадрата.
* @return возвращает радиус описанной окружности квадрата.
*/
double squareRadius(int a) {
    return (double)a / 2 * sqrt(2);
}

/* Радиус описанной окружности прямоугольника
* 
* @param a сторона прямоугольника.
* @param b сторона прямоугольника.
* @return возвращает радиус описанной окружности прямоугольника.
*/
double rectangleRadius(int a, int b) {
    return sqrt(a * a + b * b) / 2;
}

/* Радиус описанной окружности треугольника
* 
* @param a сторона треугольника.
* @param b сторона треугольника.
* @param alpha угол между сторонами.
* @return возвращает радиус описанной окружности треугольника.
*/
double triangleRadius(int a, int b, int alpha) {
    double c = sqrt(a * a + b * b - 2 * a * b * cos(alpha)); // 3 сторона
    return a * b * c / (4 * triangleArea(a, b, alpha));
}
