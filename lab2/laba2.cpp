/*Вариант 2
*Требуется написать программу которая выполняет следующие действия.
*
*Получить от пользователя значение N.
*Выделить память под двумерный массив N* N.Тип массива int.
*Заполнить массив значениями в порядке возрастания от 1 до N * N, так, что элемент[N - 1][N - 1] будет равен N* N.
*Предложить пользователю возможность выполнить манипуляции с порядком элементов в массиве.
*Восстановить прямой порядок элементов(Не является отдельным действием в рамках базовой части задание, т.е.помимо него нужно реализовать еще 2).
*Сделать обратный порядок элементов(т.е.элемент[N - 1][N - 1] будет иметь значение 1)
*Сделать порядок элементов таким, что бы они были выложены вдоль главной диагонали.
*Сделать порядок элементов таким, что бы они были выложены вдоль побочной диагонали.
*Сделать порядок элементов таким, что бы все значения закручивались по часовой стрелке спиралью от элемента[0, 0].
*/

#include <iostream>

using namespace std;

int enter_N(); // Ввод размерности массива
int** start(int N); // Изначальное заполнение массива
void choice(int** array, int N); // Выбор сортировки пользователем
void print_array(int **array,int N); // Вывод массива
void standart_position(int **array, int N); // Обычное положение
void reverse_position(int **array, int N); // Обратное положение
void main_diagonal_position(int **array, int N); // Вдоль главной диагонали
void reverse_diagonal_position(int** array, int N); // Вдоль побочной диагонали
void spiral_position(int** array, int N); // По спирали
void delete_array(int** array, int N); // Очистка динамической памяти и завершение программы

int main()
{
    setlocale(LC_ALL, "Russian"); // Руссификатор
    int N = enter_N(); // Ввод размерности
    int** array = start(N); // Начальное расположение чисел
    choice(array, N); // Выбор сортировки пользователем
}

/*
 * Ввод размерности массива
 *
 * @return возвращает количество строк/столбцов в массиве.
 */
int enter_N() {
    int N = 0;
    cout << "Введите N: ";
    cin >> N;
    return N; // Размерность массива NxN
}

/*
 * Изначальное расположение чисел в массиве
 *
 * @param N размерность.
 * @return возвращает стандартно отсортированный массив.
 */
int** start(int N) {

    // Выделение памяти для динамического массива в зависимости от введенной размерности
    int** array = new int* [N];
    for (int i = 0; i < N; i++) {
        array[i] = new int[N];
    }

    standart_position(array, N); // Заполнение массива прямым порядком
    print_array(array, N); // Вывод массива
    return array; 
}

/*
 * Выбор сортировки пользователем
 *
 * @param array массив.
 * @param N размерность.
 */
void choice(int** array, int N) {
    int select = 0;
    cout << "\n[0] Стандартное положение\n" <<
            "[1] Обратное положение\n" <<
            "[2] Вдоль главной диагонали\n" <<
            "[3] Вдоль побочной диагонали\n" <<
            "[4] По спирали\n" <<
            "[5] Закрыть программу\n\n" <<
            "Выберете вариант сортировки: ";
    cin >> select; // Выбор функции пользователем

    switch (select) {
    case 0:
        standart_position(array, N); // Стандартное положение
        break;
    case 1:
        reverse_position(array, N); // Обратное положение
        break;
    case 2:
        main_diagonal_position(array, N); // Вдоль главной диагонали
        break;
    case 3:
        reverse_diagonal_position(array, N); // Вдоль побочной диагонали
        break;
    case 4:
        spiral_position(array, N); // По спирали
        break;
    case 5:
        delete_array(array, N); // Очищение памяти и завершение программы
        break;
    default:
        cout << "Неверно выбрана команда";
        choice(array, N); // Повторный выбор сортировки
        break;
    }
}

/*
 * Очистка динамической памяти и завершение программы
 *
 * @param array массив.
 * @param N размерность.
 */
void delete_array(int** array, int N) {

    // Очистка динамической памяти
    for (int i = 0; i < N; i++) {
        delete[]array[i];
    }
    delete[]array;
    exit(0); // Завершение программы
}

/*
 * Вывод полученного массива
 *
 * @param array массив.
 * @param N размерность.
 */
void print_array(int** array, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d", array[i][j]);
        }
        cout << "\n";
    }
}

/*
 * Стандартное расположение чисел
 *
 * @param array массив.
 * @param N размерность.
 */
void standart_position(int **array, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array[i][j] = i * N + j;
        }
    }
    print_array(array,N); // Вывод массива
    choice(array, N); // Повторный выбор сортировки
}

/*
 * Обратное расположение чисел
 *
 * @param array массив.
 * @param N размерность.
 */
void reverse_position(int** array, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array[i][j] = N * N - i * N - j - 1;;
        }
    }
    print_array(array, N); // Вывод массива
    choice(array, N); // Повторный выбор сортировки
}

/*
 * Расположение чисел вдоль главной диагонали
 *
 * @param array массив.
 * @param N размерность.
 */
void main_diagonal_position(int** array, int N) {

    // Заполнение первой строки и столбца массива
    array[0][0] = (N * N - N) / 2;
    for (int i = 1; i < N; i++) {
        array[0][i] = array[0][i - 1] - (N - i);
        array[i][0] = array[i - 1][0] + N - i + 1;
    }

    // Заполнение внутреннего квадрата матрицы
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            array[i][j] = array[i - 1][j - 1] + 1;
        }
    }
    print_array(array, N); // Вывод массива
    choice(array, N); // Повторный выбор сортировки
}

/*
 * Расположение чисел вдоль побочной диагонали
 *
 * @param array массив.
 * @param N размерность.
 */
void reverse_diagonal_position(int** array, int N) {
    
    // Заполнение первой строки и последнего столбца матрицы
    array[0][N - 1] = (N * N - N) / 2;
    for (int i = 1; i < N; i++) {
        array[0][N - 1 - i] = array[0][i - 1] - (N - i);
        array[i][N - 1] = array[i - 1][N - 1] + N - i + 1;
    }

    // Заполнение внутреннего квадрата матрицы
    for (int i = 1; i < N; i++) {
        for (int j = N - 2; j >= 0; j--) {
            array[i][j] = array[i - 1][j + 1] + 1;
        }
    }
    print_array(array, N); // Вывод массива
    choice(array, N); // Повторный выбор сортировки
}

/*
 * Расположение чисел по спирали
 *
 * @param array массив.
 * @param N размерность.
 * @param turn поворот спирали.
 * @param new_i необходимый столбец.
 * @param new_j необходимая строка.
 * @param round счетчик круга.
 * @param coef1 и coef2 коэффициенты, вынесенные из основной формулы для удобства восприятия программы.
 */
void spiral_position(int** array, int N) {
    int turn = 0, new_i = 0, new_j = 0, round = 0, coef1 = 0, coef2 = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            turn = ((j + 1) - (i + 1) + N) / N;
            new_i = abs(i - N / 2) + i / (N / 2) * ((N - 1) % 2);
            new_j = abs(j - N / 2) + j / (N / 2) * ((N - 1) % 2);
            round = (N - (abs(new_i - new_j) + new_i + new_j)) / 2;
            coef1 = i + 1 - round + j - round;
            coef2 = 4 * round * (N - round);
            array[i][j] = coef2 - 1 + turn * coef1 + abs(turn - 1) * (4 * (N - 2 * round) - 2 - coef1);
        }
    }
    print_array(array, N); // Вывод массива
    choice(array, N); // Повторный выбор сортировки
}