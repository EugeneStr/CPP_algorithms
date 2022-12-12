#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>
#include <iomanip>

using namespace std;

class Divisiors {
public:
    int number;
    list<int> divs;

    Divisiors(int num) : number(num) {
        for (int i = 1; i <= number / 2; i++) {
            if (number % i == 0) {
                divs.push_back(i);
            }
        }
        divs.push_back(number);
    }

    friend ostream& operator<<(ostream& out, Divisiors obj);
};

ostream& operator<<(ostream& out, Divisiors obj)
{
    out << "Число: " << obj.number;
    out << "\nДелители: ";
    copy(obj.divs.begin(), obj.divs.end(), ostream_iterator<int>(out, " "));
    return out;
}

int randomCharacteristic(int a, int b); // Случайное число

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian"); // Руссификатор
    const int N = 10; // Количество элементов
    int d = 0, count = 0;
    list<Divisiors> numbers;
    for (int i = 0; i < N; i++) {
        Divisiors temp(randomCharacteristic(10, 50));
        numbers.push_back(temp);
    }
    copy(numbers.begin(), numbers.end(), ostream_iterator<Divisiors>(cout, "\n"));

    cout << "Введите делитель: ";
    cin >> d;
    count = count_if(numbers.begin(), numbers.end(), 
        [d](Divisiors obj) {
            for (auto div : obj.divs) {
                if (div == d) return true;
            }
            return false;
        });
    cout << "Количество чисел: " << count << "\n";

    list<double> avg;

    for_each(numbers.begin(), numbers.end(), 
        [&avg](Divisiors obj) {
            int sum = 0, c = 0;
            for (auto div : obj.divs) {
                c++;
                sum += div;
            }
            avg.push_back((double)sum / c);
        });
    for (auto el : avg) {
        printf("%.2lf\n", el);
    }
    return 0;
}

/* Случайное число */
int randomCharacteristic(int a, int b) {
    return a + rand() % (b - a + 1);
}
